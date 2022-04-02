// 081721.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "081721.h"

/*
* 윈도우는 메세지 기반의 운영체제이다.
* 특정이벤트(마우스 클릭이나 키보드 누름, 마우스 이동 등)가 발생하면 운영체제는 이름 메세지로
* 만들어준다. 
* 윈도우즈 프로그램은 메세지 큐를 가지고 있다.
* 무한루프로 메세지 루프가 돌아가며 운영체제가 메세지큐에 메세지를 넣어주면 해당 메세지를 받아와서
* 메시지에 맞는 작업을 처리해주게 된다.
* 윈도우 종료 메세지가 들어오면 무한루프도 종료가되며 프로그램이 종료가 되는 방식이다.
*/
#define MAX_LOADSTRING 100

// Global Variables:
// HINSTANCE는 실행되는 프로그램을 구분하기 위해 운영체제가 부여하는 고유한 ID값이다.
HINSTANCE hInst;                                // current instance

// WinAPI를 할때는 Unicode 문자집합을 이용하여 문자열을 사용한다.
// Multibyte 문자집합을 사용하던 기존 방식은 char배열을 이용하여 문자열을 표현하였다.
// 이는 알파벳같은 1바이트 문자를 저장할때 유용한 방법이지만 한국어, 일어, 중국어 등은
// 2개의 배열을 이용하여 표현해야 했다.
// wchar_t 라는 타입이 있다. 이 타입은 2바이트 변수이다. 2바이트 문자들도 변수 1개의 저장이 가능한 타입이다.
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// WinMain이 WinApi의 진인접 함수이다.
// 이 함수는 운영체제가 동작시켜주며 운영체제가 이 프로그램이 동작될때 고유한 ID를 만들어서
// 인자로 전달해주고 있다.
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY081721, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY081721));

    MSG msg;

    // Main message loop:
    /*
    * 윈도우의 메세지 루프입니다. GetMessage 함수를 이용해서 메세지 큐에 있는 메세지를 얻어 올 수 있다.
    * 만약 메세지큐에 아직 메세지가 없다면 메세지가 들어올때까지 이 함수에서 안 빠져나오고 대기상태로 들어가있게 된다.
    * 즉, 메세지가 생길때까지 다른 작업을 할 수 없다는 의미이다.
    * GetMessage 함수는 만약 윈도우 종료 메세지가 들어올 경우 while문을 빠져나갈 수 있게
    * FALSE가 나오게 된다.
    */
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            // WM_KEYDOWN 이라는 메세지가 있다. 이는 키보드 키를 눌렀을때 발생되는 메세지이다.
            // TranslateMessage 함수에서는 메세지를 전달해주면 WM_KEYDOWN 인지를 판단해주고
            // 눌려진 키가 문자 키인지를 판단해서 일반 문자 키라면 WM_CHAR라는 메세지를 만들어서 메시지
            // 큐에 추가해주게 된다.
            TranslateMessage(&msg);

            // DispatchMessage 함수는 메세지를 윈도우 프로시저로 보내준다.
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    // 작업영역이 바뀔때마다 다시 그리게 한다. 가로 세러 모두 다시 그린다.
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    // 메세지가 큐에 들어왔을때 해당 메세지를 꺼내오고 꺼내온 메세지를 인자로 전달해서
    // 호출해줄 함수를 지정한다.
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;

    // Window Instance를 지정한다.
    wcex.hInstance      = hInstance;

    // 실행파일이 만들어질때 사용할 아이콘 이미지를 지정한다.
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY081721));


    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY081721);
    // 유니코드 문자열을 사용할때 L""을 붙여서 사용을한다.
    // TEXT 매크로는 ""앞에 L을 붙여서 L""로 만들어준다.
    // 유니코드일때는 이렇게 해주고 multibyte 일때는 그냥 ""로 만들어준다.
    // wcex.lpszClassName  = szWindowClass;
    wcex.lpszClassName = TEXT("HELLO");
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   /*
   * CreateWindowW 함수를 이용해서 윈도우 창을 만들면 WindowHandle을 반환해준다.
   * WindowHandle은 각 윈도우마다 고유한 번호이다.
   */
   HWND hWnd = CreateWindowW(TEXT("HELLO"), TEXT("WORLD"), WS_OVERLAPPEDWINDOW,
      0, 100, 1280, 720, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   // SW_SHOW : 윈도우창을 화면에 보여준다.
   // SW_HIDE : 창을 만들긴 하지만 화면에 보여주는 것은 아니다.
   ShowWindow(hWnd, nCmdShow);

   // UpdateWindow 함수는 갱신할 부분이 있다면 갱신하라는 명령을 내려주는 함수이다.
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
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

            // HDC : 화면에 출력하기 위한 그리기 도구이다.
            HDC hdc = BeginPaint(hWnd, &ps);
            
            int height = GetSystemMetrics(SM_CYSCREEN);
            int width = GetSystemMetrics(SM_CXSCREEN);
            for (int y = 300; y < height; y += 10)
            {
                for (int x = 0; x < width; x += 10)
                {
                    if (x != 600)
                    {       
                        Rectangle(hdc, x, y, x + 10, y + 10);
                    }
                    else if(x == 600)
                    {
                        if (y >= 600)
                            Rectangle(hdc, x, y, x + 10, y + 10);
                    }
                }
            }

            for (int y = 100; y < 150; y += 10)
            {
                Rectangle(hdc, 600, y, 610, y + 10);
            }

            //Rectangle(hdc, 100, 100, 200, 200);
            //Ellipse(hdc, 200, 100, 300, 200);

            //// MoveToEx : 선의 시작점을 지정한다.
            //MoveToEx(hdc, 400, 100, nullptr);
            //// LineTo : 선의 끝점을 지정한다.
            //LineTo(hdc, 500, 200);
            //LineTo(hdc, 600, 200);

            //MoveToEx(hdc, 700, 100, nullptr);
            //LineTo(hdc, 800, 100);

            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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
