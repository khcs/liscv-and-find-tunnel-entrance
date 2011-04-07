//#include <math.h>
#include "lisCV_basicFunctions.h"

#include "math.h"

#include <stdlib.h>

void lisCV_houghElementSwap(lisCV_HoughResult *a, lisCV_HoughResult *b);
void lisCV_houghElementQuicksort(lisCV_HoughResult* houghRaum, int left, int right); 

lisCV_Image* lisCV_rgb2gray(lisCV_Image* Image)
{
	int i, j;
	int r, g, b;
	double temp;
	unsigned char gray;

	int width = Image->Infoheader.width;
	int height= Image->Infoheader.height;

	lisCV_Image* nImage = (lisCV_Image*)malloc(sizeof(lisCV_Image));
	unsigned char* image_data = (unsigned char*)malloc(sizeof(unsigned char)*width*height*3);
	nImage->imageData = image_data;

	nImage->Infoheader = Image->Infoheader;
	nImage->Header = Image->Header;
	nImage->width = Image->width;
	nImage->height = Image->height;

	for (j=height-1; j>=0 ; j--){
		for (i=0; i<width; i++){
			b = (int)Image->imageData[(width*j+i)*3];
			g = (int)Image->imageData[(width*j+i)*3+1];
			r = (int)Image->imageData[(width*j+i)*3+2];

			temp = (0.2126*(int)r + 0.7152*(int)g + 0.0722*(int)b);
			gray = (unsigned char)temp;

			nImage->imageData[(width*j+i)*3]   = gray;
			nImage->imageData[(width*j+i)*3+1] = gray;
			nImage->imageData[(width*j+i)*3+2] = gray;
		}			
	}

	return nImage;
}

//lisCV_Image* lisCV_copyImage(lisCV_Image* o_Image)
//{
//	int i, j;
//	int r, g, b;
//	double temp;
//	unsigned char gray;
//
//	int width = o_Image->Infoheader.width;
//	int height= o_Image->Infoheader.height;
//	
//	lisCV_Image* n_Image = (lisCV_Image*)malloc(sizeof(lisCV_Image));
//	unsigned char* image_data1 = malloc(sizeof(unsigned char)*width*height*3);
//	n_Image->imageData = image_data1;
//	
//	n_Image->Infoheader = o_Image->Infoheader;
//
//	for (j=height-1; j>=0 ; j--){
//		for (i=0; i<width; i++){
//			n_Image->imageData[(width*j+i)*3]   = o_Image->imageData[(width*j+i)*3];
//			n_Image->imageData[(width*j+i)*3+1] = o_Image->imageData[(width*j+i)*3+1];
//			n_Image->imageData[(width*j+i)*3+2] = o_Image->imageData[(width*j+i)*3+2];
//		}			
//	}
//	
//	return n_Image;
//}

lisCV_Image* lisCV_sobel(lisCV_Image* Image)
{
//	double op_x[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
//	double op_y[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};

	double pixel, image2, thresh=80, temp0, temp1;
	int i, j, k, l;

	int height = Image->Infoheader.height;
	int width =  Image->Infoheader.width;

	lisCV_Image* edgeImage = (lisCV_Image*)malloc(sizeof(lisCV_Image));
	//unsigned char* image_data = new unsigned char[width*height*3];
	unsigned char* image_data = (unsigned char*)malloc(sizeof(unsigned char)*width*height*3);
	edgeImage->imageData = image_data;

	edgeImage->Header = Image->Header;
	edgeImage->Infoheader = Image->Infoheader;

	edgeImage->Infoheader.height = Image->height;
	edgeImage->Infoheader.width = Image->width;

	edgeImage->height = Image->height; 
	edgeImage->width = Image->width;   

	for(i=0; i<=height-1; i++){
		for(j=0; j<width; j++){
			pixel = (double) Image->imageData[(width*i+j)*3];
			if (i==0 || i==height || j==0 || j==width)
				image2 = 0;
			else{
				temp0=0; temp1=0;

				/* Erste Variante */
				//for(k=-1; k<=1; k++){
					//for(l=-1; l<=1; l++){
						/* Als eine Erinnerung, wo ich eine Schwierigkeit gehabt habe...

							//temp0 = temp0 + (double)(Image->imageData[(width*(i+k)+j+l)*3] * op_x[1+k + (1+l)*3]);
							//temp1 = temp1 + (double)(Image->imageData[(width*(i+k)+j+l)*3] * op_y[1+k + (1+l)*3]);
						*/
						
						/* Zweite Variante */
						temp0 = abs(	(	((int)(Image->imageData[(width*(i+1)+j-1)*3]) & 0x000000FF 		)	+ 		//P3_1
									(	(	(int)(Image->imageData[(width*(i+1)+j)*3]) & 0x000000FF)<<1		)	+  	//P3_2
									(		(int)(Image->imageData[(width*(i+1)+j+1)*3]) & 0x000000FF 		)			//P3_3
								   )	-
								   (	(	(int)(Image->imageData[(width*(i-1)+j-1)*3]) & 0x000000FF		) 	+ 		//P1_1
									(	(	(int)(Image->imageData[(width*(i-1)+j)*3]) & 0x000000FF)<<1		)	+ 		//P1_2
									(		(int)(Image->imageData[(width*(i-1)+j+1)*3])& 0x000000FF 		) 			//P1_3
								   ) 
						);

						if (temp0>255)
							temp0=255;

						temp1 = abs(	(	((int)(Image->imageData[(width*(i-1)+j+1)*3]) & 0x000000FF 		)	+ 		//P3_1
									(	(	(int)(Image->imageData[(width*(i)+j+1)*3]) & 0x000000FF)<<1		)	+  	//P3_2
									(		(int)(Image->imageData[(width*(i+1)+j+1)*3]) & 0x000000FF 		)			//P3_3
								   )	-
								   (	(	(int)(Image->imageData[(width*(i-1)+j-1)*3]) & 0x000000FF		) 	+ 		//P1_1
									(	(	(int)(Image->imageData[(width*(i)+j-1)*3]) & 0x000000FF)<<1		)	+ 		//P1_2
									(		(int)(Image->imageData[(width*(i+1)+j-1)*3])& 0x000000FF 		) 			//P1_3
								   ) 
						);

						if (temp1>255)
							temp1=255;

						image2	=	(int)(temp0+temp1) >> 1;	

						//temp0 = temp0 + ((double)Image->imageData[(width*(i+k)+j+l)*3] * op_x[1+k + (1+l)*3]);
						//temp1 = temp1 + ((double)Image->imageData[(width*(i+k)+j+l)*3] * op_y[1+k + (1+l)*3]);
					//}
				//}


				/*
				temp0 = fabs(temp0);
				if(temp0>255) temp0 = 255;
				temp1 = fabs(temp1);
				if(temp1>255) temp1 = 255;

				image2 = (temp0+temp1)/2;
				*/

				if(image2 > thresh)
					image2 = 255;
				else
					image2 = 0;
			}
			/* Als eine Erinnerung, wo ich Schwierigkeit gehabt habe...

				Image2->imageData[(width*(i)+j)*3] = (unsigned char)image2;
				Image2->imageData[(width*(i)+j)*3+1] = (unsigned char)image2;
				Image2->imageData[(width*(i)+j)*3+2] = (unsigned char)image2;
			*/
			
//			*(edgeImage->imageData + (width*i + j)*3)   = (unsigned char) image2;
//			*(edgeImage->imageData + (width*i + j)*3+1) = (unsigned char) image2;
//			*(edgeImage->imageData + (width*i + j)*3+2) = (unsigned char) image2;		

			*(image_data + (width*i + j)*3)   = (unsigned char) image2;
			*(image_data + (width*i + j)*3+1) = (unsigned char) image2;
			*(image_data + (width*i + j)*3+2) = (unsigned char) image2;		
		
		}
	}

	// To handle the memory problem
	//free(image_data);
	///

	return edgeImage;
	//return Image;
}



lisCV_HoughResult* lisCV_houghTransform(lisCV_Image* Image)
{
	int i, j, k;
	double pi = 3.1415;

	int height = Image->Infoheader.height;
	int width  = Image->Infoheader.width;

	/*
	// M : height, N : width 
	int M = Image->Infoheader.height;
	int N = Image->Infoheader.width;

	// theta = linspace(-90, 0, ceil(90/dtheta) + 1); % dtheta = 1
	ntheta = 91;
	double theta[ntheta];

	for(i=0; i<ntheta; i++){
		theta[i] = -90 + i;
	}
	
	//theta = [theta-fliplr(theta(2:end-1))] -- not written
	
	double D = sqrt((M-1)*(M-1) + (N-1)*(N-1));
	int drho = 1;
	double q = ceil(D/drho);
	int nrho = (int)(2*q-1);
	double rho
	*/
 	
	int middle_x = (int)floor((double)width/2);
	int middle_y = (int)floor((double)height/2);

	int quarter_x = (int)floor((double)width/4);
	int quarter_y = (int)floor((double)height/4);

	int quarter3_x = (int)floor((double)width*0.75/*3/4*/);
	int quarter3_y = (int)floor((double)height*0.75/*3/4*/);


	int nTheta_limit = 360;
	int rho_limit = (width+height)*2;
	int nWhitePoints = 0;
/*	for(j=0; j<height; j++){
		for(i=0; i<width; i++){
			if( Image->imageData[(width*j+i)*3] == 255 )
				nWhitePoints++;
		}
	}
*/
														// width     * height
	//lisCV_HoughResult* houghRaum = new lisCV_HoughResult[nTheta_limit*rho_limit];
	lisCV_HoughResult* houghRaum = (lisCV_HoughResult*)malloc(sizeof(lisCV_HoughResult)*nTheta_limit*rho_limit);
	////lisCV_HoughResult houghRaum[360][(640+480)];
	//lisCV_HoughResult* houghResult = new lisCV_HoughResult[nTheta_limit*rho_limit];
	lisCV_HoughResult* houghResult = (lisCV_HoughResult*)malloc(sizeof(lisCV_HoughResult)*nTheta_limit*rho_limit);

	for(j=0; j<rho_limit; j++){
		for(i=0; i<nTheta_limit; i++){
			houghRaum[(nTheta_limit*j)+i].intensity = 0;
			houghRaum[(nTheta_limit*j)+i].rho = 0;
			houghRaum[(nTheta_limit*j)+i].theta = 0;
			
			houghResult[(nTheta_limit*j)+i].intensity = 0;
			houghResult[(nTheta_limit*j)+i].rho = 0;
			houghResult[(nTheta_limit*j)+i].theta = 0;
			
		}
	}	

	double theta, sin, cos;
	int rho;
	int x, y;

	for(j=1; j<height-1; j++){
		for(i=1; i<width-1; i++){
			if(j != middle_y && i != middle_x){
				if( Image->imageData[(width*j+i)*3] == 255 ){ 
					x = i-middle_x; y= j-middle_y;
					for(k=0; k<nTheta_limit; k++){
						theta = k*pi/180;
//////////////// beginning of About SINE and COSINE ///////////////
//						rho = (int)floor(x*cos(theta)+y*sin(theta));
						
						sin = lisCV_sin(theta);
						cos = lisCV_cos(theta);
						
						// compute rho
						rho = (int)floor(x*cos+y*sin);
							
//////////////// end of About SINE and COSINE /////////////////////
						if (rho<0) rho = (height+width)+rho*(-1);
						houghRaum[(nTheta_limit*rho)+k].intensity++;
					}
				}
			}
		}
	}

	for(j=0; j<rho_limit; j++){
		for(i=0; i<nTheta_limit; i++){
			houghResult[i*j].rho = j;
			houghResult[i*j].theta = i*0.0174/*pi/180*/;
			houghResult[i*j].intensity = houghRaum[(nTheta_limit*j)+i].intensity;
			//if (houghRaum[(nTheta_limit*j)+i].intensity > 0) printf("%d %d %d \n", i, j, houghRaum[(nTheta_limit*j)+i].intensity);
		}
	}


	lisCV_houghElementQuicksort(houghResult, 0, nTheta_limit*rho_limit-1);

	//
	//for(i=0; i< 6; i++){
	//	printf("%u %3f %d \n", houghResult[i].intensity, houghResult[i].theta, houghResult[i].rho);
	//}
	//
	
	// To handle the memory problem
	free(houghRaum);
	///

	return houghResult;
}


void lisCV_houghElementQuicksort(lisCV_HoughResult* houghRaum, int left, int right)
{
	unsigned int pivot, i, j;
	if(left < right){
		i = left; j = right+1;
		pivot = houghRaum[left].intensity;
		do{
			do
				i++;
			while (houghRaum[i].intensity > pivot);
			
			do
				j--;
			while (houghRaum[j].intensity < pivot);
			if (i<j)
				lisCV_houghElementSwap(&houghRaum[i], &houghRaum[j]);
		}while(i<j);
		lisCV_houghElementSwap(&houghRaum[left], &houghRaum[j]);
		lisCV_houghElementQuicksort(houghRaum, left, j-1);
		lisCV_houghElementQuicksort(houghRaum, j+1, right);
	}
}

lisCV_Image* lisCV_drawLine(lisCV_Image* Image, lisCV_Point start_point, lisCV_Point end_point, lisCV_Color color)
{
	int i;

	int height = Image->Infoheader.height;
	int width  = Image->Infoheader.width;

	double xToGo = 0;
	double yToGo = 0;
	double beforeCeilValue = 0;

	int xPoint;
	int yPoint;
	double gradient;

	if( start_point.y < end_point.y ){
		gradient = (double)(end_point.x-start_point.x)/(double)(end_point.y-start_point.y);
		if (gradient <= 1){
			xPoint = start_point.x;
			yPoint = start_point.y;
			if (start_point.x < end_point.x){
				for(i=start_point.y; i<end_point.y; i++){
					if (i > height) break;
					beforeCeilValue = ceil(xToGo);
					xToGo = xToGo + gradient;
					if (beforeCeilValue < xToGo)
						xPoint = xPoint+1;
					if (xPoint > width) break;
					Image->imageData[(width*i+xPoint)*3] = color.B;
					Image->imageData[(width*i+xPoint)*3+1] = color.G;
					Image->imageData[(width*i+xPoint)*3+2] = color.R;
				}
			}
			else if (start_point.x > end_point.x){
				for(i=start_point.x; i>end_point.x; i--){
					if (i < 1) break;
					beforeCeilValue = ceil(yToGo);
					yToGo = yToGo - 1/gradient;
					if (beforeCeilValue < yToGo)
						yPoint = yPoint + 1;
					if (yPoint > height) break;
					Image->imageData[(width*yPoint+i)*3] = color.B;
					Image->imageData[(width*yPoint+i)*3+1] = color.G;
					Image->imageData[(width*yPoint+i)*3+2] = color.R;
				}
			}
			else{
				for(i=start_point.y; i<end_point.y; i++){
					if (i > height) break;
					xPoint = start_point.x;
					Image->imageData[(width*i+xPoint)*3] = color.B;
					Image->imageData[(width*i+xPoint)*3+1] = color.G;
					Image->imageData[(width*i+xPoint)*3+2] = color.R;
				}
			}
		}
		else{ // if gradient > 1
			xPoint = start_point.x;
			yPoint = start_point.y;
			if (start_point.x < end_point.x){
				for (i=start_point.x; i<end_point.x; i++){
					if (i > width) break;
					beforeCeilValue = ceil(yToGo);
					yToGo = yToGo + 1/gradient;
					if (beforeCeilValue < yToGo)
						yPoint = yPoint+1;
					if (yPoint > height) break;
					Image->imageData[(width*yPoint+i)*3] = color.B;
					Image->imageData[(width*yPoint+i)*3+1] = color.G;
					Image->imageData[(width*yPoint+i)*3+2] = color.R;
				}
			}
			else if (start_point.x > end_point.x){
				for (i=start_point.x; i>end_point.x; i--){					
					if (i < 0) break;
					beforeCeilValue = ceil(yToGo);
					yToGo = yToGo - 1/gradient;
					if (beforeCeilValue < xToGo)
						yPoint = yPoint - 1;
					if (yPoint < 1) break;
					Image->imageData[(width*yPoint+i)*3] = color.B;
					Image->imageData[(width*yPoint+i)*3+1] = color.G;
					Image->imageData[(width*yPoint+i)*3+2] = color.R;
				}
			}
			else {
				for(i=start_point.y; i<end_point.y; i++){
					if (i > height) break;
					xPoint = start_point.x;
					Image->imageData[(width*i+xPoint)*3] = color.B;
					Image->imageData[(width*i+xPoint)*3+1] = color.G;
					Image->imageData[(width*i+xPoint)*3+2] = color.R;
				}
			}
		}
	}
	else if (start_point.y > end_point.y){
		gradient = (-1)*(double)(end_point.x-start_point.x)/(double)(end_point.y-start_point.y);
		if (abs(gradient) <= 1){
			xPoint = start_point.x;
			yPoint = start_point.y;
			if (start_point.x < end_point.x){
				for(i=start_point.y; i>end_point.y; i--){
					if (i < 1) break;
					beforeCeilValue = ceil(xToGo);
					xToGo = xToGo + gradient;
					if (beforeCeilValue < xToGo)
						xPoint = xPoint + 1;
					if (xPoint > width) break;
					Image->imageData[(width*i+xPoint)*3] = color.B;
					Image->imageData[(width*i+xPoint)*3+1] = color.G;
					Image->imageData[(width*i+xPoint)*3+2] = color.R;
				}
			}
			else if (start_point.x > end_point.x){//////////////////////////////////////////////////////////////////////////////
				for(i=start_point.y; i>end_point.y; i--){
					if (i < 1) break;
					beforeCeilValue = ceil(xToGo);
					xToGo = xToGo - gradient;
					if (beforeCeilValue < xToGo)
						xPoint = xPoint - 1;
					if (xPoint < 1) break;
					Image->imageData[(width*i+xPoint)*3] = color.B;
					Image->imageData[(width*i+xPoint)*3+1] = color.G;
					Image->imageData[(width*i+xPoint)*3+2] = color.R;
				}
			}
			else{
				for(i=start_point.y; i>end_point.y; i--){
					if (i < 1) break;
					xPoint = start_point.x;
					Image->imageData[(width*i+xPoint)*3] = color.B;
					Image->imageData[(width*i+xPoint)*3+1] = color.G;
					Image->imageData[(width*i+xPoint)*3+2] = color.R;
				}
			}
		}
		else{ // if gradient > 1
			xPoint = start_point.x;
			yPoint = start_point.y;
			if (start_point.x < end_point.x){
				for(i=start_point.x; i<end_point.x; i++){
					if (i > width) break;
					beforeCeilValue = ceil(yToGo);
					yToGo = yToGo + 1/gradient;
					if (beforeCeilValue < yToGo)
						yPoint = yPoint - 1;
					if (yPoint < 1) break;
					Image->imageData[(width*yPoint+i)*3] = color.B;
					Image->imageData[(width*yPoint+i)*3+1] = color.G;
					Image->imageData[(width*yPoint+i)*3+2] = color.R;
				}
			}
			else if (start_point.x > end_point.x){
				for(i=start_point.x; i>end_point.x; i--){
					if (i < 1) break;
					beforeCeilValue = ceil(yToGo);
					yToGo = yToGo - 1/gradient;
					if (beforeCeilValue < yToGo)
						yPoint = yPoint - 1;
					if (yPoint < 1) break;
					Image->imageData[(width*yPoint+i)*3] = color.B;
					Image->imageData[(width*yPoint+i)*3+1] = color.G;
					Image->imageData[(width*yPoint+i)*3+2] = color.R;
				}
			}
			else{
				for(i=start_point.y; i>end_point.y; i--){
					if (i < 1) break;
					xPoint = start_point.x;
					Image->imageData[(width*i+xPoint)*3] = color.B;
					Image->imageData[(width*i+xPoint)*3+1] = color.G;
					Image->imageData[(width*i+xPoint)*3+2] = color.R;					
				}
			}
		}
	}
	else{
		if (start_point.x < end_point.x){
			for(i=start_point.x; i<end_point.x; i++){
				if (i > width) break;
				Image->imageData[(width*start_point.y+i)*3] = color.B;
				Image->imageData[(width*start_point.y+i)*3+1] = color.G;
				Image->imageData[(width*start_point.y+i)*3+2] = color.R;
			}
		}
		else if(start_point.x > end_point.x){
			for(i=start_point.x; i>end_point.x; i--){
				if (i < 1) break;
				Image->imageData[(width*start_point.y+i)*3] = color.B;
				Image->imageData[(width*start_point.y+i)*3+1] = color.G;
				Image->imageData[(width*start_point.y+i)*3+2] = color.R;
			}
		}
	}

	return Image;
}

void lisCV_houghElementSwap(lisCV_HoughResult *a, lisCV_HoughResult *b)
{
	lisCV_HoughResult tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

