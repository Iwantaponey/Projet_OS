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

mot init_mot(int num_mot, int nb_char, int chiffrement, int cle, char * tab_char)
{
	mot w = 
	{num_mot, nb_char, chiffrement, cle, tab_char};
	return w;
}

message init_mess(int num_mess, int nb_mots, mot * tab_mots, int chiffrement, int cle, char * chemin)
{
	pid_t pid;
	message m=
	{pid, num_mess, nb_mots, tab_mots , chiffrement, cle, chemin};
	return m;
}

traitement init_traitement(int * chiffrements, int * cles, int nb_messages, char ** chemins)
{
	traitement t=
	{chiffrements,cles, nb_messages, chemins, malloc(nb_messages*sizeof(message))};
	return t;
}

buffer init_buffer(int taille_buff)
{
	buffer b={taille_buff, malloc(taille_buff*sizeof(char)), PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};
	return b;
}

int compte_nb_messages(char * nom_fichier)
{
	int	fd=open(nom_fichier, O_RDONLY), nb_messages=0; char s='a';
	if (fd<1) 
	{
		printf("Probleme avec l'ouverture du fichier \n");
		return -1;
	}
	while(read(fd,&s,1) && s!='\0') // premier scan du fichier principal pour déterminer le nombre de messages à traiter
	{
		//printf("Le caractère lu est %c \n",s);
		if (s=='\n') 
		{
			++nb_messages;
			printf("Il y a %d messages a traiter dans %s \n", nb_messages, nom_fichier);
		}
	}	
	
	close(fd);
	return nb_messages;	
}

int * recupere_cle(int * cle_finale, int nb_messages, int cle[nb_messages][2])
{
	int i;
	for (i=0; i<nb_messages; ++i)
	{
		if (cle[i][1]!=-1)
		{
			cle_finale[i]=((10*cle[i][0])+cle[i][1]);
			//printf("Il y a l'entier %d dans le tableau cle_finale a la case %d \n", cle_finale[i],i);
		}
		else
		{	
			cle_finale[i]=(cle[i][0]);
			//printf("Il y a l'entier %d dans le tableau cle_finale a la case %d \n", cle_finale[i],i);
		}
	}
	return cle_finale;
}

traitement extraire (char * nom_fichier)
{	
	traitement t;
	int e=1,i,j,l,fd; char s='a'; 
	int nb_messages=compte_nb_messages(nom_fichier), compteur=0;
	if (nb_messages==-1) printf ("structure traitement non remplie car probleme ouverture fichier \n");
	int cle[nb_messages][2];  int * chiffrements=malloc(nb_messages*sizeof(int));
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
			//		printf("Le caractere %c est dans le tableau chemins a la case [%d][%d]\n",s,i,j);
					j++;
					
				}
				if (s==';') ++compteur; // quand on rencontre un ; on augmente le compteur 
				if (s!=';' && compteur==1) // on recupere la cle de chaque message si compteur vaut 1 (si on a vu un seul ; )
				{
					cle[i][l]=s-48; // etant donné que la clé peut être sur 2 caracteres, on a un tableau à 2 dimensions cle [i][l] qui nous permet pour chaque message de stocker les deux chiffres composant la clé qu'on reconstituera plus tard
		//			printf("Le caractere %c est dans le tableau cle a la case [%d][%d] \n", s, i, l);
					l++;
									}
				if (s!=';' && compteur==2) // on recupere les modes de traitement pour chaque message si compteur vaut 2 (si on a vu deux ; )
				{
					if (s=='c') chiffrements[i]=1; 
					else chiffrements[i]=0;
					//printf("Le caractere %d est dans le tableau chiffrements a la case [%d] \n",chiffrements[i]	,i);
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
	cle_finale=recupere_cle(cle_finale,nb_messages, cle);
	
	for (i=0; i<nb_messages; i++)
	{
	 	printf("chiffrements[%d] = %d \n cle_finale[%d] = %d \n chemins[%d] = %s \n", i ,chiffrements[i], i, cle_finale[i], i, chemins[i]);
	}

	t=init_traitement(chiffrements, cle_finale, nb_messages, chemins); 
	return t;
}


int ** compte_nb_char(int * nb_mots, int ** nb_char, traitement t)
{
	int i,j,fd; char s='a';
		
	for (i=0; i<t.nb_messages; ++i)
	{
		fd=open(t.chemins[i], O_RDONLY);
		for(j=0; j<nb_mots[i]; ++j)
		{
			while(read(fd,&s,1)&& s!=' ' && s!='\n')
			nb_char[i][j]=nb_char[i][j]+1;
		}
		close(fd);
	}
	return nb_char;
}

int * compte_nb_mots(int * nb_mots, traitement t)
{	
	int i, fd, compteur;
	char s='a';
	for (i=0; i<t.nb_messages; ++i)
	{	
		fd=open(t.chemins[i], O_RDONLY);
		while(read(fd,&s,1) && s!='\0')
		{	
			if ((s==' '|| s=='\n' || s=='\0' || s=='\t'))
			{	if (compteur==0)
				{
					nb_mots[i]=nb_mots[i]+1; 
				}
				else
					compteur++;
			}
			else compteur=0;
		}
		close(fd);
	}
	
	return nb_mots;
}


traitement assigne_message (traitement t)
{
	int fd,i,j,k,* nb_mots = malloc(t.nb_messages*sizeof(int));
	char s='a';
	for (i=0; i<t.nb_messages; ++i)
	{
		nb_mots[i]=0;
	}
	
	nb_mots=compte_nb_mots(nb_mots,t);
	for (i=0; i<t.nb_messages; ++i)
	{
		//printf("Il y a %d mots dans le message %d \n",nb_mots[i], i);
		
	}
	
	int ** nb_char=malloc(t.nb_messages*sizeof(int*));
	
	for (i=0; i<t.nb_messages; ++i)
	{	
		nb_char[i]=malloc(nb_mots[i]*sizeof(int));
	}
	
	for (i=0; i<t.nb_messages; ++i)
	{
		for (j=0; j<nb_mots[i]; ++j)
		{
			nb_char[i][j]=0;
		}
	}
	
	nb_char=compte_nb_char(nb_mots, nb_char, t);
	
	for (i=0; i<t.nb_messages; ++i)
	{
		for(j=0; j<nb_mots[i]; ++j)
			{
	
			}
			//printf("Il y a %d char dans le mot %d du message %d \n",nb_char[i][j],j,i);
	}
	
	
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
	for (i=0; i<t.nb_messages; ++i)
	{
		fd=open(t.chemins[i], O_RDONLY);
		for (j=0; j<nb_mots[i]; ++j)
		{
			for (k=0; k<nb_char[i][j]; ++k)
				{
					read(fd,&(tab_mess[i][j][k]),1);
					while (tab_mess[i][j][k]!='\0' && (tab_mess[i][j][k]==' ' || tab_mess[i][j][k]=='\n'))
						read(fd,&(tab_mess[i][j][k]),1);
				}
		}
		
		close(fd);
	}
	
	for (i=0; i<t.nb_messages; ++i)
	{
		for (j=0; j<nb_mots[i]; ++j)
		{
//			printf("Dans la case [%d][%d] on a le mot %s \n",i,j,tab_mess[i][j]);
		}
	}
	
	message * mess = malloc(t.nb_messages * sizeof(message));
	for (i=0; i<t.nb_messages; ++i)
	{
		
		mot * mots = malloc(nb_mots[i]*sizeof(mot));
		
		for (j=0; j<nb_mots[i]; ++j)
		{
			mots[j]=init_mot(j, nb_char[i][j], t.chiffrements[i], t.cles[i], tab_mess[i][j]);
			//printf("Pour le message [%d], le mot %d est %s",i,j, mots[j].tab_char);
		}
		
		mess[i]=init_mess(i, nb_mots[i], mots, t.chiffrements[i], t.cles[i], t.chemins[i]);
	}
	
	
	t.tab_mess=mess;
	return t;
}



void affiche_traitement(traitement t)
{
	printf("Il y a %d messages dans la structure traitement \n", t.nb_messages);
	int i,j,l;
	for (i=0; i<t.nb_messages; ++i)
	{
		printf("Le message numero %d a pour chiffrement %d, pour cle %d et pour chemin %s \n",i, t.chiffrements[i], t.cles[i], t.chemins[i]);
		for (j=0; j<t.tab_mess[i].nb_mots; ++j)
			{
				printf("Mot numero %d du message %d est %s \n Il a %d char, une cle de %d et un chiffrement de %d \n\n\n\n", t.tab_mess[i].tab_mots[j].num_mot, t.tab_mess[i].num_mess, t.tab_mess[i].tab_mots[j].tab_char, t.tab_mess[i].tab_mots[j].nb_char, t.tab_mess[i].tab_mots[j].cle, t.tab_mess[i].tab_mots[j].chiffrement);
			}
	}
}

char cryptage_char(char c, int cle)
{
	if (c < 65) return c; // avant A
	if (c > 122) return c; // après z
	if (c > 90 && c < 97) return c; // entre Z et a
	if (c < 91) // si je suis une majuscule comprise entre 65 et 90
	{
		printf("majuscule \n");
		c = c + cle; // Modification du caractère
		if (c > 90) c=c-26;
	}
	else if (c > 96) // si je suis une minuscule comprise entre 97 et 122 
	{
		printf("minuscule \n");
		c = c + cle; // Modification du caractère
		if (c > 122) c=c-26;
	}
	return c;
}

char * cryptage_mot (const mot m)
{
	int k; char * retour=malloc(m.nb_char*sizeof(char));
	for (k=0; k<m.nb_char; ++k)
	{
		retour[k]=cryptage_char(m.tab_char[k], m.cle);
	}
//	printf("crypté de %s est %s avec une cle de %d \n",m.tab_char, retour,m.cle );
	return retour;
}

void * thread_buffer(void * z)
{
	int i,j=0;
	arg * a = (arg *) z;
	if (a->t.tab_mess[a->num_mess].tab_mots[a->num_mot].chiffrement)
	{
		char * retour = cryptage_mot(a->t.tab_mess[a->num_mess].tab_mots[a->num_mot]);
		pthread_mutex_lock(a->b.mutex);
		for (i=a->b.fin+1; i<(a->b.fin+1+a->t.tab_mess[a->num_mess].tab_mots[a->num_mot].nb_char) ; ++i)
		{	
			a->b.tab_buff[i]=retour[j];
			++j;
		}
		a->b.tab_buff[i]=" ";
		a->b.fin=i+1;
		pthread_mutex_unlock(a->b.mutex);
	}
	printf("buffer = %s ", a->b.tab_buff);
	
}

char * assigne_thread(traitement t, int num_mess)
{
	int j; 
	buffer buf = {taille_buffer, malloc(taille_buffer*sizeof(char)), PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER,0};
	arg a={t,buf,0};
	pthread_t * tid = malloc(t.tab_mess[num_mess].nb_mots*sizeof(tid));
	for (j=0; j<t.tab_mess[num_mess].nb_mots; ++j)
	{
		a.num_mot=j;
		pthread_create(tid+j,NULL,&thread_buffer,(void *) &a);
		pthread_join(tid[j], NULL);
	}	
	return a.b.tab_buff;
}

void retour_cryptage(char * buf, traitement t, int num_mess)
{
	int i,fd;  char s, * nom_fichier = t.tab_mess[num_mess].chemin;
	fd=open(strcat(nom_fichier, "_cypher"),O_CREAT, 0666);
	for (i=0; i<strlen(buf)+1; ++i)
	{
		write(fd,buf+i,1);
	}
	close(fd);
}

void retour_decryptage()
{
	
	
}

int traitement_message (traitement t, int num_mess)
{
	char * buf=assigne_thread(t, num_mess);
	if (t.tab_mess[num_mess].chiffrement) retour_cryptage(buf, t, num_mess);
	else retour_decryptage();
	return 0;
}

int dechiffrement_demande(traitement t)
{
	int i;
	for (i=0; i<t.nb_messages; ++i)
	{
		if (t.chiffrements[i]==0)
		return 1;
	}
	return 0;
	
}

int traitement_entier (traitement t)
{
	int i,fd; pid_t a; int * statut; char s;
	for (i=0; i<t.nb_messages; ++i)
	{
		if (!(a=fork()))
		{
			traitement_message(t, i);
		}
		else
		wait(statut);
		if (dechiffrement_demande(t))
		{
			fd=open("dechiffrement.fifo",O_RDONLY);
			do{
				read(fd,&s,1);
				write(0,&s,1);
			}while(s!='\0');
			close(fd);
			unlink("dechiffrement.fifo");
		}
	}
	return 0;
}
