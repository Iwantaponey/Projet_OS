#include "traitement.h"

char cryptage(char c, int cle)
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

int main(int argc, char ** argv)
{
	int cle = 3; // Test avec une clé de cryptage à 3
	char c = argv[1][0]; // Caractère à modifier
	char d = cryptage(c, cle);
	printf ("%c \n", d);
	return 0;
}

// Compile avec gcc traitement.c et exécute avec ./a.out
