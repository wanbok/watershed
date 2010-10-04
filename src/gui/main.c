//============================================================================
//
// Main function Source file
// 
// (c) SNUT Embedded Communications Lab.
//
//============================================================================


#if defined(WIN32) || defined(WINCE)
#include <windows.h>
#endif

#if defined(WINCE)
#include <aygshell.h>
#endif


#include "../program_config.h"
#include "../msg_proc.h"
#include "resource.h"


//----------------------------------------------------------------------------
// Declare global variables
//----------------------------------------------------------------------------


HINSTANCE	inst_handle;

#if defined(WINCE)
HWND		win_menubar_handle;
#endif


//----------------------------------------------------------------------------
// Declare private functions
//----------------------------------------------------------------------------


// 
// Window Procedure
#if defined(WIN32) || defined(WINCE)
LRESULT CALLBACK WinProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
#endif

// 
// Initialize Menu
#if defined(WINCE)
void InitializeMenu(HWND hWnd);
#endif


//----------------------------------------------------------------------------
// Define public functions
//----------------------------------------------------------------------------


// 
// main function
#if defined(WIN32) && !defined(WINCE)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
#elif defined(WIN32) && defined(WINCE)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd) {
#else
int main() {
#endif

#if defined(WIN32) || defined(WINCE)
	HWND		hWnd;
	MSG			Message;
	WNDCLASS	WinClass;

	inst_handle = hInstance;

	WinClass.cbClsExtra = 0;
	WinClass.cbWndExtra = 0;
	WinClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
#if !defined(WINCE)
	WinClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WinClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
#elif defined(WINCE)
	WinClass.hCursor = NULL;
	WinClass.hIcon = NULL;
#endif
	WinClass.hInstance = hInstance;
	WinClass.lpfnWndProc = WinProc;
	WinClass.lpszClassName = PROGRAM_NAME;
	WinClass.lpszMenuName = NULL;
	WinClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&WinClass);

#if defined(WIN32) && !defined(WINCE)
	hWnd = CreateWindow(PROGRAM_NAME, PROGRAM_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 780, 600, NULL, LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU_MAIN)), hInstance, NULL);
#elif defined(WINCE)
	hWnd = CreateWindowEx(WS_EX_TOPMOST, PROGRAM_NAME, PROGRAM_NAME, WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
#endif

	ShowWindow(hWnd, nShowCmd);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (INT)Message.wParam;
#endif // end of defined(WIN32) || defined(WINCE)

}


//----------------------------------------------------------------------------
// Define private functions
//----------------------------------------------------------------------------


//
// WinProc
#if defined(WIN32) || defined(WINCE)
LRESULT CALLBACK WinProc(IN HWND hWnd, IN UINT iMessage, IN WPARAM wParam, IN LPARAM lParam) {
	char err_str[MAX_ERR_STR_LENGTH];

	switch(iMessage) {
	case WM_DESTROY:
		prog_release(inst_handle, hWnd);
		PostQuitMessage(0);
		return 0;

	case WM_CREATE:
#if defined(WINCE)
		InitializeMenu(hWnd);
#endif

		if (prog_initialize(inst_handle, hWnd, err_str) < 0) {
#if !defined(UNICODE)
			MessageBox(hWnd, err_str, "ERROR", MB_OK | MB_ICONERROR);
#else
			WCHAR uni_err_str[MAX_ERR_STR_LENGTH];

			MultiByteToWideChar(CP_ACP, 0, err_str, MAX_ERR_STR_LENGTH, uni_err_str, MAX_ERR_STR_LENGTH*(sizeof(WCHAR)));
			MessageBox(hWnd, uni_err_str, L"ERROR", MB_OK | MB_ICONERROR);
#endif
			DestroyWindow(hWnd);
		}
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam)) {
//		case IDM_FILE_TEST:
//			if (menu_file_test(inst_handle, hWnd, err_str) < 0) {
//#if !defined(UNICODE)
//				MessageBox(hWnd, err_str, "ERROR", MB_OK | MB_ICONERROR);
//#else
//				WCHAR uni_err_str[MAX_ERR_STR_LENGTH];
//
//				MultiByteToWideChar(CP_ACP, 0, err_str, MAX_ERR_STR_LENGTH, uni_err_str, MAX_ERR_STR_LENGTH*(sizeof(WCHAR)));
//				MessageBox(hWnd, uni_err_str, L"ERROR", MB_OK | MB_ICONERROR);
//#endif
//			}
//			break;

	case ID_FILE_LOAD:
		if (load_picture(hWnd) < 0) {
#if !defined(UNICODE)
			MessageBox(hWnd, "Error occur", "ERROR", MB_OK | MB_ICONERROR);
#else
			WCHAR uni_err_str[MAX_ERR_STR_LENGTH];

			MultiByteToWideChar(CP_ACP, 0, err_str, MAX_ERR_STR_LENGTH, uni_err_str, MAX_ERR_STR_LENGTH*(sizeof(WCHAR)));
			MessageBox(hWnd, uni_err_str, L"ERROR", MB_OK | MB_ICONERROR);
#endif
		}
		break;

	case ID_WATERSHED_FROMGRAY:
		if (apply_watershed_from_gray(hWnd) < 0) {
#if !defined(UNICODE)
			MessageBox(hWnd, err_str, "ERROR", MB_OK | MB_ICONERROR);
#else
			WCHAR uni_err_str[MAX_ERR_STR_LENGTH];

			MultiByteToWideChar(CP_ACP, 0, err_str, MAX_ERR_STR_LENGTH, uni_err_str, MAX_ERR_STR_LENGTH*(sizeof(WCHAR)));
			MessageBox(hWnd, uni_err_str, L"ERROR", MB_OK | MB_ICONERROR);
#endif
		}
		break;

	case ID_WATERSHED_FROMGRADIENT:
		if (apply_watershed_from_gradient(hWnd) < 0) {
#if !defined(UNICODE)
			MessageBox(hWnd, err_str, "ERROR", MB_OK | MB_ICONERROR);
#else
			WCHAR uni_err_str[MAX_ERR_STR_LENGTH];

			MultiByteToWideChar(CP_ACP, 0, err_str, MAX_ERR_STR_LENGTH, uni_err_str, MAX_ERR_STR_LENGTH*(sizeof(WCHAR)));
			MessageBox(hWnd, uni_err_str, L"ERROR", MB_OK | MB_ICONERROR);
#endif
		}
		break;

	case ID_WATERSHED_FROMMEDIAN:
		if (apply_watershed_from_median(hWnd) < 0) {
#if !defined(UNICODE)
			MessageBox(hWnd, err_str, "ERROR", MB_OK | MB_ICONERROR);
#else
			WCHAR uni_err_str[MAX_ERR_STR_LENGTH];

			MultiByteToWideChar(CP_ACP, 0, err_str, MAX_ERR_STR_LENGTH, uni_err_str, MAX_ERR_STR_LENGTH*(sizeof(WCHAR)));
			MessageBox(hWnd, uni_err_str, L"ERROR", MB_OK | MB_ICONERROR);
#endif
		}
		break;
	case IDM_FILE_EXIT:
		DestroyWindow(hWnd);
		break;

	case IDM_HELP_ABOUT:
		{
			TCHAR about_msg[1024];

#if !defined(UNICODE)
			sprintf(about_msg, "%s\r\nVersion : %s\r\n(c) SNUT Embedded Communications Lab.", PROGRAM_NAME, VERSION);
#else
			wsprintf(about_msg, L"%s\r\nVersion : %s\r\n(c) SNUT Embedded Communications Lab.", PROGRAM_NAME, VERSION);
#endif
			MessageBox(hWnd, about_msg, TEXT("About"), MB_OK | MB_ICONINFORMATION);
		}
		break;

		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
		}
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
#endif

//
// InitializeMenu
#if defined(WINCE)
void InitializeMenu(HWND hWnd) {
	SHMENUBARINFO mbi;
	static SHACTIVATEINFO s_sai;

	memset(&mbi, 0, sizeof(SHMENUBARINFO));
	mbi.cbSize     = sizeof(SHMENUBARINFO);
	mbi.hwndParent = hWnd;
	mbi.nToolBarId = IDR_MENU_MAIN;
	mbi.hInstRes   = inst_handle;
	mbi.dwFlags = SHCMBF_HMENU;

	if (!SHCreateMenuBar(&mbi))
		win_menubar_handle = NULL;
	else
		win_menubar_handle = mbi.hwndMB;

	memset(&s_sai, 0, sizeof (s_sai));
	s_sai.cbSize = sizeof (s_sai);

	if (win_menubar_handle) {
		RECT rc;
		RECT rcMenuBar;

		GetWindowRect(hWnd, &rc);
		GetWindowRect(win_menubar_handle, &rcMenuBar);
		rc.bottom -= (rcMenuBar.bottom - rcMenuBar.top);

		MoveWindow(hWnd, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, FALSE);
	}
}
#endif
