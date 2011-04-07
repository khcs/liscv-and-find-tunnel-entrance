#include "stdafx.h"
//#include "cv.h"
//#include "highgui.h"

TunnelInfos hooFindTnlEntrance2ndTime(lisCV_Image* src, lisCV_Image* inputImage, lisCV_Image* grayInputImage, CrossingPoints cxPoints, lisCV_Image *colorImage)
{

	lisCV_Image* dst = new lisCV_Image;
	lisCV_Image* color_dst = new lisCV_Image;
	//CvMemStorage* storage = cvCreateMemStorage(0);
    //CvSeq* lines = 0;
    int i, j, k, l;
	//Lane lane1, lane2, lane3, lane4;
	//lane1.isUsed=0; lane2.isUsed=0; lane3.isUsed=0; lane4.isUsed=0;
	//cvCanny(src, dst, 50, 200, 3);
	dst=src;
    //cvCvtColor( inputImage, color_dst, CV_GRAY2RGB );
	color_dst=colorImage;
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

	int height = dst->Infoheader.height;
	int width  = dst->Infoheader.width;

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
						havePair++;
					}
					if( havePair > 3 )
					{
						darkPointsUntilHere = k;
						k=200; break;
					}
					else if ( nDarkPoints > 20 && nNonDarkPoints < 10 ) darkPointsUntilHere = k;
				
					//if ( nDarkPoints > 20 && nNonDarkPoints < 5 ) darkPointsUntilHere = k; 

				}
				if( nDarkPoints > 20 &&  havePair > 3 && nNonDarkPoints < 10 )
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

					if( ( (cxPoints.pt1.x>leftEnd.x && cxPoints.pt1.x<rightEnd.x) && (cxPoints.pt1.y+10 > bottomOfDarkPoints && cxPoints.pt1.y-30 < bottomOfDarkPoints && (cxPoints.pt1.x!=0 && cxPoints.pt1.y!=0) ) )
						||
						( (cxPoints.pt2.x>leftEnd.x && cxPoints.pt2.x<rightEnd.x) && (cxPoints.pt2.y+10 > bottomOfDarkPoints && cxPoints.pt2.y-30 < bottomOfDarkPoints && (cxPoints.pt2.x!=0 && cxPoints.pt2.y!=0) ) )
						||
						( (cxPoints.pt3.x>leftEnd.x && cxPoints.pt3.x<rightEnd.x) && (cxPoints.pt3.y+10 > bottomOfDarkPoints && cxPoints.pt3.y-30 < bottomOfDarkPoints && (cxPoints.pt3.x!=0 && cxPoints.pt3.y!=0) ) )
					)
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

	if( finalTunnelBottom.pt1.y < inputImage->height )
	{
		finalTunnelBottom.pt1.y = finalTunnelBottom.pt1.y-15;
		finalTunnelBottom.pt2.y = (int) ceil( finalTunnelBottom.pt1.y + (finalTunnelBottom.pt2.x-finalTunnelBottom.pt1.x)*0.65 ) + 10;
		finalTunnelBottom.pt1.x = finalTunnelBottom.pt1.x - 10;
		//cvRectangle( color_dst, finalTunnelBottom.pt1, finalTunnelBottom.pt2, CV_RGB(255,0,0), 1, 8, 0 );
		tunnelInfos.tunnelFound = 1;
	}
	else tunnelInfos.tunnelFound = 0;

	tunnelInfos.image = color_dst;
	tunnelInfos.pt1 = finalTunnelBottom.pt1;
	tunnelInfos.pt2 = finalTunnelBottom.pt2;
	tunnelInfos.cxPoints = cxPoints;

	return tunnelInfos;
}
