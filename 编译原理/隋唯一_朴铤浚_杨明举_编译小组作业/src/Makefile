LEX=flex
YACC=bison
CC=g++
OBJECT=parser

$(OBJECT): lex.yy.o  yacc.tab.o
	$(CC) lex.yy.o yacc.tab.o -o $(OBJECT)
	@./$(OBJECT)	#Run immediately after compilation

lex.yy.o: lex.yy.c
	$(CC) -c lex.yy.c

yacc.tab.o: yacc.tab.c
	$(CC) -c yacc.tab.c

lex.yy.c: lex.l  yacc.tab.h  prepare.h  ASTNode.h
	$(LEX) lex.l

yacc.tab.c  yacc.tab.h: yacc.y  prepare.h  ASTNode.h
	$(YACC) -d yacc.y

clean:
	@rm -f $(OBJECT) *.o yacc.tab.* lex.yy.c *.js
