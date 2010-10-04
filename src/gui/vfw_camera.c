//============================================================================
//
// VFW Camera Source file v0.9.0
// 
// (c) SNUT Embedded Communications Lab.
//
//============================================================================


#include <Windows.h>
#include <Vfw.h>

#include "vfw_camera.h"


//----------------------------------------------------------------------------
// Declare global variables
//----------------------------------------------------------------------------


vfw_camera_capture_callback_t callback_func;

unsigned int capture_width;

unsigned int capture_height;


//----------------------------------------------------------------------------
// Declare VFW Camera private functions
//----------------------------------------------------------------------------


// 
// Capture callback function
// 
LRESULT CALLBACK vfw_camera_capture_callback(HWND hWnd, LPVIDEOHDR lpVHdr);


//----------------------------------------------------------------------------
// Define VFW Camera public functions
//----------------------------------------------------------------------------


// 
// Open VFW Camera
// 
// @param vfw_cam:			IN Parameter, Mandatory, VFW Camera
// @param paremt_wnd:		IN Parameter, Mandatory, parent window
// @param x_pos:			IN Parameter, Mandatory, X position of capture window
// @param y_pos:			IN Parameter, Mandatory, Y position of capture window
// @param resolution:		IN Parameter, Mandatory, resolution
// @param capture_callback:	IN Parameter, Mandatory, callback function
// @param frame_rate:		IN Parameter, Mandatory, frame rate
// @return: result
int vfw_camera_open(vfw_camera_t *vfw_cam, void *parent_wnd, unsigned int x_pos, unsigned int y_pos, vfw_camera_resolution_t resolution, vfw_camera_capture_callback_t capture_callback, unsigned int frame_rate) {
	BITMAPINFO bm_info;
	unsigned int callback_tick;

	// 1.Sets capture resolution
	switch(resolution) {
	case VFW_CAMERA_RESOLUTION_160_120:
		capture_width = 160;
		capture_height = 120;
		break;
	case VFW_CAMERA_RESOLUTION_176_144:
		capture_width = 176;
		capture_height = 144;
		break;
	case VFW_CAMERA_RESOLUTION_320_240:
		capture_width = 320;
		capture_height = 240;
	    break;
	case VFW_CAMERA_RESOLUTION_352_288:
		capture_width = 352;
		capture_height = 288;
	    break;
	case VFW_CAMERA_RESOLUTION_640_480:
		capture_width = 640;
		capture_height = 480;
		break;
	default:
		return ERR_VFW_CAMERA_OPEN__UNSUPPORTED_RESOLUTION;
	}

	// 2.Sets VFW Camera fields
	callback_func = capture_callback;
	vfw_cam->parent_wnd = parent_wnd;

	// 3.Create capture window
	vfw_cam->cap_wnd = capCreateCaptureWindow("Capture Window", WS_CHILD | WS_VISIBLE, x_pos, y_pos, capture_width, capture_height, parent_wnd, 0);

	// 4.Sets callback
	if (capSetCallbackOnFrame((HWND)(vfw_cam->cap_wnd), vfw_camera_capture_callback) == FALSE)
		return ERR_VFW_CAMERA_OPEN__SET_CALLBACK;

	// 5.Connect driver
	if (capDriverConnect((HWND)(vfw_cam->cap_wnd), 0) == FALSE)
		return ERR_VFW_CAMERA_OPEN__DRIVER_CONNECT;

	// 6.Sets capture video format
	capGetVideoFormat((HWND)(vfw_cam->cap_wnd), &bm_info, sizeof(BITMAPINFO));
	bm_info.bmiHeader.biBitCount = 12;
	bm_info.bmiHeader.biCompression = MAKEFOURCC('I', 'Y', 'U', 'V');
	bm_info.bmiHeader.biSizeImage = (capture_width*capture_height) + (capture_width*capture_height/2);
	bm_info.bmiHeader.biWidth = capture_width;
	bm_info.bmiHeader.biHeight = capture_height;
	capSetVideoFormat((HWND)(vfw_cam->cap_wnd), &bm_info, sizeof(BITMAPINFO));

	// 7.Sets frame rate
	callback_tick = (unsigned int)((1.0 / (double)frame_rate) * 1000.0);
	capPreviewRate((HWND)(vfw_cam->cap_wnd), callback_tick);
	capPreview((HWND)(vfw_cam->cap_wnd), TRUE);

	return ERR_NONE;
}

// 
// Close VFW Camera
// 
// @param vfw_cam: IN Parameter, Mandatory, VFW Camera
void vfw_camera_close(vfw_camera_t *vfw_cam) {
	DestroyWindow((HWND)(vfw_cam->cap_wnd));
}


//----------------------------------------------------------------------------
// Define VFW Camera private functions
//----------------------------------------------------------------------------


// 
// Capture callback function
// 
LRESULT CALLBACK vfw_camera_capture_callback(HWND hWnd, LPVIDEOHDR lpVHdr) {
	unsigned char *y, *u, *v;
	unsigned int lpitch, cpitch;

	y = lpVHdr->lpData;
	u = lpVHdr->lpData + (capture_width * capture_height);
	v = lpVHdr->lpData + (capture_width * capture_height) + (capture_width * capture_height / 4);

	lpitch = capture_width;
	cpitch = capture_width / 2;

	callback_func(y, u, v, lpitch, cpitch);

	return 0;
}
