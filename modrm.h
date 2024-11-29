long getModRM(int mod, int dest, int src);

int getYoo(char *opc, char *reg);

int getYoo2(char *opc, char *reg, long imm);
int tworeg(char *op, char *r1, char *r2);
int regimm(char *op, char *reg, long immval);
int onlyOp(char *op);
int opimm(char *op, long imm);
int addrRegReg(char *op, char *r1, char *r2, long imm);
int oplabel(char *op, char *label, long current);
int addrRegImmImm(char *op, char *reg, long addrImm, long imm);
int regLbl(char *op, char *reg, char *label);
int regLblAddr(char *op, char *reg, char *label, long imm);
int regAddr(char *op, char *reg1, char *reg2);
