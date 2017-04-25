#include "traitement.h"

char cryptage(char c, int cle)
{
	if((c < 65) || ((c > 90) && (c < 97)) || (c > 122)) return c; // Test si on est bien dans les lettres minuscules ou majuscules uniquement
	c = c + cle; // Modification du caractère
	printf ("%c\n", c);
	if((c > 90) || (c > 122))  c = c - 26; // Ligne qui fait de la merde quand avec un p en entrée mais pas avec un z par exemple
	printf ("%c\n", c);
	return c;
}

int main()
{
	int cle = 3; // Test avec une clé de cryptage à 3
	char c = 'p'; // Caractère à modifier
	char d = cryptage(c, cle);
	printf ("%c\n", d);
	return 0;
}

// Compile avec gcc traitement.c et exécute avec ./a.out
