//Find
HWND g_PopupHwnd = NULL;

///Add
#if defined(CUSTOM_LOGS)
#include <memory>
class CustomLogFile : public CSingleton<CustomLogFile>
{
	public:
		CustomLogFile(const std::string& path) { 
			if (!CreateDirectory(path.c_str(), nullptr))
				return;
			for (const std::string& v : { "ITEM_ERROR.txt", "MAP_ERROR.txt" })
				m_fp.emplace_back(unique_ptr<std::FILE, decltype(&fclose)>(fopen((path + "/" + v).c_str(), "a"), &fclose));		
		}
		virtual ~CustomLogFile() {m_fp.clear();}
		void Write(const char* c_pszMsg, BYTE Type) {
			if (Type >= m_fp.size() || !m_fp.at(Type))
				return;
			const auto ct = time(nullptr);
			const auto ctm = *localtime(&ct);
			fprintf(m_fp.at(Type).get(), "%02d%02d %02d:%02d:%05d :: %s\n",
				ctm.tm_mon + 1, ctm.tm_mday, ctm.tm_hour, ctm.tm_min,
				ELTimer_GetMSec() % 60000, c_pszMsg);
			fflush(m_fp.at(Type).get());
		}
	protected:
		std::vector <std::unique_ptr<std::FILE, decltype(&fclose)>> m_fp;
};
#endif

//Find
static CLogFile gs_logfile;

///Add
#if defined(CUSTOM_LOGS)
static CustomLogFile JustForInit("logs");
void LogFileWithType(BYTE Type, const char * c_szMessage, ...)
{
	va_list args;
	va_start(args, c_szMessage);
	char szBuf[DEBUG_STRING_MAX_LEN+1];
	_vsnprintf(szBuf, sizeof(szBuf), c_szMessage, args);
	va_end(args);
	CustomLogFile::Instance().Write(szBuf, Type);
}
#endif
