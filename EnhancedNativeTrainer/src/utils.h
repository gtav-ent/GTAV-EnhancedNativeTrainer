/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#pragma once

#include <string>
#include <stdio.h>
#include <windows.h>

// returns module load path with trailing slash
std::string GetCurrentModulePath();

HMODULE GetENTModuleHandle();

bool does_file_exist(const char* name);

bool StringEndsWith(const std::string& a, const std::string& b);

bool StringStartsWith(const std::string& a, const std::string& b);

std::wstring ConvertFromUtf8ToUtf16(const std::string& str);

float degToRad(float degs);

float radToDeg(float rads);