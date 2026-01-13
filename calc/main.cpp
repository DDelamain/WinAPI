#undef UNICODE
#include<Windows.h>
#include"resource.h"

#define g_i_BUTTON_SIZE				50
#define g_i_INTERVAL				 2
#define g_i_DOUBLE_BUTTON_SIZE		g_i_BUTTON_SIZE*2 + g_i_INTERVAL
#define g_i_START_X					10
#define g_i_START_Y					10
#define g_i_DISPLAY_WIDTH			g_i_BUTTON_SIZE*5 + g_i_INTERVAL*4
#define g_i_DISPLAY_HEIGHT			22
#define g_i_BUTTON_START_X			g_i_START_X
#define g_i_BUTTON_START_Y			g_i_START_Y + g_i_DISPLAY_HEIGHT + g_i_INTERVAL

#define g_i_WINDOW_WIDTH			g_i_DISPLAY_WIDTH + g_i_START_X*2 + 16
#define g_i_WINDOW_HEIGHT			g_i_DISPLAY_HEIGHT + g_i_START_Y*2 + (g_i_BUTTON_SIZE+g_i_INTERVAL)*4 + 38 // 38 - высота строки заголовка(Title bar)

#define BUTTON_X_POSITION(SHIFT)	g_i_BUTTON_START_X + (g_i_BUTTON_SIZE+g_i_INTERVAL)*(SHIFT)
#define BUTTON_Y_POSITION(SHIFT)	g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE+g_i_INTERVAL)*(SHIFT)

CONST CHAR g_OPERATIONS[] = "+-*/";

CONST CHAR g_sz_WINDOW_CLASS[] = "Calc PV_522";
LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1) Регистрация класса окна:
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));

	wClass.style = NULL;
	wClass.cbSize = sizeof(wClass);
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;

	wClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wClass.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
	wClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

	wClass.hInstance = hInstance;
	wClass.lpszClassName = g_sz_WINDOW_CLASS;
	wClass.lpszMenuName = NULL;
	wClass.lpfnWndProc = (WNDPROC)WndProc;

	if (!RegisterClassEx(&wClass))
	{
		MessageBox(NULL, "Class registration failed", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}

	//2) Создание окна:
	HWND hwnd = CreateWindowEx
	(
		NULL,	//ExStyle
		g_sz_WINDOW_CLASS,	//Window class
		g_sz_WINDOW_CLASS,	//Window title
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,//Window style
		CW_USEDEFAULT, CW_USEDEFAULT,//Position
		g_i_WINDOW_WIDTH, g_i_WINDOW_HEIGHT,//Size
		NULL,	//Parent Window
		NULL,	//hMenu
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//3) :
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		HWND hEdit = CreateWindowEx
		(
			NULL, "Edit", "0",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
			g_i_START_X, g_i_START_Y,
			g_i_DISPLAY_WIDTH, g_i_DISPLAY_HEIGHT,
			hwnd,
			(HMENU)IDC_DISPLAY,
			GetModuleHandle(NULL),
			NULL
		);
		//////////////////////////////////////////////////////////////////
		CHAR sz_digit[2] = {};
		for (int i = 6; i >= 0; i -= 3)
		{
			for (int j = 0; j < 3; j++)
			{
				sz_digit[0] = i + j + '1';
				CreateWindowEx
				(
					NULL, "Button", sz_digit,
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					BUTTON_X_POSITION(j), BUTTON_Y_POSITION(2 - i / 3),
					//g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL)*j,//X-posistion
					//g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL)*(2 - i / 3),
					g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
					hwnd,
					HMENU(IDC_BUTTON_1 + i + j),
					GetModuleHandle(NULL),
					NULL
				);
			}
		}
		CreateWindowEx
		(
			NULL, "Button", "0",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			BUTTON_X_POSITION(0), BUTTON_Y_POSITION(3),
			g_i_DOUBLE_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			HMENU(IDC_BUTTON_0),
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", ".",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			BUTTON_X_POSITION(2), BUTTON_Y_POSITION(3),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			HMENU(IDC_BUTTON_POINT),
			GetModuleHandle(NULL),
			NULL
		);
		//////////////////////////////////////////////////////////////////
		CHAR sz_operation[2] = {};
		for (int i = 0; i < 4; i++)
		{
			sz_operation[0] = g_OPERATIONS[3 - i];
			CreateWindowEx
			(
				NULL, "Button", sz_operation,
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				BUTTON_X_POSITION(3), BUTTON_Y_POSITION(i),
				g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
				hwnd,
				HMENU(IDC_BUTTON_SLASH - i),
				GetModuleHandle(NULL),
				NULL
			);
		}
		//////////////////////////////////////////////////////////////////
		CreateWindowEx
		(
			NULL, "Button", "<-",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			BUTTON_X_POSITION(4), BUTTON_Y_POSITION(0),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			HMENU(IDC_BUTTON_BSP),
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "C",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			BUTTON_X_POSITION(4), BUTTON_Y_POSITION(1),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			HMENU(IDC_BUTTON_CLR),
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "=",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			BUTTON_X_POSITION(4), BUTTON_Y_POSITION(2),
			g_i_BUTTON_SIZE, g_i_DOUBLE_BUTTON_SIZE,
			hwnd,
			HMENU(IDC_BUTTON_EQUAL),
			GetModuleHandle(NULL),
			NULL
		);
	}
	break;
	case WM_COMMAND:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
	default:	return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}