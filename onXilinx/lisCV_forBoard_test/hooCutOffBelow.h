//#include "stdafx.h"

lisCV_Image* hooCutOffBelow(lisCV_Image* inputImage, int o_sz_height, int o_sz_width, int r_sz_height, int r_sz_width)
{
	/*
	IplImage *outputImage;

	CvRect autoFensterGroesse;
	autoFensterGroesse.x = 0;
	autoFensterGroesse.y = o_sz.height-r_sz.height;
	autoFensterGroesse.height = r_sz.height;
	autoFensterGroesse.width = r_sz.width;
	cvSetImageROI(inputImage, autoFensterGroesse);

	outputImage = cvCreateImage(r_sz, IPL_DEPTH_8U, channels);
	cvCopy(inputImage, outputImage, NULL);
	*/
	
	lisCV_Image *outputImage = inputImage;
	int i, j;
	int width = outputImage->Infoheader.width;
	int height= outputImage->Infoheader.height;
	
	for (j=height-1; j>=r_sz_height ; j--){
		for (i=0; i<r_sz_width; i++){
			inputImage->imageData[(width*j+i)*3]   = 0;
			inputImage->imageData[(width*j+i)*3+1] = 0;
			inputImage->imageData[(width*j+i)*3+2] = 0;
		}			
	}

	//lisCV_Image *outputImage = new lisCV_Image;
	//lisCV_Image * outputImage = (lisCV_Image*)malloc(sizeof(lisCV_Image));
	outputImage->Infoheader = inputImage->Infoheader;
	outputImage->Header = inputImage->Header;

	//outputImage->Infoheader.height = r_sz_height;
	//outputImage->Infoheader.width  = r_sz_width;
	outputImage->height = r_sz_height;
	outputImage->width  = r_sz_width;
	//outputImage->nSize = (r_sz_height*r_sz_width);
	//outputImage->Header.size = (r_sz_height*r_sz_width+18)*3;
	//outputImage->Infoheader.imagesize = (r_sz_height*r_sz_width*3);

	//unsigned char* imageData = new unsigned char[r_sz_width*r_sz_height*3];
//	unsigned char* imageData = (unsigned char*)malloc(sizeof(unsigned char)*r_sz_width*r_sz_height*3);

//	for (j=height-1; j>=0 ; j--){
//		for (i=0; i<width; i++){
//			*(imageData + (width*j+i)*3 )  = inputImage->imageData[(width*j+i)*3];
//			*(imageData + (width*j+i)*3+1) = inputImage->imageData[(width*j+i)*3+1];
//			*(imageData + (width*j+i)*3+2) = inputImage->imageData[(width*j+i)*3+2];
//		}			
//	}
		
//	outputImage->imageData = imageData;

	return outputImage;
}
