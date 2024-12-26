#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *objfp;
int appendToObjStr(char *str);

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        puts("Provide Input and output Files");
        exit(1);
    }
    FILE *fp = fopen(argv[1], "r");

    objfp = fopen(argv[2], "wb");

    char buffer[100];

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) <= 0)
            continue;

        appendToObjStr(buffer);
    }

    fclose(fp);
    fclose(objfp);

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

unsigned char charFromHex(const char *hex)
{
    char *endptr;
    long value = strtol(hex, &endptr, 16);
    return (*endptr == '\0' && value >= 0 && value <= 255) ? (unsigned char)value : 0;
}

int appendHexString(FILE *fp, const char *str)
{
    int i = 0;
    if (strlen(str) % 2 != 0)
        return -1;
    char buffer[3] = {0};
    for (unsigned int i = 0; i < strlen(str); i += 1)
    {

        strncpy(buffer, str + i, 2);
        // printf("%s\n", str + i);
        appendByte(fp, charFromHex(buffer));
        i++;
    }
    return i;
}

int appendToObjStr(char *str)
{

    return appendHexString(objfp, str);
}
int appendToObjVal(char val)
{

    return appendByte(objfp, val);
}

/*

0000000 00a1 0000 0000 0503 0000 0000 4000 8b48
0000010 001d 0000 0000 f3f7 e3f7 d839 0274 0875
0000020 c031 d0f7 d8f7 03eb db31 7490 7502 b807
0000030 0000 0100 05eb 00b8 0000 cd02 c380     man

0000190 00a1 0000 0300 0405 0000 4000 8b48 041d
00001a0 0000 f700 f7f3 39e3 74d8 7502 3108 f7c0
00001b0 f7d0 ebd8 3103 90db 0274 0775 01b8 0000
00001c0 eb00 b805 0002 0000 80cd 00c3 0000 0000
00001d0 2e00 6164 6174 2e00 7362 0073 742e 7865
00001e0 0074 732e 7368 7274 6174 0062 732e 6d79
00001f0 6174 0062 732e 7274 6174 0062 722e 6c65
0000200 742e 7865 0074 0000 0000 0000 0000 0000
0000210 0000 0000 0000 0000 0000 0000 0000 0000
0000220 0001 0000 0000 0000 0000 0000 0004 fff1
0000230 0000 0000 0000 0000 0000 0000 0003 0001
0000240 0000 0000 0000 0000 0000 0000 0003 0002
0000250 0000 0000 0000 0000 0000 0000 0003 0003
0000260 0005 0000 0000 0000 0000 0000 0000 0001
0000270 000a 0000 0004 0000 0000 0000 0000 0001
0000280 000f 0000 0008 0000 0000 0000 0000 0001
0000290 0016 0000 0000 0000 0000 0000 0000 0002
00002a0 0022 0000 001d 0000 0000 0000 0000 0003
00002b0 002e 0000 0025 0000 0000 0000 0000 0003
00002c0 003e 0000 0028 0000 0000 0000 0000 0003
00002d0 0048 0000 002c 0000 0000 0000 0000 0003
00002e0 0053 0000 0033 0000 0000 0000 0000 0003
00002f0 0061 0000 0038 0000 0000 0000 0000 0003
0000300 001b 0000 0000 0000 0000 0000 0010 0003
0000310 7400 732e 6e00 6d75 0031 756e 326d 7200
0000320 7365 6c75 0074 6574 706d 5f00 7473 7261
0000330 0074 616c 6562 5f6c 7165 6175 006c 616c


*/
