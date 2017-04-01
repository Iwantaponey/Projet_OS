all : cesar

run : cesar
	./cesar

cesar: main.o fonction.o
	gcc main.o fonction.o  -o  cesar

main.o : main.c fonction.h 
	gcc -c -Wall -lpthread main.c

fonction.o : fonction.c fonction.h
	gcc -c -Wall -lpthread fonction.c

clean: 
	rm -rf *.o
	rm -rf cesar
