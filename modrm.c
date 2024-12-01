#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "symb.h"
#include <ctype.h>
#include <string.h>

int isimm8bit(long imm)
{
    return imm >= -128 && imm <= 127;
}

char getModRM(char mod, char dest, char src)
{
    return mod << 6 | src << 3 | dest;
}

typedef struct singleReg
{
    char *op;
    int reg;
} SINREG;

int getSingReg(int regId, int colm, char *m8, char *m32, int rd)
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
            printf("%02X", getModRM(3, regId % 8, colm));
            count++;
        }
    }
    else if (regId < 20 && regId > 15)
    {

        printf("%02X", getModRM(3, (regId % 4) + 4, colm));
        count++;
    }
    else if (regId > 20 && regId < 24)
    {
        printf("%02X", getModRM(3, regId % 4, colm));
        count++;
    }

    return count;
}

int getYoo(char *opc, char *reg)
{
    long regId = getRegId(reg);
    if (!strcmp(opc, "inc"))
    {
        return getSingReg(regId, 0, "FE", NULL, 64);
    }
    else if (!strcmp(opc, "dec"))
    {
        return getSingReg(regId, 1, "FE", NULL, 72);
    }
    else if (!strcmp(opc, "div"))
    {

        return getSingReg(regId, 6, "F6", "F7", 0);
    }
    else if (!strcmp(opc, "mul"))
    {

        return getSingReg(regId, 4, "F6", "F7", 0);
    }
    else if (!strcmp(opc, "jmp"))
    {

        return getSingReg(regId, 4, NULL, "FF", 0);
    }
    else if (!strcmp(opc, "not"))
    {
        return getSingReg(regId, 2, "F6", "F7", 0);
    }
    else if (!strcmp(opc, "neg"))
    {
        return getSingReg(regId, 3, "F6", "F7", 0);
    }
    else
    {
        printf("Not defined");
    }
    return 0;
}

int memAddr(char *op, int colm, int regid, long imm)
{
    int imm8 = isimm8bit(imm);

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
        if (imm && imm8)
        {
            printf("%02X", getModRM(1, regid, colm));
        }
        else if (imm)
        {
            printf("%02X", getModRM(2, regid, colm));
        }
        else
        {
            printf("%02X", getModRM(0, regid, colm));
        }

        printf("24");
        count++;
    }
    else if (regid == 5) // ebp
    {
        if (imm && imm8)
        {
            printf("%02X", getModRM(1, regid, colm));
        }
        else if (imm)
        {
            printf("%02X", getModRM(2, regid, colm));
        }
        else
        {
            printf("%02X", getModRM(1, regid, colm));
            printf("00");
            count++;
        }
    }
    else if (regid < 8)
    {
        if (imm && imm8)
        {
            printf("%02X", getModRM(1, regid, colm));
        }
        else if (imm)
        {
            printf("%02X", getModRM(2, regid, colm));
        }
        else
        {
            printf("%02X", getModRM(0, regid, colm));
        }
    }
    else if (regid == 14 || regid == 15)
    {

        printf("%02X", getModRM(0, regid % 8 - 2, colm));
    }
    else if (regid == 13)
    {
        printf("%02X", getModRM(1, 6, colm));
        printf("00");
        count++;
    }
    else
    {
        printf("Invalid reg %d", regid);
        return 0;
    }

    if (imm && imm8)
    {

        printf("%02X", (unsigned char)imm);
        count++;
    }
    else if (imm)
    {
        printf("%08X", (unsigned int)imm);
        count += 4;
    }
    return count;
}

int getYoo2(char *opc, char *reg, long imm)
{

    long regId = getRegId(reg);
    if (!strcmp(opc, "inc"))
    {
        return memAddr("FF", 0, regId, imm);
    }
    else if (!strcmp(opc, "dec"))
    {
        return memAddr("FF", 1, regId, imm);
    }
    else if (!strcmp(opc, "div"))
    {
        return memAddr("FF", 6, regId, imm);
    }
    else if (!strcmp(opc, "mul"))
    {
        return memAddr("F7", 4, regId, imm);
    }
    else if (!strcmp(opc, "jmp"))
    {
        return memAddr("FF", 4, regId, imm);
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
        printf("%02X", getModRM(3, r1 % 8, r2 % 8));
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
    else if (immval <= 0xFFFF && reg < 16 && reg > 7)
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
int opimmcalc(char *opc, long imm, int m32Allowed)
{
    printf("%s", opc);
    if (m32Allowed)
    {
        printf("(%08X)", (unsigned int)imm);
        return 5;
    }
    else
    {
        if (imm >= 5 && imm <= 255)
        {
            printf("%02X", (unsigned char)imm);
            return 2;
        }
        else
        {
            printf("Exceed size");
            return 0;
        }
    }

    return 0;
}

int opimm(char *op, long imm)
{

    if (!strcmp(op, "jmp"))
    {

        return opimmcalc("EB", imm, 1);
    }
    else if (!strcmp(op, "int"))
    {

        return opimmcalc("CD", imm, 0);
    }
    else
    {

        printf("Not defined");
    }
    return 0;
}

int addrRegRegCalc(char *m8, char *m32, int r1, int r2, long imm)
{
    int isimm8 = isimm8bit(imm);
    int count = 1;

    if (r2 > 7 && r2 < 16)
    {

        printf("66");
        count++;
    }

    if (r1 >= 13 && r1 <= 15)
    {

        printf("67");
        count++;
    }

    if (r2 > 15)
    {
        printf("%s", m8);
    }
    else
    {
        printf("%s", m32);
    }

    if (r1 < 16 && r2 < 16) // r1: 32,16   r2: 32,16
    {
        if (imm && isimm8)
        {
            if (r1 == 14 || r1 == 15)
            {
                printf("%02X", getModRM(1, r1 % 8, r2 % 8) - 2);
                count++;
            }

            else
            {
                printf("%02X", getModRM(1, r1 % 8, r2 % 8));
                count++;
            }
        }
        else if (imm)
        {
            if (r1 == 14 || r1 == 15)
            {
                printf("%02X", getModRM(2, r1 % 8, r2 % 8) - 2);
                count++;
            }

            else
            {
                printf("%02X", getModRM(2, r1 % 8, r2 % 8));
                count++;
            }
        }
        else
        {

            if (r1 == 14 || r1 == 15)
            {
                printf("%02X", getModRM(0, r1 % 8, r2 % 8) - 2);
                count++;
            }

            else
            {
                printf("%02X", getModRM(0, r1 % 8, r2 % 8));
                count++;
            }
        }
    }
    else if (r1 < 8 && r2 > 15) // r1 32 bit, r1 : 8 bit
    {
        if (r2 >= 20)
        {
            printf("%02X", getModRM(0, r1, r2 % 4));
        }
        else
        {
            printf("%02X", getModRM(0, r1, r2 % 4 + 4));
        }

        count++;
    }

    if (imm && isimm8)
    {
        printf("%02X", (unsigned char)imm);
        count++;
    }
    else if (imm)
    {
        printf("%08X", (unsigned int)imm);
        count += 4;
    }

    if (r1 == 4)
    {
        printf("24");
    }

    return count;
}

int addrRegReg(char *op, char *r1, char *r2, long imm)
{
    int r1val = getRegId(r1);
    int r2val = getRegId(r2);

    if (!strcmp(op, "mov"))
    {
        return addrRegRegCalc("88", "89", r1val, r2val, imm);
    }
    else
    {
        printf("not defined");
    }

    return 0;
}
int oplabelCalc(char *opc, long location)
{

    printf("%s", opc);
    printf("%02X", (unsigned char)location - 2);

    return 2;
}

int oplabel(char *op, char *label, long currentLoc)
{
    long loc = getLblLoc(label);
    long relative = loc - currentLoc;

    if (!strcmp(op, "jmp"))
    {
        return oplabelCalc("EB", relative);
    }
    else if (!strcmp(op, "jz") || !strcmp(op, "je"))
    {
        return oplabelCalc("74", relative);
    }
    else if (!strcmp(op, "jnz") || !strcmp(op, "jne"))
    {
        return oplabelCalc("75", relative);
    }
    else
    {
        printf("Not defined");
    }
    return 0;
}

int addrRegImmImmCalc(char *m8, char *m32, int reg, long addrImm, long imm, int colm)
{
    int imm8 = isimm8bit(addrImm);

    int is8imm = isimm8bit(imm);
    int count = 2;

    if (is8imm && m8)
    {
        printf("%s", m8);
    }
    else
    {
        printf("%s", m32);
    }

    int mod;

    if (addrImm && imm8)
    {
        mod = 1;
    }
    else if (addrImm)
    {
        mod = 2;
    }
    else
    {
        mod = 0;
    }

    printf("%02X", getModRM(mod, reg, colm));

    if (addrImm && imm8)
    {
        printf("%02X", (unsigned char)addrImm);
        count++;
    }
    else if (addrImm)
    {
        printf("%08X", (unsigned int)addrImm);
        count += 4;
    }

    if (is8imm && m8)
    {
        printf("%02X", (unsigned int)imm);
        count++;
    }
    else
    {
        printf("%08X", (unsigned int)imm);
        count += 4;
    }

    return count;
}

int addrRegImmImm(char *op, char *reg, long addrImm, long imm)
{
    int regval = getRegId(reg);

    if (!strcmp(op, "mov"))
    {
        return addrRegImmImmCalc(NULL, "C7", regval, addrImm, imm, 0);
    }
    else if (!strcmp(op, "add"))
    {
        return addrRegImmImmCalc("83", "81", regval, addrImm, imm, 0);
    }
    else if (!strcmp(op, "sub"))
    {
        return addrRegImmImmCalc("83", "81", regval, addrImm, imm, 5);
    }
    else if (!strcmp(op, "xor"))
    {
        return addrRegImmImmCalc("83", "81", regval, addrImm, imm, 6);
    }
    else if (!strcmp(op, "cmp"))
    {
        return addrRegImmImmCalc("83", "81", regval, addrImm, imm, 7);
    }
    else
    {
        printf("Not defined");
    }

    return 0;
}

int regLblCalc(char *rd8, char *rd32, int reg, long label, int colm, char *m8, char *m32, char *eax8, char *eax32)
{
    int count = 0;
    if (reg > 7 && reg < 15)
    {
        printf("66");
        count++;
    }
    if (rd32 != NULL && rd8 != NULL)
    {
        long base;
        long offset;

        if (reg < 16)
        {
            base = longFromHex(rd32);
            offset = base + ((long)reg % 8);
        }
        else
        {
            base = longFromHex(rd8);

            if (reg >= 20)
            {
                offset = base + ((long)reg % 4);
            }
            else
            {
                offset = base + ((long)reg % 4) + 4;
            }
        }

        printf("%02X", (unsigned char)offset);
        count++;
    }
    else if (m32 != NULL)
    {

        if (reg == 0 || reg == 8 || reg == 20)
        {
            if (reg == 20)
            {
                printf("%s", eax8);
            }
            else
            {
                printf("%s", eax32);
            }
            count++;
        }
        else
        {
            if (reg < 16)
            {
                printf("%s", m32);
            }
            else
            {
                printf("%s", m8);
            }

            count++;
            if (reg < 16)
            {
                printf("%02X", getModRM(3, reg % 8, 0));
            }
            else
            {
                if (reg >= 20)
                {
                    printf("%02X", getModRM(3, reg % 4, 0));
                }
                else
                {
                    printf("%02X", getModRM(3, reg % 4 + 4, 0));
                }
            }

            count++;
        }
    }
    else
    {
        printf("Either column or rd needs to be null");
        return 0;
    }
    if (reg < 8)
    {
        printf("[%08X]", (unsigned int)label);
        count += 4;
    }
    else if (reg > 7 && reg < 16)
    {
        printf("[%04X]", (unsigned int)label);
        count += 2;
    }
    else
    {
        printf("[%02X]", (unsigned int)label);
        count += 1;
    }
    return count;
}

int regLbl(char *op, char *reg, char *label)
{
    long loc = getLblLoc(label);
    int regVal = getRegId(reg);

    if (!strcmp(op, "mov"))
    {
        return regLblCalc("B0", "B8", regVal, loc, -1, NULL, NULL, NULL, NULL);
    }
    else if (!strcmp(op, "add"))
    {
        return regLblCalc(NULL, NULL, regVal, loc, 0, "80", "81", "04", "05");
    }
    else
    {
        printf("Not defined");
    }
    return 0;
}

int regLblAddrCalc(char *opc, char *eaxop, int reg, long location, int isImm)
{
    int count = 0;

    if (reg == 0 && eaxop != NULL)
    {
        printf("%s", eaxop);
        count = 1;
    }
    else
    {
        printf("%s", opc);
        printf("%02X", getModRM(0, 5, reg));
        count += 2;
    }

    if (isImm)
    {
        printf("%08lX", location);
    }
    else
    {
        printf("[%08lX]", location);
    }
    count += 4;

    return count;
}

int regLblAddr(char *op, char *reg, char *label, long imm, long offset)
{
    long loc;

    if (label == NULL)
    {
        loc = imm;
    }
    else
    {
        if (offset == 0)
        {
            loc = getLblLoc(label);
        }
        else
        {
            loc = getLblLoc(label) + offset;
        }
    }

    int regVal = getRegId(reg);
    if (!strcmp(op, "mov"))
    {
        return regLblAddrCalc("8B", "A1", regVal, loc, imm ? 1 : 0);
    }
    else if (!strcmp(op, "add"))
    {
        return regLblAddrCalc("03", NULL, regVal, loc, imm ? 1 : 0);
    }
    else
    {
        printf("Not defined");
    }
    return 0;
}

int regAddrCalc(char *opc, int reg1, int reg2, long imm)
{
    int count = 1;
    printf("%s", opc);
    if (imm == 0)
    {
        printf("%02X", getModRM(0, reg2, reg1));
        count += 1;
    }
    else
    {
        printf("%02X", getModRM(1, reg2, reg1));
        printf("%02X", (unsigned char)imm);

        count += 2;
    }
    return count;
}

int regAddr(char *op, char *reg1, char *reg2, long imm)
{
    int reg1Val = getRegId(reg1);
    int reg2Val = getRegId(reg2);
    if (!strcmp(op, "mov"))
    {
        return regAddrCalc("8B", reg1Val, reg2Val, imm);
    }
    else
    {
        printf("Not defined");
    }
    return 0;
}
