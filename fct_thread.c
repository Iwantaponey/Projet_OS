
//Créer un buffer mais où ? Chef d'équipe ?


void encrypting(mot m, buffer buf) //crypter
{	
	for (i = 0; i < m.nb_char); i++)
	{
		s = ((m.tab_char[i]-65) + m.cle)%26;
		buf.tab_buff = s + 65;
	}	
}

void decrypting(mot m, buffer buf) //décrypter
{
	for (i = 0; i < m.nb_char; i++)
	{
		s = ((m.tab_char[i]-65) - m.cle)%26;
		buf.tab_buff = s + 65;
	}
}
