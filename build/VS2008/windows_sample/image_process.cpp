//============================================================================
//
// Message Procedures Source file
// 
// (c) SNUT Embedded Communications Lab.
//
//============================================================================

#ifdef __cplusplus
extern "C" {
#endif

#include "image_process.h"

//----------------------------------------------------------------------------
// Define public functions
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
int color_to_gray(unsigned char *src, unsigned char *dst, int width, int height, int type) {
	int i;
	for(i = 0; i<width*height; i++){
		dst[i] = (src[i*3]+src[i*3+1]+src[i*3+2])/3;
	}
	return 0;
}

// 
// Gradient Operation
// 
// @param src:		IN	Parameter, Mandatory, Source buffer
// @param dst:		OUT	Parameter, Mandatory, Destination buffer
// @param width:	IN	Parameter, Mandatory, Image's width
// @param height:	IN	Parameter, Mandatory, Image's height
// @return: result
int gradient(unsigned char *src, unsigned char *dst, int width, int height) {
	int h, v;
	int win_depth = 1;

	for(v = win_depth; v < height-win_depth; v++){
		for( h = win_depth; h < width-win_depth; h++){

			dst[v*width +h] = 
				abs(-src[(v-1)*width + h-1]
				-src[(v-1)*width + h]*2
				-src[(v-1)*width + h+1]
				//+src[v*width		+ h-1]
				//+src[v*width		+ h]
				//+src[v*width		+ h+1]
				+src[(v+1)*width + h-1]
				+src[(v+1)*width + h]*2
				+src[(v+1)*width + h+1])
				+
				abs(-src[(v-1)*width + h-1]
				//-src[(v-1)*width + h]
				+src[(v-1)*width + h+1]
				-src[v*width	+ h-1]*2
				//+src[v*width	+ h]
				+src[v*width		+ h+1]*2
				-src[(v+1)*width + h-1]
				//+src[(v+1)*width + h]
				+src[(v+1)*width + h+1]);

		} // h 
	} // v 
	return 0;
}

// 
// Median filter
// 
// @param src:		IN	Parameter, Mandatory, Source buffer
// @param dst:		OUT	Parameter, Mandatory, Destination buffer
// @param width:	IN	Parameter, Mandatory, Image's width
// @param height:	IN	Parameter, Mandatory, Image's height
// @param winsize:	IN	Parameter, Optional,  Window size(Default & Minimum: 3x3)
// @return: result
int median(unsigned char *src, unsigned char *dst, int width, int height, int winsize){

	int h, v;
	int win_depth = 1;
	if (winsize < 4)
	{
		unsigned char med[9];

		for(v = win_depth; v < height-win_depth; v++){
			for( h = win_depth; h < width-win_depth; h++){
				med[0] = src[(v-1)*width + h-1];
				med[1] = src[(v-1)*width + h];
				med[2] = src[(v-1)*width + h+1];
				med[3] = src[v*width	 + h-1];
				med[4] = src[v*width	 + h];
				med[5] = src[v*width	 + h+1];
				med[6] = src[(v+1)*width + h-1];
				med[7] = src[(v+1)*width + h];
				med[8] = src[(v+1)*width + h+1];
				for(int i = 0; i < 9; i++)
				{
					for(int j = 0; j < i; j++)
					{
						if (med[i] < med[j])
						{
							unsigned char tmp = med[j];
							med[j] = med[i];
							med[i] = tmp;
							break;
						}
					}
				}

				dst[v*width +h] = med[4];

			} // h 
		} // v 
	}
	else {
		unsigned char *med = (unsigned char *)malloc(winsize * winsize);

		for(v = win_depth; v < height-win_depth; v++){
			for( h = win_depth; h < width-win_depth; h++){
				for( int i = 0; i < winsize; i++)
				{
					for( int j = 0; j < winsize; j++)
					{
						med[i*winsize + j] = src[(v-(i+winsize/2))*width + h-(j+winsize/2)];
					}
				}
				for(int i = 0; i < winsize*winsize; i++)
				{
					for(int j = 0; j < i; j++)
					{
						if (med[i] < med[j])
						{
							unsigned char tmp = med[j];
							med[j] = med[i];
							med[i] = tmp;
							break;
						}
					}
				}

				dst[v*width +h] = med[(winsize*winsize)/2];

			} // h 
		} // v 
		free(med);
	}
	return 0;
}

int	merge_3bin_rgb( unsigned char *prgb, 
				   unsigned char *pbinRed, unsigned char *pbinGreen, unsigned char *pbinBlue,
				   int width, int height)
{

	int v, h;
	for(v=0;v < height; v++){
		for(h=0;h < width; h++){
			*prgb++ = *pbinBlue++;
			*prgb++ = *pbinGreen++; 
			*prgb++ = *pbinRed++;   
		}
	}
	return 0;

}


#ifdef __cplusplus
}
#endif