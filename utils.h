enum ByteType
{

	BYTE = 1,
	WORD = 2,
	DWORD = 4,
	QUAD = 8,

};

void parsenum(long n, enum ByteType bt);

int parsestr(char *s, enum ByteType bt);

int sizeFromChar(int c);

long longFromHex(char *h);
long longFromBin(char *h);