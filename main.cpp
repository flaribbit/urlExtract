#include <windows.h>
#include <commctrl.h>
#include <stdio.h>

#include "resource.h"
#include "utils.h"

HINSTANCE hInst;
char filename[MAXLEN];
char filename_txt[MAXLEN];

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
	{
	}
	return TRUE;

	case WM_CLOSE:
	{
		EndDialog(hwndDlg, 0);
	}
	return TRUE;

	case WM_COMMAND:
	{
		switch(LOWORD(wParam))
		{
			case IDC_BUTTON1:
				if(GetFile(hwndDlg,hInst,filename)){
					SetDlgItemText(hwndDlg,IDC_EDIT1,filename);
					GetTxtFileName(filename,filename_txt);
					SetDlgItemText(hwndDlg,IDC_EDIT2,filename_txt);
				}
			break;
			case IDC_BUTTON2:
				GetDlgItemText(hwndDlg,IDC_EDIT1,filename,MAXLEN);
				GetDlgItemText(hwndDlg,IDC_EDIT2,filename_txt,MAXLEN);
				GetURL(hwndDlg,filename,filename_txt);
			break;
		}
	}
	return TRUE;
	}
	return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	hInst=hInstance;
	InitCommonControls();
	return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
