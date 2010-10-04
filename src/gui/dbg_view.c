//============================================================================
//
// Debug View Source file
// 
// (c) SNUT Embedded Communications Lab.
//
//============================================================================


#include <Windows.h>

#include "dbg_view.h"


int dbg_view_open(dbg_view_t *view, void *inst_handle, void *win_handle, unsigned int x_pos, unsigned int y_pos, unsigned int width, unsigned int height) {
	view->str_cnt = 0;
	view->hor_width = width;
	view->view_handle = CreateWindow(TEXT("LISTBOX"), TEXT(""), WS_CHILD | WS_HSCROLL | WS_VSCROLL | WS_BORDER | LBS_NOTIFY | LBS_NOSEL | LBS_DISABLENOSCROLL, x_pos, y_pos, width, height, win_handle, (HMENU)1, inst_handle, NULL);
	ShowWindow(view->view_handle, SW_SHOWNORMAL);

	return ERR_NONE;
}

void dbg_view_close(dbg_view_t *view) {
	DestroyWindow(view->view_handle);
}

int dbg_view_print_str(dbg_view_t *view, const char *str) {
	HDC dc = GetDC(view->view_handle);
	HFONT font;
	SIZE str_width;

#if !defined(UNICODE)
	SendMessage(view->view_handle, LB_ADDSTRING, 0, (LPARAM)str);
#else
	WCHAR w_str[1024];

	MultiByteToWideChar(CP_ACP, 0, str, strlen(str)+1, w_str, sizeof(WCHAR)*1024);
	SendMessage(view->view_handle, LB_ADDSTRING, 0, (LPARAM)w_str);
#endif

	view->str_cnt++;

	SendMessage(view->view_handle, LB_SETCURSEL, view->str_cnt - 1, 0);

	font = (HFONT)SendMessage(view->view_handle, WM_GETFONT, 0, 0);

	SelectObject(dc, font);

#if !defined(UNICODE)
	GetTextExtentPoint32(dc, str, (int)strlen(str), (LPSIZE)(&str_width));
#else
	GetTextExtentPoint32(dc, w_str, strlen(str), (LPSIZE)(&str_width));
#endif

	if (str_width.cx+10 > view->hor_width) {
		SendMessage(view->view_handle, LB_SETHORIZONTALEXTENT, str_width.cx+10, 0);
		view->hor_width = str_width.cx+10;
	}

	DeleteObject(font);
	ReleaseDC(view->view_handle, dc);

	return 0;
}

