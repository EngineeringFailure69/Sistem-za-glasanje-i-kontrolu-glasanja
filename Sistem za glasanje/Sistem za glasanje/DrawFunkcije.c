#include "DrawFunkcije.h"

void nacrtaj_korisnicki_ekran()
{
	printf("\t\tDobrodosli u elektronski sistem za glasanje i kontrolu glasanja u Republici Srbiji\n\n");
	printf("\t\t\t -----------------------------------------------\n");
	printf("\t\t\t|\t\t\t\t\t\t|\n\t\t\t|\t 1) Procitajte sve kandidate  \t\t|\n\t\t\t|\t\t\t\t\t\t|\n");
	printf("\t\t\t|\t\t\t\t\t\t|\n\t\t\t|\t 2) Glasajte za svog kandidata \t\t|\n\t\t\t|\t\t\t\t\t\t|\n");
	printf("\t\t\t|\t\t\t\t\t\t|\n\t\t\t|\t 3) Pratite trenutne rezultate \t\t|\n\t\t\t|\t\t\t\t\t\t|\n");
	printf("\t\t\t -----------------------------------------------\n\n");
}

void nacrtaj_pocetni_ekran()
{
	printf("\t\tDobrodosli u elektronski sistem za glasanje i kontrolu glasanja u Republici Srbiji\n\n");
	printf("\t\t\t -----------------------------------------------\n");
	printf("\t\t\t|\t\t\t\t\t\t|\n\t\t\t| 1) Registrujte se da bi mogli da pristupite  \t|\n\t\t\t|\t\t\t\t\t\t|\n");
	printf("\t\t\t|\t\t\t\t\t\t|\n\t\t\t|                                            \t|\n\t\t\t|\t\t\t\t\t\t|\n");
	printf("\t\t\t|\t\t\t\t\t\t|\n\t\t\t| 2) Prijavite se ukoliko vec imate nalog \t|\n\t\t\t|\t\t\t\t\t\t|\n");
	printf("\t\t\t -----------------------------------------------\n\n");
}

void ocisti_ekran()
{
#ifdef _WIN32
	system("cls");
#endif // _WIN32
}