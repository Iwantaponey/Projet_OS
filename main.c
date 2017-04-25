#include "extraction.h"
#include "traitement.h"
#include "renvoi.h"



int main (int argc, char ** argv)
{
	traitement t;
	t=extraire(argv[1]);
	t=assigne_message(t);
	affiche_traitement(t);
	return 0;
}
