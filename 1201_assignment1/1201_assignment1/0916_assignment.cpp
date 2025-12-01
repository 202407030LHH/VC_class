// 0916_assignment.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0916_assignment.h"

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
	LoadStringW(hInstance, IDC_MY0916ASSIGNMENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0916ASSIGNMENT));

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

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0916ASSIGNMENT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY0916ASSIGNMENT);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

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
    // 그리기 도구 변수들
    HDC hdc;
    int x, y;
    HPEN myPen, osPen;
    HBRUSH myBrush, osBrush;

    // 1. [수정] 핸들 찾기: 아직 못 찾았을 때만 찾도록 변경
    if (g_childPaint == NULL)
    {
        g_childPaint = FindWindow(NULL, L"ChildPaint");
    }

    // 2. [수정] 메시지 복사: 그리기 관련 메시지만 ChildPaint로 전송
    if (g_childPaint != NULL)
    {
        if (message == WM_LBUTTONDOWN || message == WM_LBUTTONUP || message == WM_MOUSEMOVE)
        {
            PostMessage(g_childPaint, message, wParam, lParam);
        }
    }

    // 3. [수정] 메인 로직: 하나의 switch 문으로 통합
    switch (message)
    {
    case WM_LBUTTONDOWN:
        if (g_pen_flag == 0) break;

        if (g_pen_flag == 4) { g_free_flag = true; }

        g_x = LOWORD(lParam);
        g_y = HIWORD(lParam);
        break;

    case WM_MOUSEMOVE:
        if (g_pen_flag == 0) break;

        if (g_pen_flag == 4 && g_free_flag)
        {
            x = LOWORD(lParam);
            y = HIWORD(lParam);

            hdc = GetDC(hWnd);
            myPen = CreatePen(PS_SOLID, penStroke, penColor);
            osPen = (HPEN)SelectObject(hdc, myPen);

            MoveToEx(hdc, g_x, g_y, NULL);
            LineTo(hdc, x, y);

            // [수정] GDI 정리 확실하게
            SelectObject(hdc, osPen);
            DeleteObject(myPen);
            ReleaseDC(hWnd, hdc);

            g_x = x;
            g_y = y;
        }
        break;

    case WM_LBUTTONUP:
        if (g_pen_flag == 0) break;

        if (g_free_flag) { g_free_flag = false; }

        x = LOWORD(lParam);
        y = HIWORD(lParam);

        hdc = GetDC(hWnd);
        myPen = CreatePen(PS_SOLID, penStroke, penColor);
        osPen = (HPEN)SelectObject(hdc, myPen);
        myBrush = CreateSolidBrush(solidColor);
        osBrush = (HBRUSH)SelectObject(hdc, myBrush);

        if (g_pen_flag == 1)
        {
            MoveToEx(hdc, g_x, g_y, NULL);
            LineTo(hdc, x, y);
        }
        else if (g_pen_flag == 2)
        {
            Rectangle(hdc, g_x, g_y, x, y);
        }
        else if (g_pen_flag == 3)
        {
            Ellipse(hdc, g_x, g_y, x, y);
        }

        // [수정] GDI 정리
        SelectObject(hdc, osBrush);
        DeleteObject(myBrush);
        SelectObject(hdc, osPen);
        DeleteObject(myPen);
        ReleaseDC(hWnd, hdc);
        break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);

        // 메뉴 명령도 ChildPaint로 보내고 싶다면 아래 주석 해제
        if (g_childPaint != NULL) PostMessage(g_childPaint, message, wParam, lParam);

        switch (wmId)
        {
        case ID_LINE: g_pen_flag = 1; break;
        case ID_RACT: g_pen_flag = 2; break;
        case ID_ELS:  g_pen_flag = 3; break;
        case ID_FREE: g_pen_flag = 4; break;

        case ID_1PX:  penStroke = 1;  break;
        case ID_5PX:  penStroke = 5;  break;
        case ID_10PX: penStroke = 10; break;

        case ID_ST_RED:   penColor = RGB(255, 0, 0); break;
        case ID_ST_GREED: penColor = RGB(0, 255, 0); break;
        case ID_ST_BLUE:  penColor = RGB(0, 0, 255); break;

        case ID_BRUSH_BLACK:  solidColor = RGB(0, 0, 0);       break;
        case ID_BRUSH_WHITE:  solidColor = RGB(255, 255, 255); break;
        case ID_BRUSH_PURPLE: solidColor = RGB(180, 85, 162);  break;

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
