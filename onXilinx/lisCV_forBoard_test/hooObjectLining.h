//#include "stdafx.h"

int objectCheckLeft(lisCV_Image* inputImage, int x, int y, int count);	// check whether the line goes continuously left
int objectCheckRight(lisCV_Image* inputImage, int x, int y, int count);// check whether the line goes continuously right


lisCV_Image* hooObjectLining(lisCV_Image* inputImage)
{
	//CvSize sz = cvGetSize(inputImage);								// get the size of image
	int i, j;
	int width = inputImage->Infoheader.width;
	int height= inputImage->Infoheader.height;

	//lisCV_Image *outputImage = new lisCV_Image;											// output image
	lisCV_Image *outputImage = (lisCV_Image*)malloc(sizeof(lisCV_Image));
	unsigned char* imageData = (unsigned char*)malloc(sizeof(unsigned char)*width*height*3);
	//unsigned char* imageData = new unsigned char[width*height*3];

	outputImage->Header = inputImage->Header;
	outputImage->Infoheader = inputImage->Infoheader;

	outputImage->height = inputImage->height;
	outputImage->width = inputImage->width;

	for (j=height-1; j>=0 ; j--){
		for (i=0; i<width; i++){
			*(imageData + (width*j+i)*3 )  = 0;
			*(imageData + (width*j+i)*3+1) = 0;
			*(imageData + (width*j+i)*3+2) = 0;
		}			
	}

	/* how to handle pixel values in openCV */
	// I(x,y) ~ ((uchar*)(img->imageData + img->widthStep*y))[x]	

	int ptr;
	int ptr2;
	for(i=20 ; i<inputImage->Infoheader.width-20 ; i=i+2)
	{
		for(j=inputImage->Infoheader.height-20 ; j>20 ; j=j-2)
		{
			ptr = (int)inputImage->imageData[(width*j+i)*3];
			ptr2 = (int)*(imageData + (width*j+i)*3 );
			if( objectCheckLeft(inputImage, i, j, 0) > 2 || objectCheckRight(inputImage, i, j, 0) > 2 ){
				//ptr2 = 255;		// the continuousity should be larger than 3 here
				*(imageData + (width*j+i)*3 )   = 255;
				*(imageData + (width*j+i)*3 +1) = 255;
				*(imageData + (width*j+i)*3 +2) = 255;

				//ptr = 0;			// for the case of difficulty with recognizing lanes or
			}						// finding tunnel entrances, change it to > 2
			else{ // ptr2 = 0;
				*(imageData + (width*j+i)*3 )   = 0;
				*(imageData + (width*j+i)*3 +1) = 0;
				*(imageData + (width*j+i)*3 +2) = 0;
			}
		}
	}

	outputImage->imageData = imageData;

	// To handle the memory problem
	//free (imageData);
	///

	return outputImage;
}


int objectCheckLeft(lisCV_Image* inputImage, int x, int y, int count)
{	
	int i, j;
	int ptr;

	int width = inputImage->Infoheader.width;
	int height= inputImage->Infoheader.height;
	
	i=x;
	for(j=y ; j>y-8 ; j=j-2)
	{
		ptr = (int)inputImage->imageData[(width*j+i)*3];
		if( ptr == 255 ) count++;
		else if( (int)inputImage->imageData[(width*j+i-2)*3] == 255 ){
			i = i-2;
			count = objectCheckLeft(inputImage, i, j, count);
		}
		else break;

		if (count>5) break; // NOTFALL : Das war nicht der Fall beim OpenCV Version von dem Program
	}

	return count;
}

int objectCheckRight(lisCV_Image* inputImage, int x, int y, int count)
{	
	int i, j;
	int ptr;

	int width = inputImage->Infoheader.width;
	int height= inputImage->Infoheader.height;
	
	i=x;
	for(j=y ; j>y-8 ; j=j-2)
	{
		ptr = (int)inputImage->imageData[(width*j+i)*3];
		if( ptr == 255 ) count++;
		else if( (int)inputImage->imageData[(width*j+i-2)*3] == 255 ){
			i = i+2;			// actually I have not meant here [i-2]. I meant [i+2]. But error...
			count = objectCheckRight(inputImage, i, j, count);
		}
		else break;

		if (count>5) break; // NOTFALL : Das war nicht der Fall beim OpenCV Version von dem Program
	}

	return count;
}
