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

mot init_mot(pthread_t tid, int num_mot, int nb_char)
{
	mot w = 
	{tid, num_mot, nb_char, malloc(nb_char*sizeof(char))};
	return w;
}

message init_mess(pid_t pid, int num_mess, int nb_mots, int chiffrement, int cle, char * chemin)
{
	message m=
	{pid, num_mess, nb_mots, malloc(nb_mots*sizeof(mot)), chiffrement, cle, malloc(taille_max_chemin*sizeof(char))};
	return m;
}

traitement init_traitement(int * chiffrements, int * cles, int nb_messages, char ** chemins)
{
	
	traitement t=
	{malloc(nb_messages*sizeof(int)), malloc(nb_messages*sizeof(int)), nb_messages, chemins, malloc(nb_messages*sizeof(message))};
	return t;
}

traitement extraire (char * nom_fichier)
{	
	traitement t;
	int e,i,j,l,fd; char s='a'; 
	int nb_messages=0, nb_mots=0, nb_char, num_mess, num_mot, compteur=0;
	
	fd=open(nom_fichier, O_RDONLY);
	
	while(s!='\0') // premier scan du fichier principal pour déterminer le nombre de messages à traiter
	{
		read(fd,&s,1);
		if (s=='\n') 
		{
			++nb_messages;
			printf("Il y a %d messages a traiter dans %s", nb_messages, nom_fichier);
		}
	}
	int cle[nb_messages][2]; int cle_finale[nb_messages]; int chiffrements[nb_messages];
	s='a';
	char ** chemins=malloc(nb_messages*sizeof(char*));
	for (i=0; i<nb_messages; i++)
	{
		chemins[i]=malloc(taille_max_chemin*sizeof(char));
	}
	i=j=l=0;
	while(s!='\0' && i<nb_messages && j<taille_max_chemin) // boucle pour la lecture du fichier principal
	{	
		e=read(fd,&s,1);
		if (!e) printf("Erreur de lecture du fichier principal");
		if (s!='\n')
		{
			if (s!=';' && compteur==0)
			{	
				chemins[i][j]=s;
				j++;
			}
			if (s==';') ++compteur;
			if (s!=';' && compteur==1)
			{
				cle[i][l]=atoi(&s);
				l++;
			}
			if (s!=';' && compteur==2)
			{
				if (s=='c') chiffrements[i]=1;
				else chiffrements[i]=0;
			}
		}
		++i; compteur=0;
	}
	close(fd);
	
	
	return t;
}
