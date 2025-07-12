#include"Include.h"

void korisnicki_ekran(User* user)
{
	int izbor, ret;
	nacrtaj_korisnicki_ekran();
	do {
		printf("Odaberite broj 1, 2 ili 3 i pritisnite Enter: ");

		ret = scanf_s("%d", &izbor);
		if (ret != 1 || (izbor < 1 || izbor > 3)) {
			ocisti_ekran();
			nacrtaj_korisnicki_ekran();
			printf("Greska: unos mora biti broj 1, 2 ili 3.\n");
			// ocistimo ulazni bafer tako sto uklanjamo karaktere iz ulaza dok ne dodjemo do \n ili EOF
			int c;
			while ((c = getchar()) != '\n' && c != EOF) {}
		}
	} while (ret != 1 || (izbor < 1 || izbor > 3));
	if (izbor == 1)
	{
		ocisti_ekran();
		printf("Svi kandidati:\n\n");
		citanje_svih_kandidata();
	}
	else if (izbor == 2 && !user->glasao)
	{
		ocisti_ekran();
		printf("Glasajte za svog kandidata: ");
	}
	else if (user->glasao) 
	{
		printf("Greska, vec ste glasali, i ne mozete opet");
	}
	else
	{
		ocisti_ekran();
		printf("Pratite rezultate: ");
	}
}

void admin_ekran()
{
	int izbor, ret;
	nacrtaj_korisnicki_ekran();
	do {
		printf("Odaberite broj 1, 2 ili 3 i pritisnite Enter: ");

		ret = scanf_s("%d", &izbor);
		if (ret != 1 || (izbor < 1 || izbor > 3)) {
			ocisti_ekran();
			nacrtaj_admin_ekran();
			printf("Greska: unos mora biti broj 1, 2 ili 3.\n");
			// ocistimo ulazni bafer tako sto uklanjamo karaktere iz ulaza dok ne dodjemo do \n ili EOF
			int c;
			while ((c = getchar()) != '\n' && c != EOF) {}
		}
	} while (ret != 1 || (izbor < 1 || izbor > 3));
	if (izbor == 1)
	{
		ocisti_ekran();
		printf("Dodaj novog kandidata/stranku:");
	}
	else if (izbor == 2)
	{
		ocisti_ekran();
		printf("Ukloni kandidata/stranku: ");
	}
	else
	{
		ocisti_ekran();
		printf("Pratite rezultate: ");
	}
}

void pocetni_ekran(User* user)
{
	int izbor, ret;
	nacrtaj_pocetni_ekran();
	do {
		printf("Odaberite broj 1 ili 2 i pritisnite Enter: ");

		ret = scanf_s("%d", &izbor);
		if (ret != 1 || (izbor != 1 && izbor != 2)) {
			ocisti_ekran();
			nacrtaj_pocetni_ekran();
			printf("Greska: unos mora biti broj 1 ili 2\n");
			// ocistimo ulazni bafer tako sto uklanjamo karaktere iz ulaza dok ne dodjemo do \n ili EOF
			int c;
			while ((c = getchar()) != '\n' && c != EOF) {}
		}
	} while (ret != 1 || (izbor != 1 && izbor != 2));
	if (izbor == 1)
	{
		ocisti_ekran();
		kreiranje_naloga(&user);
	}
	else if (izbor == 2)
	{
		ocisti_ekran();
		prijavite_se(&user);
	}
}