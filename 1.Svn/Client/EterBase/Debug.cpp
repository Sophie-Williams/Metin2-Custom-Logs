//Find
HWND g_PopupHwnd = NULL;

///Add
#if defined(CUSTOM_LOGS)
#include <memory>
#include <filesystem>
class CustomLogFile : public CSingleton<CustomLogFile>
{
	public:
		CustomLogFile() { 
			m_fp.clear();
			const std::string path = "logs/";
			if (!std::filesystem::is_directory(path) || !std::filesystem::exists(path))
				std::filesystem::create_directory(path);
			for (const auto& v : { "ITEM_ERROR.txt", "MAP_ERROR.txt" })
				m_fp.emplace_back(std::make_unique<FILE*>(fopen(std::string(path + v).c_str(), "w")));
		}
		virtual ~CustomLogFile() {
			for (auto& v : m_fp)
				fclose(*v);
			m_fp.clear();
		}
		void Write(const char* c_pszMsg, BYTE Type) {
			if (Type >= m_fp.size() || !*m_fp[Type])
				return;
			const __time64_t ct = time(nullptr);
			const tm ctm = *localtime(&ct);
			fprintf(*m_fp[Type], "%02d%02d %02d:%02d:%05d :: %s\n",
				ctm.tm_mon + 1, ctm.tm_mday, ctm.tm_hour, ctm.tm_min,
				ELTimer_GetMSec() % 60000, c_pszMsg);
			fflush(*m_fp[Type]);
		}
	protected:
		std::vector < std::unique_ptr<FILE*> > m_fp;
};
#endif

//Find
static CLogFile gs_logfile;

///Add
#if defined(CUSTOM_LOGS)
static CustomLogFile JustForInit;
void LogFileWithType(BYTE Type, const char * c_szMessage, ...)
{
	va_list args;
	va_start(args, c_szMessage);
	char szBuf[DEBUG_STRING_MAX_LEN+1];
	_vsnprintf(szBuf, sizeof(szBuf), c_szMessage, args);
	CustomLogFile::Instance().Write(szBuf, Type);
}
#endif