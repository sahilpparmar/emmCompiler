CC = g++
#CC = clang++
CFLAGS = -c -g -O
FLEX = flex
BISON = bison

demo:	driveParse.o E--_lexer.o E--_parser.o E--.tab.h
	$(CC) -o demo E--_lexer.o E--_parser.o driveParse.o -lfl

E--_lexer.o:	E--_lexer.C E--.tab.h
	g++ $(CFLAGS) E--_lexer.C

E--_parser.C: E--_parser.y++
	$(BISON) -v -t -d -o E--_parser.C E--_parser.y++;
	mv E--_parser.H E--.tab.h

clean:
	-echo "Removing all object files!"
	-rm -f demo *.o
	-echo "Removing intermediate C files!"
	-rm -f E--_parser.C
