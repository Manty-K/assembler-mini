LINEC = $(shell wc -l < t.s)
p: p.o
	./p.o < t.s > output.lst
	# bash -c 'paste <(seq -w $(LINEC)) output.lst <(sed "s/^/\t\t\t/" t.s) -d " "'
	bash -c 'paste output.lst <(sed "s/^/\t\t\t/" t.s) -d " " | nl '
p.o: lex.yy.c p.tab.c utils.c symb.c
	gcc p.tab.c lex.yy.c utils.c symb.c modrm.c -lfl -o p.o

p.tab.c: p.y utils.h
	bison -d p.y

lex.yy.c: p.l
	flex p.l

lst:
	nasm -felf32 t.s -l t.lst
	cat t.lst
	nm t.o
	
clean:
	rm -rf *.tab.* lex.yy.c *.o
