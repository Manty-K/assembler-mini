p: p.o
	./p.o 1 < t.s > /dev/null && ./p.o 2 < t.s > output.lst && bash -c 'paste output.lst <(sed "s/^/\t\t\t/" t.s) -d " " | nl '

p.o: lex.yy.c p.tab.c utils.c symb.c modrm.c
	gcc p.tab.c lex.yy.c utils.c symb.c modrm.c -lfl -o p.o -Wall

p.tab.c: p.y utils.h
	bison -d p.y

lex.yy.c: p.l
	flex p.l

lst:
	nasm -felf32 t.s -l t.lst
	cat t.lst
	nm t.o
	
clean:
	rm -rf *.tab.* lex.yy.c *.o *.sym *.lst
