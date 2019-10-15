#pragma once
#include "stdafx.h" // use pch.h in Visual Studio 2019 and later
#include <unordered_map>
#ifdef TRACKPADDLL_EXPORTS
#define TRACKPADDLL_API __declspec(dllexport)
#else
#define TRACKPADDLL_API __declspec(dllimport)
#endif
static bool realKeyboard;
static std::unordered_map<DWORD, char> KeyStates;
extern "C" BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved);
extern "C" TRACKPADDLL_API bool isKeyPressedState(int key);
extern "C" TRACKPADDLL_API BOOL InstallHook(HWND hwndParent);
extern "C" TRACKPADDLL_API BOOL UninstallHook();