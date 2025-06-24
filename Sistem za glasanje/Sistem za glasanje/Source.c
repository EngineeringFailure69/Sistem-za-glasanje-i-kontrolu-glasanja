#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<stdbool.h>
#include<string.h>
#include<Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Potrebno je linkovati Ws2_32.lib, Mswsock.lib, Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
#define SERVER_ADDRESS "127.0.0.1"

#pragma region strukture

typedef struct 
{
	int redniBroj;
	char nazivStranke[100];
	char liderStranke[50];
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
}User;

typedef struct
{
	char jmbg[14];
	char imeKorisnika[15];
	char prezimeKorisnika[30];
	char brojTelefona[11];
	char glasackiBroj[7];
}User2;

#pragma endregion

#pragma region prototipovi f-ja
void kreiranje_naloga(User* user);
int izbroj_cifre(int number);
void nacrtaj_korisnicki_ekran();
void nacrtaj_admin_ekran();
void nacrtaj_pocetni_ekran();
void ocisti_ekran();
void korisnicki_ekran();
void admin_ekran();
void pocetni_ekran(User* user);
void ocisti_input_buffer();
bool string_sadrzi_slova_i_specijalne_karaktere(const char* s);
bool string_sadrzi_brojeve_i_specijalne_karaktere(const char* s);
char* formatiraj_string(char* s);
char* citaj_unos();
bool upisi_podatke_u_fajl(User user);
void procitaj_podatke_iz_fajla(); //Vise je kao test funkcija
bool korisnik_vec_postoji(User korisnik_koji_se_registruje);
bool proveri_da_li_je_korisnik_registrovan(const User2* korisnik);
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

bool proveri_da_li_je_korisnik_registrovan(const User2* korisnik) {
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL, * ptr = NULL, hints;
	int iResult;

	// Inicijalizacija Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup neuspesan sa greskom: %d\n", iResult);
		return false;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Adresa servera i port
	iResult = getaddrinfo(SERVER_ADDRESS, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo neuspesan sa greskom: %d\n", iResult);
		WSACleanup();
		return false;
	}

	// Pokusaj povezivanja 
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket neuspesan sa greskom: %ld\n", WSAGetLastError());
			WSACleanup();
			return false;
		}
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Neuspesna konekcija na server!\n");
		WSACleanup();
		return false;
	}

	// Saljem celu strukturu User2
	int total = 0;
	int expected = sizeof(User2);
	const char* bufptr = (const char*)korisnik;
	while (total < expected) {
		iResult = send(ConnectSocket, bufptr + total, expected - total, 0);
		if (iResult == SOCKET_ERROR) {
			printf("send neuspesan sa greskom: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return false;
		}
		total += iResult;
	}
	//printf("Poslata User2 struct, ukupno bajtova: %d\n", total);

	// Signalizacija serveru da je slanje zavrseno (opciono)
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown neuspesan sa greskom: %d\n", WSAGetLastError());
		// Mozemo nastaviti da primamo odgovor i pamtimo da je greske, ali ovde ignorisemo
	}

	// Cekamo i primamo odgovor: 1 bajt
	char resp = 0;
	total = 0;
	expected = 1; // zelimo 1 bajt
	char* resp_ptr = &resp;
	while (total < expected) {
		iResult = recv(ConnectSocket, resp_ptr + total, expected - total, 0);
		if (iResult > 0) {
			total += iResult;
		}
		else if (iResult == 0) {
			printf("Konekcija zatvorena od strane servera pre nego što je poslao odgovor\n");
			break;
		}
		else {
			printf("recv neuspesno sa greskom: %d\n", WSAGetLastError());
			break;
		}
	}

	closesocket(ConnectSocket);
	WSACleanup();

	if (total == 1) {
		//printf("Primljen odgovor od servera: %d\n", (int)resp);
		return resp == 1;
	}
	else {
		// Nismo dobili ceo bajt odgovora
		return false;
	}
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

void pocetni_ekran(User* user)
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
		kreiranje_naloga(&user);
	}
	else if (izbor == 2)
	{
		printf("Tess");
	}
}

void kreiranje_naloga(User* user)
{
	printf("Kreirajte nalog popunjavanjem svih podataka, svi podaci su obavezni: \n\n");
	user->jmbg[0] = '\0';
	user->imeKorisnika[0] = '\0';
	user->prezimeKorisnika[0] = '\0';
	user->sifra[0] = '\0';
	user->email[0] = '\0';
	user->brojTelefona[0] = '\0';
	user->glasackiBroj[0] = '\0';
	user->tipKorisnika[0] = '\0';

	user->tipKorisnika[strcspn(user->tipKorisnika, "\r\n")] = '\0';
	strncpy(user->tipKorisnika, "user", 5);
	user->tipKorisnika[4] = '\0';

	do 
	{
		printf("Unesite vas JMBG (13 cifara): ");
		char* input = citaj_unos();
		input[strcspn(input, "\r\n")] = '\0';
		if (strlen(input) != 13 || string_sadrzi_slova_i_specijalne_karaktere(input))
		{
			ocisti_ekran();
			printf("Kreirajte nalog popunjavanjem svih podataka, svi podaci su obavezni: \n\n");
			printf("Greska pri unosu, jmbg mora imati 13 cifara, i ne sme sadrzati slova i specijalne karaktere\n");
			free(input);
			input = NULL;
		}
		else if(strlen(input) == 13 && !string_sadrzi_slova_i_specijalne_karaktere(input))
		{
			strncpy(user->jmbg, input, 13);
			user->jmbg[13] = '\0';
			free(input);
			input = NULL;
			break;
		}
	} while (1);

	do
	{
		printf("Unesite vase ime: ");
		char* input = citaj_unos();
		input[strcspn(input, "\r\n")] = '\0';
		if (strlen(input) <= 0 || strlen(input) > 14 || string_sadrzi_brojeve_i_specijalne_karaktere(input))
		{
			ocisti_ekran();
			printf("Kreirajte nalog popunjavanjem svih podataka, svi podaci su obavezni: \n\n");
			printf("JMBG: %s\n", user->jmbg);
			printf("Greska pri unosu, ime mora imati od 1 do 15 karaktera i ne sme sadrzati brojeve i specijalne karaktere\n");
			free(input);
			input = NULL;
		}
		else if (strlen(input) > 0 && strlen(input) <= 14 && !string_sadrzi_brojeve_i_specijalne_karaktere(input))
		{
			strncpy(user->imeKorisnika, input, strlen(input));
			user->imeKorisnika[strlen(input)] = '\0';
			formatiraj_string(user->imeKorisnika);
			free(input);
			input = NULL;
			break;
		}

	} while (1);

	do
	{
		printf("Unesite vase prezime: ");
		char* input = citaj_unos();
		input[strcspn(input, "\r\n")] = '\0';
		if (strlen(input) <= 0 || strlen(input) > 29 || string_sadrzi_brojeve_i_specijalne_karaktere(input))
		{
			ocisti_ekran();
			printf("Kreirajte nalog popunjavanjem svih podataka, svi podaci su obavezni: \n\n");
			printf("JMBG: %s\n", user->jmbg);
			printf("Ime: %s\n", user->imeKorisnika);
			printf("Greska pri unosu, prezime mora imati od 1 do 30 karaktera i ne sme sadrzati brojeve i specijalne karaktere\n");
			free(input);
			input = NULL;
		}
		else if (strlen(input) > 0 && strlen(input) <= 29 && !string_sadrzi_brojeve_i_specijalne_karaktere(input))
		{
			strncpy(user->prezimeKorisnika, input, strlen(input));
			user->prezimeKorisnika[strlen(input)] = '\0';
			formatiraj_string(user->prezimeKorisnika);
			free(input);
			input = NULL;
			break;
		}

	} while (1);

	do
	{
		printf("Unesite vas email: ");
		char* input = citaj_unos();
		input[strcspn(input, "\r\n")] = '\0';
		if (strlen(input) <= 0 || strlen(input) > 49 || !email_je_ispravno_formatiran(input))
		{
			ocisti_ekran();
			printf("Kreirajte nalog popunjavanjem svih podataka, svi podaci su obavezni: \n\n");
			printf("JMBG: %s\n", user->jmbg);
			printf("Ime: %s\n", user->imeKorisnika);
			printf("Prezime: %s\n", user->prezimeKorisnika);
			printf("Greska pri unosu, email mora imati od 1 do 50 karaktera, i mora biti u formatu primer@gmail.com\n");
			free(input);
			input = NULL;

		}
		else if (strlen(input) > 0 && strlen(input) <= 49 && email_je_ispravno_formatiran(input))
		{
			strncpy(user->email, input, strlen(input));
			user->email[strlen(input)] = '\0';
			formatiraj_string(user->email);
			free(input);
			input = NULL;
			break;
		}

	} while (1);

	do
	{
		printf("Unesite vas broj telefona (10 cifara): ");
		char* input = citaj_unos();
		input[strcspn(input, "\r\n")] = '\0';
		if (strlen(input) != 10 || string_sadrzi_slova_i_specijalne_karaktere(input))
		{
			ocisti_ekran();
			printf("Kreirajte nalog popunjavanjem svih podataka, svi podaci su obavezni: \n\n");
			printf("JMBG: %s\n", user->jmbg);
			printf("Ime: %s\n", user->imeKorisnika);
			printf("Prezime: %s\n", user->prezimeKorisnika);
			printf("Email: %s\n", user->email);
			printf("Greska pri unosu, broj telefona mora imati 10 cifara, i ne sme sadrzati slova  i specijalne karaktere\n");
			free(input);
			input = NULL;
		}
		else if (strlen(input) == 10 && !string_sadrzi_slova_i_specijalne_karaktere(input))
		{
			strncpy(user->brojTelefona, input, 10);
			user->brojTelefona[10] = '\0';
			free(input);
			input = NULL;
			break;
		}
	} while (1);

	do
	{
		printf("Unesite sifru: ");
		char* input = citaj_unos();
		input[strcspn(input, "\r\n")] = '\0';
		if (strlen(input) < 10 || strlen(input) > 1023 || !string_sadrzi_slova_i_specijalne_karaktere(input))
		{
			ocisti_ekran();
			printf("Kreirajte nalog popunjavanjem svih podataka, svi podaci su obavezni: \n\n");
			printf("JMBG: %s\n", user->jmbg);
			printf("Ime: %s\n", user->imeKorisnika);
			printf("Prezime: %s\n", user->prezimeKorisnika);
			printf("Email: %s\n", user->email);
			printf("Broj telefona: %s\n", user->brojTelefona);
			printf("Greska pri unosu, sifra mora imati minimum duzinu 10, a maksimum 1024, i mora sadrzati bar jedno slovo sadrzati ili specijalni karakter\n");
			free(input);
			input = NULL;
		}
		else if (strlen(input) >= 10 && strlen(input) <= 1023 && string_sadrzi_slova_i_specijalne_karaktere(input))
		{
			strncpy(user->sifra, input, strlen(input));
			user->sifra[strlen(input)] = '\0';
			free(input);
			input = NULL;
			break;
		}
	} while (1);

	do
	{
		printf("Unesite vas glasacki broj koji ste dobili na kucnu adresu (6 cifara): ");
		char* input = citaj_unos();
		input[strcspn(input, "\r\n")] = '\0';
		if (strlen(input) != 6 || string_sadrzi_slova_i_specijalne_karaktere(input))
		{
			ocisti_ekran();
			printf("Kreirajte nalog popunjavanjem svih podataka, svi podaci su obavezni: \n\n");
			printf("JMBG: %s\n", user->jmbg);
			printf("Ime: %s\n", user->imeKorisnika);
			printf("Prezime: %s\n", user->prezimeKorisnika);
			printf("Email: %s\n", user->email);
			printf("Broj telefona: %s\n", user->brojTelefona);
			printf("Greska pri unosu, glasacki broj mora imati duzinu 6 i nesme da sadrzi slova i specijalne karaktere\n");
			free(input);
			input = NULL;
		}
		else if (strlen(input) == 6 && !string_sadrzi_slova_i_specijalne_karaktere(input))
		{
			strncpy(user->glasackiBroj, input, 6);
			user->glasackiBroj[6] = '\0';
			free(input);
			input = NULL;
			break;
		}
	} while (1);

	// Ispis unetih vrednosti radi provere
	User2 user2;
	strncpy(user2.jmbg, user->jmbg, 13);
	user2.jmbg[13] = '\0';
	strncpy(user2.imeKorisnika, user->imeKorisnika, strlen(user->imeKorisnika));
	user2.imeKorisnika[strlen(user->imeKorisnika)] = '\0';
	strncpy(user2.prezimeKorisnika, user->prezimeKorisnika, strlen(user->prezimeKorisnika));
	user2.prezimeKorisnika[strlen(user->prezimeKorisnika)] = '\0';
	strncpy(user2.brojTelefona, user->brojTelefona, 10);
	user2.brojTelefona[10] = '\0';
	strncpy(user2.glasackiBroj, user->glasackiBroj, 6);
	user2.glasackiBroj[6] = '\0';
	printf("\nProvera podataka, molimo sacekajte...\n\n");
	bool registrovan_birac = proveri_da_li_je_korisnik_registrovan(&user2);
	if (registrovan_birac)
	{
		bool uspesna_registracija = upisi_podatke_u_fajl(*user);
		if (uspesna_registracija) 
		{
			printf("\nRegistracija uspesna, vasi podaci:\n");
			printf("JMBG: %s\n", user->jmbg);
			printf("Ime: %s\n", user->imeKorisnika);
			printf("Prezime: %s\n", user->prezimeKorisnika);
			printf("Email: %s\n", user->email);
			printf("Broj telefona: %s\n", user->brojTelefona);
			printf("Sifra: %s\n", user->sifra);
			printf("Vas glasacki broj: %s\n", user->glasackiBroj);
			printf("Vas tip korisnika: %s\n\n", user->tipKorisnika);
			printf("\nKreiranje naloga, molimo sacekajte...\n");
			Sleep(3000); //Simulacija kreiranja naloga, moze da se obrise zbog performansi, cisto je tu zbog izgleda :)
			printf("Nalog uspesno kreiran, redirektovanje na pocetnu stranicu...\n");
			Sleep(2000);
			ocisti_ekran();
			korisnicki_ekran();
		}
		else 
		{
			printf("Birac je vec registrovan\n");
			return;
		}
	}
	else 
	{
		printf("Greska prilikom kreiranja naloga, uneti podaci ne postoje kao registrovani, molimo pokusajte opet\n");
	}
}

#pragma endregion

int main()
{
	User user;
	pocetni_ekran(&user);
	return 0;
}