///////////////////////////////////////////////////////////
//	Test.cpp
//
#define  STRICT
#define  WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

#include <Windows.h>

#define SCLASS "a;eohgqeruiopugoqeig"
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

    LPCTSTR szClass = SCLASS;

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = szClass;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    ::RegisterClass(&wc);

    int screen_width = GetSystemMetrics(SM_CXSCREEN); //Получение ширины
    int screen_heigth = GetSystemMetrics(SM_CYSCREEN); //Получение высоты

    // Create Main window
    HWND hWnd = ::CreateWindow(szClass, "Test", WS_OVERLAPPEDWINDOW,
        (int)((screen_width - (screen_width * WINDOW_WIDTH)) / 2),
        (int)((screen_heigth - (screen_heigth * WINDOW_HEIGTH)) / 2),
        (int)(screen_width * WINDOW_WIDTH),
        (int)(screen_heigth * WINDOW_HEIGTH),
        NULL, NULL, hInstance, NULL);
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
    case WM_CREATE:
        SetWindowLong(hWnd, GWLP_USERDATA, -1);
        return 0;
    case WM_SIZE:
        InvalidateRect(hWnd, NULL, true);
        return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HPEN hPen = ::CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        HGDIOBJ oPen = ::SelectObject(hdc, hPen);
        HBRUSH hBrush = ::CreateSolidBrush(RGB(255, 230, 0));

        POINT point;
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);

        point.x = clientRect.right;
        point.y = clientRect.bottom;
        
        //Отрисовка сетки
        if (GRID == 4) { // 1 вертикальная, 1 горизонтальная линия
            MoveToEx(hdc, point.x / 2, 0, NULL);
            LineTo(hdc, point.x / 2, point.y);

            MoveToEx(hdc, 0, point.y / 2, NULL);
            LineTo(hdc, point.x, point.y / 2);
        }
        else if (GRID == 6) { // 2 вертикальных, 1 горизонтальная линия
            for (int i = 1; i < GRID / 2; i++)
            {
                MoveToEx(hdc, point.x * i / 3, 0, NULL);
                LineTo(hdc, point.x * i / 3, point.y);
            }
            MoveToEx(hdc, 0, point.y / 2, NULL);
            LineTo(hdc, point.x, point.y / 2);
        }
        else if (GRID == 9) { // 2 вертикальных, 2 горизонтальных линий
            for (int i = 1; i < 3; i++)
            {
                MoveToEx(hdc, point.x * i / 3, 0, NULL);
                LineTo(hdc, point.x * i / 3, point.y);

                MoveToEx(hdc, 0, point.y * i / 3, NULL);
                LineTo(hdc, point.x, point.y * i / 3);
            }
        }
        //Вычисление координат прямоугольника
        RECT rectCoordinate;
        int i = GetWindowLong(hWnd, GWLP_USERDATA);
        int x, y;
        if (GRID == 4)
        {
            x = i % 2 * point.x / 2;
            y = i / 2 * point.y / 2;
            rectCoordinate = { x, y, x + point.x / 2, y + point.y / 2 };
        }
        else if (GRID == 6)
        {
            x = i % 3 * point.x / 3;
            y = i / 3 * point.y / 2;
            rectCoordinate = { x, y, x + point.x / 3, y + point.y / 2 };
        }
        else if (GRID == 9)
        {
            x = i % 3 * point.x / 3;
            y = i / 3 * point.y / 3;
            rectCoordinate = { x, y, x + point.x / 3, y + point.y / 3 };
        }
        FillRect(hdc, &rectCoordinate, hBrush);

        DeleteObject(hPen);
        DeleteObject(hBrush);
        EndPaint(hWnd, &ps);
    }
        return 0;
    case WM_MOUSEMOVE:
        POINT point;
        POINT mouse;
        RECT clientRect;
        RECT rectCoordinate;
        GetClientRect(hWnd, &clientRect);
        int x, y;
        mouse.x = LOWORD(lParam);
        mouse.y = HIWORD(lParam);

        if (GRID == 4)
        {
            point.x = clientRect.right / 2;
            point.y = clientRect.bottom / 2;
        }
        else if (GRID == 6)
        {
            point.x = clientRect.right / 3;
            point.y = clientRect.bottom / 2;
        }
        else if (GRID == 9)
        {
            point.x = clientRect.right / 3;
            point.y = clientRect.bottom / 3;
        }

        for (int i = 0; i < GRID; i++)
        {
            if (GRID == 4)
            {
                x = i % 2 * point.x;
                y = i / 2 * point.y;
            }
            else if (GRID == 6)
            {
                x = i % 3 * point.x;
                y = i / 3 * point.y;
            }
            else if (GRID == 9)
            {
                x = i % 3 * point.x;
                y = i / 3 * point.y;
            }

            rectCoordinate = { x, y, x + point.x, y + point.y };

            if (PtInRect(&rectCoordinate, mouse) && (GetWindowLong(hWnd, GWLP_USERDATA) != i))
            {
                SetWindowLong(hWnd, GWLP_USERDATA, i);
                InvalidateRect(hWnd, NULL, true);
            }
        }
        return 0;
    case WM_NCMOUSEMOVE:
        SetWindowLong(hWnd, GWLP_USERDATA, -1);
        InvalidateRect(hWnd, NULL, true);
        return 0;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }

    return ::DefWindowProc(hWnd, message, wParam, lParam);
}
//=========================================================