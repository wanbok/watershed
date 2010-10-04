//============================================================================
//
// Message Procedures Source file
// 
// (c) SNUT Embedded Communications Lab.
//
//============================================================================


#include <stdio.h>
#include "gui/dbg_view.h"
#include "gui/dibdraw_screen.h"
#include "image_process.h"
#include "watershed.h"
#include "bmp.h"

dbg_view_t dbg;
dibdraw_screen_t dds;
dibdraw_screen_t dds_gray_original;
dibdraw_screen_t dds_gray_watershed;
dibdraw_screen_t dds_gradient_original;
dibdraw_screen_t dds_gradient_watershed;
dibdraw_screen_t dds_median_original;
dibdraw_screen_t dds_median_watershed;
unsigned char *buf;
LPCTSTR img = "../../../data/test_image_lena.bmp";
int dds_flag = 0;

//----------------------------------------------------------------------------
// Define public functions
//----------------------------------------------------------------------------


// 
// Program initialize
// 
// @param inst_handle:	IN	Parameter, Mandatory, Instance handle
// @param win_handle:	IN	Parameter, Mandatory, Window handle
// @param err_str:		OUT	Parameter, Mandatory, Error string
// @return: result
int prog_initialize(void *inst_handle, void *win_handle, char *err_str) {

	char tmp[256];

	dbg_view_open(&dbg, inst_handle, win_handle, 10, 20, 200, 300);
	sprintf(tmp, "Print debugs");
	dbg_view_print_str(&dbg, tmp);


	return 0;
}

// 
// Program release
// 
// @param inst_handle:	IN	Parameter, Mandatory, Instance handle
// @param win_handle:	IN	Parameter, Mandatory, Window handle
void prog_release(void *inst_handle, void *win_handle) {

	dbg_view_close(&dbg);
	if (dds_flag > 0)
		dibdraw_screen_close(&dds);
	if (dds_flag > 1)
		dibdraw_screen_close(&dds_gray_original);
	if (dds_flag > 2)
		dibdraw_screen_close(&dds_gray_original);
	return;
}

// 
// Load picture
// 
int load_picture(void *win_handle) {

	int x, y;
	long s;
	
	//FILE *fd = fopen("../../../data/lena_512_512.rgb24", "rb");
	//fread(buf, 1, 512*512*3, fd);

	BYTE* a = LoadBMP ( &x, &y, &s, img );

	buf = ConvertBMPToRGBBuffer ( a, x, y );

	if(dibdraw_screen_open(&dds, win_handle, 220, 20, x, y) == 0)
		dds_flag = 1;

	dds.draw_rgb24(&dds, buf);

	return 0;
}

//
// Apply watershed from grayscale
//
int apply_watershed_from_gray(void *win_handle) {

	int x, y;
	long s;
	unsigned char *gray1;
	unsigned char *gray2;

	BYTE* a = LoadBMP ( &x, &y, &s, img );

	buf = ConvertBMPToRGBBuffer ( a, x, y );
	gray1 = (unsigned char *)malloc(sizeof(unsigned char)*x*y);
	gray2 = (unsigned char *)malloc(sizeof(unsigned char)*x*y);
	//memcpy(&dst, &buf, sizeof(sizeof(unsigned char)*3*x*y));
	if (color_to_gray(buf, gray1, x, y, 0) < 0)
		return;
	//if (gradient(gray1, gray2, x, y) < 0)
	//	return;
	//if (median(gray2, gray1, x, y, 3) < 0)
	//	return;
	if (watershed_operation(gray1, gray2, x, y) < 0)
		return;

	if (merge_3bin_rgb(buf, gray1, gray1, gray1, x, y) < 0)
		return;

	if(dibdraw_screen_open(&dds_gray_original, win_handle, 220, 20 + 10 + y, x, y) == 0)
		dds_flag = 2;
	dds_gray_original.draw_gray(&dds_gray_original, buf);

	if (merge_3bin_rgb(buf, gray2, gray2, gray2, x, y) < 0)
		return;

	if(dibdraw_screen_open(&dds_gray_watershed, win_handle, 220, 20 + 2*(10 + y), x, y) == 0)
		dds_flag = 3;
	dds_gray_watershed.draw_gray(&dds_gray_watershed, buf);

	return 0;
}

//
// Apply watershed from gradient
//
int apply_watershed_from_gradient(void *win_handle) {

	int x, y;
	long s;
	unsigned char *gray1;
	unsigned char *gray2;

	BYTE* a = LoadBMP ( &x, &y, &s, img );

	buf = ConvertBMPToRGBBuffer ( a, x, y );
	gray1 = (unsigned char *)malloc(sizeof(unsigned char)*x*y);
	gray2 = (unsigned char *)malloc(sizeof(unsigned char)*x*y);
	//memcpy(&dst, &buf, sizeof(sizeof(unsigned char)*3*x*y));
	if (color_to_gray(buf, gray1, x, y, 0) < 0)
		return;
	if (gradient(gray1, gray2, x, y) < 0)
		return;
	//if (median(gray2, gray1, x, y, 3) < 0)
	//	return;
	if (watershed_operation(gray2, gray1, x, y) < 0)
		return;

	if (merge_3bin_rgb(buf, gray2, gray2, gray2, x, y) < 0)
		return;

	if(dibdraw_screen_open(&dds_gradient_original, win_handle, 220 + 10 + x, 20 + 10 + y, x, y) == 0)
		dds_flag = 2;
	dds_gradient_original.draw_gray(&dds_gradient_original, buf);

	if (merge_3bin_rgb(buf, gray1, gray1, gray1, x, y) < 0)
		return;

	if(dibdraw_screen_open(&dds_gradient_watershed, win_handle, 220 + 10 + x, 20 + 2*(10 + y), x, y) == 0)
		dds_flag = 3;
	dds_gradient_watershed.draw_gray(&dds_gradient_watershed, buf);

	return 0;
}

//
// Apply watershed from median
//
int apply_watershed_from_median(void *win_handle) {

	int x, y;
	long s;
	unsigned char *gray1;
	unsigned char *gray2;

	BYTE* a = LoadBMP ( &x, &y, &s, img );

	buf = ConvertBMPToRGBBuffer ( a, x, y );
	gray1 = (unsigned char *)malloc(sizeof(unsigned char)*x*y);
	gray2 = (unsigned char *)malloc(sizeof(unsigned char)*x*y);
	//memcpy(&dst, &buf, sizeof(sizeof(unsigned char)*3*x*y));
	if (color_to_gray(buf, gray1, x, y, 0) < 0)
		return;
	if (gradient(gray1, gray2, x, y) < 0)
		return;
	if (median(gray2, gray1, x, y, 3) < 0)
		return;
	if (watershed_operation(gray1, gray2, x, y) < 0)
		return;

	if (merge_3bin_rgb(buf, gray1, gray1, gray1, x, y) < 0)
		return;

	if(dibdraw_screen_open(&dds_median_original, win_handle, 220 + 2 * (10 + x), 20 + 10 + y, x, y) == 0)
		dds_flag = 2;
	dds_median_original.draw_gray(&dds_median_original, buf);

	if (merge_3bin_rgb(buf, gray2, gray2, gray2, x, y) < 0)
		return;

	if(dibdraw_screen_open(&dds_median_watershed, win_handle, 220 + 2 * (10 + x), 20 + 2*(10 + y), x, y) == 0)
		dds_flag = 3;
	dds_median_watershed.draw_gray(&dds_median_watershed, buf);

	return 0;
}