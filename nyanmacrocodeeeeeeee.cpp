#include <windows.h>

volatile bool spaceHeld = false;
volatile bool enabled = true;

HHOOK hHook;
HWND hStatus;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        auto* kbd = (KBDLLHOOKSTRUCT*)lParam;

        if (!(kbd->flags & LLKHF_INJECTED) && kbd->vkCode == VK_SPACE)
        {
            if (wParam == WM_KEYDOWN) spaceHeld = true;
            if (wParam == WM_KEYUP)   spaceHeld = false;
        }                                      
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

DWORD WINAPI SpamThread(LPVOID)
{
    while (true)
    {
        if (enabled && spaceHeld)
        {
            INPUT in[2] = {};
            in[0].type = INPUT_KEYBOARD;
            in[0].ki.wVk = VK_SPACE;
            in[1] = in[0];
            in[1].ki.dwFlags = KEYEVENTF_KEYUP;

            SendInput(2, in, sizeof(INPUT));
            Sleep(25); // note for me cuz im stupid and forget the counter thingy. Thats 40 cps 
        }
        else
        {
            Sleep(1);
        }
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == 1)
        {
            enabled = !enabled;
            SetWindowTextA(
                hStatus,
                enabled ? "Status: is oon" : "Status: is oof"
            );
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, nullptr, 0);
    if (!hHook) return 1;

    CreateThread(nullptr, 0, SpamThread, nullptr, 0, nullptr);

    WNDCLASSA wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = "SpaceSpammerUI";

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowA(
        wc.lpszClassName,
        "Space Spammer",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        500, 300, 260, 140,
        nullptr, nullptr, hInst, nullptr
    );

    hStatus = CreateWindowA(
        "STATIC",
        "Status: ENABLED",
        WS_CHILD | WS_VISIBLE,
        20, 20, 200, 20,
        hwnd, nullptr, hInst, nullptr
    );

    CreateWindowA(
        "BUTTON",
        "Toggle",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        20, 60, 200, 30,
        hwnd, (HMENU)1, hInst, nullptr
    );

    ShowWindow(hwnd, SW_SHOW);
       // msg here eeeeeee
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hHook);
}




dadadddaddadadadadadawda
