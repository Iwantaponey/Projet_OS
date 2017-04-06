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

typedef struct
{
    int num; // le numéro du mot dans le message 
    pthread_t tid; // le thread assigné au traitement de ce message
    int nb_char; // taille du mot en caractères
    char * tab_char; // tableau de caractères ayant pour taille la taille du mot
}mot;

typedef struct 
{
    pid_t pid; // Identifiant du processus assigné au traitement de ce message
	int num; // Numéro du message dans le fichier principal donné en entrée
    int nb_mots; // Nombre de mots dans le message
    mot * tab_mots; // Tableau de structures mots
}message;

typedef struct // 
{
    int chiffrement; // mode de traitement (si chiffrement=0, alors on est en déchiffrement, sinon on est en chiffrement)
    int cle; // pas du chiffrement de César
    int nb_messages; // Pour créer le nombre de processus fils correspondant
    char ** chemin; // Tableau de chaînes de caractères qui pour chaque message contient son chemin, seulement pour extraire le contenu du fichier principal
    message * tab_mess; // Tableau de struct message au nombre de nb_messages

}traitement;  

typedef struct 
{
    int taille_buff; // Taille du message
    char * tab_buff; // tableau de caractères qui contiendra l’intégralité du message
    pthread_mutex_t mutex; // variable mutex qui permettra d’assurer une section critique autour du buffer, assurer le postage du mot de chaque thread dans le bon ordre pour avoir un message ayant du sens ?
    pthread_cond_t cond; // variable condition qui permettra d’assurer que tous les threads ont bien terminé leur exécution avant que le processus chef d’équipe ne vienne récupérer le contenu de tab
}buffer;

typedef struct
{
    int traite; 
    int chiffrement;    
}retour_message;



mot init_mot(pthread_t tid, int num, int nb_char);
message init_mess(pid_t pid, int num, int nb_mots);
traitement init_traitement(char chiffrement, int cle, int nb_messages, char ** chemin);
traitement extraire (traitement t, char ** parametres);
