all : cesar

run : cesar
	./cesar

cesar: main.o extraction.o traitement.o renvoi.o
	gcc main.o extraction.o traitement.o renvoi.o -o  cesar

main.o : main.c structures.h extraction.h traitement.h renvoi.h
	gcc -c -Wall -lpthread main.c

extraction.o : extraction.c extraction.h structures.h
	gcc -c -Wall -lpthread extraction.c

traitement.o : traitement.c traitement.h structures.h
	gcc -c -Wall -lpthread traitement.c
	
renvoi.o : renvoi.c renvoi.h structures.h
	gcc -c -Wall -lpthread renvoi.c

clean: 
	rm -rf *.o
	rm -rf cesar
