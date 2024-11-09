LINEC = $(shell wc -l < t.s)
p: p.o
	./p.o < t.s > output.lst
	bash -c 'paste <(seq -w $(LINEC)) output.lst <(sed "s/^/\t\t\t/" t.s) -d " "'
p.o: lex.yy.c p.tab.c
	gcc p.tab.c lex.yy.c -lfl -o p.o

p.tab.c: p.y
	bison -d p.y

lex.yy.c: p.l
	flex p.l
