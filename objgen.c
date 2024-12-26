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
