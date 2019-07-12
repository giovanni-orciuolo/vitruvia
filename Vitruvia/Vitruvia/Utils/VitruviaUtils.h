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

#pragma once
#include "../Vitruvia.h"

enum ConCol
{
	black = 0,
	dark_blue = 1,
	dark_green = 2,
	dark_aqua, dark_cyan = 3,
	dark_red = 4,
	dark_purple = 5, dark_pink = 5, dark_magenta = 5,
	dark_yellow = 6,
	dark_white = 7,
	gray = 8,
	blue = 9,
	green = 10,
	aqua = 11, cyan = 11,
	red = 12,
	purple = 13, pink = 13, magenta = 13,
	yellow = 14,
	white = 15
};
FORCEINLINE void SetConsoleTextColor(ConCol iColor)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), iColor);
}
FORCEINLINE int64 GetEpochTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

extern void ToggleConsole(bool bToggle);

extern tm GetLocalTime();
extern std::string GetLocalTimeString();
extern std::string GetCurrentDay();
extern std::string GetDateString();

extern std::string WS2S(const std::wstring& ws);
extern std::wstring S2WS(const std::string& s);

extern std::string GetAppDataFolder();
extern bool FileExists(const std::string& path);
extern bool DirectoryExists(const std::string& path);
extern std::string GetGamePath();

void Log(const char* szLog, ...);
void Err(const char* szErr, ...);
void Warn(const char* szWarn, ...);

extern DWORD FindPattern(const std::string& strModuleName, const std::string& strPattern);
extern bool IsCodePtr(void* ptr);
extern int WaitForModules(std::int32_t timeout, const std::initializer_list<std::wstring>& modules);

extern std::string ReplaceString(std::string subject, const std::string& search, const std::string& replace);
extern std::string PadStringRight(std::string text, size_t value);
extern bool Contains(const std::string& word, const std::string& sentence);
extern std::string ToLower(std::string str);
extern std::string ToUpper(std::string str);
extern void StdReplaceStr(std::string& replaceIn, const std::string& replace, const std::string& replaceWith);

extern void InitKeyValues(Source::KeyValues* key_values, const char* name);
extern void LoadFromBuffer(Source::KeyValues* key_values, const char* resource_name, const char* buffer, void* file_system, const char* path_id, void* unknown);

template<class T>
constexpr bool is_one_of(T input, std::initializer_list<T> predicates) {
	return std::find(predicates.begin(), predicates.end(), input) != predicates.end();
}

extern void SetName(const char* name, Source::ConVar* cvar_name);
extern void SetClanTag(const char* name, const char* tag);

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
	std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
	std::advance(start, dis(g));
	return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	return select_randomly(start, end, gen);
}

template<class T>
static T* FindHudElement(const char* name)
{
	static auto _this = *reinterpret_cast<DWORD**>(FindPattern(CLIENT_DLL, enc("B9 ? ? ? ? E8 ? ? ? ? 83 7D F8 ?")) + 1);
	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(FindPattern(CLIENT_DLL, enc("55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28")));
	return (T*)find_hud_element(_this, name);
}