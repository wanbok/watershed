//============================================================================
//
// DibDraw Screen Source file
// 
// (c) SNUT Embedded Communications Lab.
//
//============================================================================


#include <stdlib.h>
#include <Windows.h>
#include <Vfw.h>

#include "dibdraw_screen.h"


//----------------------------------------------------------------------------
// Declare global variable
//----------------------------------------------------------------------------


//
// Global variable for RGB, YUV convert
static long int crv_tab[256];
static long int cbu_tab[256];
static long int cgu_tab[256];

static long int cgv_tab[256];
static long int tab_76309[256];
static unsigned char _clp[1024];
static unsigned char *clp;

int RGB2YUV_YR[256], RGB2YUV_YG[256], RGB2YUV_YB[256];
int RGB2YUV_UR[256], RGB2YUV_UG[256], RGB2YUV_UB[256];
int RGB2YUV_VG[256], RGB2YUV_VR[256], RGB2YUV_VB[256];


//----------------------------------------------------------------------------
// Declare DibDraw Screen private functions
//----------------------------------------------------------------------------


// 
// Draw RGB16
// 
// @param dibdraw:	IN Parameter, Mandatory, DibDraw Screen
// @param rgb_data:	IN Parameter, Mandatory, RGB16 data
void dibdraw_screen_draw_rgb16(dibdraw_screen_t *dibdraw, unsigned char *rgb_data);

// 
// Draw RGB24
// 
// @param dibdraw:	IN Parameter, Mandatory, DibDraw Screen
// @param rgb_data:	IN Parameter, Mandatory, RGB24 data
void dibdraw_screen_draw_rgb24(dibdraw_screen_t *dibdraw, unsigned char *rgb_data);

// 
// Draw RGB32
// 
// @param dibdraw:	IN Parameter, Mandatory, DibDraw Screen
// @param rgb_data:	IN Parameter, Mandatory, RGB32 data
void dibdraw_screen_draw_rgb32(dibdraw_screen_t *dibdraw, unsigned char *rgb_data);

// 
// Draw YUV420
// 
// @param dibdraw: IN Parameter, Mandatory, DibDraw Screen
// @param y: IN Parameter, Mandatory, Y data
// @param u: IN Parameter, Mandatory, U data
// @param v: IN Parameter, Mandatory, V data
// @param lpitch: IN Parameter, Mandatory, L Pitch
// @param cpitch: IN Parameter, Mandatory, C Pitch
void dibdraw_screen_draw_yuv420(dibdraw_screen_t *dibdraw, unsigned char *y, unsigned char *u, unsigned char *v, unsigned int lpitch, unsigned int cpitch);

// 
// Draw Gray
// 
// @param dibdraw:	IN Parameter, Mandatory, DibDraw Screen
// @param gray_data:	IN Parameter, Mandatory, Gray data
void dibdraw_screen_draw_gray(dibdraw_screen_t *dibdraw, unsigned char *gray_data);

//
// Initialize RGB, YUV convert table
//
void dibdraw_screen_init_dither_tab();

// 
// Convert YUV420 to RGB24
// 
// @param y:			IN Parameter, Mandatory, Y data
// @param u:			IN Parameter, Mandatory, U data
// @param v:			IN Parameter, Mandatory, V data
// @param dest_start:	IN Parameter, Mandatory, address of destination
// @param width:		IN Parameter, Mandatory, frame width
// @param height:		IN Parameter, Mandatory, frame height
// @param lpitch:		IN Parameter, Mandatory, L Pitch
// @param cpitch:		IN Parameter, Mandatory, C Pitch
void dibdraw_screen_convert_yuv420_to_rgv24(unsigned char *y, unsigned char *u, unsigned char *v, unsigned char *dest_start, int width, int height, int lpitch, int cpitch);


//----------------------------------------------------------------------------
// Define DibDraw Screen public functions
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
int dibdraw_screen_open(dibdraw_screen_t *dibdraw, void *parent_wnd, unsigned int x_pos, unsigned int y_pos, unsigned int width, unsigned int height) {
	HDC dc_handle = GetDC((HWND)parent_wnd);
	BITMAPINFOHEADER *bitmap_info_hdr = (BITMAPINFOHEADER *)malloc(sizeof(BITMAPINFOHEADER));
	memset(bitmap_info_hdr, 0, sizeof(BITMAPINFOHEADER));

	dibdraw->handle = DrawDibOpen();
	dibdraw->parent_wnd = parent_wnd;
	dibdraw->x_pos = x_pos;
	dibdraw->y_pos = y_pos;
	dibdraw->width = width;
	dibdraw->height = height;
	dibdraw->bitmap_info_hdr = bitmap_info_hdr;

	dibdraw->draw_rgb16 = dibdraw_screen_draw_rgb16;
	dibdraw->draw_rgb24 = dibdraw_screen_draw_rgb24;
	dibdraw->draw_rgb32 = dibdraw_screen_draw_rgb32;
	dibdraw->draw_yuv420 = dibdraw_screen_draw_yuv420;
	dibdraw->draw_gray = dibdraw_screen_draw_gray;

	bitmap_info_hdr->biWidth = width;
	bitmap_info_hdr->biHeight = height;
	bitmap_info_hdr->biSize = sizeof(BITMAPINFOHEADER);
	bitmap_info_hdr->biCompression = BI_RGB;
	bitmap_info_hdr->biPlanes = 1;
	bitmap_info_hdr->biBitCount = 24;
	bitmap_info_hdr->biSizeImage = 3 * width * height;

	if (!DrawDibBegin((HDRAWDIB)(dibdraw->handle), dc_handle, 2*width, 2*height, bitmap_info_hdr, width, height, 0))
		return ERR_DIBDRAW_SCREEN_OPEN__DRAWDIBBEGIN_FAILED;

	ReleaseDC((HWND)parent_wnd, dc_handle);

	dibdraw_screen_init_dither_tab();

	return 0;
}

// 
// Close DibDraw Screen
// 
// @param dibdraw: IN Parameter, Mandatory, DibDraw Screen
void dibdraw_screen_close(dibdraw_screen_t *dibdraw) {
	HDC dc_handle = GetDC((HWND)(dibdraw->parent_wnd));

	DrawDibEnd((HDRAWDIB)(dibdraw->handle));
	DrawDibClose((HDRAWDIB)(dibdraw->handle));
	ReleaseDC((HWND)(dibdraw->parent_wnd), dc_handle);

	if (dibdraw->bitmap_info_hdr)
		free(dibdraw->bitmap_info_hdr);
}


//----------------------------------------------------------------------------
// Define DibDraw Screen private functions
//----------------------------------------------------------------------------


// 
// Draw RGB16
// 
// @param dibdraw:	IN Parameter, Mandatory, DibDraw Screen
// @param rgb_data:	IN Parameter, Mandatory, RGB16 data
void dibdraw_screen_draw_rgb16(dibdraw_screen_t *dibdraw, unsigned char *rgb_data) {
	HDC dc_handle = GetDC((HWND)(dibdraw->parent_wnd));

	if (((BITMAPINFOHEADER *)(dibdraw->bitmap_info_hdr))->biCompression != BI_RGB || 
		((BITMAPINFOHEADER *)(dibdraw->bitmap_info_hdr))->biBitCount != 16) {
			BITMAPINFOHEADER *bitmap_info_hdr = (BITMAPINFOHEADER *)(dibdraw->bitmap_info_hdr);

			bitmap_info_hdr->biCompression = BI_RGB;
			bitmap_info_hdr->biPlanes = 1;
			bitmap_info_hdr->biBitCount = 16;
			bitmap_info_hdr->biSizeImage = 2 * dibdraw->width * dibdraw->height;

			if (!DrawDibBegin((HDRAWDIB)(dibdraw->handle), dc_handle, 2*(dibdraw->width), 2*(dibdraw->height), bitmap_info_hdr, dibdraw->width, dibdraw->height, 0)) {
				ReleaseDC((HWND)(dibdraw->parent_wnd), dc_handle);
				return;
			}
	}

	if (!DrawDibDraw((HDRAWDIB)(dibdraw->handle), dc_handle, dibdraw->x_pos, dibdraw->y_pos, dibdraw->width, dibdraw->height, (BITMAPINFOHEADER *)(dibdraw->bitmap_info_hdr), rgb_data, 0, 0, dibdraw->width, dibdraw->height, DDF_SAME_DRAW | DDF_SAME_HDC)) {
		ReleaseDC((HWND)(dibdraw->parent_wnd), dc_handle);
		return;
	}

	ReleaseDC((HWND)(dibdraw->parent_wnd), dc_handle);
}

// 
// Draw RGB24
// 
// @param dibdraw:	IN Parameter, Mandatory, DibDraw Screen
// @param rgb_data:	IN Parameter, Mandatory, RGB24 data
void dibdraw_screen_draw_rgb24(dibdraw_screen_t *dibdraw, unsigned char *rgb_data) {
	HDC dc_handle = GetDC((HWND)(dibdraw->parent_wnd));

	if (((BITMAPINFOHEADER *)(dibdraw->bitmap_info_hdr))->biCompression != BI_RGB || 
		((BITMAPINFOHEADER *)(dibdraw->bitmap_info_hdr))->biBitCount != 24) {
			BITMAPINFOHEADER *bitmap_info_hdr = (BITMAPINFOHEADER *)(dibdraw->bitmap_info_hdr);

			bitmap_info_hdr->biCompression = BI_RGB;
			bitmap_info_hdr->biPlanes = 1;
			bitmap_info_hdr->biBitCount = 24;
			bitmap_info_hdr->biSizeImage = 3 * dibdraw->width * dibdraw->height;

			if (!DrawDibBegin((HDRAWDIB)(dibdraw->handle), dc_handle, 2*(dibdraw->width), 2*(dibdraw->height), bitmap_info_hdr, dibdraw->width, dibdraw->height, 0)) {
				ReleaseDC((HWND)(dibdraw->parent_wnd), dc_handle);
				return;
			}
	}

	if (!DrawDibDraw((HDRAWDIB)(dibdraw->handle), dc_handle, dibdraw->x_pos, dibdraw->y_pos, dibdraw->width, dibdraw->height, (BITMAPINFOHEADER *)(dibdraw->bitmap_info_hdr), rgb_data, 0, 0, dibdraw->width, dibdraw->height, DDF_SAME_DRAW | DDF_SAME_HDC)) {
		ReleaseDC((HWND)(dibdraw->parent_wnd), dc_handle);
		return;
	}

	ReleaseDC((HWND)(dibdraw->parent_wnd), dc_handle);
}

// 
// Draw RGB32
// 
// @param dibdraw:	IN Parameter, Mandatory, DibDraw Screen
// @param rgb_data:	IN Parameter, Mandatory, RGB32 data
void dibdraw_screen_draw_rgb32(dibdraw_screen_t *dibdraw, unsigned char *rgb_data) {
	HDC dc_handle = GetDC((HWND)(dibdraw->parent_wnd));

	if (((BITMAPINFOHEADER *)(dibdraw->bitmap_info_hdr))->biCompression != BI_RGB || 
		((BITMAPINFOHEADER *)(dibdraw->bitmap_info_hdr))->biBitCount != 32) {
			BITMAPINFOHEADER *bitmap_info_hdr = (BITMAPINFOHEADER *)(dibdraw->bitmap_info_hdr);

			bitmap_info_hdr->biCompression = BI_RGB;
			bitmap_info_hdr->biPlanes = 1;
			bitmap_info_hdr->biBitCount = 32;
			bitmap_info_hdr->biSizeImage = 4 * dibdraw->width * dibdraw->height;

			if (!DrawDibBegin((HDRAWDIB)(dibdraw->handle), dc_handle, 2*(dibdraw->width), 2*(dibdraw->height), bitmap_info_hdr, dibdraw->width, dibdraw->height, 0)) {
				ReleaseDC((HWND)(dibdraw->parent_wnd), dc_handle);
				return;
			}
	}

	if (!DrawDibDraw((HDRAWDIB)(dibdraw->handle), dc_handle, dibdraw->x_pos, dibdraw->y_pos, dibdraw->width, dibdraw->height, (BITMAPINFOHEADER *)(dibdraw->bitmap_info_hdr), rgb_data, 0, 0, dibdraw->width, dibdraw->height, DDF_SAME_DRAW | DDF_SAME_HDC)) {
		ReleaseDC((HWND)(dibdraw->parent_wnd), dc_handle);
		return;
	}

	ReleaseDC((HWND)(dibdraw->parent_wnd), dc_handle);
}

// 
// Draw YUV420
// 
// @param dibdraw: IN Parameter, Mandatory, DibDraw Screen
// @param y: IN Parameter, Mandatory, Y data
// @param u: IN Parameter, Mandatory, U data
// @param v: IN Parameter, Mandatory, V data
// @param lpitch: IN Parameter, Mandatory, L Pitch
// @param cpitch: IN Parameter, Mandatory, C Pitch
static unsigned char rgb_data[720*480*3+1024];
void dibdraw_screen_draw_yuv420(dibdraw_screen_t *dibdraw, unsigned char *y, unsigned char *u, unsigned char *v, unsigned int lpitch, unsigned int cpitch) {
	HDC dc_handle = GetDC((HWND)(dibdraw->parent_wnd));	

	//if (((BITMAPINFOHEADER *)(dibdraw->bitmap_info_hdr))->biCompression != MAKEFOURCC('I','Y','U','V')) {
	//		BITMAPINFOHEADER *bitmap_info_hdr = (BITMAPINFOHEADER *)(dibdraw->bitmap_info_hdr);

	//		bitmap_info_hdr->biCompression = MAKEFOURCC('I','Y','U','V');
	//		bitmap_info_hdr->biPlanes = 1;
	//		bitmap_info_hdr->biBitCount = 32;
	//		bitmap_info_hdr->biSizeImage = 4 * dibdraw->width * dibdraw->height;

	//		if (!DrawDibBegin((HDRAWDIB)(dibdraw->handle), dc_handle, 2*(dibdraw->width), 2*(dibdraw->height), bitmap_info_hdr, dibdraw->width, dibdraw->height, 0)) {
	//			ReleaseDC((HWND)(dibdraw->parent_wnd), dc_handle);
	//			return;
	//		}
	//}

	dibdraw_screen_convert_yuv420_to_rgv24(y, u, v, rgb_data, dibdraw->width, dibdraw->height, lpitch, cpitch);

	if (!DrawDibDraw((HDRAWDIB)(dibdraw->handle), dc_handle, dibdraw->x_pos, dibdraw->y_pos, dibdraw->width, dibdraw->height, (BITMAPINFOHEADER *)(dibdraw->bitmap_info_hdr), rgb_data, 0, 0, dibdraw->width, dibdraw->height, DDF_SAME_DRAW | DDF_SAME_HDC)) {
		ReleaseDC((HWND)(dibdraw->parent_wnd), dc_handle);
		return;
	}

	ReleaseDC((HWND)(dibdraw->parent_wnd), dc_handle);
}

// 
// Draw Gray
// 
// @param dibdraw:	IN Parameter, Mandatory, DibDraw Screen
// @param gray_data:	IN Parameter, Mandatory, Gray data
void dibdraw_screen_draw_gray(dibdraw_screen_t *dibdraw, unsigned char *gray_data) {
	HDC dc_handle = GetDC((HWND)(dibdraw->parent_wnd));

	if (((BITMAPINFOHEADER *)(dibdraw->bitmap_info_hdr))->biCompression != BI_RGB || 
		((BITMAPINFOHEADER *)(dibdraw->bitmap_info_hdr))->biBitCount != 24) {
			BITMAPINFOHEADER *bitmap_info_hdr = (BITMAPINFOHEADER *)(dibdraw->bitmap_info_hdr);

			bitmap_info_hdr->biCompression = BI_RGB;
			bitmap_info_hdr->biPlanes = 1;
			bitmap_info_hdr->biBitCount = 24;
			bitmap_info_hdr->biSizeImage = 3 * dibdraw->width * dibdraw->height;

			if (!DrawDibBegin((HDRAWDIB)(dibdraw->handle), dc_handle, 2*(dibdraw->width), 2*(dibdraw->height), bitmap_info_hdr, dibdraw->width, dibdraw->height, 0)) {
				ReleaseDC((HWND)(dibdraw->parent_wnd), dc_handle);
				return;
			}
	}

	if (!DrawDibDraw((HDRAWDIB)(dibdraw->handle), dc_handle, dibdraw->x_pos, dibdraw->y_pos, dibdraw->width, dibdraw->height, (BITMAPINFOHEADER *)(dibdraw->bitmap_info_hdr), gray_data, 0, 0, dibdraw->width, dibdraw->height, DDF_SAME_DRAW | DDF_SAME_HDC)) {
		ReleaseDC((HWND)(dibdraw->parent_wnd), dc_handle);
		return;
	}

	ReleaseDC((HWND)(dibdraw->parent_wnd), dc_handle);
}

//
// Initialize RGB, YUV convert table
//
void dibdraw_screen_init_dither_tab() {
	long int crv,cbu,cgu,cgv;
	int i;   

	crv = 104597;
	cbu = 132201;
	cgu = 25675;
	cgv = 53279;

	for (i = 0; i < 256; i++) {
		crv_tab[i] = (i-128) * crv;
		cbu_tab[i] = (i-128) * cbu;
		cgu_tab[i] = (i-128) * cgu;
		cgv_tab[i] = (i-128) * cgv;
		tab_76309[i] = 76309*(i-16);
	}

	clp = _clp + 384;
	for (i=-384 ; i<640 ; i++)
		clp[i] = (i<0) ? 0 : ((i>255) ? 255 : i);


	// RGB to YUV Table 
	// Y  =      (0.257 * R) + (0.504 * G) + (0.098 * B) + 16
	// Cr = V =  (0.439 * R) - (0.368 * G) - (0.071 * B) + 128
	// Cb = U = -(0.148 * R) - (0.291 * G) + (0.439 * B) + 128

	for (i = 0; i < 256; i++) RGB2YUV_YR[i]  =  65 * i;
	for (i = 0; i < 256; i++) RGB2YUV_YG[i]  =  129* i;
	for (i = 0; i < 256; i++) RGB2YUV_YB[i]  =  25 * i;
	for (i = 0; i < 256; i++) RGB2YUV_UR[i]  =  38 * i;
	for (i = 0; i < 256; i++) RGB2YUV_UB[i]  = 112 * i;
	for (i = 0; i < 256; i++) RGB2YUV_UG[i]  =  74 * i;
	for (i = 0; i < 256; i++) RGB2YUV_VG[i]  =  94 * i;
	for (i = 0; i < 256; i++) RGB2YUV_VB[i]  =  18 * i;
	for (i = 0; i < 256; i++) RGB2YUV_VR[i]  = 112 * i;
}

// 
// Convert YUV420 to RGB24
// 
// @param y:			IN Parameter, Mandatory, Y data
// @param u:			IN Parameter, Mandatory, U data
// @param v:			IN Parameter, Mandatory, V data
// @param dest_start:	IN Parameter, Mandatory, address of destination
// @param width:		IN Parameter, Mandatory, frame width
// @param height:		IN Parameter, Mandatory, frame height
// @param lpitch:		IN Parameter, Mandatory, L Pitch
// @param cpitch:		IN Parameter, Mandatory, C Pitch
void dibdraw_screen_convert_yuv420_to_rgv24(unsigned char *y, unsigned char *u, unsigned char *v, unsigned char *dest_start, int width, int height, int lpitch, int cpitch)
{
	int y11,y21;
	int y12,y22;
	int y13,y23;
	int y14,y24;
	int u1,v1; 
	int i,j;
	int c11, c21, c31, c41;
	int c12, c22, c32, c42;
	unsigned int DW;
	unsigned int *id1, *id2;
	unsigned char *py1,*py2,*pu,*pv;
	unsigned char *d1, *d2;

	// write from bottom to up (maybe for Windows specific)
	d1 = dest_start;
	d1 += width*height*3 - width*3;	// goto last line 
	d2 = d1 - width*3;					// goto next last line   

	py1 = y; pu = u; pv = v; // source
	py2 = py1 + lpitch;

	// 4 Bytes Access 
	id1 = (unsigned int *)d1;
	id2 = (unsigned int *)d2;


	//  y11    y12   y13   y14                 
	//   c11/c21      c12/c22
	//   c31/c41      c32/c42
	//	y21   y22   y23   y24 
	//

	for (j = 0; j < height; j += 2) { 
		// line j + 0
		for (i = 0; i < width; i += 4) {
			u1 = *pu++;
			v1 = *pv++;
			c11 = crv_tab[v1];
			c21 = cgu_tab[u1];
			c31 = cgv_tab[v1];
			c41 = cbu_tab[u1];
			u1 = *pu++;
			v1 = *pv++;
			c12 = crv_tab[v1];
			c22 = cgu_tab[u1];
			c32 = cgv_tab[v1];
			c42 = cbu_tab[u1];

			y11 = tab_76309[*py1++]; // (255/219)*65536
			y12 = tab_76309[*py1++];
			y13 = tab_76309[*py1++]; // (255/219)*65536
			y14 = tab_76309[*py1++];

			y21 = tab_76309[*py2++];
			y22 = tab_76309[*py2++];
			y23 = tab_76309[*py2++];
			y24 = tab_76309[*py2++];

			//---------------
			// 1st Line 
			//---------------

			// RGB+R
			DW = ((clp[(y11 + c41)>>16])) |
				((clp[(y11 - c21 - c31)>>16])<<8) |
				((clp[(y11 + c11)>>16])<<16) |  

				((clp[(y12 + c41)>>16])<<24);
			*id1++ = DW;
			// GB+RG
			DW = ((clp[(y12 - c21 - c31)>>16])) |
				((clp[(y12 + c11)>>16])<<8) |  

				((clp[(y13 + c42)>>16])<<16) |
				((clp[(y13 - c22 - c32)>>16])<<24);
			*id1++ = DW;

			// B+RGB
			DW = ((clp[(y13 + c12)>>16])) |  

				((clp[(y14 + c42)>>16])<<8) |
				((clp[(y14 - c22 - c32)>>16])<<16) |
				((clp[(y14 + c12)>>16])<<24);  
			*id1++ = DW;


			//--------------
			// 2nd Line
			//--------------
			// RGB+R

			DW = ((clp[(y21 + c41)>>16])) |
				((clp[(y21 - c21 - c31)>>16])<<8) |
				((clp[(y21 + c11)>>16])<<16) |  
				((clp[(y22 + c41)>>16])<<24);
			*id2++ = DW;

			// GB+RG
			DW = ((clp[(y22 - c21 - c31)>>16])) |
				((clp[(y22 + c11)>>16])<<8) |  
				((clp[(y23 + c42)>>16])<<16) |
				((clp[(y23 - c22 - c32)>>16])<<24);
			*id2++ = DW;

			// B+RGB
			DW = ((clp[(y23 + c12)>>16])) |  
				((clp[(y24 + c42)>>16])<<8) |
				((clp[(y24 - c22 - c32)>>16])<<16) |
				((clp[(y24 + c12)>>16])<<24);
			*id2++ = DW;

		}
		// go to next two lines 

		id1 -= (9 * width)>>2; // 3*2 for next two + 3*1 for back 
		id2 -= (9 * width)>>2;

		py1 += (lpitch + lpitch - width);
		py2 += (lpitch + lpitch - width);
		pu  +=  (cpitch - width/2);
		pv  +=  (cpitch - width/2);
	}           
}
