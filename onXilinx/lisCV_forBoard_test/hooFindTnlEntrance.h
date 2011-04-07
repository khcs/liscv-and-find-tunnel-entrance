//#include "stdafx.h"
//#include <stdlib.h>
#include "hooGetCrossPoints.h"

typedef struct TunnelInfos
{
	lisCV_Image* image;
	lisCV_Point pt1;
   lisCV_Point pt2;
	int tunnelFound;
	CrossingPoints cxPoints;
}TunnelInfos;

TunnelInfos hooFindTnlEntrance(lisCV_Image* src, lisCV_Image* inputImage, lisCV_Image* grayInputImage, CrossingPoints cxPoints,lisCV_Image* colorImage, int ifDoLaneDetection ) //,int brightness)
{							
// src - sobel edge filtered image
// inputImage - edge filtered image after object lining
// grayInputImage - gray scaled original image

	//lisCV_Image* dst = new lisCV_Image;
	lisCV_Image* dst = (lisCV_Image*)malloc(sizeof(lisCV_Image));
	//lisCV_Image* color_dst = new lisCV_Image;
	lisCV_Image* color_dst = (lisCV_Image*)malloc(sizeof(lisCV_Image));

	dst=src;
	color_dst=colorImage;
	//dst = lisCV_copyImage(src);
//	color_dst = lisCV_copyImage(colorImage);

	Lane lane1, lane2, lane3;
	lane1.isUsed=0; lane2.isUsed=0; lane3.isUsed=0;

	// Lane Recognition
	int i, j, k, l;
	double pi = 3.1415;
	int rho, coef;
	double theta;
	int height = dst->height; 
	int width  = dst->width;  
	int middle_x = (int)(width/2);
	int middle_y = (int)(height/2);		
	
	double sin, cos, tan;
	
//	XTime start_time, stop_time;
	
//			int writeaddr5, baseaddr5 = 0x1400000, temp5;			
//			for(j=dst->Infoheader.height-1;j>=0;j--){
//				for(i=0;i<dst->Infoheader.width;i++){	
//					temp5 = ((((*(grayInputImage->imageData + (dst->Infoheader.width*j + i)*3+2) <<8) | *(grayInputImage->imageData + (dst->Infoheader.width*j + i)*3+1)) << 8) | *(grayInputImage->imageData + (dst->Infoheader.width*j + i)*3) );
//					writeaddr5 = baseaddr5+(j*1024+i)*4;
//					XIo_Out32(writeaddr5, temp5);
//				}
//			}

	
	lisCV_Point line_start, line_end1, line_end2, line_middle; line_middle.x = middle_x; line_middle.y = middle_y;
	lisCV_Color line_color;
	line_color.R = 0; line_color.G = 255; line_color.B = 0;

//	XTime_GetTime(&start_time);
	if (ifDoLaneDetection == 1){

		lisCV_HoughResult *houghRaum;
		houghRaum = lisCV_houghTransform(dst);

		j=0, k=0; // i. 290<theta<310
		for (i=0; i<500; i++){
			rho = houghRaum[i].rho;
			theta = houghRaum[i].theta;	

			if (rho > (width+height)) rho = (rho - (width+height))*(-1); // *(-1)

			if (theta > /*290*pi/180*/5.0613 && theta < /*310*pi/180*/5.4103 && (rho<100 || rho>-100) && j<1){ //&& rho < 0){			

				cos = lisCV_cos(theta);
				sin = lisCV_sin(theta);
				tan = lisCV_tan(theta);
				line_start.x = middle_x + (int)(rho*cos);
				line_start.y = middle_y + (int)( (rho*sin) - (colorImage->Infoheader.height - height)/2 );	

				tan = cos;
				cos = sin;
				sin = tan;
		
				tan = lisCV_tan(1.5708/*pi/2*/-theta);				
				line_end1.x = (line_start.x-1000);
				line_end1.y = (int)(line_start.y+1000*tan);

				line_end2.x = (line_start.x+1000);
				line_end2.y = (int)(line_start.y-1000*tan);

				colorImage = lisCV_drawLine(colorImage, line_start, line_end1, line_color);
				colorImage = lisCV_drawLine(colorImage, line_start, line_end2, line_color);

				lane1.pt1.x = line_end1.x; lane1.pt1.y = line_end1.y;
				lane1.pt2.x = line_end2.x; lane1.pt2.y = line_end2.y;
				lane1.isUsed = 1;

				j++;
			}

			if (rho > (width+height)) rho = (rho-(width+height));
			
			if(theta > /*230*pi/180*/4.0141 && theta < /*260*pi/180*/4.5355 && (rho<100 || rho>-100) && k<1 ){ //&& rho > 0 ){			

				cos = lisCV_cos(theta);
				sin = lisCV_sin(theta);
				tan = lisCV_tan(theta);;
				line_start.x = middle_x - (int)(rho*cos);
				line_start.y = middle_y - (int)( (rho*sin) - (colorImage->Infoheader.height - height)/2 );

				tan = cos;
				cos = sin;
				sin = tan;
				tan = lisCV_tan(1.5708/*pi/2*/-theta);
				line_end1.x = (line_start.x-1000);
				line_end1.y = (int)(line_start.y+1000*tan);

				line_end2.x = (line_start.x+1000);
				line_end2.y = (int)(line_start.y-1000*tan);

				colorImage = lisCV_drawLine(colorImage, line_start, line_end1, line_color);
				colorImage = lisCV_drawLine(colorImage, line_start, line_end2, line_color);

				lane2.pt1.x = line_end1.x; lane2.pt1.y = line_end1.y;
				lane2.pt2.x = line_end2.x; lane2.pt2.y = line_end2.y;
				lane2.isUsed = 1;

				k++;
			}

			if (j>0 && k>0) break;
		}
		
		//j=0; //ii. 230<theta<260
		//for (i=0; i<500; i++){
		//	rho = houghRaum[i].rho;
		//	theta = houghRaum[i].theta;
		//	
		//	if (rho > (width+height)) rho = (rho-(width+height));
		//	
		//	if(theta > /*230*pi/180*/4.0141 && theta < /*260*pi/180*/4.5355 && (rho<100 || rho>-100) ){ //&& rho > 0 ){			

		//		cos = lisCV_cos(theta);
		//		sin = lisCV_sin(theta);
		//		tan = lisCV_tan(theta);;
		//		line_start.x = middle_x - (int)(rho*cos);
		//		line_start.y = middle_y - (int)(rho*sin);

		//		tan = cos;
		//		cos = sin;
		//		sin = tan;
		//		tan = lisCV_tan(1.5708/*pi/2*/-theta);
		//		line_end1.x = (line_start.x-1000);
		//		line_end1.y = (int)(line_start.y+1000*tan);

		//		line_end2.x = (line_start.x+1000);
		//		line_end2.y = (int)(line_start.y-1000*tan);

		//		colorImage = lisCV_drawLine(colorImage, line_start, line_end1, line_color);
		//		colorImage = lisCV_drawLine(colorImage, line_start, line_end2, line_color);

		//		lane2.pt1.x = line_end1.x; lane2.pt1.y = line_end1.y;
		//		lane2.pt2.x = line_end2.x; lane2.pt2.y = line_end2.y;
		//		lane2.isUsed = 1;

		//		j++;
		//	}
		//	
		//	if (j>1) break;
		//
		//}

//		j=0; // iii. 260<theta<280
//		for (i=0; i<1000; i++){
//			rho = houghRaum[i].rho;
//			theta = houghRaum[i].theta;	
//
//			if (rho > (width+height)) rho = (rho - (width+height))*(-1); // *(-1)
//
//			if (theta > 4.5377/*260*pi/180*/ && theta < 4.8868/*280*pi/180*/ && abs(rho) < 100){ //&& rho < 0){			
//
//				cos = lisCV_cos(theta);
//				sin = lisCV_sin(theta);
//				tan = lisCV_tan(theta);
//				line_start.x = middle_x + (int)(rho*cos);
//				line_start.y = middle_y + (int)(rho*sin);
//	
//				cos = lisCV_cos(pi/2-theta);
//				sin = lisCV_sin(pi/2-theta);
//				tan = lisCV_tan(pi/2-theta);
//				line_end1.x = (line_start.x-1000);
//				line_end1.y = (int)(line_start.y+1000*tan);
//
//				line_end2.x = (line_start.x+1000);
//				line_end2.y = (int)(line_start.y-1000*tan);
//
//				colorImage = lisCV_drawLine(colorImage, line_start, line_end1, line_color);
//				colorImage = lisCV_drawLine(colorImage, line_start, line_end2, line_color);
//
//				lane3.pt1.x = line_end1.x; lane3.pt1.y = line_end1.y;
//				lane3.pt2.x = line_end2.x; lane3.pt2.y = line_end2.y;
//				lane3.isUsed = 1;
//
//				j++;
//			}
//			if (j>2) break;
//		}

//		if (lane3.isUsed == 0){ // iv. 260<theta<280 - Once again...
//			j=0;
//			for (i=0; i<1000; i++){
//				rho = houghRaum[i].rho;
//				theta = houghRaum[i].theta;	
//
//				if (rho > (width+height)) rho = (rho - (width+height))*(-1); // *(-1)
//
//				if (theta > 260*pi/180 && theta < 280*pi/180 && abs(rho) < 100){ //&& rho < 0){			
//
//					cos = lisCV_cos(theta);
//					sin = lisCV_sin(theta);
//					tan = lisCV_tan(theta);
//					line_start.x = middle_x + (int)(rho*cos);
//					line_start.y = middle_y + (int)(rho*sin);
//	
//					cos = lisCV_cos(pi/2-theta);
//					sin = lisCV_sin(pi/2-theta);
//					tan = lisCV_tan(pi/2-theta);
//					line_end1.x = (line_start.x-1000);
//					line_end1.y = (int)(line_start.y+1000*tan);
//
//					line_end2.x = (line_start.x+1000);
//					line_end2.y = (int)(line_start.y-1000*tan);
//
//					colorImage = lisCV_drawLine(colorImage, line_start, line_end1, line_color);
//					colorImage = lisCV_drawLine(colorImage, line_start, line_end2, line_color);
//
//					lane3.pt1.x = line_end1.x; lane3.pt1.y = line_end1.y;
//					lane3.pt2.x = line_end2.x; lane3.pt2.y = line_end2.y;
//					lane3.isUsed = 1;
//
//					j++;
//				}
//				if (j>2) break;		
//			}
//		}
//		/// Lane Recognition

		//CrossingPoints cxPoints;	// crossing points of lanes
		cxPoints = hooGetCrossPoints(lane1, lane2, lane3);	// get the crossing points of three lanes

		// To handle the memory problem
		free(houghRaum);
		///
	}	
	else{
		/////////........
	}
//	XTime_GetTime(&stop_time);
//	xil_printf("\n\rLane Detection: %d ms\r\n", (stop_time - start_time)/300000 );
	

/* Marking of crossing points with red circle
		cvCircle( color_dst, cxPoints.pt1, 2, CV_RGB(255,0,0), 2, 8, 0 );
		cvCircle( color_dst, cxPoints.pt2, 2, CV_RGB(255,0,0), 2, 8, 0 );
		cvCircle( color_dst, cxPoints.pt3, 2, CV_RGB(255,0,0), 2, 8, 0 );			
*/

	int ptr;
	int ptr2;

	lisCV_Point darkPoint; 
	int nDarkPoints=0;
	int nDarkPointsSerial=0;
	int darkPointsUntilHere;
	int havePair;
	int nNonDarkPoints;
	int bottomOfDarkPoints;
	int rightEndOfBottomOfDarkPoints;
	int leftEndOfBottomOfDarkPoints;
	Lane finalTunnelBottom;
	finalTunnelBottom.pt1.y=inputImage->height; finalTunnelBottom.pt1.x=inputImage->width;
	finalTunnelBottom.pt2.y=inputImage->height; finalTunnelBottom.pt2.x=inputImage->width;


	for(i=inputImage->width ; i>200 ; i=i-1)
	{
		nDarkPointsSerial = 0; darkPoint.x=0; darkPoint.y=0;
		for(j=1; j<inputImage->height ; j=j+2 )
		{			
			ptr = (int)inputImage->imageData[(width*j+i)*3];
			if( ptr == 255 )
			{
				nDarkPoints=0;
				havePair=0;
				nNonDarkPoints=0;
				for( k=1 ; k<200 ; k++ )
				{						
					ptr = (int)inputImage->imageData[(width*j+i-k)*3];
					ptr2 = (int)grayInputImage->imageData[(width*j+i-k)*3];

					if( ptr2 < 50 ) 
					{
						nDarkPoints++;
					}
					else nNonDarkPoints++;
					if( ptr == 255 )
					{
						darkPointsUntilHere = k;
						havePair = 1;
						k=200; break;
					}
					else if ( nDarkPoints > 20 && nNonDarkPoints < 5 ) darkPointsUntilHere = k;
				}
				if( nDarkPoints > 20 && havePair == 1 && nNonDarkPoints < 5 )
				{	
					if ( darkPoint.y == j-2 ) nDarkPointsSerial++;	
					for( l=i ; l> i-darkPointsUntilHere ; l-- )
					{
						darkPoint.x=l; darkPoint.y=j;
				//		cvCircle( color_dst, darkPoint, 0.5, CV_RGB(0, 0, 255), 1, 8, 0);
					}
					if ( nDarkPointsSerial == 1 )
					{
						bottomOfDarkPoints = j;
						rightEndOfBottomOfDarkPoints = i;
						leftEndOfBottomOfDarkPoints = i-darkPointsUntilHere;
					}
				}
			}
			else
			{
				if ( nDarkPointsSerial > 1 )
				{				
					lisCV_Point rightEnd, leftEnd;
					rightEnd.y=bottomOfDarkPoints;
					leftEnd.y=rightEnd.y;
					rightEnd.x=rightEndOfBottomOfDarkPoints;
					leftEnd.x=leftEndOfBottomOfDarkPoints;
					//roiHeight = (int) ceil( (rightEnd.x-leftEnd.x)*0.65 );
				//	cvLine(color_dst, rightEnd, leftEnd, CV_RGB(255,0,255), 1, 8);

					/*
					if( ( (cxPoints.pt1.x>leftEnd.x && cxPoints.pt1.x<rightEnd.x) || (cxPoints.pt2.x>leftEnd.x && cxPoints.pt2.x<rightEnd.x) || (cxPoints.pt3.x>leftEnd.x && cxPoints.pt3.x<rightEnd.x)  ) 
						&& 
						//( (cxPoints.pt1.y+10 > bottomOfDarkPoints && cxPoints.pt1.y-30 < bottomOfDarkPoints ) || (cxPoints.pt2.y+10 > bottomOfDarkPoints && cxPoints.pt2.y-30 < bottomOfDarkPoints ) || (cxPoints.pt3.y+10 > bottomOfDarkPoints && cxPoints.pt3.y-30 < bottomOfDarkPoints ) )
						( (cxPoints.pt1.y+10 > bottomOfDarkPoints && cxPoints.pt1.y-30 < bottomOfDarkPoints && (cxPoints.pt1.x!=0 && cxPoints.pt1.y!=0) ) || (cxPoints.pt2.y+10 > bottomOfDarkPoints && cxPoints.pt2.y-30 < bottomOfDarkPoints && (cxPoints.pt2.x!=0 && cxPoints.pt2.y!=0) ) || (cxPoints.pt3.y+10 > bottomOfDarkPoints && cxPoints.pt3.y-30 < bottomOfDarkPoints && (cxPoints.pt3.x!=0 && cxPoints.pt3.y!=0) ) )
					)
					*/

					if( ( (cxPoints.pt1.x>leftEnd.x && cxPoints.pt1.x<rightEnd.x) && (cxPoints.pt1.y > bottomOfDarkPoints && cxPoints.pt1.y-30 < bottomOfDarkPoints && (cxPoints.pt1.x!=0 && cxPoints.pt1.y!=0) ) )
						||																			//+10
						( (cxPoints.pt2.x>leftEnd.x && cxPoints.pt2.x<rightEnd.x) && (cxPoints.pt2.y > bottomOfDarkPoints && cxPoints.pt2.y-30 < bottomOfDarkPoints && (cxPoints.pt2.x!=0 && cxPoints.pt2.y!=0) ) )
						||																			//+10
						( (cxPoints.pt3.x>leftEnd.x && cxPoints.pt3.x<rightEnd.x) && (cxPoints.pt3.y > bottomOfDarkPoints && cxPoints.pt3.y-30 < bottomOfDarkPoints && (cxPoints.pt3.x!=0 && cxPoints.pt3.y!=0) ) )
					)																				//+10
					{	
						if( leftEnd.y < finalTunnelBottom.pt1.y  )
						{						
							finalTunnelBottom.pt1=leftEnd;
							finalTunnelBottom.pt2=rightEnd;
						}
					}					
				}				
			}
		}
	}

	TunnelInfos tunnelInfos;

	lisCV_Color rectangleColor;
	rectangleColor.R=255; rectangleColor.G=0; rectangleColor.B=0;	// x1   x4    // pt1
	lisCV_Point x1, x2, x3, x4;										// |    |	  //
	if( finalTunnelBottom.pt1.y < inputImage->height )				// x2 - x3    //       pt2
	{
		finalTunnelBottom.pt1.y = finalTunnelBottom.pt1.y-15;
		finalTunnelBottom.pt2.y = (int) ceil( finalTunnelBottom.pt1.y + (finalTunnelBottom.pt2.x-finalTunnelBottom.pt1.x)*0.65 ) + 10;
		finalTunnelBottom.pt1.x = finalTunnelBottom.pt1.x - 10;
		//cvRectangle( color_dst, finalTunnelBottom.pt1, finalTunnelBottom.pt2, CV_RGB(255,0,0), 1, 8, 0 );
		x1.x = finalTunnelBottom.pt1.x; x1.y = finalTunnelBottom.pt1.y;
		x2.x = finalTunnelBottom.pt1.x; x2.y = finalTunnelBottom.pt2.y;
		x3.x = finalTunnelBottom.pt2.x; x3.y = finalTunnelBottom.pt2.y;
		x4.x = finalTunnelBottom.pt2.x; x4.y = finalTunnelBottom.pt1.y;
		/*
		lisCV_drawLine(color_dst, x1, x2, rectangleColor);
		lisCV_drawLine(color_dst, x2, x3, rectangleColor);
		lisCV_drawLine(color_dst, x3, x4, rectangleColor);
		lisCV_drawLine(color_dst, x4, x1, rectangleColor);
		*/
		tunnelInfos.tunnelFound = 1;		
	}
	else tunnelInfos.tunnelFound = 0;	

	tunnelInfos.image = color_dst;
	tunnelInfos.pt1 = finalTunnelBottom.pt1;
	tunnelInfos.pt2 = finalTunnelBottom.pt2;
	tunnelInfos.cxPoints = cxPoints;

	// To handle the memory problem
	//free(dst);
	//free(color_dst);
	///

	return tunnelInfos;
}

