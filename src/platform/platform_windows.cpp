// #########################################################
// Platform Implementation
// #########################################################
LRESULT CALLBACK windows_window_callback(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    switch (msg)
    {
        case WM_CLOSE:
        {
            running = false;
            break;
        }

        default:
        {
            result = DefWindowProcA(window, msg, wParam, lParam);
        }
    }
    return result;
}

bool platform_create_window(int width, int height, char* title)
{
    HINSTANCE instance = GetModuleHandleA(0);
    WNDCLASSA wc = {};
    wc.hInstance = instance;
    wc.hIcon = LoadIcon(instance, IDI_APPLICATION);
    wc.hCursor = LoadCursor(instance, IDC_ARROW);
    wc.lpszClassName = title;
    wc.lpfnWndProc = windows_message_callback;

    if (!RegisterClassA(&wc))
    {
        return false;
    }

    window = CreateWindowExA( 0, title, 
                                title, 
                                dwStyle, 
                                100, 
                                100, 
                                width, 
                                height, 
                                NULL, 
                                NULL, 
                                instance, 
                                NULL);
    if (!window)
    {
        return false;
    }
    ShowWindow(window, SW_SHOW);
    return true;
}

void platform_update_window()
{
    MSG msg;
    while(PeekMessageA(&msg, window, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}