#pragma once
#include"Strukture.h"
#include<stdbool.h>

bool string_sadrzi_slova_i_specijalne_karaktere(const char* s);
bool string_sadrzi_brojeve_i_specijalne_karaktere(const char* s);
char* formatiraj_string(char* s);
char* citaj_unos();
void procitaj_podatke_iz_fajla(); //Vise je kao test funkcija
//Ova funkcija se isto seli na server posle
bool korisnik_vec_postoji(Korisnik korisnik_koji_se_registruje, bool prijavljivanje);
bool email_je_ispravno_formatiran(char* s);
void unesi_podatke(Korisnik* korisnik);
void ocisti_podatke(Korisnik* korisnik);
SOCKET kreiraj_soket();
void citanje_svih_kandidata(int izbor);
void vrati_se_nazad(Korisnik* korisnik);