#include <stdio.h>
#include <windows.h>
#include <vector>
#include <iostream>

//time between keypresses
#define SLEEP_DUR_MS 100
#define LAUGH_SLEEP_MS 3000

//im lazy
#define KK_1  0x31
#define KK_2  0x32
#define KK_3  0x33
#define KK_4  0x34
#define KK_5  0x35
#define KK_6  0x36
#define KK_7  0x37
#define KK_8  0x38
#define KK_9  0x38
#define KK_C  0x43

std::vector<WORD> vLaugh{ KK_C, KK_C, KK_2 };
std::vector<WORD> vHelp{ KK_C, KK_3 };
std::vector<WORD> vThank{ KK_C, KK_C, KK_3 };
std::vector<WORD> vRespect{ KK_C, KK_C, KK_C, KK_4 };
std::vector<WORD> vHello{ KK_C, KK_C, KK_C, KK_2 };
std::vector<WORD> vInsult{ KK_C, KK_4 };
std::vector<WORD> vIntimidate{ KK_C, KK_5 };

HHOOK hKeyboardHook;

bool m_bLaughLoop;

void ghostInput(std::vector<WORD> v)
{
	for (std::vector<WORD>::iterator itr = v.begin(); itr != v.end(); ++itr)
	{
		INPUT ip;
		ip.type = INPUT_KEYBOARD;
		ip.ki.wScan = MapVirtualKey(*itr, MAPVK_VK_TO_VSC);
		ip.ki.time = 0;
		ip.ki.dwExtraInfo = 0;
		ip.ki.wVk = *itr;
		ip.ki.dwFlags = KEYEVENTF_SCANCODE;


		//std::cout << "sending " << *itr << std::endl;

		//down
		UINT ret = SendInput(1, &ip, sizeof(INPUT));
//		std::cout << "keydown return " << ret << std::endl;

		//up
		ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
		ret = SendInput(1, &ip, sizeof(INPUT));
//		std::cout << "keyup return " << ret << std::endl;

		//sleep
		Sleep(SLEEP_DUR_MS);
	}
}

void processKey(DWORD key)
{
	switch (key)
	{
	case VK_F1:
		m_bLaughLoop ? printf("F1 Detected. Enabling Laugh Loop \n") : printf("F1 Detected. Disabling Laugh Loop \n");
		m_bLaughLoop ? m_bLaughLoop = false : m_bLaughLoop = true;		
		break;
	case VK_F2:
		printf("F2 Detected. Sending Help \n");
		ghostInput(vHelp);
		break;
	case VK_F3:
		printf("F3 Detected. Sending Thank You\n");
		ghostInput(vThank);
		break;
	case VK_F4:
		printf("F4 Detected. Sending Respect\n");		
		ghostInput(vRespect);
		break;
	case VK_F5:
		printf("F5 Detected. Sending Hello  \n");
		ghostInput(vHello);
		break;
	case VK_F6:
		printf("F6 Detected. Sending Insult\n");
		ghostInput(vInsult);
		break;
	case VK_F7:
		printf("F7 Detected. Sending Intimidate\n");
		ghostInput(vIntimidate);
		break;
	case VK_F8:
		printf("F8 Detected. Sending nothing \n");
		break;
	default:
		break;
	}
}

__declspec(dllexport) LRESULT CALLBACK KeyboardEvent(int nCode, WPARAM wParam, LPARAM lParam)
{
	DWORD SHIFT_key = 0;
	DWORD CTRL_key = 0;
	DWORD ALT_key = 0;

	if (( nCode == HC_ACTION ) && 
		(( wParam == WM_SYSKEYDOWN) ||
		  (wParam == WM_KEYDOWN)))
	{
		KBDLLHOOKSTRUCT hooked_key = *((KBDLLHOOKSTRUCT*)lParam);
		DWORD dwMsg = 1;
		dwMsg += hooked_key.scanCode << 16;
		dwMsg += hooked_key.flags << 24;
		char lpszKeyName[1024] = { 0 };
		lpszKeyName[0] = '[';

		int i = GetKeyNameText(dwMsg, (lpszKeyName + 1), 0xFF) + 1;
		lpszKeyName[i] = ']';

		int key = hooked_key.vkCode;

		SHIFT_key = GetAsyncKeyState(VK_SHIFT);
		CTRL_key = GetAsyncKeyState(VK_CONTROL);
		ALT_key = GetAsyncKeyState(VK_MENU);

		if (key >= 'A' && key <= 'Z')
		{
			if (GetAsyncKeyState(VK_SHIFT) >= 0) key += 32;

			//printf("key = %c\n", key);
			SHIFT_key = 0;
			CTRL_key = 0;
			ALT_key = 0;
		}
		else
		{
			if (key == VK_F12)
			{
				printf("Terminating Intercept ( you pressed escape ) \n");
				PostQuitMessage(0);
			}

			//look for keys to intercept
			processKey(key);
		}
		//printf("lpszKeyName = %s\n", lpszKeyName);
	}
	return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

void MessageLoop()
{
	MSG message;
	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

DWORD WINAPI my_HotKey(LPVOID lpParm)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	if (!hInstance) hInstance = LoadLibrary((LPCSTR)lpParm);
	if (!hInstance) return 1;

	hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardEvent, hInstance, NULL);
	MessageLoop();
	UnhookWindowsHookEx(hKeyboardHook);
	return 0;
}

DWORD WINAPI my_LaughLoop(LPVOID lpParam)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	if (!hInstance) hInstance = LoadLibrary((LPCSTR)lpParam);
	if (!hInstance) return 1;

	while (true)
	{
		Sleep(LAUGH_SLEEP_MS);
		if (m_bLaughLoop)
		{
			ghostInput(vLaugh);
		}
	}


}

int main(int argc, char* argv[])
{
	HANDLE hThread;
	HANDLE hLaughLoop;

	DWORD dwThread;
	DWORD dwLaughThread;

	m_bLaughLoop = false;

	printf("Launching Mordhau Macro Program. Hit 'F12' to cancel. \n");
	printf("F1 Laugh Loop \n");
	printf("F2 Help \n");
	printf("F3 Thank You\n");
	printf("F4 Respect\n");
	printf("F5 Hello\n");
	printf("F6 Insult\n");
	printf("F7 Intimidate\n");

	hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)my_HotKey, (LPVOID)argv[0], NULL, &dwThread);
	hLaughLoop = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)my_LaughLoop, (LPVOID)argv[0], NULL, &dwLaughThread);

	//ShowWindow(FindWindowA("ConsoleWindowClass", NULL), false);
	if (hThread) return WaitForSingleObject(hThread, INFINITE);
	else return 1;
	
}