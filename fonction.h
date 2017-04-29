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

#define taille_max_chemin 1024
#define taille_buffer 100	

typedef struct
{
    int num_mot; // le numéro du mot dans le message 
    int nb_char; // taille du mot en caractères
    int chiffrement;
    int cle;
    char * tab_char; // tableau de caractères ayant pour taille la taille du mot
}mot;

typedef struct 
{
	int num_mess; // Numéro du message dans le fichier principal donné en entrée
    int nb_mots; // Nombre de mots dans le message
    mot * tab_mots; // Tableau de structures mots
    int chiffrement;// mode de traitement (si chiffrement=0, alors on est en déchiffrement, sinon on est en chiffrement)
    int cle;// pas du chiffrement de César
    char * chemin;
}message;

typedef struct // 
{
	int * chiffrements;
	int * cles;
    int nb_messages; // Pour créer le nombre de processus fils correspondant
    char ** chemins; // Tableau de chaînes de caractères qui pour chaque message contient son chemin, seulement pour extraire le contenu du fichier principal
    message * tab_mess; // Tableau de struct message au nombre de nb_messages

}traitement;  

typedef struct 
{
	char * tab_buff; // tableau de caractères qui contiendra l’intégralité du message
    pthread_mutex_t * mutex; // variable mutex qui permettra d’assurer une section critique autour du buffer, assurer le postage du mot de chaque thread dans le bon ordre pour avoir un message ayant du sens ?
    pthread_cond_t * cond; // variable mutex qui permettra d’assurer une section critique autour du buffer, assurer le postage du mot de chaque thread dans le bon ordre pour avoir un message ayant du sens ?
}buffer;

typedef struct
{
	mot w; // mot que le thread aura à traiter
	buffer b; // buffer que les threads utiliseront pour rendre leur travail
	int emplacement; // emplacement ou on commence à écrire dans le buffer 
	int nb_mots;
	int compteur;
}arg;

mot init_mot(int num_mot, int nb_char, int chiffrement, int cle, char * tab_char);
message init_mess(int num_mess, int nb_mots, mot * tab_mots, int chiffrement, int cle, char * chemin);
traitement init_traitement(int * chiffrements, int * cles, int nb_messages, char ** chemins);
traitement extraire (char * nom_fichier);
void affiche_traitement(traitement t);
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
