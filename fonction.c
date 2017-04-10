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
	{malloc(nb_messages*sizeof(int)),malloc(nb_messages*sizeof(int)), nb_messages, chemins, malloc(nb_messages*sizeof(message))};
	int i;
	for (i=0; i<nb_messages; i++)
	{
		t.chiffrements[i]=chiffrements[i];
		printf(" pour le chiffrement du message %d dans t, on a %d", i, t.chiffrements[i]);
		t.cles[i]=cles[i];
		printf(" pour la cle du message %d dans t, on a %d", i, t.cles[i]);
	}
	
	return t;
}

int compte_nb_messages(char * nom_fichier)
{
	int	fd=open(nom_fichier, O_RDONLY), nb_messages=0; char s='a';
	
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
	return nb_messages;	
}

int * recupere_cle(int nb_messages, int cle[nb_messages][2])
{
	int i,* cle_finale=malloc(nb_messages*sizeof(int));
	for (i=0; i<nb_messages; ++i)
	{
		if (cle[i][1]!=-1)
		{
			cle_finale[i]=((10*cle[i][0])+cle[i][1]);
			printf("Il y a l'entier %d dans le tableau cle_finale a la case %d \n", cle_finale[i],i);
		}
		else
		{	
			cle_finale[i]=(cle[i][0]);
			printf("Il y a l'entier %d dans le tableau cle_finale a la case %d \n", cle_finale[i],i);
		}
	}
	return cle_finale;
}

traitement extraire (char * nom_fichier)
{	
	traitement t;
	int e=1,i,j,l,fd; char s='a'; 
	int nb_messages=compte_nb_messages(nom_fichier), compteur=0;
	int cle[nb_messages][2];  int chiffrements[nb_messages];
	char ** chemins=malloc(nb_messages*sizeof(char*));
	for (i=0; i<nb_messages; i++)
	{
		chemins[i]=malloc(taille_max_chemin*sizeof(char));
		cle[i][1]=-1;
	}
	i=j=l=0;
	fd=open(nom_fichier, O_RDONLY);
	while(e && s!='\0' && i<nb_messages && j<taille_max_chemin) // boucle pour la lecture du fichier principal
	{	
		e=read(fd,&s,1); // on lit les caracteres l un apres l autre
		if (!e) {printf("Erreur de lecture du fichier principal \n");}
		else
		{
			if (s!='\n') // on traite les messages les uns apres les autres
			{
				printf("Traitement du message %d \n",i);
				if (s!=';' && compteur==0) // on recupere les chemins de chaque message si compteur vaut 0 (si on n'a pas encore vu de ; )
				{	
					chemins[i][j]=s;
					printf("Le caractere %c est dans le tableau chemins a la case [%d][%d]\n",s,i,j);
					j++;
					
				}
				if (s==';') ++compteur; // quand on rencontre un ; on augmente le compteur 
				if (s!=';' && compteur==1) // on recupere la cle de chaque message si compteur vaut 1 (si on a vu un seul ; )
				{
					cle[i][l]=s-48; // etant donné que la clé peut être sur 2 caracteres, on a un tableau à 2 dimensions cle [i][l] qui nous permet pour chaque message de stocker les deux chiffres composant la clé qu'on reconstituera plus tard
					printf("Le caractere %c est dans le tableau cle a la case [%d][%d] \n", s, i, l);
					l++;
									}
				if (s!=';' && compteur==2) // on recupere les modes de traitement pour chaque message si compteur vaut 2 (si on a vu deux ; )
				{
					if (s=='c') chiffrements[i]=1; 
					else chiffrements[i]=0;
					printf("Le caractere %d est dans le tableau chiffrements a la case [%d] \n",chiffrements[i]	,i);
				}
			}
			else
			{
			++i; compteur=0; j=0; l=0;
			}	
		}
	}
	close(fd);

	int * cle_finale = malloc(nb_messages* sizeof(int));
	cle_finale=recupere_cle(nb_messages, cle);
	
	for (i=0; i<nb_messages; i++)
	{
		printf("chiffrements[%d] = %d \n cle_finale[%d] = %d \n chemins[%d] = %s \n", i ,chiffrements[i], i, cle_finale[i], i, chemins[i]);
	}
	
	t=init_traitement(chiffrements, cle_finale, nb_messages, chemins); 
	return t;
}

int * compte_nb_mots(int * nb_mots, traitement t)
{	
	int i, fd;
	char s='a';
	fd=open(t.chemins[i], O_RDONLY);
	for (i=0; i<t.nb_messages && (read(fd,&s,1)); ++i)
	{	
		if (s==' '|| s=='\n') nb_mots[i]=nb_mots[i]+1; 
	}
	close(fd);
	return nb_mots;
}

int ** compte_nb_char(int * nb_mots, int ** nb_char, traitement t)
{
	int i,j,fd; char s='a';
		
	for (i=0; i<t.nb_messages; ++i)
	{
		fd=open(t.chemins[i], O_RDONLY);
		for(j=0; j<nb_mots[i]; ++j)
		{
			read(fd,&s,1);
			if (s!=' ' && s!='\n')
			nb_char[i][j]=nb_char[i][j]+1;
		}
		close(fd);
	}
	return nb_char;
}

traitement assigne_message (traitement t)
{
	int e,fd,i,j,k,* nb_mots = malloc(t.nb_messages*sizeof(int)), num_mess[t.nb_messages];
	char s='a';
	
	nb_mots=compte_nb_mots(nb_mots,t);
	
	int ** num_mots=malloc(t.nb_messages*sizeof(int*)),** nb_char=malloc(t.nb_messages*sizeof(int*));
	
	for (i=0; i<t.nb_messages; ++i)
	{	
		num_mots[i]=malloc(nb_mots[i]*sizeof(int));
	}
	
	for (i=0; i<t.nb_messages; ++i)
	{
		num_mess[i]=i;
		for (j=0; j<nb_mots[i]; ++j)
		{
			num_mots[i][j]=j;
			nb_char[i][j]=0;
		}
	}
	
	nb_char=compte_nb_char(nb_mots, nb_char, t);
	
	char *** tab_mess;
	tab_mess=malloc(t.nb_messages*sizeof(char**));
	for (i=0; i<t.nb_messages; ++i)
	{
		tab_mess[i]=malloc(nb_mots[i]*sizeof(char*));
		for(j=0; j<nb_mots[i]; ++j)
		{
				tab_mess[i][j]=malloc(nb_char[i][j]*sizeof(char));
		}
	}
	
	
	fd=open(t.chemins[i], O_RDONLY);
	for (i=0; i<t.nb_messages; ++i)
	{
		
		while(s!='\0')
		{
			
			
			
		}
		close(fd);
	}
	return t;
}



void affiche_traitement(traitement t)
{
	printf("Il y a %d messages dans la structure traitement \n", t.nb_messages);
	int i,j,l;
	for (i=0; i<t.nb_messages; ++i)
	{
		printf("Le message numero %d a pour chiffrement %d, pour cle %d et pour chemin %s \n",i, t.chiffrements[i], t.cles[i], t.chemins[i]);
		
	}
}
