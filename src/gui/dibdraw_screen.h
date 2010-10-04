//============================================================================
//
// DibDraw Screen Header file v0.9.0
// 
// (c) SNUT Embedded Communications Lab.
//
//============================================================================


#ifndef __DIBDRAW_SCREEN_H__
#define __DIBDRAW_SCREEN_H__


#ifndef ERR_NONE
#define ERR_NONE 0
#endif


// 
// Draw RGB16 function type
// 
// @param dibdraw:	IN Parameter, Mandatory, DibDraw Screen
// @param rgb_data:	IN Parameter, Mandatory, RGB16 data
typedef void (*draw_rgb16_t)(void *dibdraw, unsigned char *rgb_data);

// 
// Draw RGB24 function type
// 
// @param dibdraw:	IN Parameter, Mandatory, DibDraw Screen
// @param rgb_data:	IN Parameter, Mandatory, RGB24 data
typedef void (*draw_rgb24_t)(void *dibdraw, unsigned char *rgb_data);

// 
// Draw RGB32 function type
// 
// @param dibdraw:	IN Parameter, Mandatory, DibDraw Screen
// @param rgb_data:	IN Parameter, Mandatory, RGB32 data
typedef void (*draw_rgb32_t)(void *dibdraw, unsigned char *rgb_data);

// 
// Draw YUV420 function type
// 
// @param dibdraw: IN Parameter, Mandatory, DibDraw Screen
// @param y: IN Parameter, Mandatory, Y data
// @param u: IN Parameter, Mandatory, U data
// @param v: IN Parameter, Mandatory, V data
// @param lpitch: IN Parameter, Mandatory, L Pitch
// @param cpitch: IN Parameter, Mandatory, C Pitch
typedef void (*draw_yuv420_t)(void *dibdraw, unsigned char *y, unsigned char *u, unsigned char *v, unsigned int lpitch, unsigned int cpitch);

// 
// Draw Gray function type
// 
// @param dibdraw: IN Parameter, Mandatory, DibDraw Screen
// @param gray_data: IN Parameter, Mandatory, Gray data
typedef void (*draw_gray_t)(void *dibdraw, unsigned char *gray_data);


typedef struct dibdraw_screen {
	void *handle;
	void *parent_wnd;
	unsigned int width;
	unsigned int height;
	unsigned int x_pos;
	unsigned int y_pos;
	void *bitmap_info_hdr;
	draw_rgb16_t draw_rgb16;
	draw_rgb24_t draw_rgb24;
	draw_rgb32_t draw_rgb32;
	draw_yuv420_t draw_yuv420;
	draw_gray_t draw_gray;
} dibdraw_screen_t;


//----------------------------------------------------------------------------
// Declare DibDraw Screen public functions
//----------------------------------------------------------------------------


// 
// Open DibDraw Screen
// 
// @param dibdraw:		OUT	Parameter, Mandatory, DibDraw Screen
// @param parent_wnd:	IN	Parameter, Mandatory, parent window handle where DirectDraw view is placed
// @param x_pos:		IN	Parameter, Mandatory, DibDraw screen's X location
// @param y_pos:		IN	Parameter, Mandatory, DibDraw screen's Y location
// @param width:		IN	Parameter, Mandatory, DibDraw screen's width
// @param height:		IN	Parameter, Mandatory, DibDraw screen's height
// @return: result
#define ERR_DIBDRAW_SCREEN_OPEN__DRAWDIBBEGIN_FAILED -1
int dibdraw_screen_open(dibdraw_screen_t *dibdraw, void *parent_wnd, unsigned int x_pos, unsigned int y_pos, unsigned int width, unsigned int height);

// 
// Close DibDraw Screen
// 
// @param dibdraw: IN Parameter, Mandatory, DibDraw Screen
void dibdraw_screen_close(dibdraw_screen_t *dibdraw);


#endif


//============================================================================
//
// History
// 
// 2009.07.22 [v0.9.0] : Implement DibDraw Screen (by S.H.Jeong)
//
//============================================================================
