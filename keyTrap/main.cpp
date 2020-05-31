#include <stdio.h>
#include <windows.h>
#include <vector>
#include <iostream>
#include <time.h>

using namespace std;

//loop control
bool bVoiceActionToggle;
bool bEmoteLoop;
bool bPointLoop;
bool bChargeLoop;

//time between keypresses
#define SLEEP_DUR_MS 50
#define EMOTE_SLEEP_MS 5000
#define POINT_SLEEP_MS 150
#define CHARGE_SLEEP_MS 50

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
#define KK_X  0x58

//voices
//page 1
std::vector<WORD> vYes{ KK_C, KK_1 };
std::vector<WORD> vNo{ KK_C, KK_2 };
std::vector<WORD> vHelp{ KK_C, KK_3 };
std::vector<WORD> vInsult{ KK_C, KK_4 };
std::vector<WORD> vIntimidate{ KK_C, KK_5 };

//page 2
std::vector<WORD> vSorry{ KK_C, KK_C, KK_1 };
std::vector<WORD> vLaugh{ KK_C, KK_C, KK_2 };
std::vector<WORD> vThank{ KK_C, KK_C, KK_3 };
std::vector<WORD> vFriendlies{ KK_C, KK_C, KK_4 };
std::vector<WORD> vRetreat{ KK_C, KK_C, KK_5 };

//page 3
std::vector<WORD> vHold{ KK_C, KK_C, KK_C, KK_1 };
std::vector<WORD> vHello{ KK_C, KK_C, KK_C, KK_2 };
std::vector<WORD> vFollow{ KK_C, KK_C, KK_C, KK_3 };
std::vector<WORD> vRespect{ KK_C, KK_C, KK_C, KK_4 };
std::vector<WORD> vCharge{ KK_C, KK_C, KK_C, KK_5 };

//emotes
//page 1
std::vector<WORD> vFlourish{ KK_X, KK_1 };
std::vector<WORD> vRaiseWeapon{ KK_X, KK_2 };
std::vector<WORD> vSwordSalute{ KK_X, KK_3 };
std::vector<WORD> vComeAtMe{ KK_X, KK_4 };
std::vector<WORD> vThroatCut{ KK_X, KK_5 };
std::vector<WORD> vSalute{ KK_X, KK_6 };
std::vector<WORD> vFacepalm{ KK_X, KK_7 };
std::vector<WORD> vSquat{ KK_X, KK_8 };
std::vector<WORD> vYelling{ KK_X, KK_9 };

//page 2
std::vector<WORD> vCheer{ KK_X, KK_X, KK_1 };
std::vector<WORD> vDance{ KK_X, KK_X, KK_2 };
std::vector<WORD> vDisapprove{ KK_X, KK_X, KK_3 };
std::vector<WORD> vShakeFist{ KK_X, KK_X, KK_4 };
std::vector<WORD> vShrug{ KK_X, KK_X, KK_5 };
std::vector<WORD> vWhatever{ KK_X, KK_X, KK_6 };
std::vector<WORD> vRoar{ KK_X, KK_X, KK_7 };
std::vector<WORD> vYield{ KK_X, KK_X, KK_8 };
std::vector<WORD> vCower{ KK_X, KK_X, KK_9 };

//page 3
std::vector<WORD> vLaughing{ KK_X, KK_X, KK_X, KK_1 };
std::vector<WORD> vPointBack{ KK_X, KK_X, KK_X, KK_2 };
std::vector<WORD> vItsAllOver{ KK_X, KK_X, KK_X, KK_3 };
std::vector<WORD> vBow{ KK_X, KK_X, KK_X, KK_4 };
std::vector<WORD> vCharge2{ KK_X, KK_X, KK_X, KK_5 };

HHOOK hKeyboardHook;


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

		//down
		UINT ret = SendInput(1, &ip, sizeof(INPUT));

		//up
		ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
		ret = SendInput(1, &ip, sizeof(INPUT));

		//sleep
		Sleep(SLEEP_DUR_MS);
	}
}

void processKey(DWORD key)
{
	switch (key)
	{
	case VK_F1:  //toggle emotes and voices
		bVoiceActionToggle ? bVoiceActionToggle = false : bVoiceActionToggle = true;
		break;
	case VK_F2:
		bVoiceActionToggle ? ghostInput(vHelp) : ghostInput(vComeAtMe);
		break;
	case VK_F3:
		bVoiceActionToggle ? ghostInput(vThank) : ghostInput(vDance);
		break;
	case VK_F4:
		bVoiceActionToggle ? ghostInput(vRespect) : ghostInput(vRoar);
		break;
	case VK_F5:
		bVoiceActionToggle ? ghostInput(vHello) : ghostInput(vBow);
		break;
	case VK_F6:
		bVoiceActionToggle ? ghostInput(vInsult) : ghostInput(vYelling);
		break;
	case VK_F7:
		bVoiceActionToggle ? ghostInput(vIntimidate) : ghostInput(vCower);
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
	case VK_F10: //charge voice overlap
		bEmoteLoop = false;
		bPointLoop = false;
		bChargeLoop ? bChargeLoop = false : bChargeLoop = true;
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

void doEmoteInput()
{
	//randomly generate a number between 0 and 14
	srand(time(NULL));
	int iRandom = rand() % 14;

	switch(iRandom)
	{
		case 0:
			ghostInput(vYes);
			break;
		case 1:
			ghostInput(vNo);
			break;
		case 2:
			ghostInput(vHelp);
			break;
		case 3:
			ghostInput(vInsult);
			break;
		case 4:
			ghostInput(vIntimidate);		
			break;
		case 5:
			ghostInput(vSorry);
			break;
		case 6:
			ghostInput(vLaugh);
			break;
		case 7:
			ghostInput(vThank);
			break;
		case 8:
			ghostInput(vFriendlies);
			break;
		case 9:
			ghostInput(vRetreat);
			break;
		case 10:
			ghostInput(vHold); 
			break;
		case 11:
			ghostInput(vHello); 
			break;
		case 12:
			ghostInput(vFollow); 
			break;
		case 13:
			ghostInput(vRespect); 
			break;
		case 14:
			ghostInput(vCharge); 
			break;		
		default:
		break;
	}
}

DWORD WINAPI my_EmoteLoop(LPVOID lpParam)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	if (!hInstance) hInstance = LoadLibrary((LPCSTR)lpParam);
	if (!hInstance) return 1;

	while (true)
	{
		Sleep(EMOTE_SLEEP_MS);
		if (bEmoteLoop)
		{
			doEmoteInput();
		}
	}
}

DWORD WINAPI my_PointLoop(LPVOID lpParam)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	if (!hInstance) hInstance = LoadLibrary((LPCSTR)lpParam);
	if (!hInstance) return 1;

	while (true)
	{
		Sleep(POINT_SLEEP_MS);
		if (bPointLoop)
		{
			ghostInput(vThroatCut);
		}
	}
}

DWORD WINAPI my_ChargeLoop(LPVOID lpParam)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	if (!hInstance) hInstance = LoadLibrary((LPCSTR)lpParam);
	if (!hInstance) return 1;

	while (true)
	{
		Sleep(CHARGE_SLEEP_MS);
		if (bChargeLoop)
		{
			ghostInput(vCharge2);
		}
	}
}

int main(int argc, char* argv[])
{
	HANDLE hThread;
	HANDLE hEmoteLoop;
	HANDLE hPointThread;
	HANDLE hChargeThread;

	DWORD dwThread;
	DWORD dwEmoteThread;
	DWORD dwPointThread;
	DWORD dwChargeThread;

	bEmoteLoop = false;
	bPointLoop = false;
	bChargeLoop = false;
	bVoiceActionToggle = true; //start with voice

	cout << "Launching Mordhau Macro Program 2.0. Hit 'F12' to cancel. " << endl;
	cout << "F1 Toggle Voice | Action " << endl;
	cout << "F2 Help / Come At Me" << endl;
	cout << "F3 Thank You | Dance " << endl;
	cout << "F4 Respect | Roar" << endl;
	cout << "F5 Hello | Bow " << endl;
	cout << "F6 Insult | Yell " << endl;
	cout << "F7 Intimidate | Cower " << endl;
	cout << "F8 Toggle LoopRandom Emote " << endl;
	cout << "F9 Toggle Point Loop " << endl;
	cout << "F10 Toggle ChargeOverlap Loop " << endl;

	hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)my_HotKey, (LPVOID)argv[0], NULL, &dwThread);
	hEmoteLoop = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)my_EmoteLoop, (LPVOID)argv[0], NULL, &dwEmoteThread);
	hPointThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)my_PointLoop, (LPVOID)argv[0], NULL, &dwPointThread);
	hChargeThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)my_ChargeLoop, (LPVOID)argv[0], NULL, &dwChargeThread);

	//ShowWindow(FindWindowA("ConsoleWindowClass", NULL), false);
	if (hThread) return WaitForSingleObject(hThread, INFINITE);
	else return 1;
	
}