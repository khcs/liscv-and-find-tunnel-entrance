/* reads two bytes from file */
static unsigned short ReadUShort(FILE *fptr)
{
	unsigned char b0, b1;
	int numread;
	char readBuffer[2];

	numread = (int)fread(readBuffer, 1, 2, fptr);

	b0 = readBuffer[0];
	b1 = readBuffer[1];

	return ((b1 << 8) | b0);
}

/* reads fourbytes from file in little endian order */
static unsigned int ReadUIntLil(FILE *fptr)
{
	unsigned char b0, b1, b2, b3;
	int numread;
	char readBuffer[4];

	numread = (int)fread(readBuffer, 1, 4, fptr);

	b0 = readBuffer[0];
	b1 = readBuffer[1];
	b2 = readBuffer[2];
	b3 = readBuffer[3];

	return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

/* reads four bytes from file in big endian order */
static unsigned int ReadUIntBig(FILE *fptr)
{
	unsigned char b0, b1, b2, b3;
	int numread;
	char readBuffer[4];

	numread = (int)fread(readBuffer, 1, 3, fptr);

	b0 = readBuffer[0];
	b1 = readBuffer[1];
	b2 = readBuffer[2];
	b3 = readBuffer[3];

	return ((((((b0 << 8) | b1) << 8) | b2) << 8) | b3);
}

/* writes two bytes to file */
void WriteUShort(FILE *fptr, unsigned short b)
{
	unsigned char b0, b1;
	int numwrite;
	char writeBuffer[2];

	b0 = b;
	b1 = b >> 8;

	writeBuffer[0] = b0;
	writeBuffer[1] = b1;

	numwrite = (int)fwrite(writeBuffer, 1, 2, fptr);
}

/* writes fourbytes to file in little endian order */
void WriteUIntLil(FILE *fptr, unsigned int b)
{
	unsigned char b0, b1, b2, b3;
	int numwrite;
	char writeBuffer[4];

	b0 = b;
	b1 = b >> 8;
	b2 = b >> 16;
	b3 = b >> 24;

	writeBuffer[0] = b0;
	writeBuffer[1] = b1;
	writeBuffer[2] = b2;
	writeBuffer[3] = b3;

	numwrite = (int)fwrite(writeBuffer, 1, 4, fptr);
}

/* writes four bytes from file in big endian order */
void WriteUIntBig(FILE *fptr, unsigned int b)
{
	unsigned char b0, b1, b2, b3;
	int numwrite;
	char writeBuffer[4];

	b0 = b;
	b1 = b >> 8;
	b2 = b >> 16;
	b3 = b >> 24;

	writeBuffer[0] = b0;
	writeBuffer[1] = b1;
	writeBuffer[2] = b2;
	writeBuffer[3] = b3;

	numwrite = (int)fwrite(writeBuffer, 1, 3, fptr);
}