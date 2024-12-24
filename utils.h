enum ByteType
{

	BYTE = 1,
	WORD = 2,
	DBWORD = 4,
	QUAD = 8,

};

typedef struct instHex
{
	int prefix;
	int opcode;
	int modrm;
	int sib;
	int displacement;
	int imm;
} INSTHEX;

void parsenum(long n, enum ByteType bt);

int parsestr(char *s, enum ByteType bt);

int sizeFromChar(int c);

long longFromHex(char *h);
long longFromBin(char *h);
int getRegId(char *reg);
void lowercase(char *str);
int sameSizeRegs(int r1, int r2);
void appendToObjStr(char *str);
void appendToObjVal(char val);
