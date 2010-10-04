//============================================================================
//
// Image Processors Header file
// 
// (c) SNUT Embedded Communications Lab.
//
//============================================================================

#include <stdlib.h>
#include <string.h>

//----------------------------------------------------------------------------
// Declare public functions
//----------------------------------------------------------------------------


// 
// Color to Grayscale
// 
// @param src:		IN	Parameter, Mandatory, Source buffer
// @param dst:		OUT	Parameter, Mandatory, Destination buffer
// @param width:	IN	Parameter, Mandatory, Image's width
// @param height:	IN	Parameter, Mandatory, Image's height
// @param type:		IN	Parameter, Optional,  Image's type(Default : RGB)
// @return: result
int color_to_gray(unsigned char *src, unsigned char *dst, int width, int height, int type);

// 
// Gradient Operation
// 
// @param src:		IN	Parameter, Mandatory, Source buffer
// @param dst:		OUT	Parameter, Mandatory, Destination buffer
// @param width:	IN	Parameter, Mandatory, Image's width
// @param height:	IN	Parameter, Mandatory, Image's height
// @return: result
int gradient(unsigned char *src, unsigned char *dst, int width, int height);

// 
// Median filter
// 
// @param src:		IN	Parameter, Mandatory, Source buffer
// @param dst:		OUT	Parameter, Mandatory, Destination buffer
// @param width:	IN	Parameter, Mandatory, Image's width
// @param height:	IN	Parameter, Mandatory, Image's height
// @param window:	IN	Parameter, Optional,  Window size(Default: 3x3)
// @return: result
int median(unsigned char *src, unsigned char *dst, int width, int height, int winsize);

// 
// Merge 3bin RGB
// 
// @param prgb:		OUT	Parameter, Mandatory, Destination buffer
// @param pbinRed:		IN	Parameter, Mandatory, Souce Red buffer
// @param pbinGreen:	IN	Parameter, Mandatory, Souce Green buffer
// @param pbinBlue:		IN	Parameter, Mandatory, Souce Blue buffer
// @param width:	IN	Parameter, Mandatory, Image's width
// @param height:	IN	Parameter, Mandatory, Image's height
// @return: result
int	merge_3bin_rgb( unsigned char *prgb, 
				   unsigned char *pbinRed, unsigned char *pbinGreen, unsigned char *pbinBlue,
				   int width, int height);