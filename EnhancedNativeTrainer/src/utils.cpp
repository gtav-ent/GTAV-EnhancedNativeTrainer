/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "utils.h"
#include <windows.h>
#include <psapi.h>
#include <stdio.h>
#include <sys/stat.h>
#include <vector>
#include <sstream>

#include "debug\debuglog.h"

extern "C" IMAGE_DOS_HEADER __ImageBase; // MSVC specific, with other compilers use HMODULE from DllMain

bool isFiveM;

std::string cachedModulePath;

std::string GetCurrentModulePath()
{
	if (cachedModulePath.empty())
	{
		// get module path
		char modPath[MAX_PATH];
		memset(modPath, 0, sizeof(modPath));
		GetModuleFileNameA((HMODULE)&__ImageBase, modPath, sizeof(modPath));
		for (size_t i = strlen(modPath); i > 0; i--)
		{
			if (modPath[i - 1] == '\\')
			{
				modPath[i] = 0;
				break;
			}
		}
		cachedModulePath = modPath;
	}
	return cachedModulePath;
}

HMODULE GetENTModuleHandle()
{
	HMODULE hMod = NULL;
	GetModuleHandleExW(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		reinterpret_cast<LPCWSTR>(&GetENTModuleHandle),
		&hMod);

	return hMod;
}

void CheckIsHostProcessFiveM()
{
	HMODULE hMods[1024];
	DWORD cbNeeded;

	DWORD procID = GetCurrentProcessId();
	if (procID == NULL)
	{
		write_text_to_log_file("No process ID");
		return;
	}

	HANDLE currentProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, GetCurrentProcessId());

	if (currentProcess == NULL)
	{
		write_text_to_log_file("No process");
		return;
	}

	bool result = false;

	if (EnumProcessModules(currentProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			TCHAR szModName[MAX_PATH];

			// Get the full path to the module's file.

			if (GetModuleFileNameEx(currentProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
			{
				std::string moduleName(szModName);
				if (StringEndsWith(moduleName, "CoreRT.dll"))
				{
					write_text_to_log_file("Found FiveM");
					result = true;
					break;
				}
			}
		}
	}

	CloseHandle(currentProcess);

	isFiveM = result;
}

bool does_file_exist(const char* name)
{
	struct stat buffer;
	return (stat(name, &buffer) == 0);
}

bool StringEndsWith(const std::string& a, const std::string& b)
{
	if (b.size() > a.size()) return false;
	return std::equal(a.begin() + a.size() - b.size(), a.end(), b.begin());
}

bool StringStartsWith(const std::string& a, const std::string& b)
{
	if (b.size() > a.size()) return false;
	return std::equal(a.begin(), a.begin() + b.size(), b.begin());
}

std::wstring ConvertFromUtf8ToUtf16(const std::string& str)
{
	std::wstring convertedString;
	int requiredSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, 0, 0);
	if (requiredSize > 0)
	{
		std::vector<wchar_t> buffer(requiredSize);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &buffer[0], requiredSize);
		convertedString.assign(buffer.begin(), buffer.end() - 1);
	}

	return convertedString;
}

bool is_fivem()
{
	return isFiveM;
}


//Converts Radians to Degrees
float degToRad(float degs)
{
	return degs*(float)3.141592653589793 / (float)180.0;
}

//Converts Degrees to Radians
float radToDeg(float rads)
{
	return rads * ((float)180.0 / (float)3.141592653589793);
}