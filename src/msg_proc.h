//============================================================================
//
// Message Procedures Header file
// 
// (c) SNUT Embedded Communications Lab.
//
//============================================================================


#ifndef __MSG_PROC_H__
#define __MSG_PROC_H__


//----------------------------------------------------------------------------
// Declare public functions
//----------------------------------------------------------------------------


// 
// Program initialize
// 
// @param inst_handle:	IN	Parameter, Mandatory, Instance handle
// @param win_handle:	IN	Parameter, Mandatory, Window handle
// @param err_str:		OUT	Parameter, Mandatory, Error string
// @return: result
int prog_initialize(void *inst_handle, void *win_handle, char *err_str);

// 
// Program release
// 
// @param inst_handle:	IN	Parameter, Mandatory, Instance handle
// @param win_handle:	IN	Parameter, Mandatory, Window handle
void prog_release(void *inst_handle, void *win_handle);

int load_picture(void *win_handle);

int apply_watershed_from_gray(void *win_handle);
int apply_watershed_from_gradient(void *win_handle);
int apply_watershed_from_median(void *win_handle);

#endif