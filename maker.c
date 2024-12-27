#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>

int file_exists(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file)
    {
        fclose(file);
        return 1;
    }
    return 0;
}

void makeeverything(char *file)
{
    char command[100];

    system("flex p.l");
    system("bison -d p.y --warnings=none");
    system("gcc objgen.c -o objgen.o");
    system("gcc p.tab.c lex.yy.c utils.c symb.c modrm.c -lfl -o p.o -w");
    bzero(command, sizeof(command));
    sprintf(command, "./p.o 1 < %s > /dev/null && ./p.o 2 < %s > output.lst", file, file);
    system(command);
    bzero(command, sizeof(command));
    system("awk '/SECTION TEXT/ {found=1} found' output.lst | sed '1d' | cut -f2 -d ' '|tr -d '[,' |tr -d '],' > textsection.tmp");
    system("awk '/SECTION DATA/ {found=1} found' output.lst | sed '1d' | awk '/SECTION/ {exit} {print}' | cut -f2 -d ' '> datasection.tmp");
    system("cut -f 3 p.sym  | sed 's/b/02/g' | sed 's/d/01/g'| sed 's/t/03/g' > nums.tmp");
    system("paste nums.tmp p.sym | cut -f 1,2,5| tr -d '\t' > symb.tmp");
    system("cut -f 2 p.sym | sed 's/$$/./g' > symlbl.tmp");
    system("./objgen.o textsection.tmp textsection.bin");
    system("./objgen.o datasection.tmp datasection.bin");
    system("./objgen.o symb.tmp symb.bin");
    system("cat datasection.bin >> output.o");
    system("cat textsection.bin >> output.o");
    system("cat symb.bin >> output.o");
    system("cat symlbl.tmp >> output.o");
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {

        puts("No input File");
        exit(1);
    }

    if (!file_exists(argv[1]))
    {
        puts("File does not exists");
        exit(1);
    }
    char filename[256];

    strcpy(filename, argv[1]);
    makeeverything(filename);

    int option = 0;

    if (argc > 2)
    {

        if (!strcmp("-s", argv[2]))
        {
            option = 1; // Symbol Table
        }
        else if (!strcmp("-l", argv[2]))
        {
            option = 2; // Listing File
        }

        else if (!strcmp("-o", argv[2]))
        {
            option = 0; // Object File
        }
        else if (!strcmp("-D", argv[2]))
        {
            option = 3; // Show Data setion
        }
        else if (!strcmp("-T", argv[2]) || !strcmp("-t", argv[2]))
        {
            option = 4; // Show Data setion
        }
        else
        {
            puts("Invalid Argument %s. Use \n-s\n-l\n-o\n-t\n-d");
            exit(1);
        }
    }

    switch (option)
    {
    case 0: // Object Code
        system("cat output.o");
        break;
    case 1: // Symbol Table
        system("cat p.sym");
        break;
    case 2: // Listing File
        system("cat output.lst");
        break;
    case 3: // Data Section
        system("awk '/SECTION DATA/ {found=1} found' output.lst | sed '1d' | awk '/SECTION/ {exit} {print}' | grep .");
        break;
    case 4: // Text Section
        system("awk '/SECTION TEXT/ {found=1} found' output.lst | sed '1d' | tr -d '[,' |tr -d '],'| grep .");
        break;

    default:
        puts("Invalid option");
        exit(1);
        break;
    }

    return 0;
}
