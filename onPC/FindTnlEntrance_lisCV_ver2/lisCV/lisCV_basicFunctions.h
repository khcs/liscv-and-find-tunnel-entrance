#include "stdafx.h"
#include <string.h>
#include "lisCV_dataTypes.h"
#include "lisCV_groundFunctions.h"

/* opens and reads a 640x480 bitmap file 
   and return the image as lisCV_Image type */
lisCV_Image* lisCV_readBMP(char* FileName)
{
	unsigned char readBuffer[1920];
	FILE *infile;

    int i, j, h, numread;
	char FileName2[20]="";

	lisCV_BMP_HEADER header;
	lisCV_BMP_INFOHEADER infoheader;

	unsigned long int colormap[256];	// colormap von Christopher
	lisCV_Image *Image = new lisCV_Image;

	infile = fopen(FileName, "rb");

	if(infile){
		printf("Reading File ");
		printf(FileName);
		printf("\n");

		strncpy(FileName2, FileName, 13);

		/* Read and check theh header */
		header.type = ReadUShort(infile);
		header.size = ReadUIntLil(infile);
		header.reserved1 = ReadUShort(infile);
		header.reserved2 = ReadUShort(infile);
		header.offset = ReadUIntLil(infile);

		infoheader.size = ReadUIntLil(infile);
		infoheader.width = ReadUIntLil(infile);
		infoheader.height = ReadUIntLil(infile);

		infoheader.planes = ReadUShort(infile);
		infoheader.bits = ReadUShort(infile);

		infoheader.compression = ReadUIntLil(infile);
		infoheader.imagesize = ReadUIntLil(infile);
		infoheader.xresolution = ReadUIntLil(infile);
		infoheader.yresolution = ReadUIntLil(infile);
		infoheader.ncolors = ReadUIntLil(infile);
		infoheader.importantcolors = ReadUIntLil(infile);


		unsigned char* image_data = new unsigned char[infoheader.width*infoheader.height*3];

		/* Process the data */		
		switch (infoheader.bits)
		{
		case 24:// Process 24-bit BMPs, start in the lower left corner
			for (j=infoheader.height-1; j>=0 ; j--){
				// read e.g. 3*640 pixelvalues + pad words. A pixel line is always padded to a multiple of 4.
				// Example 2 pixels in one line: C0 00 40 C0 00 40 00 00
				numread = (int)fread(readBuffer, 1, (3*infoheader.width)+(infoheader.width%4), infile);

				for (i=0; i<infoheader.width; i++){
					*(image_data + (infoheader.width*j + i)*3) = readBuffer[i*3];
					*(image_data + (infoheader.width*j + i)*3+1) = readBuffer[i*3+1];
					*(image_data + (infoheader.width*j + i)*3+2) = readBuffer[i*3+2];
				}			
			}
			break;
		case 8:	// Process 8-bit BMPs, start in the lower left corner
				// fill color map
			for (h=0; h<256; h++){
				fread(readBuffer, 1, 4, infile);
				colormap[h] = colormap[h] = ((((readBuffer[2] << 8) | readBuffer[1]) << 8) | readBuffer[0]);
			}
			for (j=infoheader.height-1; j>=0; j--){
				// if the width is a multiple of 4
				if ((infoheader.width%4)==0){
					numread = (int)fread(readBuffer, 1, infoheader.width, infile);
				}
				else{
					numread = (int)fread(readBuffer, 1, (infoheader.width)+(4-(infoheader.width%4)), infile);
				}
				for (i=0; i<infoheader.width; i++)
				{
					*(image_data + (infoheader.width*j + i)*3) = readBuffer[i*3];
					*(image_data + (infoheader.width*j + i)*3+1) = readBuffer[i*3+1];
					*(image_data + (infoheader.width*j + i)*3+2) = readBuffer[i*3+2];

				}
			}
			break;

		default: printf("Undefined color depth\r\n");
		}	

		// For details see User's Manual
		Image->isUnempty = true;
		Image->imageData = image_data;
		Image->width = infoheader.width;
		Image->height = infoheader.height;
		Image->nSize = infoheader.width*infoheader.height;
		Image->depth = infoheader.bits;
		//Image.nChannel = 
		//Image.widthStep =

		Image->Header = header;
		Image->Infoheader = infoheader;
		///

		fclose(infile);

		return Image;
	}
	else{
		Image->isUnempty = false;
		return Image;
	}
}


int lisCV_writeBMP(char* FileName, lisCV_Image* Image)
{
	unsigned char writeBuffer[1920];
	FILE *outfile;

	int i, j, h, numwrite;
	lisCV_BMP_HEADER header;
	lisCV_BMP_INFOHEADER infoheader;

	header = Image->Header;
	infoheader = Image->Infoheader;

	//unsigned long int colormap[256];	// colormap von Christopher

	outfile = fopen(FileName, "wb");

	if(outfile){
		// Write and check the header 
		WriteUShort(outfile, header.type);
		WriteUIntLil(outfile, header.size);
		WriteUShort(outfile, header.reserved1);
		WriteUShort(outfile, header.reserved2);
		WriteUIntLil(outfile, header.offset);

		WriteUIntLil(outfile, infoheader.size);
		WriteUIntLil(outfile, infoheader.width);
		WriteUIntLil(outfile, infoheader.height);

		WriteUShort(outfile, infoheader.planes);
		WriteUShort(outfile, infoheader.bits);

		WriteUIntLil(outfile, infoheader.compression);
		WriteUIntLil(outfile, infoheader.imagesize);
		WriteUIntLil(outfile, infoheader.xresolution);
		WriteUIntLil(outfile, infoheader.yresolution);
		WriteUIntLil(outfile, infoheader.ncolors);
		WriteUIntLil(outfile, infoheader.importantcolors);

		switch (infoheader.bits)
		{
			case 24:// Process 24-bit BMPs, start in the lower left corner
				for (j=infoheader.height-1; j>=0 ; j--){
					// read e.g. 3*640 pixelvalues + pad words. A pixel line is always padded to a multiple of 4.
					// Example 2 pixels in one line: C0 00 40 C0 00 40 00 00
					
					//numread = (int)fread(readBuffer, 1, (3*infoheader.width)+(infoheader.width%4), infile);

					for (i=0; i<infoheader.width; i++){

						writeBuffer[i*3] = Image->imageData[(infoheader.width*j+i)*3];
						writeBuffer[i*3+1] = Image->imageData[(infoheader.width*j+i)*3+1];
						writeBuffer[i*3+2] = Image->imageData[(infoheader.width*j+i)*3+2];

						//temp = ((((readBuffer[i*3+2] << 8) | readBuffer[(i*3)+1]) << 8) | readBuffer[(i*3)]);
						//writeaddr = baseaddr+(j*1024+i)*4;
						//XIo_Out32(writeaddr, temp);				
					}					
					numwrite = (int)fwrite(writeBuffer, 1, (3*infoheader.width)+(infoheader.width%4), outfile);
				}
				break;
			case 8:	// Process 8-bit BMPs, start in the lower left corner
					// fill color map
				for (h=0; h<256; h++){
					//fread(readBuffer, 1, 4, infile);
					//colormap[h] = colormap[h] = ((((readBuffer[2] << 8) | readBuffer[1]) << 8) | readBuffer[0]);
				}
				for (j=infoheader.height-1; j>=0; j--){
					// if the width is a multiple of 4
					for (i=0; infoheader.width; i++){
						writeBuffer[i*3] = Image->imageData[(infoheader.width*j+i)*3];
						writeBuffer[i*3+1] = Image->imageData[(infoheader.width*j+i)*3+1];
						writeBuffer[i*3+2] = Image->imageData[(infoheader.width*j+i)*3+2];
						//writeaddr = baseaddr+(j*1024+i)*4;
						//Xlo_Out32(writeaddr, colopmap[readBuffer[i]]);
					}
					if ((infoheader.width%4)==0){
						numwrite = (int)fwrite(writeBuffer, 1, infoheader.width, outfile);
					}
					else{
						numwrite = (int)fwrite(writeBuffer, 1, (infoheader.width)+(4-(infoheader.width%4)), outfile);
					}					
				}
				break;

			default: printf("Undefined color depth\r\n");
		}
		fclose(outfile);
		return 1;
	}
	else{
		return 0;
	}

}
