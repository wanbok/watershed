//============================================================================
//
// VFW Camera Header file v1.0.0
// 
// (c) SNUT Embedded Communications Lab.
//
//============================================================================


#ifndef __VFW_CAMERA_H__
#define __VFW_CAMERA_H__


#ifndef ERR_NONE
#define ERR_NONE 0
#endif

typedef void (*vfw_camera_capture_callback_t)(unsigned char *y, unsigned char *u, unsigned char *v, unsigned int lpitch, unsigned int cpitch);

typedef enum vfw_camera_resolution {
	VFW_CAMERA_RESOLUTION_160_120, 
	VFW_CAMERA_RESOLUTION_176_144, 
	VFW_CAMERA_RESOLUTION_320_240, 
	VFW_CAMERA_RESOLUTION_352_288, 
	VFW_CAMERA_RESOLUTION_640_480
} vfw_camera_resolution_t;

typedef struct vfw_camera {
	void *parent_wnd;
	void *cap_wnd;
} vfw_camera_t;


//----------------------------------------------------------------------------
// Declare VFW Camera public functions
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
#define ERR_VFW_CAMERA_OPEN__UNSUPPORTED_RESOLUTION -1
#define ERR_VFW_CAMERA_OPEN__DRIVER_CONNECT -2
#define ERR_VFW_CAMERA_OPEN__SET_CALLBACK -3
int vfw_camera_open(vfw_camera_t *vfw_cam, void *parent_wnd, unsigned int x_pos, unsigned int y_pos, vfw_camera_resolution_t resolution, vfw_camera_capture_callback_t capture_callback, unsigned int frame_rate);

// 
// Close VFW Camera
// 
// @param vfw_cam: IN Parameter, Mandatory, VFW Camera
void vfw_camera_close(vfw_camera_t *vfw_cam);


#endif


//============================================================================
//
// History
// 
// 2009.07.22 [v1.0.0] : Implement VFW Camera (by S.H.Jeong)
//
//============================================================================
