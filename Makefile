all : cesar

run : cesar
	./cesar

cesar: main.o fonction.o
	gcc main.o fonction.o -lpthread -o cesar

main.o : main.c fonction.h 
	gcc -c -Wall main.c -lpthread 

fonction.o : fonction.c fonction.h
	gcc -c -Wall fonction.c -lpthread

clean: 
	rm -rf *.o
	rm -rf cesar
	rm -rf *_cypher
	rm -rf *_decypher
