/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "file_dialog.h"
#include <Windows.h>
#include <commdlg.h>

#include "..\utils.h"
#include "..\debug\debuglog.h"

#include <sstream>

static HWND parentWindow = 0;

struct SaveDialogThreadContext
{
	std::string title;
	SaveFileDialogCallback* request;
};

struct LoadDialogThreadContext
{
	std::string title;
	LoadFileDialogCallback* request;
};


void show_save_dialog_in_thread(std::string title, SaveFileDialogCallback* request)
{
	DWORD myThreadID;
	SaveDialogThreadContext* ctx = new SaveDialogThreadContext{ title, request };
	HANDLE myHandle = CreateThread(0, 0, show_save_dialog_thread_call, ctx, 0, &myThreadID);
	CloseHandle(myHandle);
}

void show_load_dialog_in_thread(std::string title, LoadFileDialogCallback* request)
{
	DWORD myThreadID;
	LoadDialogThreadContext* ctx = new LoadDialogThreadContext{ title, request };
	HANDLE myHandle = CreateThread(0, 0, show_open_dialog_thread_call, ctx, 0, &myThreadID);
	CloseHandle(myHandle);
}

void find_parent_window()
{
	EnumWindows(EnumWindowsProc, NULL);
}

DWORD WINAPI show_save_dialog_thread_call(LPVOID lpParameter)
{
	SaveDialogThreadContext *ctx = static_cast<SaveDialogThreadContext*>(lpParameter);
	show_file_save_dialog(ctx->title, ctx->request);
	delete ctx;
	return 0;
}

DWORD WINAPI show_open_dialog_thread_call(LPVOID lpParameter)
{
	LoadDialogThreadContext *ctx = static_cast<LoadDialogThreadContext*>(lpParameter);
	show_file_open_dialog(ctx->title, ctx->request);
	delete ctx;
	return 0;
}

void show_file_open_dialog(std::string title, LoadFileDialogCallback* callback)
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
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn) == TRUE)
	{
		hf = CreateFile(ofn.lpstrFile,
			GENERIC_READ,
			0,
			(LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			(HANDLE)NULL);

		if (hf == INVALID_HANDLE_VALUE)
		{
			write_text_to_log_file("CreateFile returned invalid handle");
			std::ostringstream ss;
			ss << "Selected file was: " << ofn.lpstrFile << " and error " << GetLastError();
			write_text_to_log_file(ss.str());
			callback->success = false;
		}
		else
		{
			CloseHandle(hf);
			callback->filePath = ofn.lpstrFile;
			callback->success = true;
		}
	}
	else
	{
		write_text_to_log_file("GetOpenFileName returned false");
		callback->success = false;
	}

	callback->complete = true;
}

void show_file_save_dialog(std::string title, SaveFileDialogCallback* callback)
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
	sfn.Flags = OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	// Display the Open dialog box. 

	if (GetSaveFileName(&sfn) == TRUE)
	{
		hf = CreateFile(sfn.lpstrFile,
			GENERIC_WRITE,
			0,
			(LPSECURITY_ATTRIBUTES)NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			(HANDLE)NULL);

		if (hf == INVALID_HANDLE_VALUE)
		{
			callback->success = false;
		}
		else
		{
			CloseHandle(hf);
			callback->filePath = sfn.lpstrFile;
			callback->success = true;
		}
	}
	else
	{
		callback->success = false;
	}

	callback->complete = true;
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

			std::ostringstream ss;
			ss << "Window title: " << title << " and class: " << class_name;
			write_text_to_log_file(ss.str());

			if (strcmp(class_name, "DIEmWin") == 0)
			{
				parentWindow = hwnd;
				return FALSE;
			}
		}
	}
	return TRUE;
}