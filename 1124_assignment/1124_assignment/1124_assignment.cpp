// 1124_assignment.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "1124_assignment.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY1124ASSIGNMENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1124ASSIGNMENT));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1124ASSIGNMENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1124ASSIGNMENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


/// 펜 전환 플레그 생성
int g_pen_flag;

/// 펜 좌표 전역변수로 저장
int g_x, g_y;

/// 클릭 플레그 생성
BOOL g_free_flag;

LPARAM Lcoord;
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
/// 윈도우 메시징 시스템의 기본 구조
/// WndProc의 함수를 내가 호출한다! ==> WndProc도 함수!
/// 4개의 인수를 내가 설정해서 전달해야 한다!
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    /// 마우스 좌표 저장
    int x=0, y=0;

    /// 접근 방법 마우스를 움직일 때 입력을 받아야하니 HWND 그림판 생성
    HWND Hmsprint;
    Hmsprint = FindWindow(NULL, L"제목 없음 - 그림판");
    HWND Htest;
    Htest = FindWindowEx(Hmsprint, NULL, L"MSPaintView", NULL);
   

    switch (message)
    {
    case WM_CREATE:
    {
        g_pen_flag = 4;
        x = 0;
        y = 0;
    }
    break;
    case WM_LBUTTONDOWN:
    {
        
        if (g_pen_flag == 0) break;
        /// 마우스 클릭 할 때 플래그 활성화
        if (g_pen_flag == 4) { g_free_flag = true; }
        /// 펜 좌표 입력
        g_x = LOWORD(lParam);
        g_y = HIWORD(lParam);
 
        if (NULL == Hmsprint)
        {
            MessageBox(hWnd, L"못 찾았음", L"msprint.exe", MB_OK);
            break;
            if(NULL == Htest)
            {
                MessageBox(hWnd, L"Htest", L"못 찾았음", MB_OK);
                break;
              
            }
             
        }
      
        SendMessageW(Htest, WM_LBUTTONDOWN, wParam, lParam);
        
    }
    break;
    case WM_MOUSEMOVE:
    {
        HDC hdc = GetDC(hWnd);
        if (g_pen_flag == 0) break;
        if (g_pen_flag == 4 && g_free_flag)
        {
            x = LOWORD(lParam);
            y = HIWORD(lParam);

            MoveToEx(hdc, g_x, g_y, NULL);
            LineTo(hdc, x, y);

            g_x = x;
            g_y = y;
        }
        SendMessageW(Htest, WM_MOUSEMOVE, wParam, lParam);
        ReleaseDC(hWnd, hdc);
    }
        break;
    case WM_LBUTTONUP:
    {
        HDC hdc = GetDC(hWnd);
        if (g_pen_flag == 0) break;
        /// 클릭 해제 시 비활성화
        if (g_free_flag) { g_free_flag = false; }

        x = LOWORD(lParam);
        y = HIWORD(lParam);
        if (g_pen_flag == 1)
        {
            /// 직선 그리기
            MoveToEx(hdc, g_x, g_y, NULL);
            LineTo(hdc, x, y);
        }
        else if (g_pen_flag == 2)
        {
            /// 사각형 그리기
            Rectangle(hdc, g_x, g_y, x, y);
        }
        else if (g_pen_flag == 3)
        {
            /// 타원 그리기
            Ellipse(hdc, g_x, g_y, x, y);
        }
        SendMessageW(Htest, WM_LBUTTONUP, wParam, lParam);
        ReleaseDC(hWnd, hdc);
    }
    break;
    case WM_KEYDOWN:
    {
        /// HANDLE == HWND
        HWND notepad;
        notepad = FindWindow(NULL, L"제목 없음 - 메모장");
        if (NULL == notepad)
        {
            MessageBox(hWnd, L"메모장", L"못 찾았음", MB_OK);
            break;
        }
        //MessageBox(hWnd, L"메모장", L"찾았음~~~~~~~~~", MB_OK);
        ////////////////////////////// 메모장 프로세스의 윈도우 핸들 찾았다.
        HWND textbox;
        textbox = FindWindowEx(notepad, NULL, L"NotepadTextBox", NULL);
        if (NULL == textbox)
        {
            MessageBox(hWnd, L"TextBox", L"못 찾았음", MB_OK);
            break;
        }
        HWND edit;
        edit = FindWindowEx(textbox, NULL, L"RichEditD2DPT", NULL);
        if (NULL == edit)
        {
            MessageBox(hWnd, L"Edit", L"못 찾았음", MB_OK);
            break;
        }

        SendMessage(edit, WM_CHAR, wParam, lParam);
    }
    break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
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
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
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

// 정보 대화 상자의 메시지 처리기입니다.
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
