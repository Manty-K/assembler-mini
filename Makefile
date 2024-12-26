p: p.o objgen.o
	./p.o 1 < t.s > /dev/null && ./p.o 2 < t.s > output.lst && bash -c 'paste output.lst <(sed "s/^/\t\t\t/" t.s) -d " " | nl '
	awk '/SECTION TEXT/ {found=1} found' output.lst | sed '1d' | cut -f2 -d ' '|tr -d '[,' |tr -d '],' > textsection.lst
	awk '/SECTION DATA/ {found=1} found' output.lst | sed '1d' | awk '/SECTION/ {exit} {print}' | cut -f2 -d ' '> datasection.lst
	cut -f 3 p.sym  | sed 's/b/02/g' | sed 's/d/01/g'| sed 's/t/03/g' > nums.sym
	paste nums.sym p.sym | cut -f 1,2,5| tr -d '\t' > symb.tmp
	cut -f 2 p.sym | sed 's/$$/./g' > symlbl.tmp
	./objgen.o textsection.lst textsection.o
	./objgen.o datasection.lst datasection.o
	./objgen.o symb.tmp symb.bin

	cat datasection.o >> output.o
	cat textsection.o >> output.o
	cat symb.bin >> output.o
	cat symlbl.tmp >> output.o
	xxd output.o
 
p.o: lex.yy.c p.tab.c utils.c symb.c modrm.c
	gcc p.tab.c lex.yy.c utils.c symb.c modrm.c -lfl -o p.o -Wall

debug.o: lex.yy.c p.tab.c utils.c symb.c modrm.c
	gcc p.tab.c lex.yy.c utils.c symb.c modrm.c -lfl -o debug.o -Wall -g

p.tab.c: p.y utils.h
	bison -d p.y

objgen.o: objgen.c
	gcc objgen.c -o objgen.o

lex.yy.c: p.l
	flex p.l

lst:
	nasm -felf32 t.s -l t.lst
	cat t.lst
	nm t.o
	
clean:
	rm -rf *.tab.* lex.yy.c *.o *.sym *.lst *.bin *.tmp

