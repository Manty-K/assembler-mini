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

int getSingReg(char *opc, int regId, int colm, char *m8, char *m32, int rd)
{
    int count = 0;

    /// For 16 bit
    if (regId > 7 && regId < 16)
    {
        printf("66");
        count++;
    }
    /// Opcode
    if (regId < 16)
    {
        if (rd)
        {
            printf("%02lX", (long)rd + regId % 8);
            count++;
        }
        else
        {
            printf("%s", m32);
            count++;
        }
    }
    else
    {
        if (m8 != NULL)
        {

            printf("%s", m8);
            count++;
        }
        else
        {
            puts("Invalid combination");
            return 0;
        }
    }

    /// rest
    if (regId < 16)
    {
        if (!rd)
        {
            printf("%02lX", getModRM(3, regId % 8, colm));
            count++;
        }
    }
    else if (regId < 20 && regId > 15)
    {

        printf("%02lX", getModRM(3, (regId % 4) + 4, colm));
        count++;
    }
    else if (regId > 20 && regId < 24)
    {
        printf("%02lX", getModRM(3, regId % 4, colm));
        count++;
    }

    return count;
}

int getYoo(char *opc, char *reg)
{
    long regId = getRegId(reg);
    if (!strcmp(opc, "inc"))
    {
        return getSingReg(opc, regId, 0, "FE", NULL, 64);
    }
    else if (!strcmp(opc, "dec"))
    {
        return getSingReg(opc, regId, 1, "FE", NULL, 72);
    }
    else if (!strcmp(opc, "div"))
    {

        return getSingReg(opc, regId, 6, "F6", "F7", 0);
    }
    else if (!strcmp(opc, "mul"))
    {

        return getSingReg(opc, regId, 4, "F6", "F7", 0);
    }
    else if (!strcmp(opc, "jmp"))
    {

        return getSingReg(opc, regId, 4, NULL, "FF", 0);
    }
    else if (!strcmp(opc, "not"))
    {
        return getSingReg(opc, regId, 2, "F6", "F7", 0);
    }
    else if (!strcmp(opc, "neg"))
    {
        return getSingReg(opc, regId, 3, "F6", "F7", 0);
    }
    else
    {
        printf("Not defined");
    }
    return 0;
}

int memAddr(char *op, int colm, int regid)
{
    if (regid > 15 || regid == 12)
    {
        puts("Not supported");
        return 0;
    }
    int count = 2;
    if (regid >= 13)
    {
        printf("67");
        count++;
    }

    printf("%s", op);

    if (regid == 4) // esp
    {
        printf("%02lX", getModRM(0, regid, colm));
        printf("24");
        count++;
    }
    else if (regid == 5) // ebp
    {
        printf("%02lX", getModRM(1, regid, colm));
        printf("00");
        count++;
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
        count++;
    }
    else
    {
        printf("Invalid reg %d", regid);
        return 0;
    }
    return count;
}

int getYoo2(char *opc, char *reg)
{

    long regId = getRegId(reg);
    if (!strcmp(opc, "inc"))
    {
        return memAddr("FF", 0, regId);
    }
    else if (!strcmp(opc, "dec"))
    {
        return memAddr("FF", 1, regId);
    }
    else if (!strcmp(opc, "div"))
    {
        return memAddr("FF", 6, regId);
    }
    else if (!strcmp(opc, "mul"))
    {
        return memAddr("F7", 4, regId);
    }
    else if (!strcmp(opc, "jmp"))
    {
        return memAddr("FF", 4, regId);
    }
    else
    {
        printf("Not defined");
    }
    return 0;
}

long r8modrm(int r1, int r2)
{
    int a;
    int b;
    if (r1 >= 20)
    {
        a = r1 % 4;
    }
    if (r2 >= 20)
    {
        b = r2 % 4;
    }
    if (r1 < 20)
    {
        a = r1 % 4 + 4;
    }
    if (r2 < 20)
    {
        b = r2 % 4 + 4;
    }
    return getModRM(3, a, b);
}

int tworegcalc(char *m8, char *m32, int r1, int r2)
{
    int count = 2;
    if (!sameSizeRegs(r1, r2))
    {
        puts("reg mismatch");
        return 0;
    }

    /// For 16 bit
    if (r1 > 7 && r1 < 16)
    {
        printf("66");
        count++;
    }

    /// OP
    if (r1 > 15)
    {
        printf("%s", m8);
    }
    else
    {
        printf("%s", m32);
    }

    /// rest
    if (r1 < 16)
    {
        printf("%02lX", getModRM(3, r1 % 8, r2 % 8));
    }
    else
    {
        printf("%02lX", r8modrm(r1, r2));
    }
    return count;
}

int tworeg(char *op, char *r1, char *r2)
{

    long r1val = getRegId(r1);
    long r2val = getRegId(r2);

    if (!strcmp(op, "add"))
    {
        return tworegcalc("00", "01", r1val, r2val);
    }
    else if (!strcmp(op, "sub"))
    {
        return tworegcalc("28", "29", r1val, r2val);
    }
    else if (!strcmp(op, "mov"))
    {
        return tworegcalc("88", "89", r1val, r2val);
    }
    else if (!strcmp(op, "xor"))
    {
        return tworegcalc("30", "31", r1val, r2val);
    }
    else if (!strcmp(op, "cmp"))
    {
        return tworegcalc("38", "39", r1val, r2val);
    }
    else if (!strcmp(op, "and"))
    {
        return tworegcalc("20", "21", r1val, r2val);
    }
    else if (!strcmp(op, "or"))
    {
        return tworegcalc("08", "09", r1val, r2val);
    }
    else
    {
        printf("Not defined");
    }

    return 2;
}

int regimmcalc(char *m8, char *m32, int reg, long immval)
{
    int count = 1;
    if (reg >= 8 && reg < 16)
    {
        printf("66");
        count++;
    }

    if (reg > 19)
    {

        printf("%02lX", longFromHex(m8) + (reg % 4));
    }
    else if (reg > 15)
    {
        printf("%02lX", longFromHex(m8) + reg % 4 + 4);
    }
    else
    {
        printf("%02lX", longFromHex(m32) + reg % 8);
    }

    if (immval <= 0xFF && reg > 15)
    {
        printf("%02lX", immval);
        count += 1;
    }
    else if (immval <= 0xFFFF && reg < 16)
    {
        printf("%04lX", immval);
        count += 2;
    }
    else if (immval <= 0xFFFFFFFF && reg < 8)
    {
        printf("%08lX", immval);
        count += 4;
    }
    else
    {
        printf("value out of range");
        return 0;
    }

    return count;
}

int regimm(char *op, char *reg, long immval)
{
    int r = getRegId(reg);

    if (!strcmp(op, "mov"))
    {
        return regimmcalc("B0", "B8", r, immval);
    }
    else
    {
        printf("Not defined");
    }
    return 0;
}

int onlyOp(char *op)
{

    if (!strcmp(op, "ret"))
    {
        printf("C3");
        return 1;
    }
    else if (!strcmp(op, "nop"))
    {
        printf("90");
        return 1;
    }
    else
    {
        printf("Not defined");
        return 0;
    }
}
