#include "stdafx.h"
#include "cv.h"
#include "highgui.h"


IplImage* hooEdgeFilter(IplImage* inputImage, CvSize sz)
{
	// Sobel filtering
	IplImage* input = cvCreateImage(sz, IPL_DEPTH_8U, 1);
	IplImage* output= cvCreateImage(sz, IPL_DEPTH_16S, 1);

	cvConvert(inputImage, input);
	cvSobel(input, output, 1, 0, 3 );
	cvConvert(output, inputImage);
	/// end of Sobel filtering
	
	// making the image binary edge filtered image using treshold value
	IplImage *outputImage;
	outputImage = cvCreateImage(sz, IPL_DEPTH_8U, 1);
	cvSetZero(outputImage);

	int i, j;
	uchar* ptr;
	uchar* ptr2;
	for(i=0 ; i<inputImage->width ; i++)
	{
		for(j=0 ; j<inputImage->height ; j++)
		{
			ptr = &((uchar*)(inputImage->imageData + inputImage->widthStep*j))[i];
			ptr2 = &((uchar*)(outputImage->imageData + outputImage->widthStep*j))[i];
			if(*ptr > 50) *ptr2 = 255;			// threshold value is 50
		}										// when bigger then 100, then it's set to 255
	}
	/// end of making binaried image

	return outputImage;
}
