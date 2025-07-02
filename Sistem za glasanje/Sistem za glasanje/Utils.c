//#include "Utils.h"
//#include<stdbool.h>
//#include<stdio.h>
//#include "Strukture.h"

#define _CRT_SECURE_NO_WARNINGS

#include"Include.h"

int izbroj_cifre(int number)
{
	int counter = 0;
	int digit;
	while (number != 0)
	{
		digit = number % 10;
		number = number / 10;
		counter++;
	}
	return counter;
}

void ocisti_input_buffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {}
}

bool string_sadrzi_slova_i_specijalne_karaktere(const char* s)
{
	for (int i = 0; s[i] != '\0'; i++)
	{
		if (s[i] < '0' || s[i] > '9')
			return true;
	}
	return false;
}

bool string_sadrzi_brojeve_i_specijalne_karaktere(const char* s) {
	for (int i = 0; i < strlen(s); i++)
	{
		if (!isalpha(s[i]))
			return true;
	}
	return false;
}

bool email_je_ispravno_formatiran(char* s)
{
	char format[] = "@gmail.com";
	bool found = false;
	int formatlen = strlen(format), indeks = 0, len = strlen(s);
	for (int i = 0; i < len; i++)
	{
		if (s[i] == '@')
		{
			found = true;
			break;
		}
		indeks += 1;
	}

	if (found == false)
		return false;

	for (int i = 0; i <= formatlen; i++)
	{
		if (s[indeks + i] != format[i])
			return false;
	}
	return true;
}

char* formatiraj_string(char* s)
{
	s[0] = toupper(s[0]);
	for (int i = 1; s[i] != '\0'; i++)
		s[i] = tolower(s[i]);
	return s;
}

char* citaj_unos()
{
	char* line = NULL, * tmp = NULL;
	size_t size = 0, index = 0;
	int ch = EOF;
	while (ch) {
		ch = getc(stdin);

		/* Provera za kraj */
		if (ch == EOF || ch == '\n')
			ch = 0;

		/* Provera da li treba da se prosiri rezervisan prostor */
		if (size <= index) {
			size += sizeof(ch);
			tmp = realloc(line, size);
			if (!tmp) {
				free(line);
				line = NULL;
				break;
			}
			line = tmp;
		}

		/* Upisi karakter */
		line[index++] = ch;
	}

	return line;
}

bool upisi_podatke_u_fajl(User user)
{
	int upisano = 0;
	if (!korisnik_vec_postoji(user))
	{
		FILE* fajl;
		fajl = fopen("registrovani_korisnici.bin", "ab");
		if (fajl == NULL)
		{
			return false;
		}
		upisano = fwrite(&user, sizeof(User), 1, fajl);
		fclose(fajl);
	}
	else
		printf("Korisnik vec postoji u bazi, ne mozete da se registrujete dva puta kao isti korisnik!\n\n");

	if (upisano == 1)
	{
		printf("Uspesna registracija, podaci zabelezeni!\n");
		return true;
	}
	else
	{
		printf("Greska prilikom registracije i upisa podataka!\n");
		return false;
	}
}

void procitaj_podatke_iz_fajla()
{
	User user;
	FILE* fajl;
	fajl = fopen("registrovani_korisnici.bin", "rb");
	if (fajl == NULL)
	{
		fprintf(stderr, "Greska prilikom otvaranja fajla!\n");
		exit(1);
	}
	while (fread(&user, sizeof(user), 1, fajl) == 1)
	{
		printf("JMBG: %s\n", user.jmbg);
		printf("Ime: %s\n", user.imeKorisnika);
		printf("Prezime: %s\n", user.prezimeKorisnika);
		printf("Email: %s\n", user.email);
		printf("Broj telefona: %s\n", user.brojTelefona);
		printf("Sifra: %s\n", user.sifra);
		printf("Glasacki broj: %s\n", user.glasackiBroj);
		printf("Vas tip korisnika: %s\n", user.tipKorisnika);
		printf("\n");
	}
	fclose(fajl);
}

bool korisnik_vec_postoji(User korisnik_koji_se_registruje)
{
	User user;
	FILE* fajl;
	fajl = fopen("registrovani_korisnici.bin", "rb");
	if (fajl == NULL)
	{
		return false;
	}
	while (fread(&user, sizeof(user), 1, fajl) == 1)
	{
		if (strcmp(korisnik_koji_se_registruje.jmbg, user.jmbg) == 0)
		{
			fclose(fajl);
			return true;
		}
		if (strcmp(korisnik_koji_se_registruje.email, user.email) == 0)
		{
			fclose(fajl);
			return true;
		}
		if (strcmp(korisnik_koji_se_registruje.brojTelefona, user.brojTelefona) == 0)
		{
			fclose(fajl);
			return true;
		}
		if (strcmp(korisnik_koji_se_registruje.glasackiBroj, user.glasackiBroj) == 0)
		{
			fclose(fajl);
			return true;
		}
	}
	fclose(fajl);
	return false;
}