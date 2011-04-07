// FindTnlEntrance_lisCV_ver2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <time.h>
#include "lisCV.h"

#include "hooCutOffBelow.h"
#include "hooObjectLining.h"
#include "hooFindTnlEntrance.h"
#include "hooFindTnlEntrance2ndTime.h"

FILE *stream;

int _tmain(int argc, _TCHAR* argv[])
{
	char inputFilename[20];
	char cnLaneDetection;
	int nLaneDetection; 

	CrossingPoints cxPoints;
	bool ifDoLaneDetection;
	int nTnlNotFound=0;

	std::cout<<"Please enter the name of the video squence files: ";
	std::cin.getline( inputFilename, 20, '\n');
	
	std::cout<<"\nHow often do you want lane detection done?\n";
	std::cout<<" 0: Not at all \n   (And say, the crossing points of lanes will be probably\n    in the middle of the frames \n 1: Only at the start\n";
	std::cout<<" 2: Sometimes \n   (When a tunnel entrance is not found in more than 20 frames\n    in series, then lane detection is done)\n";
	std::cout<<" 3: Continuously \n\n ?> ";
	std::cin.get(cnLaneDetection);

	if (cnLaneDetection == '0')
		nLaneDetection = 0;
	else if (cnLaneDetection == '1')
		nLaneDetection = 1;
	else if (cnLaneDetection == '2')
		nLaneDetection = 2;
	else if (cnLaneDetection == '3')
		nLaneDetection = 3;
	else{
		std::cout<<"Input a correct number\n";
		std::cout<<"Lane detection will be done continuously...\n";
	}

	stream = fopen( "MeasuredTime.txt", "w" );

	lisCV_Image *image;			// image of below side cutted off
	lisCV_Image *image1;		// to load the original image
	lisCV_Image *image_gray;	// image of below side cutted off, gray scale image
	lisCV_Image *image_gray1;	// to load the gray scaled image
	lisCV_Image *image_gray_edge;
	lisCV_Image *image_gray_RGB;// to draw and test the process
	lisCV_Image *image_gray_OL; // image after Object Lining
	lisCV_Image *image_gray_OLD;// previous frame used for Motion Estimation
	TunnelInfos tunnelInfo, tunnelInfo2, tunnelInfo3; //infomations about found tunnels from
		// tunnelInfo: findTnlEntrance, tunnelInfo2: fundTnlEntrance2ndTime, tunnelInfo3: MotionEstimator
	lisCV_Point oldPoint1;		// previous found tunnel ROI, left bottom
	lisCV_Point oldPoint2;		// previous found tunnel ROI, right top
	int tnlOnceFound=0;			// is the tunnel entrance once found? yes=1, no=0
	int oldArea=1;				// previous found tunnel entrance size
	int newArea=1;				// new found tunnel entrance size
	lisCV_Point oldMiddlePoint;
	lisCV_Point newMiddlePoint;

	int brightness;
	char string[100];		// string to write something
	char filename[30];		// filename to save the frames when needed

	clock_t start, finish;	// to measure times
	double duration;
	double tFTE, tFTE2, tME, tOL, tEF, tSHOW;
	int ntFTE2, ntME;
	tFTE=0; tFTE2=0; tME=0; tOL=0; tEF=0; tSHOW=0;
	ntFTE2=0; ntME=0;

	int c;

	int i=1; int temp = 0;

	char numBuffer[10], inputFilenameBuffer[20];
	lisCV_Color rectangleColor;
	rectangleColor.R=255; rectangleColor.G=0; rectangleColor.B=0;
	lisCV_Point x1, x2, x3, x4;	
	int ifSpring, ifSpringX, ifSpringY;

	for(i=0;i<=200;i++)
	{	
		ifSpring = 0, ifSpringX=0, ifSpringY=0;

		strcpy(inputFilenameBuffer, inputFilename);
		if (i<10){
			sprintf(numBuffer, "-000%d.bmp", i);
			strcat(inputFilenameBuffer, numBuffer);
			strcpy(filename, ".\\"); strcat(filename, inputFilename); strcat(filename, "\\");
			strcat(filename, inputFilenameBuffer);
		}
		else if (i>= 10 && i<100){
			sprintf(numBuffer, "-00%d.bmp", i);
			strcat(inputFilenameBuffer, numBuffer);
			strcpy(filename, ".\\"); strcat(filename, inputFilename); strcat(filename, "\\");
			strcat(filename, inputFilenameBuffer);
		}
		else if (i>=100 && i<1000){
			sprintf(numBuffer, "-0%d.bmp", i);
			strcat(inputFilenameBuffer, numBuffer);
			strcpy(filename, ".\\"); strcat(filename, inputFilename); strcat(filename, "\\");
			strcat(filename, inputFilenameBuffer);
		}

		//printf("%s\n", filename);
		image1 = lisCV_readBMP(filename);
		image_gray1 = lisCV_rgb2gray(image1);

		fprintf(stream, "\n%dth frame\n",i);

		if (image_gray1 != 0){
			image = hooCutOffBelow(image1, image1->Infoheader.height, image1->Infoheader.width, (int)ceil(image1->Infoheader.height*(1-0.1953)), image1->Infoheader.width);	// function which cuts the below side off
			image_gray = hooCutOffBelow(image_gray1, image1->Infoheader.height, image1->Infoheader.width, (int)ceil(image1->Infoheader.height*(1-0.1953)), image1->Infoheader.width);
					

			//
			start = clock();//
			image_gray_edge = lisCV_sobel(image_gray);	// edge filtering
			finish = clock();//
			duration = (double)(finish - start) / CLOCKS_PER_SEC;//
			fprintf(stream, "%f seconds for edge filtering\n", duration );//
			tEF = tEF+duration;
			///

			//
			start = clock();//
			image_gray_OL = hooObjectLining(image_gray_edge);	// object lining
			finish = clock();//
			duration = (double)(finish - start) / CLOCKS_PER_SEC;//
			fprintf(stream, "%f seconds for hooObjectLining\n", duration );//
			tOL = tOL+duration;
			///
			
			//
			start = clock();//

			cxPoints.pt1.x = (int)(image_gray->width/2); 
			cxPoints.pt1.y = (int)(image_gray->height/2);
			cxPoints.pt2 = cxPoints.pt1; cxPoints.pt2.y = cxPoints.pt2.y+20;
			cxPoints.pt3 = cxPoints.pt2; cxPoints.pt3.y = cxPoints.pt3.y+20;
			if (nLaneDetection == 0){
				ifDoLaneDetection = false;
			}
			else if (nLaneDetection == 1){
				if (i == 0) ifDoLaneDetection = true;
				else{
					ifDoLaneDetection = false;
					cxPoints = tunnelInfo.cxPoints;
				}
			}
			else if (nLaneDetection == 2){
				if (nTnlNotFound >20 || i==0) ifDoLaneDetection = true;
				else{
					ifDoLaneDetection = false;
					cxPoints = tunnelInfo.cxPoints;
				}
			}
			else{
				ifDoLaneDetection = true;
			}
			tunnelInfo = hooFindTnlEntrance(image_gray_edge ,image_gray_OL, image_gray, cxPoints,image, ifDoLaneDetection);// find tunnel entrance including lane recognition
			finish = clock();//
			duration = (double)(finish - start) / CLOCKS_PER_SEC;//
			fprintf(stream, "%f seconds for hooFindTnlEntrance\n", duration );//
			tFTE=tFTE+duration;
			///


			//
			image_gray_RGB = tunnelInfo.image;
			if (tnlOnceFound > 0){
				oldArea = (oldPoint2.y-oldPoint1.y)*(oldPoint2.x-oldPoint1.x);
				newArea = (tunnelInfo.pt2.y-tunnelInfo.pt1.y)*(tunnelInfo.pt2.x-tunnelInfo.pt1.x);
				oldMiddlePoint.x = (int)((oldPoint2.x+oldPoint1.x)/2);
				oldMiddlePoint.y = (int)((oldPoint2.y+oldPoint2.y)/2);
				newMiddlePoint.x = (int)((tunnelInfo.pt2.x+tunnelInfo.pt1.x)/2);
				newMiddlePoint.y = (int)((tunnelInfo.pt2.y+tunnelInfo.pt1.y)/2);
				if (newMiddlePoint.x > oldPoint2.x || newMiddlePoint.x < oldPoint1.x) ifSpringX = 1;
				if (newMiddlePoint.y > oldPoint2.y || newMiddlePoint.y < oldPoint1.y) ifSpringY = 1;
				if (ifSpringX == 1 || ifSpringY == 1) ifSpring = 1;
				//commonArea = abs( (oldPoint1.x-tunnelInfo.pt2.x)*(tunnelInfo.pt2.y-oldPoint1.y) );
			}

			if(tunnelInfo.tunnelFound == 1 && (double)newArea/(double)oldArea < (double)3 && newArea != 0 && ifSpring != 1 ){
				//cvRectangle( image_gray_RGB, tunnelInfo.pt1, tunnelInfo.pt2, CV_RGB(255,0,0), 1, 8, 0 );
				oldPoint1 = tunnelInfo.pt1;
				oldPoint2 = tunnelInfo.pt2;
				tnlOnceFound++;
				temp=1;

				strcat(string, "Tnl found: Yes\n");

				if( (double)newArea/(double)oldArea < (double)3/4 ){
					strcat(string, "findTnlEntrance2ndTme\n");
					//
					start = clock();//
					tunnelInfo2 = hooFindTnlEntrance2ndTime(image_gray_edge ,image_gray_OL, image_gray, tunnelInfo.cxPoints, image);
					finish = clock();//
					duration = (double)(finish - start) / CLOCKS_PER_SEC;//
					fprintf(stream, "%f seconds for hooFindTnlEntrance2ndTime\n", duration );//
					tFTE2 = tFTE2+duration;
					ntFTE2++;
					///
					newArea = (tunnelInfo2.pt2.y-tunnelInfo2.pt1.y)*(tunnelInfo2.pt2.x-tunnelInfo2.pt1.x);
					if( tunnelInfo2.tunnelFound == 1 && (double)newArea/(double)oldArea < 3 && (double)newArea/(double)oldArea > (double)3/4 && newArea != 0){
							//cvRectangle( image_gray_RGB, tunnelInfo2.pt1, tunnelInfo2.pt2, CV_RGB(255,0,0), 1, 8, 0 );
							x1.x = tunnelInfo2.pt1.x; x1.y = tunnelInfo2.pt1.y;
							x2.x = tunnelInfo2.pt1.x; x2.y = tunnelInfo2.pt2.y;
							x3.x = tunnelInfo2.pt2.x; x3.y = tunnelInfo2.pt2.y;
							x4.x = tunnelInfo2.pt2.x; x4.y = tunnelInfo2.pt1.y;
							lisCV_drawLine(image_gray_RGB, x1, x2, rectangleColor);
							lisCV_drawLine(image_gray_RGB, x2, x3, rectangleColor);
							lisCV_drawLine(image_gray_RGB, x3, x4, rectangleColor);
							lisCV_drawLine(image_gray_RGB, x4, x1, rectangleColor);
							/// cvRactangle
							oldPoint1 = tunnelInfo2.pt1;
							oldPoint2 = tunnelInfo2.pt2;
							printf(" tunnel Find 2nd Time \n");

					}
					else{						
						if ( oldArea >= 14454 && i>2){ // -- wegkommentiert im Ver2 -- && ifUseMotionEstimator){
							//tunnelInfo3 = hooMotionEstimator(image_gray_OL, image_gray, oldPoint1, oldPoint2, i);
							/*
							start = clock();//
							tunnelInfo3 = hooMotionEstimator3(image_gray_OLD, image_gray, oldPoint1, oldPoint2, i);
							finish = clock();//
							duration = (double)(finish - start) / CLOCKS_PER_SEC;//
							fprintf(stream, "%f seconds for hooMotionEstimator3\n", duration );//
							tME=tME+duration;
							ntME++;
							///
							//cvRectangle( image_gray_RGB, oldPoint1, oldPoint2, CV_RGB(255,0,0), 1, 8, 0 );
							cvRectangle( image_gray_RGB, tunnelInfo3.pt1, tunnelInfo3.pt2, CV_RGB(255,0,0), 1, 8, 0 );
							oldPoint1 = tunnelInfo3.pt1;
							oldPoint2 = tunnelInfo3.pt2;
							sprintf(string, "Motion Estimator on run\n");						
							*/
						}
						else{
							//cvRectangle( image_gray_RGB, oldPoint1, oldPoint2, CV_RGB(255,0,0), 1, 8, 0 );
							x1.x = oldPoint1.x; x1.y = oldPoint1.y;
							x2.x = oldPoint1.x; x2.y = oldPoint2.y;
							x3.x = oldPoint2.x; x3.y = oldPoint2.y;
							x4.x = oldPoint2.x; x4.y = oldPoint1.y;
							lisCV_drawLine(image_gray_RGB, x1, x2, rectangleColor);
							lisCV_drawLine(image_gray_RGB, x2, x3, rectangleColor);
							lisCV_drawLine(image_gray_RGB, x3, x4, rectangleColor);
							lisCV_drawLine(image_gray_RGB, x4, x1, rectangleColor);
							/// cvRactangle

						}						
					}
				}
				else{
					//cvRectangle( image_gray_RGB, tunnelInfo.pt1, tunnelInfo.pt2, CV_RGB(255,0,0), 1, 8, 0 );
					x1.x = tunnelInfo.pt1.x; x1.y = tunnelInfo.pt1.y;
					x2.x = tunnelInfo.pt1.x; x2.y = tunnelInfo.pt2.y;
					x3.x = tunnelInfo.pt2.x; x3.y = tunnelInfo.pt2.y;
					x4.x = tunnelInfo.pt2.x; x4.y = tunnelInfo.pt1.y;
					lisCV_drawLine(image_gray_RGB, x1, x2, rectangleColor);
					lisCV_drawLine(image_gray_RGB, x2, x3, rectangleColor);
					lisCV_drawLine(image_gray_RGB, x3, x4, rectangleColor);
					lisCV_drawLine(image_gray_RGB, x4, x1, rectangleColor);
					/// cvRactangle
					oldPoint1 = tunnelInfo.pt1;
					oldPoint2 = tunnelInfo.pt2;

				}
			}
			else if( tnlOnceFound > 0 ){
				//cvRectangle( image_gray_RGB, oldPoint1, oldPoint2, CV_RGB(255,0,0), 1, 8, 0 );
				x1.x = oldPoint1.x; x1.y = oldPoint1.y;
				x2.x = oldPoint1.x; x2.y = oldPoint2.y;
				x3.x = oldPoint2.x; x3.y = oldPoint2.y;
				x4.x = oldPoint2.x; x4.y = oldPoint1.y;
				lisCV_drawLine(image_gray_RGB, x1, x2, rectangleColor);
				lisCV_drawLine(image_gray_RGB, x2, x3, rectangleColor);
				lisCV_drawLine(image_gray_RGB, x3, x4, rectangleColor);
				lisCV_drawLine(image_gray_RGB, x4, x1, rectangleColor);
				/// cvRactangle
				strcat(string, "Tnl found: No\n");
			}
			//
			
		}
		printf("\t\t\t\t\t\t%d\n", tunnelInfo.tunnelFound);
		fprintf(stream, "Is tunnel found? %d \n", tunnelInfo.tunnelFound );//
		if (tunnelInfo.tunnelFound == 1) nTnlNotFound=0;
		else nTnlNotFound++;
		filename[3]='N';
		lisCV_writeBMP(filename, image_gray_RGB);
	}	

	tEF=tEF/i;
	tOL=tOL/i;
	tFTE=tFTE/i;
	tFTE2=tFTE2/ntFTE2;
	tME=tME/ntME;
	tSHOW=tSHOW/i;

	fprintf(stream, "\n Average durations: \n  hooEdgeFilter:%f \n  hooObjectLining:%f \n  hooFindTnlEntrance:%f \n  hooFindTnlEntrance2ndTime:%f \n  \n" , tEF, tOL, tFTE, tFTE2 );
	fclose( stream );	

	return 0;
}
