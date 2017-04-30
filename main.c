/**
 * \file main.c
 * \brief Fichier main du programme du chiffrement de Cesar
 * 
 * Fichier d'appel aux fonctions
 * 
 */

#include "fonction.h"

/**
 * \fn int main (int argc, char ** argv)
 * \brief Fonction principale ou directrice du programme
 * 
 * \param argc Nombre de paramètres donnés lors de l'appel au programme
 * \param argv Tableau de chaînes de caractères
 * 		On ne se sert que de argv[1], qui est le nom du fichier dans lequel les paths des fichiers messages sont stockés
 * 
 * \return
 */

int main (int argc, char ** argv)
{
	if(argc == 2)
	{
		traitement t;
		t = extraire(argv[1]);
		if (t.initialise)					/*!< Si tout s'est bien passé alors le champs d'initialisation de traitement est valide donc aucune erreur n'a été détectée et on peut faire la suite du programme */
		{	
			t = assigne_message(t);
			traitement_entier(t);
			libere_traitement(t);
		}
	}
	else printf("Le nombre d'argument d'appel au programme n'est pas correct \n");
	return 0;
}
