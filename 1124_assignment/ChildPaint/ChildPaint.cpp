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
	if (!InitInstance(hInstance, nCmdShow))
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CHILDPAINT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CHILDPAINT);
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
int g_x, g_y;
int g_penStroke;
COLORREF g_Pcolor;
COLORREF g_Bcolor;
BOOL g_free_flag;

HWND g_Hmsprint;


HPEN SelectPen(HDC hdc, int g_strok, COLORREF g_pColor)
{
	/// 나의 펜 설정 영역
	int pStrok = 1;
	if (ID_1PX == g_strok) { pStrok = 1; }
	else if (ID_5PX == g_strok) { pStrok = 5; }
	else if (ID_10PX == g_strok) { pStrok = 10; }

	COLORREF pColor = RGB(0, 0, 0);
	if (ID_ST_RED == g_pColor) { pColor = RGB(255, 0, 0); }
	else if (ID_ST_GREEN == g_pColor) { pColor = RGB(0, 255, 0); }
	else if (ID_ST_BLUE == g_pColor) { pColor = RGB(0, 0, 255); }

	HPEN myPen = CreatePen(BS_SOLID, pStrok, pColor);
	return myPen;
}

HBRUSH SelectBrush(HDC hdc, COLORREF g_fColor)
{
	/// 나의 브러쉬 설정 영역
	HBRUSH myBrush = nullptr;

	if (ID_BRUSH_BLACK == g_fColor) { myBrush = CreateSolidBrush(RGB(0, 0, 0)); }
	else if (ID_BRUSH_WHITE == g_fColor) { myBrush = CreateSolidBrush(RGB(255, 255, 255)); }
	else if (ID_BRUSH_PURPLE == g_fColor) { myBrush = CreateSolidBrush(RGB(255, 0, 255)); }

	return myBrush;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	/// 마우스 좌표 저장
	int x = 0, y = 0;

	/// 접근 방법 마우스를 움직일 때 입력을 받아야하니 HWND 그림판 생성

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
		ReleaseDC(hWnd, hdc);
	}
	break;
	case WM_LBUTTONUP:
	{
		HDC hdc = GetDC(hWnd);
		if (g_pen_flag == 0) break;
		/// 클릭 해제 시 비활성화
		if (g_free_flag) { g_free_flag = false; }

		HPEN myPen,osPen;
		HBRUSH myBrush,osBrush;

		x = LOWORD(lParam);
		y = HIWORD(lParam);

		myPen = SelectPen(hdc, g_penStroke, g_Pcolor);
		osPen = (HPEN)SelectObject(hdc, myPen);

		myBrush = SelectBrush(hdc, g_Bcolor);
		osBrush = (HBRUSH)SelectObject(hdc, myBrush);


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
		ReleaseDC(hWnd, hdc);
	}
	break;
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
			g_penStroke = 1;
			break;
		case ID_5PX:
			g_penStroke = 5;
			break;
		case ID_10PX:
			g_penStroke = 10;
			break;
			/// 선 색 커맨드
		case ID_ST_RED:
			g_Pcolor= RGB(255, 0, 0);
			break;
		case ID_ST_GREED:
			g_Pcolor = RGB(0, 255, 0);
			break;
		case ID_ST_BLUE:
			g_Pcolor = RGB(0, 0, 255);
			break;
			/// 면 색 커맨드
		case ID_BRUSH_BLACK:
			g_Bcolor = RGB(0, 0, 0);
			break;
		case ID_BRUSH_WHITE:
			g_Bcolor = RGB(255, 255, 255);
			break;
		case ID_BRUSH_PURPLE:
			g_Bcolor = RGB(180, 85, 162);
			break;

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
