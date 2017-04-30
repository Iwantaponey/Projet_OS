/**
 * \file fonction.c
 * \brief Fichier contenant toutes les fonctions utilisées dans le
 *  	  programme de cryptage et de décryptage par le chiffrement de Cesar.
 */

#include "fonction.h"


/// Implémentation des fonctions du programme


/**
 * \fn mot init_mot(int num_mot, int nb_char, int chiffrement, int cle, char * tab_char);
 * \brief Fonction d'initialisation d'un mot
 * 
 * \param num_mot Numéro du mot dans le fichier message
 * \param nb_char Nombre de caractères dans le mot
 * \param chiffrement Mode de traitement pour le mot : cryptage (1) ou décryptage (0)
 * \param cle Pas du décalage dans l'alphabet
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
 * \param num_mess Numéro du message dans le fichier principal
 * \param nb_mots Nombre de mots dans le fichier message
 * \param tab_mots Tableau de structures mots contenant chaque mot du message
 * \param chiffrement Mode de traitement du mot : cryptage (1) ou décryptage (0)
 * \param cle Pas du décalage dans l'alphabet
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
 * \param chiffrement Tableau des modes de traitement de taille nb_messages : cryptage (1) ou décryptage (0)
 * \param cles Tableau des clés de cryptage ou de décryptage
 * \param nb_messages Nombre de messages dans le fichier principal
 * \param chemins Tableau de tableaux de caractères contenant le path de chaque fichier message
 * 
 * \return Un traitement initialisé avec les paramètres d'appel de la fonction
 */
traitement init_traitement(int * chiffrements, int * cles, int nb_messages, char ** chemins)
{
	traitement t = { 1, chiffrements, cles, nb_messages, chemins, malloc(nb_messages * sizeof(message)) };	/*!< On valide l'initialisation et on alloue le tableau de message puisqu'on a récupéré le nombre de messages à traiter */
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
	b.tab_buff = malloc(taille_buffer * sizeof(char));				/*!< taille_buffer est une constante */
	return b;
}


/**
 * \fn int compte_nb_messages(char * nom_fichier)
 * \brief Fonction comptant le nombre de message contenus dans le fichier principal
 * 
 * \param nom_fichier Nom du fichier principal
 * 
 * \return Le nombre de messages dans le fichier principal
 */
int compte_nb_messages(char * nom_fichier)
{
	int	fd = open(nom_fichier, O_RDONLY), nb_messages = 0; char s = 'a';
	if (fd == -1)						/*!< Vérification qu'open n'a pas renvoyé un code d'erreur et donc qu'il a bien pu ouvrir le fichier principal */ 
	{
		return -1;						/*!< Si on a un problème d'ouverture du fichier principal */
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
 * \param cle_finale Tableau d'entier déjà alloué qui contiendra les clés 
 * \param nb_messages Nombre de message à traiter
 * \param cle[nb_messages][2] Tableau d'entiers à deux dimensions contenant les chiffres de la clé lus dans le fichier principal
 * 
 * \return Le tableau d'entiers contenant les clés correctes
 */
int * recupere_cle(int * cle_finale, int nb_messages, int cle[nb_messages][2])
{
	int i;
	for (i = 0; i < nb_messages; ++i)
	{
		if (cle[i][1] != -1)										/*!< Si on a quelquechose dans la dizaine de la clé */
		{
			cle_finale[i] = ((10 * cle[i][0]) + cle[i][1])%26;		/*!< On multiple la dizaine par 10, on ajoute l'unité puis on fait modulo 26 du tout */
		}
		else
		{	
			cle_finale[i] = (cle[i][0]);							/*!< Si on a pas de dizaine on met juste l'unité dans le tableau cle_finale */ 
		}
	}
	return cle_finale;
}


/**
 * \fn traitement extraire(char * nom_fichier)
 * \brief Fonction d'extraction des données pour remplir la structure traitement
 * 
 * \param nom_fichier Nom du fichier principal
 * 
 * \return Une structure traitement remplie pour les chiffrements, les clés, le nombre de messages et les chemins des messages 
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
		cle[i][1] = -1;												/*!< On considère que la clé est comprise entre 1 et 9, si elle est supérieure à 9 alors le deuxième caractère est mis dans [i][1] et n'est plus à -1. Attention l'unité de la clé est dans [i][0] seulement s'il n'y a pas de dizaine, autrement elle est dans la case [i][1], et la dizaine est toujours dans [i][0] */
	}
	i = j = l = k = 0;												/*!< Initialisation de tous les itérateurs de boucle */
	fd = open(nom_fichier, O_RDONLY);
	while(e && (i < nb_messages) && (j < taille_max_chemin)) 		/*!< Lecture du fichier principal, la boucle s'arrête juste avant la fin du fichier et ne conduit pas à une erreur de lecture si toutes les lectures précédentes se sont bien déroulées */
	{	
		e = read(fd, &s, 1); 										/*!< Lecture des caractères l'un après l'autre, e permet de savoir si on a réussi à lire ou pas */
		if (!e) 													/*!< Vérification que le fichier principal n'est pas vide et qu'on peut lire dedans */
		{													
			printf("Erreur de lecture du fichier principal \n");
			for (i=0; i<nb_messages; ++i)
			{
				free(chemins[i]);
			}
			free(chemins);
			free(chiffrements);
			close(fd);
			return t;
		}
		else
		{
			if (s != '\n') 											/*!< Traiter les messages les uns après les autres */
			{
				if ((s != ';') && (compteur == 0)) 					/*!< Récupérer les chemins de chaque message si compteur vaut 0 (donc si on n'a pas encore vu de ';' ) */
				{	
					chemins[i][j] = s;
					j++;
				}
				if (s == ';') ++compteur; 							/*!< Quand on rencontre un ';' on augmente le compteur */
				if ((s != ';') && (compteur == 1)) 					/*!< Récupérer la clé de chaque message si compteur vaut 1 (donc si on a vu un seul ';' ) */
				{
					if ((s < 48) || (s > 57))
					{
						printf("Erreur : la clé n'est pas correcte pour le message %d \n", i+1);
						for (i=0; i<nb_messages; ++i)
						{
							free(chemins[i]);
						}
						free(chemins);
						free(chiffrements);
						close(fd);
						return t;
					}
					if (l > 1)										/*!< l est le compteur de nombre de caractère de la clé */
					{												/*!< Si l est supérieur à 1 alors on a déjà lu deux caractères de clé mais si on ne lit toujours pas de ';' alors le fichier principal est faux donc on n'exécute rien */
						printf("Erreur : la clé est supérieure à 99 pour le message %d \n", i+1);
						for (i=0; i<nb_messages; ++i)
						{
							free(chemins[i]);
						}
						free(chemins);
						free(chiffrements);
						close(fd);
						return t;
					}
					cle[i][l] = s - 48; 							/*!< Puisque la clé peut être sur 2 caractères, on a un tableau à 2 dimensions cle[i][l] qui nous permet pour chaque message de stocker les deux chiffres composant la clé qu'on reconstituera plus tard */
																	/*!< Dans la table ASCII le 0 est codé par 48 donc pour récupérer la valeur entière du caractère s, on fait s - 48 */
					l++;
				}
				if ((s != ';') && (compteur == 2)) 					/*!< Récupérer les modes de traitement pour chaque message si compteur vaut 2 (donc si on a vu deux ';' ) */
				{
					if (k > 0)										/*!< k est le compteur de nombre de caractère du mode de chiffrement */
					{
						printf("Erreur : le mode de traitement du message %d contient trop de caractères \n", i+1);
						for (i=0; i<nb_messages; ++i)
						{
							free(chemins[i]);
						}
						free(chemins);
						free(chiffrements);
						close(fd);
						return t;
					}
					if (s == 'c') 
					{
						chiffrements[i] = 1; 
						++k;
					}
					else if (s == 'd')
					{
						chiffrements[i] = 0;
						++k;
					}
					else 
					{
						printf ("Erreur : le mode de traitement n'est ni c ni d pour le message %d \n", i+1);
						for (i=0; i<nb_messages; ++i)
						{
							free(chemins[i]);
						}
						free(chemins);
						free(chiffrements);
						close(fd);
						return t;
					}
				}
				if (compteur > 2) 									/*!< Si jamais on a trop d'arguments dans la ligne, le fichier principal est faux donc on ne traite rien */
				{
					printf("Erreur du nombre d'arguments dans le fichier principal \n");
					for (i=0; i<nb_messages; ++i)
					{
						free(chemins[i]);
					}
					free(chemins);
					free(chiffrements);
					close(fd);
					return t;
				} 
			}
			else
			{
			++i; 													/*!< On passe au message suivant */
			compteur = j = l = k = 0;								/*!< On remet tous les compteurs à 0 pour passer au message suivant */
			}	
		}
	}
	close(fd);
	int * cle_finale = malloc(nb_messages * sizeof(int));
	cle_finale = recupere_cle(cle_finale, nb_messages, cle);
	for (i = 0; i < nb_messages; i++)								/*!< Dernier niveau de contrôle : on vérifie la validité du chemin */
	{
		fd = open(chemins[i], O_RDONLY);
		if (fd == -1) 
		{
			printf("Erreur lors de l'ouverture fichier message %d \n",i+1);
			for (i=0; i<nb_messages; ++i)
			{
				free(chemins[i]);
			}
			free(chemins);
			free(chiffrements);
			free(cle_finale);
			close(fd);
			return t;
		}
	}
	t = init_traitement(chiffrements, cle_finale, nb_messages, chemins); 
	return t;
}


/**
 * \fn int ** compte_nb_char(int * nb_mots, int ** nb_char, traitement t)
 * \brief Fonction de comptage des caractères de chaque mot de chaque message
 * 
 * \param nb_mot Tableau d'entiers qui contient pour chaque message son nombre de mots
 * \param nb_char Tableau de tableaux d'entiers déjà alloué qui contiendra pour chaque mot de chaque message le nombre de caractères du mot
 * \param t Structure traitement qui contient le nombre de messages et le chemin pour chaque message
 * 
 * \return Un tableau de tableaux d'entier contenant le nombre de caractères de chaque mot pour chaque message
 */
int ** compte_nb_char(int * nb_mots, int ** nb_char, traitement t)
{
	int i = 0, j = 0, fd = -1; char s = 'a';
		
	for (i = 0; i < t.nb_messages; ++i)
	{
		fd = open(t.chemins[i], O_RDONLY);
		for(j = 0; j < nb_mots[i]; ++j)
		{
			while(read(fd, &s, 1) && (s != ' ') && (s != '\n') && (s!='\t'))   	/*!< Tant qu'on arrive à lire un caractère qui n'est ni un blanc, ni un retour à la ligne ni une tabulation alors on est en train de lire un caractère lettre et on augmente alors le nombre de caractères du mot */
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
 * \param nb_mots Tableau d'entiers déjà alloué qui contiendra le nombre de mot pour chaque message
 * \param t Structure traitement qui contient le nombre de messages et le chemin pour chaque message
 * 
 * \return Un tableau d'entiers contenant le nombre de mot pour chaque message
 */
int * compte_nb_mots(int * nb_mots, traitement t)
{	
	int i, fd = -1, compteur = 0;
	char s = 'a'; 											/*!< Initialisation d'un char pour être sûr de la valeur qui est contenue dedans */
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
				else compteur++;							/*!< Si on a plusieurs caractère blancs à la suite, on ne doit pour autant pas augmenter le nombre de mots */
			}
			else compteur = 0;
		}
		close(fd);
	}
	return nb_mots;
}


/**
 * \fn traitement assigne_message(traitement t)
 * \brief Remplir les structures message et mot avec tous les champs nécessaires
 * 
 * \param t Structure traitement contenant le nombre de messages et le chemin pour chaque message
 * 
 * \return Structure traitement qui contient tous les messages et tous les mots avec tous les champs nécessaires
 */
traitement assigne_message(traitement t)
{
	int fd = -1, i = 0, j = 0, k = 0;								/*!< fd est à -1 pour s'assurer que fd change */
	int * nb_mots = malloc(t.nb_messages * sizeof(int));
	for (i = 0; i < t.nb_messages; ++i)
	{
		nb_mots[i] = 0;												/*!< Initialisation du nombre de mot à 0 pour chaque message */
	}
	nb_mots = compte_nb_mots(nb_mots, t);							/*!< Récupérer le nombre de mots */
	int ** nb_char = malloc(t.nb_messages * sizeof(int*));			/*!< Tableau à deux dimensions : la première dimension est le numéro du message et la deuxième dimension est le numéro du mot dont on compte le nombre de caractères */
	
	for (i = 0; i < t.nb_messages; ++i)
	{	
		nb_char[i] = malloc(nb_mots[i] * sizeof(int));
	}
	for (i = 0; i < t.nb_messages; ++i)
	{
		for (j = 0; j < nb_mots[i]; ++j)
		{
			nb_char[i][j] = 0;										/*!< Initialisation du nombre de caractères à 0 */
		}
	}
	nb_char = compte_nb_char(nb_mots, nb_char, t);					/*!< Récupérer le nombre de caractères */
	char *** tab_mess;												/*!< Tableau à trois dimensions, la première dimension contient le numéro du message, la deuxième dimension contient le numéro du mot et la troisième dimension contient le numéro du caractère */
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
					while ((tab_mess[i][j][k] == ' ') || (tab_mess[i][j][k] == '\n') || (tab_mess[i][j][k] == '\t')) 	/*!< On relit jusqu'à ce qu'on trouve un caractère autre que blanc, retour à la ligne et tabulation */
						read(fd, &(tab_mess[i][j][k]), 1);
				}
		}
		close(fd);
	}
	message * mess = t.tab_mess;
	for (i = 0; i < t.nb_messages; ++i)
	{
		mot * mots = malloc(nb_mots[i] * sizeof(mot)); 				/*!< Pour chaque message on a un tableau de mots */
		for (j = 0; j < nb_mots[i]; ++j)
		{
			mots[j] = init_mot(j, nb_char[i][j], t.chiffrements[i], t.cles[i], tab_mess[i][j]);		/*!< Initialiser tous nos mots pour chaque message */
		}
		mess[i] = init_mess(i, nb_mots[i], mots, t.chiffrements[i], t.cles[i], t.chemins[i]);		/*!< Mettre les tableau de mots qu'on vient juste d'allouer dans le tableau de de messages */
	}
	t.tab_mess = mess;
	for (i = 0; i < t.nb_messages; ++i)								/*!< Libérer les tableaux d'entiers dont on a plus besoin car ils ont été recopié déjà */
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
		if (c + cle < 128)						/*!< Si on obtient un caractère qui ne sort pas de la table ASCII on ne doit pas forcément faire le -26 */
		{	
			c = c + cle; 						/*!< Modification du caractère */
			if (c > 90) c = c - 26;
		}
		else		
		{										/*!< Si on obtient un caractère qui sort de la table ASCII alors on fait forcément -26 */
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
 * \brief Fonction qui crée le fichier crypté
 * 
 * \param buf Buffer dans lequel est stocké le message crypté sans caractère blanc, retour à la ligne et tabulation
 * \param m Message d'origine
 */
void retour_cryptage(char * buf, message m)
{
	int i, fd, fd2;  
	char c = 'a';
	char * nom_fichier = m.chemin;
	fd = open(nom_fichier, O_RDONLY); 								/*!< On lit le fichier original le fichier original */
	fd2 = open(strcat(nom_fichier, "_cypher"), O_CREAT | O_WRONLY, 0666);
	i = 0;
	while (read(fd, &c, 1))
	{
		if((c != ' ') && (c != '\t') && (c != '\n'))				/*!< Si on est en train de lire une lettre alors on écrit ce qu'il y a dans le buffer (le caractère crypté) */		
		{	
			write(fd2, buf + i, 1);
			++i;
		}
		else		
		{															/*!< Si on est en train de lire un caractère autre qu'une lettre ou un signe, on avance pas dans le buffer et on recopie ce qu'il y a dans le fichier original */
			write(fd2, &c, 1);
		}
	}
	close(fd);
	close(fd2);
}


/**
 * \fn void retour_decryptage(char * buf, message m)
 * \brief Fonction qui crée le fichier decypher à partir du buffer
 * 
 * \param buf Tableau de caractères dans lequel est stocké le message déchiffré sans espace, retour à la ligne et tabulation
 * \param m Message d'origine
 */
void retour_decryptage(char * buf, message m)
{
	int j = 0, fd;  
	char * nom_fichier = strcat(m.chemin, "_decypher");				/*!< On créé le fichier decypher qui contient le contenu du buffer sans caractère blanc, retour à la ligne et tabulation afin que le processus directeur puisse lire dedans */
	fd = open(nom_fichier, O_CREAT | O_WRONLY, 0666);
	do{
		write(fd, buf + j, 1);
		++j;
	}while(buf[j] != '\0');
	close(fd);
}


/**
 * \fn void affiche_decryptage(const message m)
 * \brief Fonction qui affiche le message décrypté. Exécutée par le processus directeur
 * 
 * \param m Message d'origine
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
	{																/*!< Pour conserver les caractères blancs, retour à la ligne et tabulation on utilise la même méthode que dans retour_cryptage */
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
	write (0, &s, 1);												/*!< On sépare deux déchiffrements d'un retour à la ligne */
	close(fd);
	close(fd2);
	unlink(nom_fichier_d);											/*!< On supprime le fichier decypher car il n'y a qu'un seul lien dessus et qu'on supprime ce lien */
}


/**
 * \fn void * thread_buffer(void * z)
 * \brief Fonction exécutée par les threads qui appelle le cryptage ou le decryptage du message et le stocke dans un buffer
 * 
 * \param z Pointeur sur une structure arg qui contient tous les arguments nécessaire à l'exécution de la fonction par les threads
 */
void * thread_buffer(void * z)
{
	arg * a = (arg *) z; 										/*!< Cast */
	int i = a -> emplacement, j = 0; 
	char * retour; 												/*!< retour va contenir le mot traité */
	if (a -> w.chiffrement)										/*!< S'il faut décrypter */
	{
		retour = cryptage_mot(a -> w);
		while(i < (a -> emplacement) + (a -> w.nb_char)) 		/*!< Commencer au prochain caractère libre et écrire tout le mot traité dans le buffer, le mot traité ayant la même taille que le mot non traité */
		{
			a -> b.tab_buff[i] = retour[j]; 					/*!< Ecrire le mot traité caractère par caractère dans le buffer */
			++j; 	
			++i;				
		}
		free(retour); 											/*!< Libérer le mot traité car il est mis dans le buffer */
	}
	else
	{
		retour = decryptage_mot(a -> w);
		while(i < (a -> emplacement) + (a -> w.nb_char)) 		/*!< Commencer au prochain caractère libre et écrire tout le mot traité dans le buffer, le mot traité ayant la même taille que le mot non traité */
		{		
			a -> b.tab_buff[i] = retour[j]; 					/*!< Ecrit le mot traité caractère par caractère dans le buffer */
			++j; 	
			++i;				
		}
		free(retour); 											/*!< Libérer le mot traité car il est mis dans le buffer */
	}
	return NULL; 
}


/**
 * \fn void traitement_message(message m)
 * \brief Fonction exécutée par les processus fils qui va superviser le traitement d'un message par les threads
 * 
 * \param m Message à traiter par les threads
 */
void traitement_message(message m)
{
	buffer b = init_buffer(taille_buffer); 							/*!< Buffer qui sera utilisé par les threads pour écrire le message après traitement */
	arg * tab_arg = malloc(m.nb_mots * sizeof(arg)); 				/*!< Un arg est l'argument passé au thread */
	int emplace[m.nb_mots];											/*!< Contient l'emplacement à partir duquel le thread va commencer à écrire dans le buffer */
	pthread_t * tab_thread = malloc(m.nb_mots * sizeof(pthread_t)); 		/*!< On aura autant de threads que de mots à traiter */
	int i = 0; 
	emplace[0] = 0;
	while(i < m.nb_mots - 1)										/*!< Déterminer où le thread doit écrire en fonction de là où le thread traitant le mot précédent s'arrête d'écrire */
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
}


/**
 * \fn void traitement_entier(traitement t)
 * \brief Fonction exécutée par le processus directeur qui va créer les processus fils et initier le traitement de chaque message
 * 		Elle appelle la fonction affiche_decryptage pour que le processus directeur affiche le déchiffrement de chaque message
 * 
 * \param t Structure traitement contenant tous les messages et leurs données de traitement
 */
void traitement_entier(traitement t)
{
	int i; 
	pid_t a;
	for (i = 0; i < t.nb_messages; ++i)								/*!< Pour chaque message on fait un fork */
	{
		if (!(a = fork()))											/*!< Si on est le fils */
		{
			traitement_message(t.tab_mess[i]);
			exit(0);
		}
		else
		{															/*!< Si on est le père */
			wait(NULL);
			if (t.tab_mess[i].chiffrement == 0)
			{	
				affiche_decryptage(t.tab_mess[i]);
			}
		}	
	}	
}


/**
 * \fn void libere_mot(mot m)
 * \brief Fonction qui libère la mémoire allouée pour un mot
 * 
 * \param m Le mot dont on veut libérer la mémoire
 */
void libere_mot(mot m)
{
	free(m.tab_char);
}


/**
 * \fn void libere_message(message m)
 * \brief Fonction qui libère la mémoire allouée pour un message
 * 
 * \param m Le message dont on veut libérer la mémoire
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
 * \brief Fonction qui libère la mémoire allouée pour la structure traitement
 * 
 * \param t Structure traitement dont on veut libérer la mémoire
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
