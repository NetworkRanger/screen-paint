#include <windows.h>
  
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevinstace, PSTR szCmdline,
        int iCmdShow) {
    WNDCLASS wc;
    static TCHAR *szAppName = TEXT("iphpjs");
    HWND hwnd = NULL;
    MSG msg;
  
    int nScreenWidth, nScreenHeight;
    nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
  
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, IDI_WINLOGO);
    wc.hCursor = LoadCursor(NULL, IDC_CROSS);
    wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszClassName = szAppName;
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, TEXT("该程序只能在windows NT下运行！"), szAppName, MB_ICONERROR);
        return 0;
    }
    hwnd = CreateWindowEx(WS_EX_LAYERED, szAppName,
            TEXT("屏幕画笔-iphpjs.com"),
            WS_POPUP, 0, 0, nScreenWidth, nScreenHeight,
            NULL,
            NULL, hInstance,
            NULL);
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);
  
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
  
    return msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam,
        LPARAM lParam) {
    int nScreenWidth, nScreenHeight;
    nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    long xPos, yPos;
    HDC hdc;
    static POINT pts[5000];
    static int i = 0;
    static int leftDown = 0;
    static HPEN hPenRed;
    static HPEN hPenGreen;
    static HPEN hPenBlue;
    static HPEN hPenCur;
    switch (message) {
    case WM_CREATE:
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        hPenRed = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
        hPenGreen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
        hPenBlue = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
        hPenCur = hPenRed;
        return 0;
    case WM_KEYDOWN:
        switch (wParam) {
        case 'R':
            hPenCur = hPenRed;
            break;
        case 'G':
            hPenCur = hPenGreen;
            break;
        case 'B':
            hPenCur = hPenBlue;
            break;
        case VK_ESCAPE:
            PostQuitMessage(0);
        }
        return 0;
    case WM_MOUSEMOVE:
        if (0 == leftDown) return 0;
        xPos = LOWORD(lParam);
        yPos = HIWORD(lParam);
        hdc = GetDC(NULL);
        pts.x = xPos;
        pts[i++].y = yPos;
        InvalidateRect(hwnd, NULL, FALSE);
        ReleaseDC(NULL, hdc);
        return 0;
    case WM_LBUTTONDOWN:
        leftDown = 1;
        i = 0;
        return 0;
    case WM_LBUTTONUP:
        leftDown = 0;
        return 0;
    case WM_PAINT:
        hdc = GetDC(NULL);
        SelectObject(hdc, hPenCur);
        Polyline(hdc, pts, i);
        ReleaseDC(NULL, hdc);
        ValidateRect(hwnd, NULL);
        return 0;
    case WM_DESTROY:
        DeleteObject(hPenRed);
        DeleteObject(hPenGreen);
        DeleteObject(hPenBlue);
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}