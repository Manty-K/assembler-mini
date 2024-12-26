typedef struct entry
{
    char *label;
    long location;
    char section;
    long value;

} ENTRY;

ENTRY *getEntry(char *label);

void addLabel(char *label, long location, char section, long value);

void displaySymbolTable();
void saveSymbolTable();
void importSymbolTable();
long getLblLoc(char *label);