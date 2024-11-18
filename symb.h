typedef struct entry
{
    char *label;
    long location;
    char section;

} ENTRY;

ENTRY *getEntry(char *label);

void addLabel(char *label, long location, char section);

void displaySymbolTable();
void saveSymbolTable();
void importSymbolTable();
long getLblLoc(char *label);