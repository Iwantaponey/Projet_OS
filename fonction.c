/**
 * \file fichier.c
 * \brief Fichier contenant toutes les fonctions utilisées dans le
 *  	  programme de cryptage et de décryptage par le chiffrement de Cesar.
 */

#include "fonction.h"


/// Implémentation des fonctions du programme


/**
 * \fn mot init_mot(int num_mot, int nb_char, int chiffrement, int cle, char * tab_char);
 * \brief Fonction d'initialisation d'un mot
 * 
 * \param num_mot Numéro du mot dans le fichier entré lors de l'exécution (entier)
 * \param nb_char Nombre de caractères du mot (entier)
 * \param chiffrement Mode d'action du thread : cryptage (1) ou de décryptage (0)
 * \param cle Pas du décalage dans l'alphabet (entier)
 * \param tab_char Tableau de caractères contenant les caractères du mot
 * 
 * \return Un mot initialisé avec les paramètres d'appel de la fonction
 */
mot init_mot(int num_mot, int nb_char, int chiffrement, int cle, char * tab_char)
{
	mot w = { num_mot, nb_char, chiffrement, cle, tab_char };
	return w;
}


/**
 * \fn message init_mess(int num_mess, int nb_mots, mot * tab_mots, int chiffrement, int cle, char * chemin);
 * \brief Fonction d'initialisation d'un message
 * 
 * \param num_mess Numéro du message dans le fichier entré lors de l'exécution (entier strictement positif)
 * \param nb_mots Nombre de mots dans le fichier message (entier supérieur ou égal à 0)
 * \param tab_mots Tableau de structures mots contenant chaque mot du message
 * \param chiffrement Mode d'action du thread : cryptage (1) ou de décryptage (0)
 * \param cle Pas du décalage dans l'alphabet (entier strictement positif)
 * \param chemin Tableau de caractères contenant le path du fichier message
 * 
 * \return Un message initialisé avec les paramètres d'appel de la fonction
 */
message init_mess(int num_mess, int nb_mots, mot * tab_mots, int chiffrement, int cle, char * chemin)
{
	message m = { num_mess, nb_mots, tab_mots , chiffrement, cle, chemin };
	return m;
}


/**
 * \fn traitement init_traitement(int * chiffrements, int * cles, int nb_messages, char ** chemins)
 * \brief Fonction d'initialisation d'un traitement
 * 
 * \param chiffrement Tableau des modes d'action du thread : cryptage (1) ou de décryptage (0)
 * \param cles Tableau des clés de cryptage ou décryptage
 * \param nb_messages Nombre de messages dans le fichier d'entrée (entier)
 * \param chemins Tableau de tableau de caractères contenant le path de chaque fichier message
 * 
 * \return Un traitement initialisé avec les paramètres d'appel de la fonction
 */
traitement init_traitement(int * chiffrements, int * cles, int nb_messages, char ** chemins)
{
	traitement t = {1, chiffrements, cles, nb_messages, chemins, malloc(nb_messages * sizeof(message)) };
	return t;
}


/**
 * \fn buffer init_buffer(int taille_buff)
 * \brief Fonction d'initialisation du buffer
 * 
 * \param taille_buff Taille du buffer
 * 
 * \return Un buffer de stockage des caractères d'un message après cryptage ou décryptage
 */
buffer init_buffer(int taille_buff)
{
	buffer b;
	b.tab_buff = malloc(taille_buffer * sizeof(char));
	return b;
}


/**
 * \fn int compte_nb_messages(char * nom_fichier)
 * \brief Fonction comptant le nombre de message contenus dans le fichier d'entrée
 * 
 * \param nom_fichier Nom du fichier contenant les paths de chaque fichier message
 * 
 * \return Le nombre de messages dans le fichier d'entrée
 */
int compte_nb_messages(char * nom_fichier)
{
	int	fd = open(nom_fichier, O_RDONLY), nb_messages = 0; char s = 'a';
	if (fd < 1) 
	{
		return -1;
	}
	while(read(fd, &s, 1)) 				/*!< Premier scan du fichier principal pour déterminer le nombre de messages à traiter */
	{
		if (s == '\n') 
		{
			++ nb_messages;
		}
	}	
	close(fd);
	return nb_messages;	
}


/**
 * \fn int * recupere_cle(int * cle_finale, int nb_messages, int cle[nb_messages][2])
 * \brief Fonction de récupération des clés à utiliser pour chaque message
 * 
 * \param cle_finale Tableau d'entier contenant les clés 
 * \param nb_messages Nombre de message pour lesquels on récupère la clé
 * \param cle[nb_messages][2] ????????????????????????????????????????????????????????
 * 
 * \return Un tableau de clés
 */
int * recupere_cle(int * cle_finale, int nb_messages, int cle[nb_messages][2])
{
	int i;
	for (i = 0; i < nb_messages; ++i)
	{
		if (cle[i][1] != -1)
		{
			cle_finale[i] = ((10 * cle[i][0]) + cle[i][1])%26;
		}
		else
		{	
			cle_finale[i] = (cle[i][0]);
		}
	}
	return cle_finale;
}


/**
 * \fn traitement extraire(char * nom_fichier)
 * \brief Fonction principale d'extraction des données du fichier d'entrée
 * 
 * \param nom_fichier Nom du fichier contenu dans un tableau de caractère
 * 
 * \return Une structure traitement remplie par les messages et les mots 
 * ????????????????????????????????????????????????????????????????????????????????????
 */
traitement extraire(char * nom_fichier)
{	
	traitement t; t.initialise=0;
	int e = 1, i, j, l, k, fd = -1; char s = 'a'; 
	int nb_messages = compte_nb_messages(nom_fichier), compteur = 0;
	if (nb_messages == -1)
	{
		printf ("Structure traitement non remplie, problème d'ouverture du fichier principal \n");
		return t;
	}	
	
	int cle[nb_messages][2];  
	int * chiffrements = malloc(nb_messages * sizeof(int));
	char ** chemins = malloc(nb_messages * sizeof(char*));
	for (i = 0; i < nb_messages; i++)
	{
		chemins[i] = malloc(taille_max_chemin * sizeof(char));
		cle[i][1] = -1;
	}
	i = j = l = k = 0;
	fd = open(nom_fichier, O_RDONLY);
	while(e && (i < nb_messages) && (j < taille_max_chemin)) 		/*!< Lecture du fichier principal */
	{	
		e = read(fd, &s, 1); 										/*!< Lecture des caractères l'un après l'autre */
		if (!e) 
		{													
			printf("Erreur de lecture du fichier principal \n");
			return t;
		}
		else
		{
			if (s != '\n') 											/*!< Traiter les messages les uns après les autres */
			{
				if (s != ';' && compteur == 0) 						/*!< Récupérer les chemins de chaque message si compteur vaut 0 (donc si on n'a pas encore vu de ';' ) */
				{	
					chemins[i][j] = s;
					j++;
				}
				if (s == ';') ++compteur; 							/*!< Quand on rencontre un ';' on augmente le compteur */
				if ((s != ';') && (compteur == 1)) 					/*!< Récupérer la clé de chaque message si compteur vaut 1 (donc si on a vu un seul ';' ) */
				{
					if ((s<48) || (s>57))
					{
						printf("erreur : la cle n est pas correcte pour le message %d \n", i+1);
						close(fd);
						return t;
					}
					if (l>1)
					{
						printf("erreur : la cle est supérieure à 99 pour le message %d \n", i+1);
						close(fd);
						return t;
					}
					cle[i][l] = s - 48; 							/*!< Puisque la clé peut être sur 2 caracteres, on a un tableau à 2 dimensions cle [i][l] qui nous permet pour chaque message de stocker les deux chiffres composant la clé qu'on reconstituera plus tard */
					l++;
				}
				if ((s != ';') && (compteur == 2)) 					/*!< Récupérer les modes de traitement pour chaque message si compteur vaut 2 (donc si on a vu deux ';' ) */
				{
					if (k>0)
					{
						printf("Erreur : Le mode de traitement du message %d contient trop de caractères \n", i+1);
						close(fd);
						return t;
					}
					if (s == 'c') 
					{
						chiffrements[i] = 1; 
						++k;
					}
					else if (s=='d')
					{
						chiffrements[i] = 0;
						++k;
					}
					else 
					{
						printf ("erreur le mode de traitement n est ni c ni d pour le message %d \n", i+1);
						close(fd);
						return t;
						
					}
				}
				if (compteur>2) 
				{
					printf("erreur du nombre d arguments dans le fichier principal \n");
					close(fd);
					return t;
				} 
			}
			else
			{
			++i; 
			compteur = j = l= k = 0;
			}	
		}
	}
	close(fd);
	int * cle_finale = malloc(nb_messages * sizeof(int));
	cle_finale = recupere_cle(cle_finale, nb_messages, cle);
	
	for (i = 0; i < nb_messages; i++) // UTILE ????????????????????????????????????????????????
	{
		fd=open(chemins[i], O_RDONLY);
		if (fd==-1) 
		{
			printf("erreur ouverture fichier message %d \n",i+1);
			close(fd);
			return t;
		}
	}
	t = init_traitement(chiffrements, cle_finale, nb_messages, chemins); 
	return t;
}


/**
 * \fn int ** compte_nb_char(int * nb_mots, int ** nb_char, traitement t)
 * \brief Fonction de comptage des caractères de chaque mot d'un message
 * 
 * \param nb_mot Nombre de mots dans le message
 * \param nb_char Nombre de caractères dans le message
 * \param t Structure unique qui contient les messages et mots
 * 
 * \return Un tableau d'entier contenant le nombre de caractères de chaque mot 
 * 		   pour chacun des messages
 * ????????????????????????????????????????????????????????????????????????????????????
 */
int ** compte_nb_char(int * nb_mots, int ** nb_char, traitement t)
{
	int i = 0, j = 0, fd = -1; char s = 'a';
		
	for (i = 0; i < t.nb_messages; ++i)
	{
		fd = open(t.chemins[i], O_RDONLY);
		for(j = 0; j < nb_mots[i]; ++j)
		{
			while(read(fd, &s, 1) && (s != ' ') && (s != '\n'))
			nb_char[i][j] = nb_char[i][j] + 1;
		}
		close(fd);
	}
	return nb_char;
}


/**
 * \fn int * compte_nb_mots(int * nb_mots, traitement t)
 * \brief Fonction qui compte le nombre de mot pour chaque message
 * 
 * \param nb_mots Tableau d'entiers contenant le nombre de mot pour chaque message
 * \param t Structure unique qui contient la totalité des messages et mots
 * 
 * \return Un tableau d'entiers contenant le nombre de mot de chaque message
 */
int * compte_nb_mots(int * nb_mots, traitement t)
{	
	int i, fd = -1, compteur = 0;
	char s = 'a';
	for (i = 0; i < t.nb_messages; ++i)
	{	
		fd = open(t.chemins[i], O_RDONLY);
		while(read(fd, &s, 1))
		{	
			if ((s == ' ') || (s == '\n') || (s == '\t'))
			{	
				if (compteur == 0)
				{
					nb_mots[i] = nb_mots[i] + 1; 
				}
				else
					compteur++;
			}
			else compteur = 0;
		}
		close(fd);
	}
	return nb_mots;
}


/**
 * \fn traitement assigne_message(traitement t)
 * \brief
 * 
 * \param t Structure unique contenant tous les mots, modes, clés et messages 
 * 		  des fichiers messages et du fichier principal
 * 
 * \return Une structure traitement unique pour chaque exécution du programme
 * 		   contenant tous les messages et mots à traiter
 */
traitement assigne_message(traitement t)
{
	int fd = -1, i = 0, j = 0, k = 0;
	int * nb_mots = malloc(t.nb_messages * sizeof(int));
	for (i = 0; i < t.nb_messages; ++i)
	{
		nb_mots[i] = 0;
	}
	
	nb_mots = compte_nb_mots(nb_mots, t);
	
	int ** nb_char = malloc(t.nb_messages * sizeof(int*));
	
	for (i = 0; i < t.nb_messages; ++i)
	{	
		nb_char[i] = malloc(nb_mots[i] * sizeof(int));
	}
	for (i = 0; i < t.nb_messages; ++i)
	{
		for (j = 0; j < nb_mots[i]; ++j)
		{
			nb_char[i][j] = 0;
		}
	}
	
	nb_char = compte_nb_char(nb_mots, nb_char, t);
	
	char *** tab_mess;
	tab_mess = malloc(t.nb_messages * sizeof(char**));
	for (i = 0; i < t.nb_messages; ++i)
	{
		tab_mess[i] = malloc(nb_mots[i] * sizeof(char*));
		for(j = 0; j < nb_mots[i]; ++j)
		{
			tab_mess[i][j] = malloc(nb_char[i][j] * sizeof(char));
		}
	}
	
	for (i = 0; i < t.nb_messages; ++i)
	{
		fd = open(t.chemins[i], O_RDONLY);
		for (j = 0; j < nb_mots[i]; ++j)
		{
			for (k = 0; k < nb_char[i][j]; ++k)
				{
					read(fd, &(tab_mess[i][j][k]), 1);
					while ((tab_mess[i][j][k] == ' ') || (tab_mess[i][j][k] == '\n'))
						read(fd, &(tab_mess[i][j][k]), 1);
				}
		}
		close(fd);
	}
	
	message * mess = t.tab_mess;
	for (i = 0; i < t.nb_messages; ++i)
	{
		mot * mots = malloc(nb_mots[i] * sizeof(mot));
		
		for (j = 0; j < nb_mots[i]; ++j)
		{
			mots[j] = init_mot(j, nb_char[i][j], t.chiffrements[i], t.cles[i], tab_mess[i][j]);
		}
		mess[i] = init_mess(i, nb_mots[i], mots, t.chiffrements[i], t.cles[i], t.chemins[i]);
	}
	t.tab_mess = mess;
	for (i = 0; i < t.nb_messages; ++i)
	{
		free(nb_char[i]);
	}
	free(nb_char);
	free(nb_mots);
	for (i = 0; i < t.nb_messages; ++i)
	{
		free(tab_mess[i]);
	}
	free(tab_mess);
	return t;
}


/**
 * \fn char cryptage_char(char c, int cle)
 * \brief Fonction de cryptage d'un caractère
 * 
 * \param c Caractère à crypter
 * \param cle Entier positif ou nul servant à crypter
 *
 * \return Un caractère crypté
 */
char cryptage_char(char c, int cle) 			/*!< Utilisation de la table ASCII */
{
	
	if (c < 65) return c; 						/*!< Si le caractère est avant le début du codage des lettres (avant A) */
	if (c > 122) return c; 						/*!< Si le caractère est après la fin du codage des lettres (après z) */
	if ((c > 90) && (c < 97)) return c; 		/*!< Si le caractère est après le début du codage des lettres majuscules et avant le codage des lettres minuscules */
	if (c < 91) 								/*!< Si le caractère est une majuscule (codé entre 65 et 90) */
	{
		if (c + cle < 128)
		{	
			c = c + cle; 						/*!< Modification du caractère */
			if (c > 90) c = c - 26;
		}
		else
		{	
			c = c + cle - 26;
		}
	}
	else if (c > 96) 							/*!< Si le caractère est une majuscule (codé entre 97 et 122) */
	{
		if (c + cle < 128)
		{
			c = c + cle; 						/*!< Modification du caractère */
			if (c > 122) c = c - 26;
		}
		else
		{	
			c = c + cle - 26;
		}
	}
	return c;
}


/**
 * \fn char * cryptage_mot(const mot m)
 * \brief Fonction de cryptage d'un mot
 * 
 * \param m Mot à crypter
 * 
 * \return Un tableau de caractères cryptés
 */
char * cryptage_mot(const mot m)
{
	int k; 
	char * retour = malloc(m.nb_char * sizeof(char));
	for (k = 0; k < m.nb_char; ++k)
	{
		retour[k] = cryptage_char(m.tab_char[k], m.cle);
	}
	return retour;
}


/**
 * \fn char decryptage_char(char c, int cle)
 * \brief Fonction de décryptage d'un caractère
 * 
 * \param c Caractère à décrypter
 * \param cle Entier positif ou nul servant à décrypter
 *
 * \return Un caractère décrypté
 */
char decryptage_char(char c, int cle)
{
	if (c < 65) return c; 						/*!< Si le caractère est avant le début du codage des lettres (avant A) */
	if (c > 122) return c; 						/*!< Si le caractère est après la fin du codage des lettres (après z) */
	if ((c > 90) && (c < 97)) return c; 		/*!< Si le caractère est après le début du codage des lettres majuscules et avant le codage des lettres minuscules */
	if (c < 91) 								/*!< Si le caractère est une majuscule (codé entre 65 et 90) */
	{
		c = c - cle; 							/*!< Modification du caractère */
		if (c < 65) c = c + 26;
	}
	else if (c > 96) 							/*!< Si le caractère est une majuscule (codé entre 97 et 122) */
	{
		c = c - cle; 							/*!< Modification du caractère */
		if (c < 97) c = c + 26;
	}
	return c;
}


/**
 * \fn char * decryptage_mot(const mot m)
 * \brief Fonction de décryptage d'un mot
 * 
 * \param m Mot à décrypter
 *
 * \return Un mot (tableau de caractères) décrypté
 */
char * decryptage_mot(const mot m)
{
	int k; 
	char * retour = malloc(m.nb_char * sizeof(char));
	for (k = 0; k < m.nb_char; ++k)
	{
		retour[k] = decryptage_char(m.tab_char[k], m.cle);
	}
	return retour;
}


/**
 * \fn void retour_cryptage(char * buf, message m)
 * \brief Fonction qui fait le retour du message après son cryptage
 * 
 * \param buf Buffer dans lequel on stocke les caractères cryptés ou décryptés ????????????
 * \param m Message traité
 * 
 * \return Aucun
 */
void retour_cryptage(char * buf, message m)
{
	int i, fd, fd2;  
	char c = 'a';
	char * nom_fichier = m.chemin;
	fd = open(nom_fichier, O_RDONLY);
	fd2 = open(strcat(nom_fichier, "_cypher"), O_CREAT | O_WRONLY, 0666);
	i = 0;
	while (read(fd, &c, 1))
	{
		if((c != ' ') && (c != '\t') && (c != '\n'))
		{	
			write(fd2, buf + i, 1);
			++i;
		}
		else
		{
			write(fd2, &c, 1);
		}
	}
	close(fd);
	close(fd2);
}


/**
 * \fn void retour_decryptage(char * buf, message m)
 * \brief Fonction qui fait le retour du message après son décryptage
 * 
 * \param buf Tableau de caractères qui sert à stocker le message après son traitement
 * \param m Message traité
 * 
 * \return Aucun
 */
void retour_decryptage(char * buf, message m)
{
	int j = 0, fd;  
	char * nom_fichier = strcat(m.chemin, "_decypher");
	fd = open(nom_fichier, O_CREAT | O_WRONLY, 0666);
	do{
		write(fd, buf + j, 1);
		++j;
	}while(buf[j] != '\0');
	close(fd);
}


/**
 * \fn void affiche_decryptage(const message m) ???????????????????????????????????????????
 * \brief Fonction qui affiche le message décrypté
 * 
 * \param m Message décrypté
 * 
 * \return Aucun
 */
void affiche_decryptage(const message m)
{	
	int fd, fd2;  
	char c = 'a', s = 'a';
	char * nom_fichier = m.chemin; 
	fd = open(nom_fichier, O_RDONLY);
	char * nom_fichier_d = strcat(m.chemin, "_decypher");
	fd2 = open(nom_fichier_d, O_RDONLY);
	while (read(fd, &c, 1))
	{
		if((c != ' ') && (c != '\t') && (c != '\n'))
		{	
			read(fd2, &s, 1);
			write(0, &s, 1);
		}
		else
		{
			write(0, &c, 1);
		}
	}
	s = '\n'; 
	write (0, &s, 1);
	close(fd);
	close(fd2);
	unlink(nom_fichier_d);
}


/**
 * \fn void * thread_buffer(void * z)
 * \brief Fonction qui appelle le traitement du message et le stocke dans un buffer
 * 
 * \param z ????????????????????????????????????????????????????????????????????????????
 *
 * \return Aucun
 */
void * thread_buffer(void * z)
{
	arg * a = (arg *) z; 
	int i = a -> emplacement, j = 0; 
	char * retour; 									/*!< retour va contenir le mot traité */
	if (a -> w.chiffrement)							/*!< S'il faut décrypter */
	{
		retour = cryptage_mot(a -> w);
		while(i < (a -> emplacement) + (a -> w.nb_char)) 		/*!< Commencer au prochain caractère libre et écrire tout le mot traité dans le buffer, le mot traité ayant la même taille que le mot non traité */
		{
			a -> b.tab_buff[i] = retour[j]; 		/*!< Ecrire le mot traité caractère par caractère dans le buffer */
			++j; 	
			++i;				
		}
		free(retour); 								/*!< Libérer le mot traité car il est mis dans le buffer */
	}
	else
	{
		retour = decryptage_mot(a -> w);
		while(i < (a -> emplacement) + (a -> w.nb_char)) 		/*!< Commencer au prochain caractère libre et écrire tout le mot traité dans le buffer, le mot traité ayant la même taille que le mot non traité */
		{		
			a -> b.tab_buff[i] = retour[j]; 		/*!< Ecrit le mot traité caractère par caractère dans le buffer */
			++j; 	
			++i;				
		}
		free(retour); 								/*!< Libérer le mot traité car il est mis dans le buffer */
	}
	return NULL; 
}


/**
 * \fn char * traitement_message(message m)
 * \brief Fonction qui va créer les threads pour un message
 * 
 * \param m Message à traiter par les threads
 * 
 * \return Un tableau de caractères contenant les buffers ??????????????????????????????????
 */
char * traitement_message(message m)
{
	buffer b = init_buffer(taille_buffer); 				/*!< Buffer qui sera utilisé par les threads pour écrire le message après traitement */
	arg * tab_arg = malloc(m.nb_mots * sizeof(arg)); 
	int emplace[m.nb_mots];
	pthread_t * tab_thread = malloc(m.nb_mots * sizeof(pthread_t)); 		/*!< On aura autant de threads que de mots à traiter */
	int i=0; 
	emplace[0] = 0;
	while(i < m.nb_mots - 1)
	{
		emplace[i + 1] = emplace[i] + (m.tab_mots[i].nb_char);
		++i;
	}
	for (i = 0; i < m.nb_mots; ++i)
	{
		tab_arg[i].w = m.tab_mots[i];
		tab_arg[i].b = b;
		tab_arg[i].nb_mots = m.nb_mots;
		tab_arg[i].emplacement = emplace[i];
		tab_arg[i].compteur = 0;
	}
	for (i = 0; i < m.nb_mots; ++i)
	{
		pthread_create(tab_thread + i, NULL, &thread_buffer, &(tab_arg[i]));
		pthread_join(tab_thread[i], NULL);
	}
	if (m.chiffrement) retour_cryptage(b.tab_buff, m);
	else retour_decryptage(b.tab_buff, m);
	free(b.tab_buff);
	free(tab_arg);
	free(tab_thread);
	return b.tab_buff;
}


/** A QUOI ELLE SERT ???????????????????????????????????????????????????????????????????
 * \fn int dechiffrement_demande(traitement t)
 * \brief Fonction qui regarde quels sont les messages à déchiffrer
 * 
 * \param t Structure unique contenant tous les messages et leurs données de traitement
 *
 * \return 
 */
int dechiffrement_demande(traitement t)
{
	int i;
	for (i = 0; i < t.nb_messages; ++i)
	{
		if (t.chiffrements[i] == 0)
		return i;
	}
	return -1;
}


/**
 * \fn int traitement_entier(traitement t)
 * \brief Fonction qui appelle les sous-fonctions traitement pour tous les messages
 * 
 * \param t Structure unique contenant tous les messages et leurs données de traitement
 * 
 * \return 
 */
int traitement_entier(traitement t)
{
	int i; 
	pid_t a;
	for (i = 0; i < t.nb_messages; ++i)
	{
		if (!(a = fork()))
		{
			traitement_message(t.tab_mess[i]);
			exit(0);
		}
		else
		{
			wait(NULL);
			if (t.tab_mess[i].chiffrement == 0)
			{	
				affiche_decryptage(t.tab_mess[i]);
			}
		}	
	}	
	return 0;
}


/**
 * \fn void libere_mot(mot m)
 * \brief Fonction qui libère la mémoire allouée à un mot
 * 
 * \param m Le mot dont on veut libérer la mémoire
 *
 * \return Aucun
 */
void libere_mot(mot m)
{
	free(m.tab_char);
}


/**
 * \fn void libere_message(message m)
 * \brief Fonction qui libère la mémoire allouée à un message
 * 
 * \param m Le message dont on veut libérer la mémoire
 *
 * \return Aucun
 */
void libere_message(message m)
{
	int i;
	for (i = 0; i < m.nb_mots; ++i)
	{
		libere_mot(m.tab_mots[i]);
	}	
	free(m.tab_mots);
	
}


/**
 * \fn void libere_traitement(traitement t)
 * \brief Fonction qui libère la mémoire allouée à la structure traitement
 * 
 * \param t Structure traitement dont on veut libérer la mémoire
 * 
 * \return 
 */
void libere_traitement(traitement t)
{
	int i;
	free(t.chiffrements);
	for (i = 0; i < t.nb_messages; ++i)
	{
		free(t.chemins[i]);
	}
	free(t.chemins);
	free(t.cles);
	for (i = 0; i < t.nb_messages; ++i)
	{
		libere_message(t.tab_mess[i]);
	}
	free(t.tab_mess);
}
