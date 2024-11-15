#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include <ctype.h>
#include <string.h>
char *modrm_table[][8] = {
    {"00", "01", "02", "03", "04", "05", "06", "07"},
    {"08", "09", "0A", "0B", "0C", "0D", "0E", "0F"},
    {"10", "11", "12", "13", "14", "15", "16", "17"},
    {"18", "19", "1A", "1B", "1C", "1D", "1E", "1F"},
    {"20", "21", "22", "23", "24", "25", "26", "27"},
    {"28", "29", "2A", "2B", "2C", "2D", "2E", "2F"},
    {"30", "31", "32", "33", "34", "35", "36", "37"},
    {"38", "39", "3A", "3B", "3C", "3D", "3E", "3F"},
    {"40", "41", "42", "43", "44", "45", "46", "47"},
    {"48", "49", "4A", "4B", "4C", "4D", "4E", "4F"},
    {"50", "51", "52", "53", "54", "55", "56", "57"},
    {"58", "59", "5A", "5B", "5C", "5D", "5E", "5F"},
    {"60", "61", "62", "63", "64", "65", "66", "67"},
    {"68", "69", "6A", "6B", "6C", "6D", "6E", "6F"},
    {"70", "71", "72", "73", "74", "75", "76", "77"},
    {"78", "79", "7A", "7B", "7C", "7D", "7E", "7F"},
    {"80", "81", "82", "83", "84", "85", "86", "87"},
    {"88", "89", "8A", "8B", "8C", "8D", "8E", "8F"},
    {"90", "91", "92", "93", "94", "95", "96", "97"},
    {"98", "99", "9A", "9B", "9C", "9D", "9E", "9F"},
    {"A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7"},
    {"A8", "A9", "AA", "AB", "AC", "AD", "AE", "AF"},
    {"B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7"},
    {"B8", "B9", "BA", "BB", "BC", "BD", "BE", "BF"},
    {"C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7"},
    {"C8", "C9", "CA", "CB", "CC", "CD", "CE", "CF"},
    {"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7"},
    {"D8", "D9", "DA", "DB", "DC", "DD", "DE", "DF"},
    {"E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7"},
    {"E8", "E9", "EA", "EB", "EC", "ED", "EE", "EF"},
    {"F0", "F1", "F2", "F3", "F4", "F5", "F6", "F7"},
    {"F8", "F9", "FA", "FB", "FC", "FD", "FE", "FF"},
};

long getModRM(int mod, int dest, int src)
{
    return longFromHex(modrm_table[8 * mod + src][dest]);
}

typedef struct singleReg
{
    char *op;
    int reg;
} SINREG;

INSTHEX getIncSingleReg(char *opc, char *reg, char *op, int offset, int src)
{

    INSTHEX insthex;
    long regId = getRegId(reg);

    if (regId > 7 && regId < 16)
    {
        insthex.prefix = 102;
        printf("66");
    }

    if (regId < 16)
    {
        insthex.modrm = offset + regId % 8;
        printf("%lX", offset + regId % 8);
    }
    else if (regId < 20)
    {
        printf("%s", op);

        printf("%lX", getModRM(3, (regId % 4) + 4, src));
    }
    else
    {
        printf("%s", op);
        printf("%lX", getModRM(3, regId % 4, src));
    }

    return insthex;
}

void getSingReg(char *opc, int regId, int colm, char *m8, char *m32, int rd)
{

    /// For 16 bit
    if (regId > 7 && regId < 16)
    {
        printf("66");
    }
    /// OPcode
    if (regId < 16)
    {
        if (rd)
        {
            printf("%lX", (long)rd + regId % 8);
        }
        else
        {
            printf("%s", m32);
        }
    }
    else
    {
        if (m8 != NULL)
        {

            printf("%s", m8);
        }
        else
        {
            puts("Invalid combination");
            return;
        }
    }

    /// rest
    if (regId < 16)
    {
        if (!rd)
        {
            printf("%lX", getModRM(3, regId % 8, colm));
        }
    }
    else if (regId < 20)
    {

        printf("%lX", getModRM(3, (regId % 4) + 4, colm));
    }
    else
    {
        printf("%lX", getModRM(3, regId % 4, colm));
    }
}

void getYoo(char *opc, char *reg)
{
    long regId = getRegId(reg);
    if (!strcmp(opc, "inc"))
    {
        getSingReg(opc, regId, 0, "FE", NULL, 64);
    }
    else if (!strcmp(opc, "dec"))
    {
        getSingReg(opc, regId, 1, "FE", NULL, 72);
    }
    else if (!strcmp(opc, "div"))
    {

        getSingReg(opc, regId, 6, "F6", "F7", 0);
    }
    else if (!strcmp(opc, "mul"))
    {

        getSingReg(opc, regId, 4, "F6", "F7", 0);
    }
    else if (!strcmp(opc, "jmp"))
    {

        getSingReg(opc, regId, 4, NULL, "FF", 0);
    }
    else
    {
        printf("Not defined");
    }
}

void memAddr(char *op, int colm, int regid)
{
    if (regid > 15 || regid == 12)
    {
        puts("Not supported");
        return;
    }
    if (regid >= 13)
    {
        printf("67");
    }

    printf("%s", op);

    if (regid == 4) // esp
    {
        printf("%02lX", getModRM(0, regid, colm));
        printf("24");
    }
    else if (regid == 5) // ebp
    {
        printf("%02lX", getModRM(1, regid, colm));
        printf("00");
    }
    else if (regid < 8)
    {
        printf("%02lX", getModRM(0, regid, colm));
    }
    else if (regid == 14 || regid == 15)
    {
        printf("%02lX", getModRM(0, regid % 8 - 2, colm));
    }
    else if (regid == 13)
    {
        printf("%02lX", getModRM(1, 6, colm));
        printf("00");
    }
    else
    {
        printf("Invalid reg %d", regid);
    }
}

void getYoo2(char *opc, char *reg)
{

    long regId = getRegId(reg);
    if (!strcmp(opc, "inc"))
    {
        memAddr("FF", 0, regId);
    }
    else if (!strcmp(opc, "dec"))
    {
        memAddr("FF", 1, regId);
    }
    else if (!strcmp(opc, "div"))
    {
        memAddr("FF", 6, regId);
    }
    else if (!strcmp(opc, "mul"))
    {
        memAddr("F7", 4, regId);
    }
    else if (!strcmp(opc, "jmp"))
    {
        memAddr("FF", 4, regId);
    }
    else
    {
        printf("Not defined");
    }
}
