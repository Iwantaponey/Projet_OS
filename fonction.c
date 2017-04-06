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
	m.chemin=chemin;
	return m;
}

traitement init_traitement(int * chiffrements, int * cles, int nb_messages, char ** chemins)
{
	
	traitement t=
	{malloc(nb_messages*sizeof(int)), malloc(nb_messages*sizeof(int)), nb_messages, chemins, malloc(nb_messages*sizeof(message))};
	t.chiffrements=chiffrements;
	t.cles=cles;
	t.chemins=chemins;
	return t;
}

traitement extraire (char * nom_fichier)
{	
	traitement t;
	int e=1,i,j,l,fd; char s='a'; 
	int nb_messages=0, nb_mots=0, nb_char, num_mess, num_mot, compteur=0;
	
	fd=open(nom_fichier, O_RDONLY);
	
	while(read(fd,&s,1) && s!='\0') // premier scan du fichier principal pour déterminer le nombre de messages à traiter
	{
		printf("Le caractère lu est %c \n",s);
		if (s=='\n') 
		{
			++nb_messages;
			printf("Il y a %d messages a traiter dans %s \n", nb_messages, nom_fichier);
		}
	}
	
	close(fd);
	int cle[nb_messages][2]; int cle_finale[nb_messages]; int chiffrements[nb_messages];
	s='a';
	
	
	char ** chemins=malloc(nb_messages*sizeof(char*));
	for (i=0; i<nb_messages; i++)
	{
		chemins[i]=malloc(taille_max_chemin*sizeof(char));
		cle[i][2]=-1;
	}
	i=j=l=0;
	
	fd=open(nom_fichier, O_RDONLY);
	while(e && s!='\0' && i<nb_messages && j<taille_max_chemin) // boucle pour la lecture du fichier principal
	{	
		e=read(fd,&s,1); // on lit les caracteres l un apres l autre
		if (!e) {printf("Erreur de lecture du fichier principal \n");}
		else
		{
			printf("Boucle else \n");
			if (s!='\n') // on traite les messages les uns apres les autres
			{
				printf("Traitement du message %d \n",i);
				if (s!=';' && compteur==0) // on recupere les chemins de chaque message si compteur vaut 0 (si on n'a pas encore vu de ; )
				{	
					chemins[i][j]=s;
					j++;
					printf("Le caractere %c est dans le tableau chemins a la case [%d][%d]\n",s,i,j);
				}
				if (s==';') ++compteur; // quand on rencontre un ; on augmente le compteur 
				if (s!=';' && compteur==1) // on recupere la cle de chaque message si compteur vaut 1 (si on a vu un seul ; )
				{
					cle[i][l]=s-48; // etant donné que la clé peut être sur 2 caracteres, on a un tableau à 2 dimensions cle [i][l] qui nous permet pour chaque message de stocker les deux chiffres composant la clé qu'on reconstituera plus tard
					l++;
					printf("Le caractere %c est dans le tableau cle a la case [%d][%d] \n", s, i, l);
				}
				if (s!=';' && compteur==2) // on recupere les modes de traitement pour chaque message si compteur vaut 2 (si on a vu deux ; )
				{
					if (s=='c') chiffrements[i]=1; 
					else chiffrements[i]=0;
					printf("Le caractere %c est dans le tableau chiffrements a la case [%d] \n",s,i);
				}
			}
			else
			{
			++i; compteur=0; j=0;
			}	
		}
	}
	close(fd);
	
	for (i=0; i<nb_messages; ++i)
	{
		if (cle[i][2]!=-1)
		cle_finale[i]=(10*cle[i][0]+cle[i][1])%26;
		else
		cle_finale[i]=(cle[i][0])%26;
		
	}
	t=init_traitement(chiffrements, cle_finale, nb_messages, chemins); 
	return t;
}


void affiche_traitement(traitement t)
{
	printf("Il y a %d messages dans la structure traitement \n", t.nb_messages);
	int i,j,l;
	for (i=0; i<t.nb_messages; ++i)
	{
		printf("Le message numero %d a pour chiffrement %d, pour cle %d et pour chemin %s \n",i+1, t.chiffrements[i], t.cles[i], t.chemins[i]);
		
	}
}
