


#include "framework.h"
#include "Window_API_1.h"

#define MAX_LOADSTRING 100

typedef struct _rect {
    float left, top, right, bottom;
}RECTANGLE, *PRECTANGLE;





HINSTANCE hInst;                               
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            
HWND g_hWnd;
HDC g_hDC;
bool g_bLoop = true;
RECTANGLE g_tPlayerRC = { 100, 100, 200, 200 };


LARGE_INTEGER g_tSecond;
LARGE_INTEGER g_tTime;
float g_fDeltaTime;


ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void Run();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);




    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWAPI1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

  
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

   
    g_hDC = GetDC(g_hWnd);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWAPI1));

    MSG msg = {};

    QueryPerformanceFrequency(&g_tSecond);
    QueryPerformanceCounter(&g_tTime);

    
    while (g_bLoop)
    {
       


        if (PeekMessage(&msg,nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        else
        {
            Run();
         
        }
    }

    ReleaseDC(g_hWnd, g_hDC);

    return (int) msg.wParam;
}








ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWAPI1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_WINDOWAPI1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}












BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;

  

   RECT rc = { 0, 0, 800, 600 };
   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);



   SetWindowPos(hWnd, HWND_TOPMOST, 100, 100, rc.right - rc.left,
       rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}












LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
           
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            EndPaint(hWnd, &ps);
        }
        break;
        
    case WM_DESTROY:
        g_bLoop = false;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void Run()
{

    // DeltaTime
    LARGE_INTEGER tTime;
    QueryPerformanceCounter(&tTime);

    g_fDeltaTime = (tTime.QuadPart - g_tTime.QuadPart) / 
        (float)g_tSecond.QuadPart;

    g_tTime = tTime;

    // 500px/s
    float fSpeed = 500 * g_fDeltaTime;

    if (GetAsyncKeyState('D') & 0x8000)
    {
        g_tPlayerRC.left += fSpeed;
        g_tPlayerRC.right += fSpeed;
    }

    if (GetAsyncKeyState('A') & 0x8000)
    {
        g_tPlayerRC.left -= fSpeed;
        g_tPlayerRC.right -= fSpeed;
    }

    if (GetAsyncKeyState('S') & 0x8000)
    {
        g_tPlayerRC.top += fSpeed;
        g_tPlayerRC.bottom += fSpeed;
    }

    if (GetAsyncKeyState('W') & 0x8000)
    {
        g_tPlayerRC.top -= fSpeed;
        g_tPlayerRC.bottom -= fSpeed;
    }

    RECT rcWindow;
    GetClientRect(g_hWnd, &rcWindow);


    if (g_tPlayerRC.left < 0)
    {
        g_tPlayerRC.left = 0;
        g_tPlayerRC.right = 100;
    }

    else if (g_tPlayerRC.right > rcWindow.right)
    {
        g_tPlayerRC.right = (float)rcWindow.right;
        g_tPlayerRC.left = (float)rcWindow.right - 100;
    }

    if (g_tPlayerRC.top < 0.0)
    {
        g_tPlayerRC.top = 0.0;
        g_tPlayerRC.bottom = 100.0;
    }

    else if (g_tPlayerRC.bottom > (float)rcWindow.bottom)
    {
        g_tPlayerRC.bottom = (float)rcWindow.bottom;
        g_tPlayerRC.top = (float)rcWindow.bottom - 100;
    }

    Rectangle(g_hDC, (int)g_tPlayerRC.left, (int)g_tPlayerRC.top, (int)g_tPlayerRC.right,
        (int)g_tPlayerRC.bottom);
}