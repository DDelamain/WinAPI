#include<Windows.h>
#include"resource.h"

CONST CHAR g_sz_INVITE[] = "Введите имя пользователя";//на паре

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:	//Инициализация окна диалога. Эта секция отрабатывает 1 раз - при запуске окна.
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);
		//SetDlgItemTextA(hwnd, IDC_EDIT_LOGIN, "Введите имя пользователя");//сам
		HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);//на паре
		SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)g_sz_INVITE);//на паре
	}
	break;
	case WM_COMMAND:	//В этой секции обрабатываются нажатия кнопок, клавиш и другие события
	//мое самостоятельное решение
	/* {
		if (LOWORD(wParam) == IDC_EDIT_LOGIN) {//Проверяем, что выбрана именно строка с логином
			if (HIWORD(wParam) == EN_SETFOCUS)//Проверка, что возвращаемое значение - это EN_SETFOCUS
			{
				SetDlgItemTextA(hwnd, IDC_EDIT_LOGIN, "");
			}
			if (HIWORD(wParam) == EN_KILLFOCUS)
			{
				HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
				int textLength = GetWindowTextLength(hEditLogin);
				if (textLength == 0)
				{
					SetWindowText(hEditLogin, "Введите имя пользователя");
				}
				//SetDlgItemTextA(hwnd, IDC_EDIT_LOGIN, "Введите имя пользователя");
			}
			
		}
	}*/
	switch (LOWORD(wParam))
	{
	case IDC_EDIT_LOGIN://на паре
	{
		CONST INT SIZE = 256;//на паре
		CHAR sz_buffer[SIZE] = {};//на паре
		//HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);//на паре
		SendMessage((HWND)lParam, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);// на паре

		if (HIWORD(wParam) == EN_SETFOCUS && strcmp(sz_buffer, g_sz_INVITE) == 0)//на паре
				SendMessage((HWND)lParam, WM_SETTEXT, 0, (LPARAM)"");//на паре
				//SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)"");//на паре
		if (HIWORD(wParam) == EN_KILLFOCUS && strcmp(sz_buffer, "") == 0)//на паре
				SendMessage((HWND)lParam, WM_SETTEXT, 0, (LPARAM)g_sz_INVITE);//на паре
				//SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)g_sz_INVITE);//на паре
	}
	break;
	case IDC_BUTTON_COPY:
	{
		CONST INT SIZE = 256;
		CHAR sz_buffer[SIZE] = {};
		HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
		HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
		SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
		SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)sz_buffer);
	}
	break;
	case IDOK: MessageBox(hwnd, "Была нажата кнопка ОК", "Info", MB_OK | MB_ICONINFORMATION); break;
	case IDCANCEL: EndDialog(hwnd, 0); break;
	}
	break;
	case WM_CLOSE:		//Отрабатывает 1 раз при закрытии окна
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}