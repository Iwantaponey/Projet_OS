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
	mot w = 
	{tid, num, nb_char, malloc(nb_char*sizeof(char))};
	return w;
}

message init_mess(pid_t pid, int num, int nb_mots)
{
	message m=
	{pid, num, nb_mots, malloc(nb_mots*sizeof(mot))};
	return m;
}

traitement init_traitement(char chiffrement, int cle, int nb_messages, char ** chemin)
{
	
	traitement t=
	{chiffrement, cle, nb_messages, chemin };
	return t;
}

traitement extraire (traitement t, char ** parametres)
{	
	
	return t;
}
