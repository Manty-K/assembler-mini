enum ByteType{

	BYTE = 1,
	WORD = 2,
	DWORD = 4,
	QUAD = 8,

};

void parsenum(long  n,enum ByteType bt);

void parsestr(char * s ,enum ByteType bt);
