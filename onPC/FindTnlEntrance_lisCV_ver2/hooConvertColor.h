#include "stdafx.h"
#include "cv.h"
#include "highgui.h"

IplImage* hooConvertColor(IplImage* inputImage, CvSize sz)
{
	IplImage* outputImage = cvCreateImage(sz, IPL_DEPTH_8U, 3);

	int i, j;
	uchar* ptr;
	uchar* ptr2_1;
	uchar* ptr2_2;
	uchar* ptr2_3;


	for(i=0 ; i<inputImage->width ; i++)
	{
		for(j=0 ; j<inputImage->height ; j++)
		{
			ptr = &((uchar*)(inputImage->imageData + inputImage->widthStep*j))[i];
			ptr2_1 = &((uchar*)(outputImage->imageData + outputImage->widthStep*j))[i*3];
			ptr2_2 = &((uchar*)(outputImage->imageData + outputImage->widthStep*j))[i*3+1];
			ptr2_3 = &((uchar*)(outputImage->imageData + outputImage->widthStep*j))[i*3+2];

			ptr2_1 = ptr; ptr2_2 = ptr; ptr2_3 = ptr;
		}
	}

	return outputImage;

}