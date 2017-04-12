#include "fonction.h"
//Créer un buffer mais où ? Chef d'équipe ?
pthread_t tid;
mot m= {tid, 0, 6, 1, 8, "patate"}; 

void encrypting(mot m, buffer buf) //crypter
{	
	int i; char s;
	for (i = 0; i < m.nb_char; i++)
	{
		s = ((m.tab_char[i]-65) + m.cle)%26;
		buf.tab_buff = s + 65;
	}	
}

void decrypting(mot m, buffer buf) //décrypter
{
	int i; char s;
	for (i = 0; i < m.nb_char; i++)
	{
		s = ((m.tab_char[i]-65) - m.cle)%26;
		buf.tab_buff = s + 65;
	}
}


int main()
{
	
	return 0;
}
