#pragma once
#include"Strukture.h"
#include<stdbool.h>

bool string_sadrzi_slova_i_specijalne_karaktere(const char* s);
bool string_sadrzi_brojeve_i_specijalne_karaktere(const char* s);
char* formatiraj_string(char* s);
char* citaj_unos();
bool email_je_ispravno_formatiran(char* s);
void unesi_podatke(Korisnik* korisnik);
void ocisti_podatke(Korisnik* korisnik);
SOCKET kreiraj_soket(char* portServera);
void citanje_svih_kandidata(int izbor);
void vrati_se_nazad(Korisnik* korisnik);
void kopiraj_strukture(Korisnik* korisnik, Korisnik2* korisnik2, int tipOperacije);
void uspesna_verifikacija_koriscenjem_email_koda(SOCKET serverSocket);