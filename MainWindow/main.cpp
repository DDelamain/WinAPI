#undef UNICODE
#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<stdio.h>

CONST CHAR g_sz_CLASS_NAME[] = "Main Window PV_522";

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));
	wClass.style = NULL;
	wClass.cbSize = sizeof(wClass);
	wClass.cbClsExtra = 0;//cb - Count Bytes
	wClass.cbWndExtra = 0;

	wClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

	wClass.hInstance = hInstance;
	wClass.lpszMenuName = NULL;//Главное меню
	wClass.lpszClassName = g_sz_CLASS_NAME;
	wClass.lpfnWndProc = WndProc;

	if (!RegisterClassEx(&wClass))
	{
		MessageBox(NULL, "Class registration failed", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}

	INT screenHeight = GetSystemMetrics(SM_CYSCREEN);
	INT screenWidth = GetSystemMetrics(SM_CXSCREEN);
	
	INT screenSizeHeight = screenHeight * 75 / 100;
	INT screenSizeWidth = screenWidth * 75 / 100;
	
	INT screenCenterHeight = (screenHeight - screenSizeHeight) / 2;
	INT screenCenterWidth = (screenWidth - screenSizeWidth) / 2;
	
	HWND hwnd = CreateWindowEx
	(
		NULL,				//exStyles
		g_sz_CLASS_NAME,	//ClassName
		g_sz_CLASS_NAME,	//Window title
		WS_OVERLAPPEDWINDOW,//Стиль окна. Наюор стилей всегда зависит от класса окна. Стиль главного окна всегда WS_OVERLAPPEDWINDOW
		screenCenterWidth, screenCenterHeight,//начальная позиция окна
		screenSizeWidth, screenSizeHeight, //Размер окна
		NULL,//parent window
		NULL,//hMenu. Для главного окна этот параметр определяет главное меню.
		//Для дочернего окна (Control) этот параметр содержит ResourceID дочернего окна.
		//Благодаря hMenu, дочернее окно всегда можно найти при помоци функции GetDlgItem()
		hInstance,
		NULL
	);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);


	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		HWND hStatic = CreateWindowEx
		(
			NULL,//exStyle
			"Static",//WindowClass
			"Этот текст создан функцией CreateWindowEx()",//Window text SendMessage(hwnd,WM_SETTEXT,...)
			WS_CHILD | WS_VISIBLE, // Window styles
			10, 10,//Position
			500, 25,//size
			hwnd, //ParentWindow
			(HMENU)1000, //Resource ID
			GetModuleHandle(NULL), //hInstance
			NULL
		);
		HWND hEdit = CreateWindowEx
		(
			NULL,
			"Edit",
			"",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER,
			10, 40,
			500, 22,
			hwnd,
			(HMENU)1001,
			GetModuleHandle(NULL),
			NULL
		);
		/*100 ... 999 - окна
		1000 ... 40000 - элементы управления окном*/
		CreateWindowEx
		(
			NULL,
			"Button",
			"Применить",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			460, 70,
			100, 25,
			hwnd,
			(HMENU)1002,
			GetModuleHandle(NULL),
			NULL
		);
	}
		break;
	case WM_SIZE:

	case WM_MOVE:
	{
		RECT rect;
		GetWindowRect(hwnd, &rect);
		CHAR sz_title[MAX_PATH] = {};
		sprintf(
			sz_title, "%s - Position: %ix%i, Size%ix%i",
			g_sz_CLASS_NAME, rect.left, rect.top,
			rect.right - rect.left, rect.bottom-rect.top
		);
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_title);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 1002:
		{
			CHAR sz_buffer[MAX_PATH] = {};
			HWND hStatic = GetDlgItem(hwnd, 1000);
			HWND hEdit = GetDlgItem(hwnd, 1001);
			SendMessage(hEdit, WM_GETTEXT, MAX_PATH, (LPARAM)sz_buffer);
			SendMessage(hStatic, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	default:	return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}