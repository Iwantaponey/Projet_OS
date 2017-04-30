/**
 * \file fonction.h
 * \brief Fichier header du fichier fonction.c
 * 
 * Fichier contenant les structures utilisées dans le programme
 * et les prototypes des fonctions implémentées dans le fichier fonction.c
 * 
 */
 
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>
#include <limits.h>

#define taille_max_chemin 1000000 		/*!< Taille max que peut faire le chemin d'un fichier */
#define taille_buffer INT_MAX			/*!< Taille max que peut faire le buffer */


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
	int num_mess; 			/*!< Numéro du message dans le fichier principal  */
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
	int initialise;			/*!< Entier qui permet de savoir si l'initialisation a eu lieu correctement ou non */
	int * chiffrements;		/*!< Tableau d'entiers contenant les modes de traitement pour chaque message */
	int * cles;				/*!< Tableau d'entiers contenant les clés pour chaque message */
    int nb_messages; 		/*!< Entier qui contient le nombre de messages à traiter */
    char ** chemins; 		/*!< Tableau de chaînes de caractères qui, pour chaque 
								fichier message, contient son chemin  */
    message * tab_mess; 	/*!< Tableau de struct message de taille nb_messages */
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
 * Structures servant à donner aux threads tout ce qui est nécessaire pour leur traitement
 */
typedef struct
{
	mot w; 					/*!< Mot que le thread aura à traiter */
	buffer b; 				/*!< Buffer unique par message que les threads utiliseront pour rendre leur travail */
	int emplacement; 		/*!< Emplacement où le thread commence à écrire dans le buffer */
}arg;


/// Prototypes des fonctions


mot init_mot(int num_mot, int nb_char, int chiffrement, int cle, char * tab_char);
message init_mess(int num_mess, int nb_mots, mot * tab_mots, int chiffrement, int cle, char * chemin);
traitement init_traitement(int * chiffrements, int * cles, int nb_messages, char ** chemins);
buffer init_buffer(int taille_buff);
int compte_nb_messages(char * nom_fichier);
int * recupere_cle(int * cle_finale, int nb_messages, int cle[nb_messages][2]);
traitement extraire(char * nom_fichier);
int ** compte_nb_char(int * nb_mots, int ** nb_char, traitement t);
int * compte_nb_mots(int * nb_mots, traitement t);
traitement assigne_message(traitement t);
char cryptage_char(char c, int cle);
char * cryptage_mot(const mot m);
char decryptage_char(char c, int cle);
char * decryptage_mot(const mot m);
void retour_cryptage(char * buf, message m);
void retour_decryptage(char * buf, message m);
void affiche_decryptage(const message m);
void * thread_buffer(void * z);
void traitement_message(message m);
void traitement_entier(traitement t);
void libere_mot(mot m);
void libere_message(message m);
void libere_traitement(traitement t);
