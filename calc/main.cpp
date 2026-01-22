#define _CRT_SECURE_NO_WARNINGS
#undef UNICODE
#include<Windows.h>
#include<cstdio>
#include<iostream>
#include"resource.h"

//#define DEBUG

#define g_i_BUTTON_SIZE				50
#define g_i_INTERVAL				 1
#define g_i_DOUBLE_BUTTON_SIZE		g_i_BUTTON_SIZE*2 + g_i_INTERVAL
#define g_i_START_X					10
#define g_i_START_Y					10
#define g_i_DISPLAY_WIDTH			g_i_BUTTON_SIZE*5 + g_i_INTERVAL*4
#define g_i_DISPLAY_HEIGHT			22
#define g_i_BUTTON_START_X			g_i_START_X
#define g_i_BUTTON_START_Y			g_i_START_Y + g_i_DISPLAY_HEIGHT + g_i_INTERVAL

#define g_i_WINDOW_WIDTH			g_i_DISPLAY_WIDTH + g_i_START_X*2 + 16
#define g_i_WINDOW_HEIGTH			g_i_DISPLAY_HEIGHT + g_i_START_Y*2 + (g_i_BUTTON_SIZE+g_i_INTERVAL)*4 + 38	//38 - ������ ������ ��������� (TitleBar)

#define BUTTON_X_POSITION(SHIFT)	g_i_BUTTON_START_X + (g_i_BUTTON_SIZE+g_i_INTERVAL)*(SHIFT)
#define BUTTON_Y_POSITION(SHIFT)	g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE+g_i_INTERVAL)*(SHIFT)

CONST CHAR g_OPERATIONS[] = "+-*/";
enum Skin { SquareBlue, MetalMistral };
enum Color { MainBackgroud, DisplayBackground, Font };
CONST CHAR* g_SKINS[] = { "square_blue", "metal_mistral" };
CONST COLORREF g_COLORS[2][3] =
{
	{ RGB(0,0,200), RGB(0,0,100), RGB(200,200,200) },
	{ RGB(100,100,100), RGB(50,50,50), RGB(50,200,50) },
};

CONST CHAR g_sz_WINDOW_CLASS[] = "Calc PV_522";
LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID SetSkin(HWND hwnd, CONST CHAR sz_skin[]);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1) ����������� ������ ����:
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

	//2) �������� ����:
	HWND hwnd = CreateWindowEx
	(
		NULL,	//ExStyle
		g_sz_WINDOW_CLASS,	//Window class
		g_sz_WINDOW_CLASS,	//Window title
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,//Window style
		CW_USEDEFAULT, CW_USEDEFAULT,//Position
		g_i_WINDOW_WIDTH, g_i_WINDOW_HEIGTH,//Size
		NULL,	//Parent Window
		NULL,	//hMenu
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//3) ������ ����� ���������:
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
	static Skin skin = Skin::SquareBlue;
	switch (uMsg)
	{
	case WM_CREATE:
	{
#ifdef DEBUG
		AllocConsole();
#endif // DEBUG


		freopen("CONOUT$", "w", stdout);
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
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
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
		HWND hButton0 = CreateWindowEx
		(
			NULL, "Button", "0",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			BUTTON_X_POSITION(0), BUTTON_Y_POSITION(3),
			g_i_DOUBLE_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			HMENU(IDC_BUTTON_0),
			GetModuleHandle(NULL),
			NULL
		);
		//https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadimagea
		HBITMAP bmpButton0 = (HBITMAP)LoadImage
		(
			GetModuleHandle(NULL),
			"button_0.bmp",
			IMAGE_BITMAP,
			g_i_DOUBLE_BUTTON_SIZE, g_i_BUTTON_SIZE,
			LR_LOADFROMFILE
		);
		SendMessage(hButton0, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpButton0);
		CreateWindowEx
		(
			NULL, "Button", ".",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
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
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
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
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
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
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
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
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			BUTTON_X_POSITION(4), BUTTON_Y_POSITION(2),
			g_i_BUTTON_SIZE, g_i_DOUBLE_BUTTON_SIZE,
			hwnd,
			HMENU(IDC_BUTTON_EQUAL),
			GetModuleHandle(NULL),
			NULL
		);
		SetSkin(hwnd, "square_blue");
	}
	break;
	////////////////////////////////////////////////////////////////////////
	case WM_CTLCOLOREDIT:
	{
		HDC hdc = (HDC)wParam;	//Handler to Device Context.
		//�������� ���������� - ��� ����� ��������, ����������� � ������������� ����������,
		//����������� ��������� � ����� ���������� ����������� �������.
		//� �� Windows ��������� ��� ������ ���� ����� �������� �������� ���������� ��� ������ ������� GetDC(HWND
		//SetBkMode(hdc, OPAQUE);	//������ ������������� ����� ����������� hEditDisplay.
		HBRUSH hBackground = CreateSolidBrush(g_COLORS[skin][Color::MainBackgroud]);
		SetBkColor(hdc, g_COLORS[skin][Color::DisplayBackground]);
		SetTextColor(hdc, g_COLORS[skin][Color::Font]);
		/*SetBkColor(hdc, RGB(0, 0, 100));		//������ ���� ���� ��� EditControl
		SetTextColor(hdc, RGB(200, 200, 200));	//������ ���� ������ ��� EditControl
		HBRUSH hBackground = CreateSolidBrush(RGB(0, 0, 200));*/	//������ ����� ��� ���� ����� ��������� ������� ����.
		SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG)hBackground);	//��������� ���� ���� � ������ �������� ����.
		//UpdateWindow(hwnd);
		SendMessage(hwnd, WM_ERASEBKGND, wParam, 0);	//������� ������ ��� � �������� ����.
		return (LRESULT)hBackground;
	}
	break;
	////////////////////////////////////////////////////////////////////////
	case WM_COMMAND:
	{
		static DOUBLE	a = DBL_MIN, b = DBL_MIN;	//����������-��������� ��������, ������� ����� ������� 'double'.
		static INT		operation = 0;
		static BOOL		input = FALSE;	//����������� ���� �����;
		static BOOL		input_operation = FALSE;	//����������� ���� �������� +, -, *, / ;
		static BOOL		executed = FALSE;

		CHAR sz_digit[2] = {};
		CHAR sz_display[MAX_PATH] = {};
		HWND hEditDisplay = GetDlgItem(hwnd, IDC_DISPLAY);
		SendMessage(hEditDisplay, WM_GETTEXT, MAX_PATH, (LPARAM)sz_display);
		if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_9)
		{
			input_operation = FALSE;
			executed = FALSE;
			if (input == FALSE)ZeroMemory(sz_display, sizeof(sz_display));
			sz_digit[0] = LOWORD(wParam) - IDC_BUTTON_0 + '0';
			if (sz_display[0] == '0' && sz_display[1] != '.')
				strcpy(sz_display, sz_digit);
			else
				strcat(sz_display, sz_digit);
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
			input = TRUE;
			break;
		}
		if (LOWORD(wParam) == IDC_BUTTON_POINT)
		{
			input_operation = FALSE;
			if (strchr(sz_display, '.'))break;	//https://legacy.cplusplus.com/reference/cstring/strchr/
			strcat(sz_display, ".");
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
			input = TRUE;
			break;
		}
		if (LOWORD(wParam) == IDC_BUTTON_BSP)
		{
			sz_display[strlen(sz_display) - 1] = 0;	//NULL-Terminated Lines
			if (sz_display[0] == 0)sz_display[0] = '0';
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		if (LOWORD(wParam) == IDC_BUTTON_CLR)
		{
			a = DBL_MIN, b = DBL_MIN;	//����������-��������� ��������, ������� ����� ������� 'double'.
			operation = 0;
			input = FALSE;
			input_operation = FALSE;
			executed = FALSE;
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)"0");
		}
		if (LOWORD(wParam) >= IDC_BUTTON_PLUS && LOWORD(wParam) <= IDC_BUTTON_SLASH)
		{
#ifdef DEBUG
			std::cout << "OPERATIONAL BUTTONS:\n";
			std::cout << "a = " << a << "\tb = " << b << std::endl;;
			std::cout << "Operation:\t\t" << operation << std::endl;;
			std::cout << "Input:\t\t" << input << std::endl;;
			std::cout << "InputOperation:\t" << input_operation << std::endl;;
			std::cout << "Executed:\t" << executed << std::endl;;
			std::cout << "\n--------------------------------------\n" << std::endl;;
#endif // DEBUG

			if (input)
			{
				(a == DBL_MIN ? a : b) = atof(sz_display);//https://legacy.cplusplus.com/reference/cstdlib/atof/?kw=atof
				input = false;
			}
			if (!input_operation && !executed)SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_EQUAL), 0);
			operation = LOWORD(wParam);
			input_operation = TRUE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_EQUAL)
		{

#ifdef DEBUG
			std::cout << "EQUAL BUTTON:\n";
			std::cout << "a = " << a << "\tb = " << b << std::endl;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
			std::cout << "Input:\t\t" << input << std::endl;;
			std::cout << "InputOperation:\t" << input_operation << std::endl;;
			std::cout << "Executed:\t" << executed << std::endl;;
			std::cout << "\n--------------------------------------\n" << std::endl;;
#endif // DEBUG

			if (input)
			{
				(a == DBL_MIN ? a : b) = atof(sz_display);//https://legacy.cplusplus.com/reference/cstdlib/atof/?kw=atof
				input = FALSE;
			}
			else if (b == DBL_MIN) b = a;
			//if (operation && a == DBL_MIN)a = atof(sz_display);
			//if (a == DBL_MIN || b == DBL_MIN || operation == 0)break;
			if (a != DBL_MIN && b != DBL_MIN && operation != 0)
			{
				switch (operation)
				{
				case IDC_BUTTON_PLUS:	a += b;		break;
				case IDC_BUTTON_MINUS:	a -= b;		break;
				case IDC_BUTTON_ASTER:	a *= b;		break;
				case IDC_BUTTON_SLASH:	a /= b;		break;
				}
				input_operation = FALSE;
				executed = TRUE;
				if (a != DBL_MIN)
				{
					sprintf(sz_display, "%g", a);
					SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
				}
			}
		}

		SetFocus(hwnd);
	}
	break;
	////////////////////////////////////////////////////////////////////////
	case WM_KEYDOWN:
	{
		CHAR sz_key[8] = {};
		sprintf(sz_key, "%i", wParam);
		//std::cout << sz_key << std::endl;
		//MessageBox(hwnd, sz_key, "Info", MB_OK);
		if (GetKeyState(VK_SHIFT) < 0 && wParam == '8')
		{
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, TRUE, 0);
		}
		else if (GetKeyState(VK_SHIFT) < 0 && wParam == VK_OEM_PLUS)
		{
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_PLUS), BM_SETSTATE, TRUE, 0);
		}
		else if (wParam == VK_OEM_PLUS || wParam == VK_RETURN)
		{
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_EQUAL), BM_SETSTATE, TRUE, 0);
		}
		else if (wParam >= '0' && wParam <= '9')
		{
			SendMessage(GetDlgItem(hwnd, wParam - '0' + IDC_BUTTON_0), BM_SETSTATE, TRUE, NULL);
		}
		else if (wParam >= VK_NUMPAD0 && wParam <= VK_NUMPAD9)
		{
			SendMessage(GetDlgItem(hwnd, wParam - VK_NUMPAD0 + IDC_BUTTON_0), BM_SETSTATE, TRUE, NULL);
		}
		switch (wParam)
		{
		case VK_ADD:		SendMessage(GetDlgItem(hwnd, IDC_BUTTON_PLUS), BM_SETSTATE, TRUE, 0); break;
		case VK_OEM_MINUS:
		case VK_SUBTRACT:	SendMessage(GetDlgItem(hwnd, IDC_BUTTON_MINUS), BM_SETSTATE, TRUE, 0); break;
		case VK_MULTIPLY:	SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, TRUE, 0); break;
		case VK_OEM_2:
		case VK_DIVIDE:		SendMessage(GetDlgItem(hwnd, IDC_BUTTON_SLASH), BM_SETSTATE, TRUE, 0); break;
		case VK_OEM_PERIOD:
		case VK_DECIMAL:	SendMessage(GetDlgItem(hwnd, IDC_BUTTON_POINT), BM_SETSTATE, TRUE, 0); break;

		case VK_BACK:		SendMessage(GetDlgItem(hwnd, IDC_BUTTON_BSP), BM_SETSTATE, TRUE, 0); break;
		case VK_ESCAPE:		SendMessage(GetDlgItem(hwnd, IDC_BUTTON_CLR), BM_SETSTATE, TRUE, 0); break;
			//case VK_OEM_PLUS:	
			//case VK_RETURN:		SendMessage(GetDlgItem(hwnd, IDC_BUTTON_EQUAL), BM_SETSTATE, TRUE, 0); break;
		}
	}
	break;
	case WM_KEYUP:
	{
		//https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-keydown
		if (GetKeyState(VK_SHIFT) < 0 && wParam == '8')
		{
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_ASTER), 0);
		}
		else if (GetKeyState(VK_SHIFT) < 0 && wParam == VK_OEM_PLUS)
		{
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_PLUS), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_PLUS), 0);
		}
		else if (wParam == VK_OEM_PLUS || wParam == VK_RETURN)
		{
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_EQUAL), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_EQUAL), 0);
		}
		else if (wParam >= '0' && wParam <= '9')
		{
			SendMessage(GetDlgItem(hwnd, wParam - '0' + IDC_BUTTON_0), BM_SETSTATE, FALSE, NULL);
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam - '0' + IDC_BUTTON_0), 0);
		}
		else if (wParam >= VK_NUMPAD0 && wParam <= VK_NUMPAD9)
		{
			SendMessage(GetDlgItem(hwnd, wParam - VK_NUMPAD0 + IDC_BUTTON_0), BM_SETSTATE, FALSE, NULL);
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam - VK_NUMPAD0 + IDC_BUTTON_0), 0);
		}
	LABEL:
		switch (wParam)
		{
			//case VK_OEM_PLUS:
		case VK_ADD:
			//int a = 2;
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_PLUS), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_PLUS), BM_SETSTATE, FALSE, 0);	break;
		case VK_OEM_MINUS:
		case VK_SUBTRACT:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_MINUS), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_MINUS), BM_SETSTATE, FALSE, 0); break;
		case VK_MULTIPLY:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_ASTER), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, FALSE, 0); break;
		case VK_OEM_2:
		case VK_DIVIDE:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_SLASH), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_SLASH), BM_SETSTATE, FALSE, 0); break;
		case VK_OEM_PERIOD:
		case VK_DECIMAL:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_POINT), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_POINT), BM_SETSTATE, FALSE, 0); break;

		case VK_BACK:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_BSP), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_BSP), BM_SETSTATE, FALSE, 0); break;
		case VK_ESCAPE:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_CLR), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_CLR), BM_SETSTATE, FALSE, 0); break;
			//case VK_RETURN:		SendMessage(GetDlgItem(hwnd, IDC_BUTTON_EQUAL), BM_SETSTATE, FALSE, 0); break;
		}
	}
	break;
	case WM_CONTEXTMENU:
	{
		HMENU hMenu = CreatePopupMenu();
		InsertMenu(hMenu, 0, MF_BYPOSITION | MF_STRING, IDR_EXIT, "Exit");
		InsertMenu(hMenu, 0, MF_BYPOSITION | MF_SEPARATOR, NULL, NULL);
		InsertMenu(hMenu, 0, MF_BYPOSITION | MF_STRING, IDR_METAL_MISTRAL, "Metal mistral");
		InsertMenu(hMenu, 0, MF_BYPOSITION | MF_STRING, IDR_SQUARE_BLUE, "Square blue");
		INT item = TrackPopupMenuEx
		(
			hMenu,
			TPM_RIGHTALIGN | TPM_BOTTOMALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON | TPM_HORNEGANIMATION | TPM_VERNEGANIMATION,
			LOWORD(lParam), HIWORD(lParam),
			(HWND)wParam,
			NULL
		);
		switch (item)
		{
		case IDR_SQUARE_BLUE:	SetSkin(hwnd, "square_blue");	break;
		case IDR_METAL_MISTRAL: SetSkin(hwnd, "metal_mistral");	break;
		case IDR_EXIT:			SendMessage(hwnd, WM_CLOSE, 0, 0);
		}
		DestroyMenu(hMenu);
		skin = Skin(item - IDR_SQUARE_BLUE);
		HWND hEditDisplay = GetDlgItem(hwnd, IDC_DISPLAY);
		HDC hdc = GetDC(hwnd);
		SendMessage(hwnd, WM_CTLCOLOREDIT, (WPARAM)hdc, (LPARAM)hEditDisplay);
		ReleaseDC(hwnd, hdc);
		SetFocus(hEditDisplay);
	}
	break;
	case WM_DESTROY:
		FreeConsole();
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
	default:	return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}
VOID SetSkin(HWND hwnd, CONST CHAR sz_skin[])
{
	CONST CHAR* sz_NAMES[] =
	{
		"button_0",
		"button_1",
		"button_2",
		"button_3",
		"button_4",
		"button_5",
		"button_6",
		"button_7",
		"button_8",
		"button_9",
		"button_point",
		"button_plus",
		"button_minus",
		"button_aster",
		"button_slash",
		"button_bsp",
		"button_clr",
		"button_equal"
	};
	for (int i = 0; i < 18; i++)
	{
		HWND hButton = GetDlgItem(hwnd, IDC_BUTTON_0 + i);
		CHAR sz_filename[MAX_PATH] = {};
		sprintf(sz_filename, "ButtonsBMP\\%s\\%s.bmp", sz_skin, sz_NAMES[i]);
		//sprintf(sz_filename, "ButtonsBMP\\%s\\button_%i.bmp", sz_skin, i);
		HBITMAP bmpButton = (HBITMAP)LoadImage
		(
			GetModuleHandle(NULL),
			sz_filename,
			IMAGE_BITMAP,
			i > 0 ? g_i_BUTTON_SIZE : g_i_DOUBLE_BUTTON_SIZE,
			i < 17 ? g_i_BUTTON_SIZE : g_i_DOUBLE_BUTTON_SIZE,
			LR_LOADFROMFILE
		);
		SendMessage(hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpButton);
	}
}