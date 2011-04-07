typedef struct Lane
{
	CvPoint pt1;
    CvPoint pt2;
	int isUsed;
}Lane;

typedef struct CrossingPoints
{
	CvPoint pt1;
    CvPoint pt2;
	CvPoint pt3;
}CrossingPoints;

typedef struct TunnelInfos
{
	IplImage* image;
	CvPoint pt1;
    CvPoint pt2;
	int tunnelFound;
	CrossingPoints cxPoints;
}TunnelInfos;