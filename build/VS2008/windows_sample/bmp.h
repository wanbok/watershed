#ifndef __BMP_H__
#define __BMP_H__


/*************************************************************************
	
	file created on:	2002/08/30   19:33
	filename: 			Bmp.cpp
	author:				Andreas Hartl

		visit http://www.runicsoft.com for updates and more information

	purpose:	functions to load raw bmp data,
	                      to save raw bmp data,
						  to convert RGB data to raw bmp data,
						  to convert raw bmp data to RGB data
						  and to use the WinAPI to select
							a bitmap into a device context

	file updated on 2010/09/13
		in the 8 years since i first wrote this the windows file functions
		have changed their input from char* to LPCTSTR. 
		Updated this in the code here

**************************************************************************/

#include <windows.h>
#include <stdio.h>       // for memset

#define TRUE 1
#define FALSE 0
/*******************************************************************
BYTE* ConvertRGBToBMPBuffer ( BYTE* Buffer, int width, 
		int height, long* newsize )


This function takes as input an array of RGB values, it's width
and height.
The buffer gets then transformed to an array that can be used
to write to a windows bitmap file. The size of the array
is returned in newsize, the array itself is the
return value of the function.
Both input and output buffers must be deleted by the
calling function.

The input buffer is expected to consist of width * height
RGB triplets. Thus the total size of the buffer is taken as
width * height * 3.

The function then transforms this buffer so that it can be written 
to a windows bitmap file:
First the RGB triplets are converted to BGR.
Then the buffer is swapped around since .bmps store
images uside-down.
Finally the buffer gets DWORD ( 32bit ) aligned, 
meaning that each scanline ( 3 * width bytes ) gets
padded with 0x00 bytes up to the next DWORD boundary


*******************************************************************/

BYTE* ConvertRGBToBMPBuffer ( BYTE* Buffer, int width, int height, long* newsize );

/***************************************************************
BYTE* ConvertBMPToRGBBuffer ( BYTE* Buffer, 
		int width, int height )

This function takes as input the data array
from a bitmap and its width and height.
It then converts the bmp data into an RGB array.
The calling function must delete both the input
and output arrays.
The size of the returned array will be 
width * height * 3
On error the returb value is NULL, else the
RGB array.


The Buffer is expected to be the exact data read out
from a .bmp file.  
The function will swap it around, since images
are stored upside-down in bmps.
The BGR triplets from the image data will
be converted to RGB.
And finally the function removes padding bytes.
The returned arraay consits then of
width * height RGB triplets.

*****************************************************************/

BYTE* ConvertBMPToRGBBuffer ( BYTE* Buffer, int width, int height );

/***********************************************
bool LoadBMPIntoDC ( HDC hDC, LPCTSTR bmpfile )

Takes in a device context and the name of a
bitmap to load. If an error occurs the function
returns FALSE, else the contents of the bmp
are blitted to the HDC 

************************************************/

int LoadBMPIntoDC ( HDC hDC, LPCTSTR bmpfile );


/***************************************************************
bool SaveBMP ( BYTE* Buffer, int width, int height, 
		long paddedsize, LPCTSTR bmpfile )

Function takes a buffer of size <paddedsize> 
and saves it as a <width> * <height> sized bitmap 
under the supplied filename.
On error the return value is FALSE.

***************************************************************/

int SaveBMP ( BYTE* Buffer, int width, int height, long paddedsize, LPCTSTR bmpfile );

/*******************************************************************
BYTE* LoadBMP ( int* width, int* height, long* size 
		LPCTSTR bmpfile )

The function loads a 24 bit bitmap from bmpfile, 
stores it's width and height in the supplied variables
and the whole size of the data (padded) in <size>
and returns a buffer of the image data 

On error the return value is NULL. 

  NOTE: make sure you [] delete the returned array at end of 
		program!!!
*******************************************************************/

BYTE* LoadBMP ( int* width, int* height, long* size, LPCTSTR bmpfile );

void TestBMPCopy (LPCTSTR input, LPCTSTR output);

void TestBMPCopy2(LPCTSTR input, LPCTSTR output);


#endif
//void main ();