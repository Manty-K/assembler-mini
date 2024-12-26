#include <limits.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include <ctype.h>

extern FILE *objfp;
extern int pass;

void parsenum(long n, enum ByteType bt)
{

	if (bt == BYTE && (n < SCHAR_MIN || n > SCHAR_MAX))
	{
		puts("Out of range");
		return;
	}

	if (bt == WORD && (n < SHRT_MIN || n > SHRT_MAX))
	{
		puts("Out of range");
		return;
	}

	if (bt == DBWORD && (n < INT_MIN || n > INT_MAX))
	{
		puts("Out of range");
		return;
	}

	if (bt == QUAD && (n < LONG_MIN || n > LONG_MAX))
	{
		puts("Out of range");
		return;
	}

	if (bt == BYTE)
	{

		printf("%02lX", n);
	}
	else if (bt == WORD)
	{

		printf("%04lX", n);
	}
	else if (bt == DBWORD)
	{

		printf("%08lX", n);
	}
	else if (bt == QUAD)
	{

		printf("%06lX", n);
	}
	else
	{
		puts("Not Defined");
	}
}
int parsestr(char *s, enum ByteType bt)
{
	int count = 0;
	int i = 0;
	while (*(s + i) != 0)
	{

		printf("%02X", *(s + i));
		i++;
		count++;
	}

	int r = strlen(s) % bt;

	if (bt > 1 && r != 0)
	{
		for (i = 0; i < bt - r; i++)
		{
			printf("00");
			count++;
		}
	}
	return count;
}

int sizeFromChar(int c)
{

	switch (c)
	{
	case 'b':
		return 1;
	case 'w':
		return 2;
	case 'd':
		return 4;
	case 'q':
		return 8;

	default:
		fprintf(stderr, "Invalid type : %c", c);
		return -1;
	}
}

long longFromHex(char *h)
{
	return strtol(h, NULL, 16);
}

long longFromBin(char *h)
{
	return strtol(h, NULL, 2);
}

int getRegId(char *reg)
{
	/*
	eax to edi  0 -7
	ax - di - 8 - 15
	ah - dh - 16 - 19
	al - dl - 20 - 23
	*/
	if (!strcmp(reg, "eax"))
	{
		return 0;
	}
	if (!strcmp(reg, "ecx"))
	{
		return 1;
	}
	if (!strcmp(reg, "edx"))
	{
		return 2;
	}
	if (!strcmp(reg, "ebx"))
	{
		return 3;
	}
	if (!strcmp(reg, "esp"))
	{
		return 4;
	}
	if (!strcmp(reg, "ebp"))
	{
		return 5;
	}
	if (!strcmp(reg, "esi"))
	{
		return 6;
	}
	if (!strcmp(reg, "edi"))
	{
		return 7;
	}
	if (!strcmp(reg, "ax"))
	{
		return 8;
	}
	if (!strcmp(reg, "cx"))
	{
		return 9;
	}
	if (!strcmp(reg, "dx"))
	{
		return 10;
	}
	if (!strcmp(reg, "bx"))
	{
		return 11;
	}
	if (!strcmp(reg, "sp"))
	{
		return 12;
	}
	if (!strcmp(reg, "bp"))
	{
		return 13;
	}
	if (!strcmp(reg, "si"))
	{
		return 14;
	}
	if (!strcmp(reg, "di"))
	{
		return 15;
	}

	if (!strcmp(reg, "ah"))
	{
		return 16;
	}
	if (!strcmp(reg, "ch"))
	{
		return 17;
	}
	if (!strcmp(reg, "dh"))
	{
		return 18;
	}
	if (!strcmp(reg, "bh"))
	{
		return 19;
	}
	if (!strcmp(reg, "al"))
	{
		return 20;
	}
	if (!strcmp(reg, "cl"))
	{
		return 21;
	}
	if (!strcmp(reg, "dl"))
	{
		return 22;
	}
	if (!strcmp(reg, "bl"))
	{
		return 23;
	}
	return -1;
}

void lowercase(char *str)
{
	for (int i = 0; str[i]; i++)
	{
		str[i] = tolower(str[i]);
	}
}

int sameSizeRegs(int r1, int r2)
{

	if (r1 >= 0 && r1 < 8 && r2 >= 0 && r2 < 8)
	{
		return 1;
	}

	if (r1 >= 8 && r1 < 16 && r2 >= 8 && r2 < 16)
	{
		return 1;
	}
	if (r1 >= 16 && r2 >= 16)
	{
		return 1;
	}

	return 0;
}

int appendByte(FILE *fp, char val)
{
	if (fwrite(&val, 1, 1, fp) != 1)
	{
		perror("Error writing to file");
		return -1;
	}

	return 1;
}

int appendLong(FILE *fp, long val)
{
	if (fwrite(&val, sizeof(long), 1, fp) != 1)
	{
		perror("Error writing to file");
		return -1;
	}

	return sizeof(long);
}

unsigned char charFromHex(const char *hex)
{
	char *endptr;
	long value = strtol(hex, &endptr, 16);
	return (*endptr == '\0' && value >= 0 && value <= 255) ? (unsigned char)value : 0;
}

int appendHexString(FILE *fp, const char *str)
{
	int i;
	if (strlen(str) % 2 != 0)
		return -1;
	char buffer[3] = {0};
	for (i = 0; i < strlen(str); i += 1)
	{
		strncpy(buffer, str + i, 2);
		appendByte(fp, charFromHex(buffer));
		i++;
	}

	return i / 2;
}

int appendToObjStr(char *str)
{

	if (pass == 2)
	{
		return appendHexString(objfp, str);
	}
	return -1;
}
int appendToObjVal(char val)
{
	if (pass == 2)
	{
		return appendByte(objfp, val);
	}
	return -1;
}

long appendToObjLong(long val)
{
	if (pass == 2)
	{
		return appendLong(objfp, val);
	}
	return -1;
}
void asciiToHexHelper(const char *asciiStr, char *hexStr)
{
	int i;
	for (i = 0; asciiStr[i] != '\0'; i++)
	{
		// Convert each character to its hexadecimal equivalent
		sprintf(&hexStr[i * 2], "%02X", (unsigned char)asciiStr[i]);
	}
	hexStr[i * 2] = '\0'; // Null-terminate the resulting string
}

char *asciiToHexStr(char *str)
{

	char *output = calloc(strlen(str) + 1, 2);

	asciiToHexHelper(str, output);

	return output;
}
