#pragma once

#include <stdbool.h>

typedef struct
{
	char punNazivStranke[100];
	char skracenica[6];
	char imeLidera[30];
	char prezimeLidera[30];
	int redniBroj;
	int brojGlasova;
}Kandidat;

typedef struct
{
	char jmbg[14];
	char tipKorisnika[6];
	char imeKorisnika[30];
	char prezimeKorisnika[30];
	char sifra[1024];
	char email[50];
	char brojTelefona[11];
	char glasackiBroj[7];
	bool glasao;
}User;

typedef struct
{
	char jmbg[14];
	char imeKorisnika[15];
	char prezimeKorisnika[30];
	char brojTelefona[11];
	char glasackiBroj[7];
}User2;