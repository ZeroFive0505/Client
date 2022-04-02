// 081721.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "081721.h"

/*
* ������� �޼��� ����� �ü���̴�.
* Ư���̺�Ʈ(���콺 Ŭ���̳� Ű���� ����, ���콺 �̵� ��)�� �߻��ϸ� �ü���� �̸� �޼�����
* ������ش�. 
* �������� ���α׷��� �޼��� ť�� ������ �ִ�.
* ���ѷ����� �޼��� ������ ���ư��� �ü���� �޼���ť�� �޼����� �־��ָ� �ش� �޼����� �޾ƿͼ�
* �޽����� �´� �۾��� ó�����ְ� �ȴ�.
* ������ ���� �޼����� ������ ���ѷ����� ���ᰡ�Ǹ� ���α׷��� ���ᰡ �Ǵ� ����̴�.
*/
#define MAX_LOADSTRING 100

// Global Variables:
// HINSTANCE�� ����Ǵ� ���α׷��� �����ϱ� ���� �ü���� �ο��ϴ� ������ ID���̴�.
HINSTANCE hInst;                                // current instance

// WinAPI�� �Ҷ��� Unicode ���������� �̿��Ͽ� ���ڿ��� ����Ѵ�.
// Multibyte ���������� ����ϴ� ���� ����� char�迭�� �̿��Ͽ� ���ڿ��� ǥ���Ͽ���.
// �̴� ���ĺ����� 1����Ʈ ���ڸ� �����Ҷ� ������ ��������� �ѱ���, �Ͼ�, �߱��� ����
// 2���� �迭�� �̿��Ͽ� ǥ���ؾ� �ߴ�.
// wchar_t ��� Ÿ���� �ִ�. �� Ÿ���� 2����Ʈ �����̴�. 2����Ʈ ���ڵ鵵 ���� 1���� ������ ������ Ÿ���̴�.
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// WinMain�� WinApi�� ������ �Լ��̴�.
// �� �Լ��� �ü���� ���۽����ָ� �ü���� �� ���α׷��� ���۵ɶ� ������ ID�� ����
// ���ڷ� �������ְ� �ִ�.
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
    * �������� �޼��� �����Դϴ�. GetMessage �Լ��� �̿��ؼ� �޼��� ť�� �ִ� �޼����� ��� �� �� �ִ�.
    * ���� �޼���ť�� ���� �޼����� ���ٸ� �޼����� ���ö����� �� �Լ����� �� ���������� �����·� ���ְ� �ȴ�.
    * ��, �޼����� ���涧���� �ٸ� �۾��� �� �� ���ٴ� �ǹ��̴�.
    * GetMessage �Լ��� ���� ������ ���� �޼����� ���� ��� while���� �������� �� �ְ�
    * FALSE�� ������ �ȴ�.
    */
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            // WM_KEYDOWN �̶�� �޼����� �ִ�. �̴� Ű���� Ű�� �������� �߻��Ǵ� �޼����̴�.
            // TranslateMessage �Լ������� �޼����� �������ָ� WM_KEYDOWN ������ �Ǵ����ְ�
            // ������ Ű�� ���� Ű������ �Ǵ��ؼ� �Ϲ� ���� Ű��� WM_CHAR��� �޼����� ���� �޽���
            // ť�� �߰����ְ� �ȴ�.
            TranslateMessage(&msg);

            // DispatchMessage �Լ��� �޼����� ������ ���ν����� �����ش�.
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

    // �۾������� �ٲ𶧸��� �ٽ� �׸��� �Ѵ�. ���� ���� ��� �ٽ� �׸���.
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    // �޼����� ť�� �������� �ش� �޼����� �������� ������ �޼����� ���ڷ� �����ؼ�
    // ȣ������ �Լ��� �����Ѵ�.
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;

    // Window Instance�� �����Ѵ�.
    wcex.hInstance      = hInstance;

    // ���������� ��������� ����� ������ �̹����� �����Ѵ�.
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY081721));


    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY081721);
    // �����ڵ� ���ڿ��� ����Ҷ� L""�� �ٿ��� ������Ѵ�.
    // TEXT ��ũ�δ� ""�տ� L�� �ٿ��� L""�� ������ش�.
    // �����ڵ��϶��� �̷��� ���ְ� multibyte �϶��� �׳� ""�� ������ش�.
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
   * CreateWindowW �Լ��� �̿��ؼ� ������ â�� ����� WindowHandle�� ��ȯ���ش�.
   * WindowHandle�� �� �����츶�� ������ ��ȣ�̴�.
   */
   HWND hWnd = CreateWindowW(TEXT("HELLO"), TEXT("WORLD"), WS_OVERLAPPEDWINDOW,
      0, 100, 1280, 720, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   // SW_SHOW : ������â�� ȭ�鿡 �����ش�.
   // SW_HIDE : â�� ����� ������ ȭ�鿡 �����ִ� ���� �ƴϴ�.
   ShowWindow(hWnd, nCmdShow);

   // UpdateWindow �Լ��� ������ �κ��� �ִٸ� �����϶�� ����� �����ִ� �Լ��̴�.
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

            // HDC : ȭ�鿡 ����ϱ� ���� �׸��� �����̴�.
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

            //// MoveToEx : ���� �������� �����Ѵ�.
            //MoveToEx(hdc, 400, 100, nullptr);
            //// LineTo : ���� ������ �����Ѵ�.
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
