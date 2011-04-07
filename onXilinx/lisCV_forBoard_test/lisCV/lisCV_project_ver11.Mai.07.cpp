// lisCV_project_ver11.Mai.07.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "lisCV_advancedFunctions.h"
#include <math.h>
#include <time.h>

//
FILE *stream;
///

int _tmain(int argc, _TCHAR* argv[])
{
	lisCV_Image *Image;

	//Image = lisCV_readBMP("HoughTest5.bmp");
	Image = lisCV_readBMP("LaneTst6-0000.bmp");

	//
	stream = fopen( "MeasuredTime.txt", "w" );
	double tEF, duration; tEF=0;
	clock_t start, finish;
	///

	//
	printf("Doing rgb2gray\n");//
	start = clock();//
	Image = lisCV_rgb2gray(Image);
	finish = clock();//
	duration = (double)(finish - start) / CLOCKS_PER_SEC;//
	fprintf(stream, "%f seconds for rgb2gray\n", duration );//
	tEF = tEF+duration;
	///

	//
	printf("Doing Sobel edge-filterting\n");//
	start = clock();//
	Image = lisCV_sobel(Image);
	finish = clock();//
	duration = (double)(finish - start) / CLOCKS_PER_SEC;//
	fprintf(stream, "%f seconds for Sobel edge-filtering\n", duration );//
	tEF = tEF+duration;
	///

	//printf("%f %f %f %f", sin(45.0), sin(3.14), sin(90.0), sin(3.14/2));
	
	lisCV_HoughResult *houghRaum;
	//
	printf("Doing Hough-transformation\n");
	start = clock();//
	houghRaum = lisCV_houghTransform(Image);
	finish = clock();//
	duration = (double)(finish - start) / CLOCKS_PER_SEC;//
	fprintf(stream, "%f seconds for Hough-transformation\n", duration );//
	tEF = tEF+duration;
	///

	// Lane Recognition
	int i, j;
	double pi = 3.1415;
	int rho, coef;
	double theta;
	int height = Image->Infoheader.height;
	int width  = Image->Infoheader.width;
	int middle_x = (int)(width/2);
	int middle_y = (int)(height/2);	
	
	lisCV_Point line_start, line_end1, line_end2, line_middle; line_middle.x = middle_x; line_middle.y = middle_y;
	lisCV_Color line_color;
	line_color.R = 0; line_color.G = 255; line_color.B = 0;

	j=0; // i. theta > pi/2
	for (i=0; i<4; i++){
		rho = houghRaum[i].rho;
		theta = houghRaum[i].theta;	

		if (rho > (width+height)) rho = (rho - (width+height))*(-1); // *(-1)

		//if (theta < 135*pi/180 && theta > 90*pi/180 && abs(rho) < 100){ //&& rho < 0){			

			line_start.x = middle_x + (int)(rho*cos(theta));
			line_start.y = middle_y + (int)(rho*sin(theta));
	
			line_end1.x = (line_start.x-1000);
			line_end1.y = (int)(line_start.y+1000*tan(pi/2-theta));

			line_end2.x = (line_start.x+1000);
			line_end2.y = (int)(line_start.y-1000*tan(pi/2-theta));

			Image = lisCV_drawLine(Image, line_start, line_end1, line_color);
			Image = lisCV_drawLine(Image, line_start, line_end2, line_color);

			j++;
		//}
		if (j>2) break;
		
	}
/*
	j=0; // ii. theta < pi/2
	for (i=0; i<1000; i++){
		rho = houghRaum[i].rho;
		theta = houghRaum[i].theta;	

		if (rho > (width+height)) rho = (rho - (width+height));
		if (rho <= 0) rho = rho*(-1); // *(-1)

		if(theta > 0 && theta < 90*pi/180 && abs(rho) < 100){ //&& rho > 0 ){			

			line_start.x = middle_x - (int)(rho*cos(theta));
			line_start.y = middle_y - (int)(rho*sin(theta));

			//Image = lisCV_drawLine(Image, line_middle, line_start, line_color);

			line_end1.x = (line_start.x-1000);
			double a = tan(pi/2-theta);
			int b = (int)tan(pi/2-theta);
			line_end1.y = (int)(line_start.y+1000*tan(pi/2-theta));

			line_end2.x = (line_start.x+1000);
			line_end2.y = (int)(line_start.y-1000*tan(pi/2-theta));

			Image = lisCV_drawLine(Image, line_start, line_end1, line_color);
			Image = lisCV_drawLine(Image, line_start, line_end2, line_color);

			j++;
		}
		if (j>2) break;
		
	}

	j=0; // iii. Kkak Ttu Gi
	for (i=0; i<1000; i++){
		rho = houghRaum[i].rho;
		theta = houghRaum[i].theta;	

		if (rho > (width+height)) rho = (rho - (width+height))*(-1); // *(-1)

		if (theta >= 135*pi/180 && theta < 180*pi/180 && abs(rho)<100){ //&& rho < 0){			

			line_start.x = middle_x + (int)(rho*cos(theta));
			line_start.y = middle_y + (int)(rho*sin(theta));
	
			line_end1.x = (line_start.x-1000);
			line_end1.y = (int)(line_start.y+1000*tan(pi/2-theta));

			line_end2.x = (line_start.x+1000);
			line_end2.y = (int)(line_start.y-1000*tan(pi/2-theta));

			Image = lisCV_drawLine(Image, line_start, line_end1, line_color);
			Image = lisCV_drawLine(Image, line_start, line_end2, line_color);

			j++;
		}
		if (j>2) break;
		
	}
	/// Lane Recognition
*/
/*
	lisCV_Point x1, x2, x3, x4;
	x1.x=0; x1.y=240; x2.x=500; x2.y=0;
	lisCV_Color line_color;
	line_color.R = 0; line_color.G = 255; line_color.B = 0;
	Image = lisCV_drawLine(Image, x1, x2, line_color);
*/

	lisCV_writeBMP("MOV01391-0131_N.bmp", Image);	
	//lisCV_writeBMP("houghTest_N.bmp", Image);

	//
	fclose(stream);
	///

	return 0;
}

