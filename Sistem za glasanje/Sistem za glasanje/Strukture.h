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

typedef enum 
{
	prikazKandidata = 1,
	glasanjeZaKandidata = 2,
	pracenjeRezultata = 3,
	praznaOperacija = 4
}IzborEkrana;

typedef enum 
{
	uspesnoGlasanje = 1,
	ponovnoGlasanje = 2,
	serverskaGreska = 3,
	nepostojeciKandidat = 4,
	nedefinisanOdgovor = 5
}OdgovoriServera;

typedef enum 
{
	uspesnoKreiranje = 1,
	vecRegistrovanBirac = 2,
	nepoznataOperacijaKreiranja = 3,
	neispravanVerifikacioniKod = 4,
}KreiranjeNalogaOperacije;

typedef enum
{
	uspesnoPrijavljivanje = 1,
	nepostojeciPodaci = 2,
	nepoznataOperacijaPrijavljivanja = 3,
}PrijavljivanjeNaNalogOperacije;