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
#include "fonction.h"

mot init_mot(pthread_t tid, int num, int nb_char)
{
	mot m = 
	{tid, num, nb_char, malloc(nb_char*sizeof(char))};
	return m;
}

traitement extraire (traitement t, char ** parametres)
{	
	
	return t;
}
