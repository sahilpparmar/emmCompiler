CC= g++
#CC= clang++
CFLAGS = -c -g -O 
FLEX = flex

demo:	drivLex.o E--_lexer.o
	$(CC) -o demo E--_lexer.o drivLex.o -lfl

E--_lexer.o:	E--_lexer.C
	$(CC) $(CFLAGS) E--_lexer.C

E--_lexer.C:	E--_lexer.l++
	$(FLEX) -p -8 -Ce -o E--_lexer.C E--_lexer.l++

drivLex.o: drivLex.C
	$(CC) $(CFLAGS) drivLex.C

clean:
	-echo "Removing all object files!"
	-rm -f demo *.o
	-echo "Removing intermediate C files!"
	-rm -f E--_lexer.C
