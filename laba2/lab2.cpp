///////////////////////////////////////////////////////////
//	Test.cpp
//
#define  STRICT
#define  WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

#include <Windows.h>

#define GRID 4 //Сетка
#define WINDOW_HEIGTH 0.5 //Высота
#define WINDOW_WIDTH 0.5 //Ширина

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI  WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE, // hPrevInstance,
    _In_ LPSTR, // lpCmdLine,
    _In_ int nCmdShow
)
{

    LPCTSTR szClass = TEXT("a;eohgqeruiopugoqeig");

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = szClass;

    ::RegisterClass(&wc);

    int screen_width = GetSystemMetrics(SM_CXSCREEN); //Получение ширины
    int screen_heigth = GetSystemMetrics(SM_CYSCREEN); //Получение высоты

    // Create Main window
    HWND hWnd = ::CreateWindow(szClass, "Test", WS_OVERLAPPEDWINDOW,
        (screen_width - (screen_width * WINDOW_WIDTH)) / 2,
        (screen_heigth - (screen_heigth * WINDOW_HEIGTH)) / 2,
        screen_width * WINDOW_WIDTH, screen_heigth * WINDOW_HEIGTH, NULL, NULL, hInstance, NULL);
    if (hWnd == NULL) {
        return -1;
    }

    ::ShowWindow(hWnd, nCmdShow);
    // ::UpdateWindow(hWnd);

    MSG msg;
    while (::GetMessage(&msg, NULL, 0, 0)) { // idle
        ::DispatchMessage(&msg);
    }

    return 0;
}
//=========================================================

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_SIZE:
        InvalidateRect(hWnd, NULL, TRUE);
        return 0;
    case WM_PAINT:
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        return 0;

    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }

    return ::DefWindowProc(hWnd, message, wParam, lParam);
}
//=========================================================