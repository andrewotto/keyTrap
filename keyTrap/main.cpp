#include <stdio.h>
#include <windows.h>
#include <vector>
#include <iostream>
#include <time.h>

#include "sequences.h"

using namespace std;

//loop control
bool bVoiceToggle;
bool bEmoteLoop;
bool bPointLoop;
bool bSpazLoop;

//time between keypresses
#define SLEEP_DUR_MS 50
//time between random emotes
#define EMOTE_SLEEP_MS 5000
//time between point actions
#define POINT_SLEEP_MS 150
//time between mouse circular loops
#define SPAZ_SLEEP_MS 25

//MOVE ME
#define RADIUS 80000

HHOOK hKeyboardHook;

void ghostMouse(LONG dx, LONG dy)
{
	INPUT ip;
	ip.type = INPUT_MOUSE;
	ip.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
	ip.mi.dx = dx;
	ip.mi.dy = dy;
	ip.mi.mouseData = 0;
	ip.mi.dwExtraInfo = 0;
	ip.mi.time = 0;
	UINT ret = SendInput(1, &ip, sizeof(INPUT));
}

void ghostKeySequence(std::vector<WORD> v)
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

		//down
		UINT ret = SendInput(1, &ip, sizeof(INPUT));

		//up
		ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
		ret = SendInput(1, &ip, sizeof(INPUT));

		//sleep
		Sleep(SLEEP_DUR_MS);
	}
}


void randomEmote()
{
	//randomly generate a number between 0 and 14
	srand(time(NULL));
	int iRandom = rand() % 14;

	switch (iRandom)
	{
	case 0:
		ghostKeySequence(vYes);
		break;
	case 1:
		ghostKeySequence(vNo);
		break;
	case 2:
		ghostKeySequence(vHelp);
		break;
	case 3:
		ghostKeySequence(vInsult);
		break;
	case 4:
		ghostKeySequence(vIntimidate);
		break;
	case 5:
		ghostKeySequence(vSorry);
		break;
	case 6:
		ghostKeySequence(vLaugh);
		break;
	case 7:
		ghostKeySequence(vThank);
		break;
	case 8:
		ghostKeySequence(vFriendlies);
		break;
	case 9:
		ghostKeySequence(vRetreat);
		break;
	case 10:
		ghostKeySequence(vHold);
		break;
	case 11:
		ghostKeySequence(vHello);
		break;
	case 12:
		ghostKeySequence(vFollow);
		break;
	case 13:
		ghostKeySequence(vRespect);
		break;
	case 14:
		ghostKeySequence(vCharge);
		break;
	default:
		break;
	}
}

void processKey(DWORD key)
{
	switch (key)
	{
	case VK_F1:  //toggle emotes and voices
		if (bVoiceToggle) {
			bVoiceToggle = false;
			ghostKeySequence(vYes);
		}
		else
		{
			bVoiceToggle = true;
			ghostKeySequence(vNo);
		}
		break;
	case VK_F2:
		bVoiceToggle ? ghostKeySequence(vHelp) : ghostKeySequence(vThumbsUp);
		break;
	case VK_F3:
		bVoiceToggle ? ghostKeySequence(vThank) : ghostKeySequence(vThumbsDown);
		break;
	case VK_F4:
		bVoiceToggle ? ghostKeySequence(vNo) : ghostKeySequence(vRoar);
		break;
	case VK_F5:
		bVoiceToggle ? ghostKeySequence(vHello) : ghostKeySequence(vBow);
		break;
	case VK_F6:
		bVoiceToggle ? ghostKeySequence(vInsult) : ghostKeySequence(vYelling);
		break;
	case VK_F7:
		bVoiceToggle ? ghostKeySequence(vIntimidate) : ghostKeySequence(vCower);
		break;
	case VK_F8:   //always loop
		if (bEmoteLoop)
			cout << "F8 Detected. Disabling Random Emote Loop " << endl;
		else
			cout << "F8 Detected. Enabling Random Emote Loop" << endl;

		bEmoteLoop ? bEmoteLoop = false : bEmoteLoop = true;
		break;
	case VK_F9: //spam throat cut
		bEmoteLoop = false;
		bPointLoop ? bPointLoop = false : bPointLoop = true;
		break;
	case VK_F10:
		//capture point TBD cycle in the loop not here
		//bEmoteLoop = false;
		bPointLoop = false;
		bSpazLoop ? bSpazLoop = false : bSpazLoop = true;
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
				cout << "Terminating Intercept ( you pressed escape ) " << endl;
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

DWORD WINAPI hotKeyLoop(LPVOID lpParm)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	if (!hInstance) hInstance = LoadLibrary((LPCSTR)lpParm);
	if (!hInstance) return 1;

	hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardEvent, hInstance, NULL);
	MessageLoop();
	UnhookWindowsHookEx(hKeyboardHook);
	return 0;
}

DWORD WINAPI emoteLoop(LPVOID lpParam)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	if (!hInstance) hInstance = LoadLibrary((LPCSTR)lpParam);
	if (!hInstance) return 1;

	while (true)
	{
		Sleep(EMOTE_SLEEP_MS);
		if (bEmoteLoop)
		{
			randomEmote();
		}
	}
}

DWORD WINAPI pointLoop(LPVOID lpParam)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	if (!hInstance) hInstance = LoadLibrary((LPCSTR)lpParam);
	if (!hInstance) return 1;

	while (true)
	{
		Sleep(POINT_SLEEP_MS);
		if (bPointLoop)
		{
			ghostKeySequence(vThroatCut);
		}
	}
}

DWORD WINAPI spazLoop(LPVOID lpParam)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	POINT centerScreen;

	centerScreen.x = GetSystemMetrics(SM_CXSCREEN) / 2;
	centerScreen.y = GetSystemMetrics(SM_CYSCREEN) / 2;
	
	if (!hInstance) hInstance = LoadLibrary((LPCSTR)lpParam);
	if (!hInstance) return 1;

	while (true)
	{
		Sleep(SPAZ_SLEEP_MS);

		if ( bSpazLoop )
		{ 
			ghostKeySequence(vComeAtMe);			

				for (double d = 0; d < 360.0; d+=.25)
				{
					if (!bSpazLoop) break;

					LONG dx = (centerScreen.x * (65536 / GetSystemMetrics(SM_CXSCREEN))) + (RADIUS * cos(d));
					LONG dy = (centerScreen.y * (65536 / GetSystemMetrics(SM_CYSCREEN))) + (RADIUS * sin(d));

					//cout << "cur.x= " << point.x << " cur.y= " << point.y << std::endl;
					//ccc3cout << "dx= " << dx << " dy= " << dy << " deg= " << d << std::endl;

					ghostMouse(dx, dy);
					Sleep(1);					
				}			
		}
	}
}

int main(int argc, char* argv[])
{
	HANDLE hThread, hEmoteLoop, hPointThread, hSpazThread;
	DWORD dwThread, dwEmoteThread, dwPointThread, dwSpazThread;

	bEmoteLoop = false;
	bPointLoop = false;
	bSpazLoop = false;
	bVoiceToggle = true; //start with voice

	cout << "Launching Mordhau Macro Program 2.0." << endl << "F1 Toggle Voice | Action " << endl << "F2 Help | Come At Me" << endl
		<< "F3 Thank You | Dance " << endl << "F4 No | Roar" << endl << "F5 Hello | Bow " << endl << "F6 Insult | Yell " << endl << "F7 Intimidate | Cower "
		<< endl << "F8 Toggle LoopRandom Emote " << endl << "F9 Toggle Point Loop " << endl << "F10 Toggle Spaz" << endl << "F12 Exit";

	hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)hotKeyLoop, (LPVOID)argv[0], NULL, &dwThread);
	hEmoteLoop = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)emoteLoop, (LPVOID)argv[0], NULL, &dwEmoteThread);
	hPointThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)pointLoop, (LPVOID)argv[0], NULL, &dwPointThread);
	hSpazThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)spazLoop, (LPVOID)argv[0], NULL, &dwSpazThread);
	
	//wait on main thread
	if (hThread) return WaitForSingleObject(hThread, INFINITE);
	else return 1;
	
}