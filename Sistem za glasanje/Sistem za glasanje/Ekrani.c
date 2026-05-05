#include"Include.h"

void korisnicki_ekran(Korisnik* korisnik)
{
	int izbor, ret;
	nacrtaj_korisnicki_ekran();
	do 
	{
		printf("Odaberite broj 1, 2 ili 3 i pritisnite Enter: ");

		ret = scanf_s("%d", &izbor);
		if (ret != 1 || (izbor < 1 || izbor > 3)) 
		{
			ocisti_ekran();
			nacrtaj_korisnicki_ekran();
			printf("Greska: unos mora biti broj 1, 2 ili 3.\n");
			// ocistim ulazni bafer tako sto uklanjam karaktere iz ulaza dok ne dodjemo do \n ili EOF
			int c;
			while ((c = getchar()) != '\n' && c != EOF) {}
		}
	} while (ret != 1 || (izbor < 1 || izbor > 3));
	if (izbor == 1)
	{
		ocisti_ekran();
		printf("Svi kandidati:\n\n");
		citanje_svih_kandidata(1);
		vrati_se_nazad(korisnik);
	}
	else if (izbor == 2)
	{
		int brojListe, odgovorServera;
		ocisti_ekran();
		citanje_svih_kandidata(2);
		printf("Glasajte za svog kandidata unosenjem broja liste: ");
		scanf_s("%d", &brojListe);
		odgovorServera = glasajte(brojListe, korisnik);
		while (odgovorServera == 5 || odgovorServera == 3)
		{
			ocisti_ekran();
			citanje_svih_kandidata(2);
			printf("Doslo je do greske na serveru, probajte ponovo.\nGlasajte za svog kandidata unosenjem broja liste: ");
			scanf_s("%d", &brojListe);
		}
		if (odgovorServera == 1)
			printf("Uspesno ste glasali za svog kandidata!\n");
		if(odgovorServera == 2)
			printf("Vec ste glasali, ne mozete da glasate ponovo!\n");
		if (odgovorServera == 4)
			printf("Kandidat za koga pokusavate da glasate ne postoji!\n");
		vrati_se_nazad(korisnik);
	}
	//else if (korisnik->glasao)
	//{
	//	printf("Greska, vec ste glasali, i ne mozete opet");
	//}
	else
	{
		ocisti_ekran();
		printf("Pratite rezultate: \n\n");
		citanje_svih_kandidata(3);
		vrati_se_nazad(korisnik);
	}
}

void pocetni_ekran(Korisnik* korisnik)
{
	int izbor, ret;
	nacrtaj_pocetni_ekran();
	do 
	{
		printf("Odaberite broj 1 ili 2 i pritisnite Enter: ");

		ret = scanf_s("%d", &izbor);
		if (ret != 1 || (izbor != 1 && izbor != 2)) 
		{
			ocisti_ekran();
			nacrtaj_pocetni_ekran();
			printf("Greska: unos mora biti broj 1 ili 2\n");
			int c;
			while ((c = getchar()) != '\n' && c != EOF) {}
		}
	} while (ret != 1 || (izbor != 1 && izbor != 2));
	if (izbor == 1)
	{
		ocisti_ekran();
		kreiranje_naloga(korisnik);
	}
	else if (izbor == 2)
	{
		ocisti_ekran();
		prijavite_se(korisnik);
	}
}