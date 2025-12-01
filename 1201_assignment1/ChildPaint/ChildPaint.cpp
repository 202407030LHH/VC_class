// ChildPaint.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "ChildPaint.h"

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
    LoadStringW(hInstance, IDC_CHILDPAINT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CHILDPAINT));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CHILDPAINT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CHILDPAINT);
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

/// 펜 굵기 생성
/// 근데 왜 이걸 지역변수로 선언하면 안되는거지?
/// A. 윈도우가 호출 될때마다 지역변수가 초기화되기 때문에 값이 변경되지 않는거처럼 보임.
int penStroke = 1;

/// 펜 색상 생성
COLORREF penColor = RGB(0, 0, 0);

/// 면 색상 생성
COLORREF solidColor = RGB(255, 255, 255);


/// 추가 과제
HWND g_childPaint;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hWnd);
    /// 마우스 좌표 저장
    int x, y;

    /// 펜 생성, os에 반환해야함
    HPEN myPen, osPen;

    /// 펜 생성
    myPen = CreatePen(PS_SOLID, penStroke, penColor);
    osPen = (HPEN)SelectObject(hdc, myPen);

    /// 브러쉬 생성, 딜리트 해줘야함.
    HBRUSH myBrush, osBrush;
    myBrush = CreateSolidBrush(solidColor);
    osBrush = (HBRUSH)SelectObject(hdc, myBrush);

    switch (message)
    {
    case WM_LBUTTONDOWN:
    {
        if (g_pen_flag == 0) break;
        /// 마우스 클릭 할 때 플래그 활성화
        if (g_pen_flag == 4) { g_free_flag = true; }
        /// 펜 좌표 입력
        g_x = LOWORD(lParam);
        g_y = HIWORD(lParam);
        break;
    }
    /// 자유형 버튼이 클릭 되었을 때
    case WM_MOUSEMOVE:
    {
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
        break;

    }
    case WM_LBUTTONUP:
    {
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
        /// 사용한 오브젝트 반환
        DeleteObject(myBrush);
        DeleteObject(osBrush);
        DeleteObject(myPen);
        DeleteObject(osPen);
        ReleaseDC(hWnd, hdc);

        break;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
            /// 선 선택 커맨드
        case ID_LINE:
        {
            g_pen_flag = 1;
            break;
        }
        case ID_RACT:
        {
            g_pen_flag = 2;
            break;
        }
        case ID_ELS:
        {
            g_pen_flag = 3;
            break;
        }
        case ID_FREE:
        {
            g_pen_flag = 4;
            break;
        }
        /// 선 스트로크 커맨드
        case ID_1PX:
            penStroke = 1;
            break;
        case ID_5PX:
            penStroke = 5;
            break;
        case ID_10PX:
            penStroke = 10;
            break;
            /// 선 색 커맨드
        case ID_ST_RED:
            penColor = RGB(255, 0, 0);
            break;
        case ID_ST_GREED:
            penColor = RGB(0, 255, 0);
            break;
        case ID_ST_BLUE:
            penColor = RGB(0, 0, 255);
            break;
            /// 면 색 커맨드
        case ID_BRUSH_BLACK:
            solidColor = RGB(0, 0, 0);
            break;
        case ID_BRUSH_WHITE:
            solidColor = RGB(255, 255, 255);
            break;
        case ID_BRUSH_PURPLE:
            solidColor = RGB(180, 85, 162);
            break;
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
