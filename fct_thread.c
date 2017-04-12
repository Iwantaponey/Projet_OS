#include "fonction.h"

//Créer un buffer mais où ? Dans le processus chef d'équipe ?


void encrypting(mot m, buffer buf) // Crypter
{	
	int i; char s;
	for (i = 0; i < m.nb_char; i++)
	{
		s = ((m.tab_char[i]-65) + m.cle)%26;
		buf.tab_buff = s + 65;
	}	
}

void decrypting(mot m, buffer buf) // Décrypter
{
	int i; char s;
	for (i = 0; i < m.nb_char; i++)
	{
		s = ((m.tab_char[i]-65) - m.cle)%26;
		buf.tab_buff = s + 65;
	}
}

// Fonction qui est réalisée par le processus fils (chef d'équipe) uniquement
void traiter(mot m) //Décide le mode de traitement en regardant la variable chiffrement
{
	buffer buf = {m.nb_char, malloc(m.nb_char*sizeof(char)), PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};
	if(m.chiffrement == 0) // On est en déchiffrement
	{
		pthread_t* tid = malloc(m.nb_char * sizeof(pthread_t));
		pthread_create(tid, NULL, &decrypting, &m, &buf); // Si j'ai trop d'argument comment je fais pour passer le buffer dans la fonction?
		pthread_join(*tid, NULL);
	}
	else if(m.chiffrement == 1) // On est en chiffrement
	{
		pthread_t* tid = malloc(m.nb_char * sizeof(pthread_t));
		pthread_create(tid, NULL, &encrypting, &m, &buf); // Si j'ai trop d'argument comment je fais pour passer le buffer dans la fonction?
		pthread_join(*tid, NULL);
	}
} // Ne compile pas ...



// Fonction qui affiche le contenu du buffer : vérifier que tout se passe bien
void affichagebuffer(buffer buf)
{
	int i = 0;
	for (; i < buf.taille_buff; i++)
	{
		printf("%c", buf.tab_buff[i]);
	}
}

int main()
{
	
	return 0;
}
