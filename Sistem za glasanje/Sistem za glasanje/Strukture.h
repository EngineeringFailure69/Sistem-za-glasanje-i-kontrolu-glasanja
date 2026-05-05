#pragma once

#include <stdbool.h>

typedef struct
{
	char punNazivStranke[101];
	char skracenica[7];
	char imeLidera[16];
	char prezimeLidera[31];
	int redniBroj;
	int brojGlasova;
}Kandidat;

typedef struct
{
	char jmbg[14];
	char imeKorisnika[16];
	char prezimeKorisnika[31];
	char sifra[1025];
	char email[51];
	char brojTelefona[11];
	char glasackiBroj[7];
}Korisnik;

typedef struct
{
	char jmbg[14];
	char imeKorisnika[16];
	char prezimeKorisnika[31];
	char sifra[1025];
	char email[51];
	char brojTelefona[11];
	char glasackiBroj[7];
	int tipOperacije;
}Korisnik2;

typedef enum
{
	kreiranjeNaloga = 1,
	prijavljivanjeNaNalog = 2,
	glasanje = 3
}TipOperacije;