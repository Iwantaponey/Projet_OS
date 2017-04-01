#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <time.h>
#include <sys/times.h>
#include <pthread.h>

typedef struct
{
	int chiffrement;
	int clé;
	char * chemin;
	
}traitement;

traitement extraire (char ** parametres);

