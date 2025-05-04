#include "utils.h"

#include <wtypes.h>
#include <stdio.h>



void print_h_result(HRESULT hr)
{
    LPWSTR lp_msg_buf = NULL;
    DWORD bufLen = 0;

    if (SUCCEEDED(hr))
    {
        wprintf(L"HRESULT: 0x%08lX - Success\n", (DWORD)hr);
        return;
    }

    bufLen = FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        (DWORD)hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&lp_msg_buf,
        0, NULL
    );

    if (bufLen == 0)
    {
        wprintf(L"HRESULT: 0x%08lX - Unknown error\n", (DWORD)hr);
    }
    else
    {
        wprintf(L"HRESULT: 0x%08lX - %s\n", (DWORD)hr, lp_msg_buf);
        LocalFree(lp_msg_buf);
    }
}

void wait_for_console_enter(void)
{
    while (true)
    {
        DWORD events = 0;
        MSG msg = { 0 };

        GetNumberOfConsoleInputEvents(GetStdHandle(STD_INPUT_HANDLE), &events);

        if (events > 0)
        {
            INPUT_RECORD ir = { 0 };
            DWORD events_read = 0;
            if (ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &ir, 1, &events_read) && events_read > 0)
            {
                if (ir.EventType == KEY_EVENT && ir.Event.KeyEvent.uChar.AsciiChar == '\r' && ir.Event.KeyEvent.bKeyDown)
                {
                    break;
                }
            }
        }

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}
