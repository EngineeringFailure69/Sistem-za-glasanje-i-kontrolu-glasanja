#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<stdbool.h>
#include<string.h>
#include<Windows.h>

#pragma region prototipovi f-ja
void kreiranje_naloga();
int izbroj_cifre(int number);
void nacrtaj_korisnicki_ekran();
void nacrtaj_admin_ekran();
void nacrtaj_pocetni_ekran();
void ocisti_ekran();
void korisnicki_ekran();
void admin_ekran();
void pocetni_ekran();
void ocisti_input_buffer();
bool string_sadrzi_slova_i_specijalne_karaktere(const char* s);
bool string_sadrzi_brojeve_i_specijalne_karaktere(const char* s);
char* formatiraj_string(char* s);
char* citaj_unos();
#pragma endregion

#pragma region strukture

typedef struct 
{
	int redniBroj;
	char nazivStranke[100];
	char liderStranke[50];
}Kandidat;

typedef struct 
{
	char jmbg[15];
	char tipKorisnika[5];
	char imeKorisnika[50];
	char prezimeKorisnika[50];
	char sifra[1024];
	char email[50];
	char brojTelefona[12];
}User;

#pragma endregion

#pragma region pomocne f-je

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

	for(int i = 0; i<=formatlen; i++)
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

#pragma endregion

#pragma region draw f-je
void nacrtaj_korisnicki_ekran()
{
	printf("\t\tDobrodosli u elektronski sistem za glasanje i kontrolu glasanja u Republici Srbiji\n\n");
	printf("\t\t\t -----------------------------------------------\n");
	printf("\t\t\t|\t\t\t\t\t\t|\n\t\t\t|\t 1) Procitajte sve kandidate  \t\t|\n\t\t\t|\t\t\t\t\t\t|\n");
	printf("\t\t\t|\t\t\t\t\t\t|\n\t\t\t|\t 2) Glasajte za svog kandidata \t\t|\n\t\t\t|\t\t\t\t\t\t|\n");
	printf("\t\t\t|\t\t\t\t\t\t|\n\t\t\t|\t 3) Pratite trenutne rezultate \t\t|\n\t\t\t|\t\t\t\t\t\t|\n");
	printf("\t\t\t -----------------------------------------------\n\n");
}

void nacrtaj_admin_ekran()
{
	printf("\t\tDobrodosli u elektronski sistem za glasanje i kontrolu glasanja u Republici Srbiji\n\n");
	printf("\t\t\t -----------------------------------------------\n");
	printf("\t\t\t|\t\t\t\t\t\t|\n\t\t\t|\t 1) Dodaj novog kandidata/stranku  \t\t|\n\t\t\t|\t\t\t\t\t\t|\n");
	printf("\t\t\t|\t\t\t\t\t\t|\n\t\t\t|\t 2) Ukloni kandidata/stranku \t\t|\n\t\t\t|\t\t\t\t\t\t|\n");
	printf("\t\t\t|\t\t\t\t\t\t|\n\t\t\t|\t 3) Pratite trenutne rezultate \t\t|\n\t\t\t|\t\t\t\t\t\t|\n");
	printf("\t\t\t -----------------------------------------------\n\n");
}

void nacrtaj_pocetni_ekran()
{
	printf("\t\tDobrodosli u elektronski sistem za glasanje i kontrolu glasanja u Republici Srbiji\n\n");
	printf("\t\t\t -----------------------------------------------\n");
	printf("\t\t\t|\t\t\t\t\t\t|\n\t\t\t| 1) Registrujte se da bi mogli da pristupite  \t|\n\t\t\t|\t\t\t\t\t\t|\n");
	printf("\t\t\t|\t\t\t\t\t\t|\n\t\t\t|                                            \t|\n\t\t\t|\t\t\t\t\t\t|\n");
	printf("\t\t\t|\t\t\t\t\t\t|\n\t\t\t| 3) Ulogujte se ukoliko vec imate nalog \t|\n\t\t\t|\t\t\t\t\t\t|\n");
	printf("\t\t\t -----------------------------------------------\n\n");
}
#pragma endregion

#pragma region screen f-je

void ocisti_ekran()
{
#ifdef _WIN32
	system("cls");
#endif // _WIN32
}

void korisnicki_ekran() 
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
		printf("Svi kandidati:");
	}
	else if (izbor == 2)
	{
		ocisti_ekran();
		printf("Glasajte za svog kandidata: ");
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

void pocetni_ekran()
{
	int izbor, ret;
	nacrtaj_pocetni_ekran();
	do {
		printf("Odaberite broj 1 ili 2 i pritisnite Enter: ");

		ret = scanf_s("%d", &izbor);
		if (ret != 1 || izbor != 1 || izbor != 2) {
			ocisti_ekran();
			nacrtaj_pocetni_ekran();
			printf("Greska: unos mora biti broj 1 ili 2\n");
			// ocistimo ulazni bafer tako sto uklanjamo karaktere iz ulaza dok ne dodjemo do \n ili EOF
			int c;
			while ((c = getchar()) != '\n' && c != EOF) {}
		}
	} while (ret != 1 || izbor != 1);
	if (izbor == 1)
	{
		ocisti_ekran();
		kreiranje_naloga();
	}
	else if (izbor == 2)
	{
		printf("Tess");
	}
}

void kreiranje_naloga()
{
	printf("Kreirajte nalog popunjavanjem svih podataka, svi podaci su obavezni: \n\n");
	User user;
	user.jmbg[0] = '\0';
	user.imeKorisnika[0] = '\0';
	user.prezimeKorisnika[0] = '\0';
	user.sifra[0] = '\0';
	user.email[0] = '\0';
	user.brojTelefona[0] = '\0';

	strcpy_s(user.tipKorisnika, sizeof(user.tipKorisnika), "user");

	do 
	{
		printf("Unesite vas JMBG (13 cifara): ");
		fgets(user.jmbg, sizeof(user.jmbg), stdin);
		user.jmbg[strcspn(user.jmbg, "\n")] = '\0';
		if (strlen(user.jmbg) != 13 || string_sadrzi_slova_i_specijalne_karaktere(user.jmbg))
		{
			ocisti_ekran();
			printf("Kreirajte nalog popunjavanjem svih podataka, svi podaci su obavezni: \n\n");
			printf("Greska pri unosu, jmbg mora imati 13 cifara, i ne sme sadrzati slova i specijalne karaktere\n");
		}
		else if(strlen(user.jmbg) == 13 && !string_sadrzi_slova_i_specijalne_karaktere(user.jmbg))
		{
			break;
		}
	} while (1);

	do
	{
		printf("Unesite vase ime: ");
		char* input = citaj_unos();
		input[strcspn(input, "\n")] = '\0';
		if (strlen(input) <= 0 || strlen(input) > 49 || string_sadrzi_brojeve_i_specijalne_karaktere(input))
		{
			ocisti_ekran();
			printf("Kreirajte nalog popunjavanjem svih podataka, svi podaci su obavezni: \n\n");
			printf("JMBG: %s\n", user.jmbg);
			printf("Greska pri unosu, ime mora imati od 1 do 50 karaktera i ne sme sadrzati brojeve i specijalne karaktere\n");
			input = '\0';
		}
		else if (strlen(input) > 0 && strlen(input) < 49 && !string_sadrzi_brojeve_i_specijalne_karaktere(input))
		{
			strcpy(user.imeKorisnika, input);
			formatiraj_string(user.imeKorisnika);
			break;
		}

	} while (1);

	do
	{
		printf("Unesite vase prezime: ");
		char* input = citaj_unos();
		input[strcspn(input, "\n")] = '\0';
		if (strlen(input) <= 0 || strlen(input) > 49 || string_sadrzi_brojeve_i_specijalne_karaktere(input))
		{
			ocisti_ekran();
			printf("Kreirajte nalog popunjavanjem svih podataka, svi podaci su obavezni: \n\n");
			printf("JMBG: %s\n", user.jmbg);
			printf("Ime: %s\n", user.imeKorisnika);
			printf("Greska pri unosu, prezime mora imati od 1 do 50 karaktera i ne sme sadrzati brojeve i specijalne karaktere\n");
			input = '\0';
		}
		else if (strlen(input) > 0 && strlen(input) < 49 && !string_sadrzi_brojeve_i_specijalne_karaktere(input))
		{
			strcpy(user.prezimeKorisnika, input);
			formatiraj_string(user.prezimeKorisnika);
			break;
		}

	} while (1);

	do
	{
		printf("Unesite vas email: ");
		char* input = citaj_unos();
		input[strcspn(input, "\n")] = '\0';
		if (strlen(input) <= 0 || strlen(input) > 49 || !email_je_ispravno_formatiran(input))
		{
			ocisti_ekran();
			printf("Kreirajte nalog popunjavanjem svih podataka, svi podaci su obavezni: \n\n");
			printf("JMBG: %s\n", user.jmbg);
			printf("Ime: %s\n", user.imeKorisnika);
			printf("Prezime: %s\n", user.prezimeKorisnika);
			printf("Greska pri unosu, email mora imati od 1 do 50 karaktera, i mora biti u formatu primer@gmail.com\n");
			input = '\0';
		}
		else if (strlen(input) > 0 && strlen(input) < 49 && email_je_ispravno_formatiran(input))
		{
			strcpy(user.email, input);
			formatiraj_string(user.email);
			break;
		}

	} while (1);

	do
	{
		printf("Unesite vas broj telefona (10 cifara): ");
		fgets(user.brojTelefona, sizeof(user.brojTelefona), stdin);
		user.brojTelefona[strcspn(user.brojTelefona, "\n")] = '\0';
		if (strlen(user.brojTelefona) != 10 || string_sadrzi_slova_i_specijalne_karaktere(user.brojTelefona))
		{
			ocisti_ekran();
			printf("Kreirajte nalog popunjavanjem svih podataka, svi podaci su obavezni: \n\n");
			printf("JMBG: %s\n", user.jmbg);
			printf("Ime: %s\n", user.imeKorisnika);
			printf("Prezime: %s\n", user.prezimeKorisnika);
			printf("Email: %s\n", user.email);
			printf("Greska pri unosu, broj telefona mora imati 10 cifara, i ne sme sadrzati slova  i specijalne karaktere\n");
		}
		else if (strlen(user.brojTelefona) == 10 && !string_sadrzi_slova_i_specijalne_karaktere(user.brojTelefona))
		{
			break;
		}
	} while (1);

	do
	{
		printf("Unesite sifru: ");
		char* input = citaj_unos();
		input[strcspn(input, "\n")] = '\0';
		if (strlen(input) < 10 || strlen(input) > 1023 || !string_sadrzi_slova_i_specijalne_karaktere(input))
		{
			ocisti_ekran();
			printf("Kreirajte nalog popunjavanjem svih podataka, svi podaci su obavezni: \n\n");
			printf("JMBG: %s\n", user.jmbg);
			printf("Ime: %s\n", user.imeKorisnika);
			printf("Prezime: %s\n", user.prezimeKorisnika);
			printf("Email: %s\n", user.email);
			printf("Broj telefona: %s\n", user.brojTelefona);
			printf("Greska pri unosu, sifra mora imati minimum duzinu 10, a maksimum 1024, i mora sadrzati bar jedno slovo sadrzati ili specijalni karakter\n");
		}
		else if (strlen(input) >= 10 && strlen(input) < 1024 && string_sadrzi_slova_i_specijalne_karaktere(input))
		{
			strcpy(user.sifra, input);
			break;
		}
	} while (1);

	// Ispis unetih vrednosti radi provere
	printf("\nRegistracija uspesna, vasi podaci:\n");
	printf("JMBG: %s\n", user.jmbg);
	printf("Ime: %s\n", user.imeKorisnika);
	printf("Prezime: %s\n", user.prezimeKorisnika);
	printf("Email: %s\n", user.email);
	printf("Broj telefona: %s\n", user.brojTelefona);
	printf("Sifra: %s\n", user.sifra);
	printf("Vas tip korisnika: %s\n", user.tipKorisnika);
	printf("\nKreiranje naloga, molimo sacekajte...\n");
	Sleep(3000); //Simulacija kreiranja naloga, moze da se obrise zbog performansi, cisto je tu zbog izgleda :)
	printf("Nalog uspesno kreiran, redirektovanje na pocetnu stranicu...");
	Sleep(2000);
	ocisti_ekran();
	korisnicki_ekran();
}

#pragma endregion

int main()
{
	pocetni_ekran();
	return 0;
}