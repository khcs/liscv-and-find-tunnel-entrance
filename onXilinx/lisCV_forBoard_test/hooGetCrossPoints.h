//#include "stdafx.h"

typedef struct Lane
{
	lisCV_Point pt1;
    lisCV_Point pt2;
	int isUsed;
}Lane;

typedef struct CrossingPoints
{
	lisCV_Point pt1;
    lisCV_Point pt2;
	lisCV_Point pt3;
}CrossingPoints;

CrossingPoints hooGetCrossPoints(Lane lane1, Lane lane2, Lane lane3)
{
	double a1, b1, a2, b2, a3, b3;

	a1 = (double)(lane1.pt2.y-lane1.pt1.y)/(double)(lane1.pt2.x-lane1.pt1.x);
	b1 = a1*lane1.pt2.x*(-1) + lane1.pt2.y;

	a2 = (double)(lane2.pt2.y-lane2.pt1.y)/(double)(lane2.pt2.x-lane2.pt1.x);
	b2 = a2*lane2.pt2.x*(-1) + lane2.pt2.y;

//	a3 = (double)(lane3.pt2.y-lane3.pt1.y)/(double)(lane3.pt2.x-lane3.pt1.x);
//	b3 = a3*lane3.pt2.x*(-1) + lane3.pt2.y;

	CrossingPoints cxPoints;

	cxPoints.pt1.x = (int) ( (b2-b1)/(a1-a2) );
	cxPoints.pt1.y = (int) ( a1*cxPoints.pt1.x+b1 );

//	cxPoints.pt2.x = (int) ( (b3-b2)/(a2-a3) );
//	cxPoints.pt2.y = (int) ( a2*cxPoints.pt2.x+b2 );
//
//	cxPoints.pt3.x = (int) ( (b3-b1)/(a1-a3) );
//	cxPoints.pt3.y = (int) (a1*cxPoints.pt3.x+b1 );

	cxPoints.pt2 = cxPoints.pt1;
	cxPoints.pt3 = cxPoints.pt1;

	return cxPoints;
}
