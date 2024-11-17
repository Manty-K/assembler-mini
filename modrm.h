long getModRM(int mod, int dest, int src);

int getYoo(char *opc, char *reg);

int getYoo2(char *opc, char *reg);
int tworeg(char *op, char *r1, char *r2);
int regimm(char *op, char *reg, long immval);
int onlyOp(char *op);
int opimm(char *op, long imm);
int addrRegReg(char *op, char *r1, char *r2);
int oplabel(char *op, char *label);
