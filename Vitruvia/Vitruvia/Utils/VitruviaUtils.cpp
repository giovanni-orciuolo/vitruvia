/*
	____   ____.__  __                     .__        
	\   \ /   /|__|/  |________ __ _____  _|__|____   
	 \   Y   / |  \   __\_  __ \  |  \  \/ /  \__  \  
	  \     /  |  ||  |  |  | \/  |  /\   /|  |/ __ \_
	   \___/   |__||__|  |__|  |____/  \_/ |__(____  /
												   \/ 
	Premium Internal Multihack for Counter-Strike: Global Offensive
	Made with love by double v - 2017
*/

#include "VitruviaUtils.h"
#include "CrashLogger/CrashLogger.h"

#define INRANGE(x,a,b)    (x >= a && x <= b)
#define GET_BITS( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define GET_BYTE( x )    (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))

// lil' global to store pointer to console stream
FILE* pConsole = nullptr;
void ToggleConsole(bool bToggle)
{
#ifdef ENABLE_CONSOLE
	if (bToggle)
	{
		// Allocate a new console for CS:GO :)
		AllocConsole();

		// Open the console stream to be output stream with write access ("w")
		// Store the value of console stream pointer into pConsole
		freopen_s(&pConsole, enc("CONOUT$"), enc("w"), stdout);

		// Set a console title, just to be fancy
		SetConsoleTitle(enc("Vitruvia Developer Console"));

		// Sets the default console color to be plain white
		SetConsoleTextColor(white);

		// Will make the blinking annoying cursor disappear. We won't need it since I never read stuff in the console
		CONSOLE_CURSOR_INFO CurInfo{ 1, false };
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);

		// Greetings!
		w_printf(enc("Welcome to Vitruvia Console!\n"));
	}
	else
	{
		// IMPORTANT: fclose closes the stream, allowing the console to be freed and closed by FreeConsole()
		// If fclose isn't called, the console will stay opened.
		fclose(pConsole);
		FreeConsole();
	}
#endif
}

tm GetLocalTime()
{
	// Use c++11 shit to retrieve local time in a C style (with tm)
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	time_t t = std::chrono::system_clock::to_time_t(now);
	return *localtime(&t);
}
std::string GetLocalTimeString()
{
	tm local_tm = GetLocalTime();
	std::stringstream time_str;
	time_str 
		<< "[" 
		<< ((local_tm.tm_hour >= 10) ? "" : "0") 
		<< local_tm.tm_hour
		<< ":" << ((local_tm.tm_min >= 10) ? "" : "0") 
		<< local_tm.tm_min
		<< ":" << ((local_tm.tm_sec >= 10) ? "" : "0") 
		<< local_tm.tm_sec
		<< "]";
	return time_str.str();
}
std::string GetCurrentDay()
{
	tm local_tm = GetLocalTime();
	switch (local_tm.tm_wday)
	{
	case 0: return "Sunday"; break;
	case 1: return "Monday"; break;
	case 2: return "Tuesday"; break;
	case 3: return "Wednesday"; break;
	case 4: return "Thursday"; break;
	case 5: return "Friday"; break;
	case 6: return "Saturday"; break;
	default: return "Unknown"; break;
	}
	return "Unknown";
}
std::string GetDateString()
{
	tm local_tm = GetLocalTime();
	std::stringstream date_str;
	date_str << local_tm.tm_mday << "/" << local_tm.tm_mon + 1 << "/" << 1900 + local_tm.tm_year;
	return date_str.str();
}

std::string WS2S(const std::wstring& ws)
{
	static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	try
	{
		return converter.to_bytes(ws);
	}
	catch (std::exception e)
	{
		Err(enc("WS2S failed on wstring %s"), ws.c_str());
		Err(enc("Additional information:\n%s"), e.what());
		return std::string();
	}
}
std::wstring S2WS(const std::string& s)
{
	static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	try
	{
		return converter.from_bytes(s);
	}
	catch (std::exception e)
	{
		Err(enc("S2WS failed on string %s"), s.c_str());
		Err(enc("Additional information:\n%s"), e.what());
		return std::wstring();
	}
}

std::string GetAppDataFolder()
{
	wchar_t* buffer = nullptr;
	HRESULT res = SHGetKnownFolderPath(FOLDERID_RoamingAppData, NULL, NULL, &buffer);
	if (res == S_OK)
	{
		std::wstringstream wss;
		wss << buffer << L"\\" << CHEAT_NAME << "\\";
		CoTaskMemFree((void*)(buffer));
		CreateDirectory(WS2S(wss.str()).c_str(), NULL);
		return WS2S(wss.str());
	}
	else
	{
		if (res == E_FAIL)
			Err(enc("Failed to get AppData! (ReferenceID folder is virtual)."));
		else if (res == E_INVALIDARG)
			Err(enc("Failed to get AppData! (ReferenceID not present on this system)."));
		else
			Err(enc("Failed to get AppData! (Unknown Reason)"));
	}
	return enc("");
}
bool FileExists(const std::string& path)
{
	struct stat info;
	return stat(path.c_str(), &info) == 0 && S_ISREG(info.st_mode);
}
bool DirectoryExists(const std::string& path)
{
	struct stat info;
	return stat(path.c_str(), &info) == 0 && S_ISDIR(info.st_mode);
}
std::string GetGamePath()
{
	char szPath[MAX_PATH];
	GetModuleFileNameA(NULL, szPath, MAX_PATH);
	std::string path(szPath);
	return path.substr(0, path.find_last_of("\\") + 1);
}

FORCEINLINE void log_start()
{
	// Standard behaviour before logs
	SetConsoleTextColor(cyan);
	w_printf(enc("%s "), GetLocalTimeString().c_str());
	SetConsoleTextColor(white);
}
FORCEINLINE void log_end()
{
	// Standard behaviour after logs
	SetConsoleTextColor(white);
	w_printf(enc("\n"));
}

void Log(const char* szLog, ...)
{
	char buffer[989];
	va_list list;
	va_start(list, szLog);
	vsprintf(buffer, szLog, list);
	va_end(list);
#ifdef ENABLE_CONSOLE
	log_start();
	SetConsoleTextColor(green);
	w_printf(buffer, list);
	log_end();
#endif
	Vitruvia::g_CrashLog.Add(std::string(buffer));
}
void Err(const char* szErr, ...)
{
	char buffer[989];
	va_list list;
	va_start(list, szErr);
	vsprintf(buffer, szErr, list);
	va_end(list);
#ifdef ENABLE_CONSOLE
	log_start();
	SetConsoleTextColor(red);
	w_printf(buffer, list);
	log_end();
#endif
	Vitruvia::g_CrashLog.Add(std::string(buffer));
}
void Warn(const char* szWarn, ...)
{
	char buffer[989];
	va_list list;
	va_start(list, szWarn);
	vsprintf(buffer, szWarn, list);
	va_end(list);
#ifdef ENABLE_CONSOLE
	log_start();
	SetConsoleTextColor(yellow);
	w_printf(buffer, list);
	log_end();
#endif
	Vitruvia::g_CrashLog.Add(std::string(buffer));
}

// Made by learn_more but you already know it I guess
// Really easy and fast to use
DWORD FindPattern(const std::string& strModuleName, const std::string& strPattern)
{
	const char* pat = strPattern.c_str();

	DWORD firstMatch = 0;

	DWORD rangeStart = (DWORD)GetModuleHandleA(strModuleName.c_str());
	MODULEINFO miModInfo; 
	GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;

	for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
	{
		if (!*pat)
			return firstMatch;
		if (*(PBYTE)pat == '\?' || *(PBYTE)pCur == GET_BYTE(pat))
		{
			if (!firstMatch)
				firstMatch = pCur;

			if (!pat[2])
				return firstMatch;

			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
				pat += 3;
			else
				pat += 2;
		}
		else
		{
			pat = strPattern.c_str();
			firstMatch = 0;
		}
	}
	
	Err(enc("FindPattern() with sig '%s' failed!!!"), strPattern.c_str());
	return NULL;
}
bool IsCodePtr(void* ptr)
{
	constexpr const DWORD protect_flags = PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;

	MEMORY_BASIC_INFORMATION out;
	VirtualQuery(ptr, &out, sizeof out);

	return out.Type
		&& !(out.Protect & (PAGE_GUARD | PAGE_NOACCESS))
		&& out.Protect & protect_flags;
}
int WaitForModules(std::int32_t timeout, const std::initializer_list<std::wstring>& modules)
{
	bool signaled[32] = { 0 };
	bool success = false;

	std::uint32_t totalSlept = 0;

	if (timeout == 0) {
		for (auto& mod : modules) {
			if (GetModuleHandleW(std::data(mod)) == NULL)
				return WAIT_TIMEOUT;
		}
		return WAIT_OBJECT_0;
	}

	if (timeout < 0)
		timeout = INT32_MAX;

	while (true) {
		for (auto i = 0u; i < modules.size(); ++i) {
			auto& module = *(modules.begin() + i);
			if (!signaled[i] && GetModuleHandleW(std::data(module)) != NULL) {
				signaled[i] = true;

				//
				// Checks if all modules are signaled
				//
				bool done = true;
				for (auto j = 0u; j < modules.size(); ++j) {
					if (!signaled[j]) {
						done = false;
						break;
					}
				}
				if (done) {
					success = true;
					goto exit;
				}
			}
		}
		if (totalSlept > std::uint32_t(timeout)) {
			break;
		}
		Sleep(10);
		totalSlept += 10;
	}

exit:
	return success ? WAIT_OBJECT_0 : WAIT_TIMEOUT;
}

std::string ReplaceString(std::string subject, const std::string& search, const std::string& replace)
{
	size_t pos = 0;

	while ((pos = subject.find(search, pos)) != std::string::npos)
	{
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}

	return subject;
}
std::string PadStringRight(std::string text, size_t value)
{
	text.insert(text.length(), value - text.length(), ' ');
	return text;
}
bool Contains(const std::string& word, const std::string& sentence) {
	if (word == "" || sentence == "")
		return true;
	return sentence.find(word) != std::string::npos;
}
std::string ToLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}
std::string ToUpper(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}
void StdReplaceStr(std::string& replaceIn, const std::string& replace, const std::string& replaceWith)
{
	size_t const span = replace.size();
	size_t const step = replaceWith.size();
	size_t index = 0;

	while (true)
	{
		index = replaceIn.find(replace, index);

		if (index == std::string::npos)
			break;

		replaceIn.replace(index, span, replaceWith);
		index += step;
	}
}

void InitKeyValues(Source::KeyValues* key_values, const char* name)
{
	// Find address to InitKeyValues
	static offset_t ikv_address = FindPattern(CLIENT_DLL, enc("68 ? ? ? ? 8B C8 E8 ? ? ? ? 89 45 FC EB 07 C7 45 ? ? ? ? ? 8B 03 56")) + 7;
	// Deref to the effective function
	static offset_t ikv_function = ikv_address + *rcast<PDWORD_PTR>(ikv_address + 1) + 5;

	if (!ikv_function) return;

	// Naked gate to call the function
	__asm
	{
		push name
		mov ecx, key_values
		call ikv_function
	}
}
void LoadFromBuffer(Source::KeyValues* key_values, const char* resource_name, const char* buffer, void* file_system, const char* path_id, void* unknown)
{
	// Find the function
	static offset_t lfb_function = FindPattern(CLIENT_DLL, enc("55 8B EC 83 EC 48 53 56 57 8B F9 89 7D F4"));

	if (!lfb_function) return;

	// Naked gate to call the function
	__asm
	{
		push unknown
		push path_id
		push file_system
		push buffer
		push resource_name
		mov ecx, key_values
		call lfb_function
	}
}

void SetName(const char* name, Source::ConVar* cvar_name)
{
	*(int*)((DWORD)&cvar_name->m_fnChangeCallback + 0xC) = NULL;
	cvar_name->SetValue(name);
}

void SetClanTag(const char* name, const char* tag)
{
	typedef void(*SetClanTag_t)(const char*, const char*);
	SetClanTag_t SetClanTagFn = (SetClanTag_t)(ENGINE_BASE + Vitruvia::Offsets::SetClanTag);

	SetClanTagFn(name, tag);
}
