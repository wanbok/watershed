//============================================================================
//
// Debug View Header file
// 
// (c) SNUT Embedded Communications Lab.
//
//============================================================================


#ifndef __DBG_VIEW_H__
#define __DBG_VIEW_H__


#ifndef ERR_NONE
#define ERR_NONE 0
#endif

typedef struct dbg_view {
	void *view_handle;
	void *parent_wnd;
	int hor_width;
	unsigned int str_cnt;
} dbg_view_t;

int dbg_view_open(dbg_view_t *view, void *inst_handle, void *win_handle, unsigned int x_pos, unsigned int y_pos, unsigned int width, unsigned int height);

void dbg_view_close(dbg_view_t *view);

int dbg_view_print_str(dbg_view_t *view, const char *str);


#endif