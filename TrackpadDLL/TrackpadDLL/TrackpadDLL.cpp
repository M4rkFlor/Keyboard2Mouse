// TrackpadDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h" // use pch.h in Visual Studio 2019 and later
#include "TrackpadDLL.h"

UINT const WM_HOOK = WM_APP + 1;
HWND hwndServer = NULL;
HINSTANCE instanceHandle = NULL;

//Hooks
HHOOK hookHandle = NULL;
HHOOK hookHandle2 = NULL;
HHOOK hookHandle3 = NULL;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		instanceHandle = hModule;
		hookHandle = NULL;
		hookHandle2 = NULL;
		hookHandle3 = NULL;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
//Checks if key is pressed
bool isKeyPressedState(int key) {
	//convert sfml key to vkCode
	//https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
	if (0 <= key && key <= 25) {
		key += 65; //a-z
	}
	else if (26 <= key && key <= 35) {
		////vkCode 48-57
		key += 22; //0-9
	}
	else if (85 <= key && key <= 99) {
		////vkCode 112‬-126‬
		key += 27; //F1-F15
	}
	else if (key == 36) {
		key = VK_ESCAPE;//escape
	}
	else if (key==57) {
		key = VK_SPACE;
	}
	else if (key == 46) {
		key = VK_OEM_4; //[
	}
	else if (key == 47) {
		key = VK_OEM_6; //]
	}
	else if (key == 48) {
		key = VK_OEM_1; //;
	}
	else if (key == 49) {
		key = VK_OEM_COMMA; //,
	}
	else if (key == 50) {
		key = VK_OEM_PERIOD; //.
	}
	else if (key == 51) {
		key = VK_OEM_7; //'
	}
	else if (key == 52) {
		key = VK_OEM_2; // /
	}
	else if (key == 53) {
		key = VK_OEM_5; //"\"
	}
	else if (key == 54) {
		key = VK_OEM_3; //~
	}
	else if (key == 55) {
		key = VK_OEM_PLUS; // += 
	}
	else if (key == 56) {
		key = VK_OEM_MINUS; //-
	}
	else if (key == 58) {
		key = VK_RETURN;
	}
	else if (key == 42) {
		key = VK_RSHIFT;//R-shift
	}
	else if (key == 38) {
		key = VK_LSHIFT;//L-shift
	}
	else if (key == 39) {
		//key = VK_LMENU;//L-alt
	}
	else if (key == 43) {
		//key = VK_RMENU;//R-alt these are wrong
	}
	else if (key == 71) {
		key = VK_LEFT;//arrow keys
	}
	else if (key == 72) {
		key = VK_RIGHT; 
	}
	else if (key == 73) {
		key = VK_UP;
	}
	else if (key == 74) {
		key = VK_DOWN;
	}
	else if (key == 75) {
		key = VK_NUMPAD0;
	}
	else if (key == 41) {
		key = VK_RCONTROL;
	}
	return (KeyStates[(DWORD)key] == 1) ? true:false;
}


//Keyboard Hook procedure
static LRESULT CALLBACK LowLevelKeyboardProc(int code, WPARAM wParam, LPARAM lParam) {
	if ( code < 0) {
		return CallNextHookEx(hookHandle, code, wParam, lParam);
	} 
	//determine if key is injected
	KBDLLHOOKSTRUCT * info = (KBDLLHOOKSTRUCT *)lParam;
	bool injected = (info->flags & LLKHF_INJECTED);// bit mask
	if (!injected) {
		realKeyboard = true;
		//Record The key state and thats it
		if (wParam == WM_KEYDOWN) {
				KeyStates[info->vkCode] = 1;
		}
		else if (wParam == WM_KEYUP) {
			KeyStates[info->vkCode] = 0;
		}
		return 1;
	}
	else {
		realKeyboard = false;
		//do nothing and let it pass
	}
	return CallNextHookEx(hookHandle, code, wParam, lParam);
}

BOOL InstallHook(HWND hwndParent) {
	//Check if already hooked
	if (hwndServer != NULL) {
		return FALSE;
	}

	//Register keyboard Hook LL
	hookHandle = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)LowLevelKeyboardProc, instanceHandle, 0);
	if (hookHandle == NULL) {
		return FALSE;
	}

	hwndServer = hwndParent;
	return TRUE;
}

BOOL UninstallHook() {
	//Check if already unhooked
	if (hookHandle == NULL) {
		return TRUE;
	}
	//If unhook attempt fails, check whether it is because of invalid handle (in that case continue)
	if (!UnhookWindowsHookEx(hookHandle)) {
		DWORD error = GetLastError();
		if (error != ERROR_INVALID_HOOK_HANDLE) {
			return FALSE;
		}
	}

	hwndServer = NULL;
	hookHandle = NULL;

	return TRUE;
}
