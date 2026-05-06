#include"Include.h"

void korisnicki_ekran(Korisnik* korisnik)
{
	int izbor, ret;
	nacrtaj_korisnicki_ekran();
	do 
	{
		printf("Odaberite broj 1, 2 ili 3 i pritisnite Enter: ");

		ret = scanf_s("%d", &izbor);
		if (ret != 1 || (izbor < prikazKandidata || izbor > pracenjeRezultata)) 
		{
			ocisti_ekran();
			nacrtaj_korisnicki_ekran();
			printf("Greska: unos mora biti broj 1, 2 ili 3.\n");
			// ocistim ulazni bafer tako sto uklanjam karaktere iz ulaza dok ne dodjemo do \n ili EOF
			int c;
			while ((c = getchar()) != '\n' && c != EOF) {}
		}
	} while (ret != 1 || (izbor < prikazKandidata || izbor > pracenjeRezultata));
	if (izbor == prikazKandidata)
	{
		ocisti_ekran();
		printf("Svi kandidati:\n\n");
		citanje_svih_kandidata(praznaOperacija);
		vrati_se_nazad(korisnik);
	}
	else if (izbor == glasanjeZaKandidata)
	{
		int brojListe, odgovorServera;
		ocisti_ekran();
		citanje_svih_kandidata(praznaOperacija);
		printf("Glasajte za svog kandidata unosenjem broja liste: ");
		scanf_s("%d", &brojListe);
		odgovorServera = glasajte(brojListe, korisnik);
		while (odgovorServera == nedefinisanOdgovor || odgovorServera == serverskaGreska)
		{
			ocisti_ekran();
			citanje_svih_kandidata(praznaOperacija);
			printf("Doslo je do greske na serveru, probajte ponovo.\nGlasajte za svog kandidata unosenjem broja liste: ");
			scanf_s("%d", &brojListe);
		}
		if (odgovorServera == uspesnoGlasanje)
			printf("Uspesno ste glasali za svog kandidata!\n");
		if(odgovorServera == ponovnoGlasanje)
			printf("Vec ste glasali, ne mozete da glasate ponovo!\n");
		if (odgovorServera == nepostojeciKandidat)
			printf("Kandidat za koga pokusavate da glasate ne postoji!\n");
		vrati_se_nazad(korisnik);
	}
	else
	{
		ocisti_ekran();
		printf("Pratite rezultate: \n\n");
		citanje_svih_kandidata(pracenjeRezultata);
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
		if (ret != 1 || (izbor != kreiranjeNaloga && izbor != prijavljivanjeNaNalog)) 
		{
			ocisti_ekran();
			nacrtaj_pocetni_ekran();
			printf("Greska: unos mora biti broj 1 ili 2\n");
			int c;
			while ((c = getchar()) != '\n' && c != EOF) {}
		}
	} while (ret != 1 || (izbor != 1 && izbor != 2));
	if (izbor == kreiranjeNaloga)
	{
		ocisti_ekran();
		kreiranje_naloga(korisnik);
	}
	else if (izbor == prijavljivanjeNaNalog)
	{
		ocisti_ekran();
		prijavite_se(korisnik);
	}
}