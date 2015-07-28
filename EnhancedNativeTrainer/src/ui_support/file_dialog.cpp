/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "mfc_dialog.h"
#include <Windows.h>
#include <commdlg.h>

#include "..\utils.h"
#include "..\debug\debuglog.h"

#include <sstream>

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

static HWND parentWindow = 0;

void find_parent_window()
{
	EnumWindows(EnumWindowsProc, NULL);
}

void show_file_open_dialog(std::string title)
{
	if (parentWindow == 0)
	{
		find_parent_window();
	}

	OPENFILENAME ofn;       // common dialog box structure
	char szFile[MAX_PATH];       // buffer for file name
	HANDLE hf;              // file handle

	DWORD procID = GetCurrentProcessId();

	EnumWindows(EnumWindowsProc, NULL);

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = parentWindow;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "ENT XML Files\0*.XML\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrTitle = title.c_str();
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn) == TRUE)
		hf = CreateFile(ofn.lpstrFile,
		GENERIC_READ,
		0,
		(LPSECURITY_ATTRIBUTES)NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);
}

void show_file_save_dialog(std::string title)
{
	if (parentWindow == 0)
	{
		find_parent_window();
	}

	OPENFILENAME sfn;       // common dialog box structure
	char szFile[MAX_PATH];       // buffer for file name
	HANDLE hf;              // file handle

	DWORD procID = GetCurrentProcessId();

	EnumWindows(EnumWindowsProc, NULL);

	// Initialize OPENFILENAME
	ZeroMemory(&sfn, sizeof(sfn));
	sfn.lStructSize = sizeof(sfn);
	sfn.hwndOwner = parentWindow;
	sfn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	sfn.lpstrFile[0] = '\0';
	sfn.nMaxFile = sizeof(szFile);
	sfn.lpstrFilter = "ENT XML Files\0*.XML\0\0";
	sfn.nFilterIndex = 1;
	sfn.lpstrFileTitle = NULL;
	sfn.lpstrTitle = title.c_str();
	sfn.nMaxFileTitle = 0;
	sfn.lpstrInitialDir = NULL;
	sfn.Flags = OFN_PATHMUSTEXIST;

	// Display the Open dialog box. 

	if (GetSaveFileName(&sfn) == TRUE)
		hf = CreateFile(sfn.lpstrFile,
		GENERIC_WRITE,
		0,
		(LPSECURITY_ATTRIBUTES)NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	DWORD procID = GetCurrentProcessId();
	DWORD windowID;
	if (GetWindowThreadProcessId(hwnd, &windowID))
	{
		if (procID == windowID)
		{
			char class_name[80];
			char title[80];
			GetClassName(hwnd, class_name, sizeof(class_name));
			GetWindowText(hwnd, title, sizeof(title));
			if (strcmp(class_name, "grcWindow") == 0)
			{
				std::ostringstream ss;
				ss << "Window title: " << title << " and class: " << class_name;
				write_text_to_log_file(ss.str());
				parentWindow = hwnd;
			}
		}
	}
	return TRUE;
}