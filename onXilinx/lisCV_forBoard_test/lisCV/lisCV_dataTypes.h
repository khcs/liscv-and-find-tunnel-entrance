/* structs for bitmaps */
typedef struct{
	unsigned short int type;				 // Magic identifier
	unsigned int size;						 // File size in bytes
	unsigned short int reserved1, reserved2; 
	unsigned int offset;					 // Offset to image data, bytes
} lisCV_BMP_HEADER;

typedef struct {
	unsigned int size;				// Header size in bytes
	int width, height;				// Width and height of image
	unsigned short int planes;		// Number of color planes
	unsigned short int bits;		// Bits per pixel
	unsigned int compression;		// Compression type
	unsigned int imagesize;			// Image size in bytes
	int xresolution, yresolution;	// Pixels per meter
	unsigned int ncolors;			// Number of colors
	unsigned int importantcolors;	// Important colors
} lisCV_BMP_INFOHEADER;

/* struct for images */
typedef struct{
	bool isUnempty;	// image is empty -> 1

	int width;		// image width in pixels
	int height;		// image height in pixels

	int nSize;		// size of image
	int nChannel;	// 1 or 3
	int widthStep;	// size of aligned image row in bytes
	int depth;

	unsigned char *imageData; // pointer to aligned image data

	lisCV_BMP_HEADER Header;
	lisCV_BMP_INFOHEADER Infoheader;
} lisCV_Image;

/* struct for Hough transformation */
typedef struct{
	double theta;
	int rho;
	unsigned int intensity;
} lisCV_HoughResult;

typedef struct{
	int x;
	int y;
} lisCV_Point;

typedef struct
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
}lisCV_Color;
