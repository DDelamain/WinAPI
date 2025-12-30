#undef UNICODE
#include<Windows.h>

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
		break;
	case WM_COMMAND:
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