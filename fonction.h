/**
 * \file fonction.h
 * \brief Fichier header du fichier fonction.c
 * 
 * Fichier contenant les structures utilisées dans le programme
 * et les prototypes des fonctions implémentée dans le fichier 
 * fonction.c
 * 
 */
 
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
#include <limits.h>

#define taille_max_chemin 1024 		/*!< On en a besoin de ces constantes ??????? */
#define taille_buffer INT_MAX			/*!< ???????????????????????????????????????? */


/// Définition des structures du programme


/**
 * \struct mot
 * \brief Définition d'un mot
 */
typedef struct
{
    int num_mot; 			/*!< Numéro du mot dans le message */
    int nb_char; 			/*!< Taille du mot en caractères */
    int chiffrement; 		/*!< Mode de chiffrement : cryptage ou décryptage 
								(si chiffrement=0, alors on est en déchiffrement, 
								sinon on est en chiffrement) */
    int cle;				/*!< Pas du décalage dans l'alphabet */
    char * tab_char; 		/*!< Tableau de caractères de taille la taille du mot */
}mot;


/**
 * \struct message
 * \brief Définition d'un message
 */
typedef struct 
{
	int num_mess; 			/*!< Numéro du message dans le fichier principal 
								donné en entrée */
    int nb_mots; 			/*!< Nombre de mots dans le message */
    mot * tab_mots; 		/*!< Tableau de structures mots */
    int chiffrement;		/*!< Mode de chiffrement : cryptage ou décryptage 
								(si chiffrement=0, alors on est en déchiffrement, 
								sinon on est en chiffrement) */
    int cle;				/*!< Pas du décalage dans l'alphabet */
    char * chemin;			/*!< Chemin du fichier dans l'arborescence des fichiers */
}message;


/**
 * \struct traitement
 * \brief Définition de traitement 
 * 
 * Champs nécessaires à la fabrication de la structure unique portant le reste des sous-structures
 * 
 */
typedef struct
{
	int initialise;
	int * chiffrements;		/*!< Tableau d'entiers contenant les modes d'action des 
								threads pour chaque message */
	int * cles;				/*!< Tableau d'entiers contenant les clés pour chaque message */
    int nb_messages; 		/*!< Pour créer le nombre de processus fils correspondant */
    char ** chemins; 		/*!< Tableau de chaînes de caractères qui, pour chaque 
								fichier message, contient son chemin (seulement pour 
								extraire le contenu du fichier principal) */
    message * tab_mess; 	/*!< Tableau de struct message au nombre de nb_messages */
}traitement;  


/**
 * \struct buffer
 * \brief Définition du buffer
 */
typedef struct 
{
	char * tab_buff; 		/*!< tableau de caractères qui contiendra l’intégralité du message */
}buffer;


/**
 * \struct arg
 * \brief Définition de l'argument de thread
 * 
 * Structures servant à gérer l'écriture du mode dans le buffer après cryptage ou décryptage du mot
 * 
 */
typedef struct
{
	mot w; 					/*!< Mot que le thread aura à traiter */
	buffer b; 				/*!< Buffer que les threads utiliseront pour rendre leur travail */
	int emplacement; 		/*!< Emplacement où on commence à écrire dans le buffer */
	int nb_mots;			/*!< Nombre de mots ??????????????????????????????????? */
	int compteur;			/*!< ????????????????????????????????????????? */
}arg;


/// Prototypes des fonctions


mot init_mot(int num_mot, int nb_char, int chiffrement, int cle, char * tab_char);
message init_mess(int num_mess, int nb_mots, mot * tab_mots, int chiffrement, int cle, char * chemin);
traitement init_traitement(int * chiffrements, int * cles, int nb_messages, char ** chemins);
traitement extraire (char * nom_fichier);
traitement assigne_message (traitement t);
void * thread_buffer(void * z);
char cryptage_char(char c, int cle);
char * cryptage_mot (const mot m);
char decryptage_char(char c, int cle);
char * decryptage_mot (const mot m);
void * thread_buffer(void * z);
char * assigne_thread(traitement t, int num_mess);
void retour_cryptage(char * buf, message m);
void retour_decryptage(char * buf, message m);
char * traitement_message (message m;);
int traitement_entier (traitement t);
void libere_mot(mot m);
void libere_message(message m);
void libere_traitement(traitement t);
void barrier(arg * a);
