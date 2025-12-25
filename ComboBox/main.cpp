#include<Windows.h>
#include"resource.h"

CONST CHAR* g_sz_VALUES[] = { "This", "is","my","first", "Combo","Box"};

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	switch (uMsg) 
	{
	case WM_INITDIALOG:
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);
		HWND hCombo = GetDlgItem(hwnd, IDC_COMBO);
		for (int i = 0; i < sizeof(g_sz_VALUES) / sizeof(g_sz_VALUES[0]); i++)
		{
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)g_sz_VALUES[i]);
		}
	}
		break;
	case WM_COMMAND:
	switch (LOWORD(wParam))
	{
	case IDOK: 
	{
		HWND hCombo = GetDlgItem(hwnd, IDC_COMBO);
		int index = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
		int lenOfText = SendMessage(hCombo, CB_GETLBTEXTLEN, index, 0);
		TCHAR* buffer = new TCHAR[lenOfText];
		SendMessage(hCombo, CB_GETLBTEXT, index, (LPARAM)buffer);
		TCHAR line[256];
		wsprintf(line, "Был выбран пункт %d %s", index+1, buffer);
		MessageBox(hwnd, line, "Info", MB_OK | MB_ICONINFORMATION);
		delete[] buffer; 
	}
	case IDCANCEL: EndDialog(hwnd, 0);
	}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}