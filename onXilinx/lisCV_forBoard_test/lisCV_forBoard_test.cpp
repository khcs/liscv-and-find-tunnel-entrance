#include "stdafx.h"
#include <stdio.h>

#include "lisCV.h"

#include "hooCutOffBelow.h"
#include "hooObjectLining.h"
#include "hooFindTnlEntrance.h"
//#include "hooFindTnlEntrance2ndTime.h"

int main()
{
  //char file[] = "image01.bmp";
	char file[] = "MOV01392-0135.bmp";
	//char file2[]= "MNV01392-0100.bmp";
  //char file[] = "a:slides\image01.bmp";
//  char file[] = "a:\\M92V000.bmp";
//	char file[] = "a:\\M92V103.bmp";

  int count = 0;
  
  char cnLaneDetection;
  int nLaneDetection;
  
   int i, j;
  
	char string[100];

	lisCV_Image *image;
//	lisCV_Image *image_rz = (lisCV_Image*)malloc(sizeof(lisCV_Image));
	lisCV_Image *image_gray = (lisCV_Image*)malloc(sizeof(lisCV_Image));
	lisCV_Image *image_edge = (lisCV_Image*)malloc(sizeof(lisCV_Image));
	lisCV_Image *image_edge_OL = (lisCV_Image*)malloc(sizeof(lisCV_Image));
	lisCV_Image *image_gray_RGB = (lisCV_Image*)malloc(sizeof(lisCV_Image));
     
  int k;

  TunnelInfos tunnelInfo;
  CrossingPoints cxPoints;
  int ifDoLaneDetection = 1;
  lisCV_Point x1, x2, x3, x4;
  lisCV_Color rectangleColor;
  rectangleColor.R=255; rectangleColor.G=0; rectangleColor.B=0;
  
  for(k=0; k<1; k++){

 		//	if (file[12] == '9') {
			//	if (file[11] == '9'){
			//		file[10]++;
			//		file[11] -=9;
			//	}else{
			//		file[11]++;
			//	}
			//	file[12] -= 9;
			//} else {
			//	file[12]++;
			//}

			//if (file2[12] == '9') {
			//	if (file2[11] == '9'){
			//		file2[10]++;
			//		file2[11] -=9;
			//	}else{
			//		file2[11]++;
			//	}
			//	file2[12] -= 9;
			//} else {
			//	file2[12]++;
			//}

		image = lisCV_readBMP(file);
		
//		xil_printf("hooCutOffBelow\n\r");
		image = hooCutOffBelow(image, image->Infoheader.height, image->Infoheader.width, (int)ceil(image->Infoheader.height*(1-0.1953)), image->Infoheader.width);	// function which cuts the below side off		
//			for(j=infoheader.height-1;j>=0;j--){
//				for(i=0;i<infoheader.width;i++){	
//	
//					temp1	= ((((*(image->imageData + (infoheader.width*j + i)*3+2) <<8) | *(image->imageData + (infoheader.width*j + i)*3+1)) << 8) | *(image->imageData + (infoheader.width*j + i)*3) );
//					writeaddr1 = baseaddr1+(j*1024+i)*4;
//					XIo_Out32(writeaddr1, temp1);
//				}
//			}
			
		image_gray = lisCV_rgb2gray(image);
//			for(j=infoheader.height-1;j>=0;j--){
//				for(i=0;i<infoheader.width;i++){
//					temp2 = ((((*(image_gray->imageData + (infoheader.width*j + i)*3+2) <<8) | *(image_gray->imageData + (infoheader.width*j + i)*3+1)) << 8) | *(image_gray->imageData + (infoheader.width*j + i)*3) );
//					writeaddr2 = baseaddr2+(j*1024+i)*4;
//					XIo_Out32(writeaddr2, temp2);
//				}
//			}

		cxPoints.pt1.x = (int)(image_gray->width/2); 
		cxPoints.pt1.y = (int)(image_gray->height/2); 
		cxPoints.pt2 = cxPoints.pt1; cxPoints.pt2.y = cxPoints.pt2.y+20;
		cxPoints.pt3 = cxPoints.pt2; cxPoints.pt3.y = cxPoints.pt3.y+20;

			
		image_edge = lisCV_sobel(image_gray);
//			for(j=infoheader.height-1;j>=0;j--){
//				for(i=0;i<infoheader.width;i++){
//					temp3 = ((((*(image_edge->imageData + (infoheader.width*j + i)*3+2) <<8) | *(image_edge->imageData + (infoheader.width*j + i)*3+1)) << 8) | *(image_edge->imageData + (infoheader.width*j + i)*3) );
//					writeaddr3 = baseaddr3+(j*1024+i)*4;
//					XIo_Out32(writeaddr3, temp3);
//				}
//			}
			
		image_edge_OL = hooObjectLining(image_edge);	// object lining
//			for(j=infoheader.height-1;j>=0;j--){
//				for(i=0;i<infoheader.width;i++){		
//					temp4 = ((((*(image_edge_OL->imageData + (infoheader.width*j + i)*3+2) <<8) | *(image_edge_OL->imageData + (infoheader.width*j + i)*3+1)) << 8) | *(image_edge_OL->imageData + (infoheader.width*j + i)*3) );
//					writeaddr4 = baseaddr4+(j*1024+i)*4;
//					XIo_Out32(writeaddr4, temp4);
//				}
//			}
		
		tunnelInfo = hooFindTnlEntrance(image_edge ,image_edge_OL, image_edge, cxPoints, image, ifDoLaneDetection);// find tunnel entrance including lane recognition
		image = tunnelInfo.image;

		printf("\t\t\t%d\n", tunnelInfo.tunnelFound);

		x1.x = tunnelInfo.pt1.x; x1.y = tunnelInfo.pt1.y;
		x2.x = tunnelInfo.pt1.x; x2.y = tunnelInfo.pt2.y;
		x3.x = tunnelInfo.pt2.x; x3.y = tunnelInfo.pt2.y;
		x4.x = tunnelInfo.pt2.x; x4.y = tunnelInfo.pt1.y;
		lisCV_drawLine(image, x1, x2, rectangleColor);
		lisCV_drawLine(image, x2, x3, rectangleColor);
		lisCV_drawLine(image, x3, x4, rectangleColor);
		lisCV_drawLine(image, x4, x1, rectangleColor);
				

		lisCV_writeBMP("grayImage.bmp", image_gray);
		//lisCV_writeBMP(file2, image);
		 
	}

  return 0;
}