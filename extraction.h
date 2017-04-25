#include "structures.h"


mot init_mot(int num_mot, int nb_char, int chiffrement, int cle, char * tab_char); // a faire par le processus principal
message init_mess(int num_mess, int nb_mots, mot * tab_mots, int chiffrement, int cle, char * chemin); // principal
traitement init_traitement(int * chiffrements, int * cles, int nb_messages, char ** chemins); // principal
traitement extraire (char * nom_fichier); // principal
void affiche_traitement(traitement t);
traitement assigne_message (traitement t); // chef d equipe
