#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27016"
#define SERVER_ADDRESS "127.0.0.1"

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<stdbool.h>
#include<string.h>
#include<Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

typedef struct
{
	char punNazivStranke[100];
	char skracenica[6];
	char imeLidera[30];
	char prezimeLidera[30];
	int redniBroj;
	int brojGlasova;
}Kandidat;

void ocisti_ekran();
char* citaj_unos();
bool string_sadrzi_slova_i_specijalne_karaktere(const char* s);
bool string_sadrzi_brojeve_i_specijalne_karaktere(const char* s, bool imePrezime);
bool posalji_podatke_za_upis(const Kandidat* kandidat);
char* formatiraj_string(char* s, int izbor);
void unesi_podatke(Kandidat* kandidat);
void ocisti_podatke(Kandidat* kandidat);
void kreiranje_kandidata(Kandidat* kandidat);
void pocetni_ekran(Kandidat* kandidat);
void nacrtaj_pocetni_ekran();
void procitaj_podatke_iz_fajla();;

void ocisti_ekran()
{
#ifdef _WIN32
	system("cls");
#endif // _WIN32
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

bool string_sadrzi_slova_i_specijalne_karaktere(const char* s)
{
	for (int i = 0; s[i] != '\0'; i++)
	{
		if (s[i] < '0' || s[i] > '9')
			return true;
	}
	return false;
}

bool string_sadrzi_brojeve_i_specijalne_karaktere(const char* s, bool imePrezime) {
	for (int i = 0; i < strlen(s); i++)
	{
		if (!isalpha(s[i]) && imePrezime)
			return true;
		if (!isalpha(s[i]) && !imePrezime && s[i] != ' ')
			return true;

	}
	return false;
}

bool posalji_podatke_za_upis(const Kandidat* kandidat) {
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

	// Saljem celu strukturu Kandidat
	int total = 0;
	int expected = sizeof(Kandidat);
	const char* bufptr = (const char*)kandidat;
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
		return resp == 1;
	}
	else {
		// Nismo dobili ceo bajt odgovora
		return false;
	}
}

char* formatiraj_string(char* s, int izbor)
{
	if (izbor == 1) //ime ili prezime
	{
		s[0] = toupper(s[0]);
		for (int i = 1; s[i] != '\0'; i++)
			s[i] = tolower(s[i]);
		return s;
	}
	else if (izbor == 2) //skracenica 
	{
		for (int i = 0; s[i] != '\0'; i++)
			s[i] = toupper(s[i]);
		return s;
	}
	else if (izbor == 3) //naziv stranke
	{
		for (int i = 0; s[i] != '\0'; i++)
			if (i == 0 || s[i-1] == ' ')
				s[i] = toupper(s[i]);
		return s;
	}
	return s;
}

void unesi_podatke(Kandidat* kandidat)
{
	ocisti_podatke(kandidat);

	do
	{
		printf("Unesite ime stranke: ");
		char* input = citaj_unos();
		input[strcspn(input, "\r\n")] = '\0';
		if (strlen(input) <= 0 || strlen(input) > 99 || string_sadrzi_brojeve_i_specijalne_karaktere(input, false))
		{
			ocisti_ekran();
			printf("Svi podaci su obavezni: \n\n");
			printf("Greska pri unosu, ime mora imati od 1 do 99 karaktera i ne sme sadrzati brojeve i specijalne karaktere\n");
			free(input);
			input = NULL;
		}
		else if (strlen(input) > 0 && strlen(input) <= 99 && !string_sadrzi_brojeve_i_specijalne_karaktere(input, false))
		{
			strncpy(kandidat->punNazivStranke, input, strlen(input));
			kandidat->punNazivStranke[strlen(input)] = '\0';
			formatiraj_string(kandidat->punNazivStranke, 3);
			free(input);
			input = NULL;
			break;
		}

	} while (1);

	do
	{
		printf("Unesite skraenicu stranke: ");
		char* input = citaj_unos();
		input[strcspn(input, "\r\n")] = '\0';
		if (strlen(input) <= 0 || strlen(input) > 5 || string_sadrzi_brojeve_i_specijalne_karaktere(input, false))
		{
			ocisti_ekran();
			printf("Svi podaci su obavezni: \n\n");
			printf("Naziv stranke: %s", kandidat->punNazivStranke);
			printf("Greska pri unosu, ime mora imati od 1 do 5 karaktera i ne sme sadrzati brojeve i specijalne karaktere\n");
			free(input);
			input = NULL;
		}
		else if (strlen(input) > 0 && strlen(input) <= 5 && !string_sadrzi_brojeve_i_specijalne_karaktere(input, false))
		{
			strncpy(kandidat->skracenica, input, strlen(input));
			kandidat->skracenica[strlen(input)] = '\0';
			formatiraj_string(kandidat->skracenica, 2);
			free(input);
			input = NULL;
			break;
		}

	} while (1);

	do
	{
		printf("Unesite ime lidera stranke: ");
		char* input = citaj_unos();
		input[strcspn(input, "\r\n")] = '\0';
		if (strlen(input) <= 0 || strlen(input) > 29 || string_sadrzi_brojeve_i_specijalne_karaktere(input, true))
		{
			ocisti_ekran();
			printf("Svi podaci su obavezni: \n\n");
			printf("Naziv stranke: %s\n", kandidat->punNazivStranke);
			printf("Skracenica: %s\n", kandidat->skracenica);
			printf("Greska pri unosu, ime mora imati od 1 do 29 karaktera i ne sme sadrzati brojeve i specijalne karaktere\n");
			free(input);
			input = NULL;
		}
		else if (strlen(input) > 0 && strlen(input) <= 29 && !string_sadrzi_brojeve_i_specijalne_karaktere(input, true))
		{
			strncpy(kandidat->imeLidera, input, strlen(input));
			kandidat->imeLidera[strlen(input)] = '\0';
			formatiraj_string(kandidat->imeLidera, 1);
			free(input);
			input = NULL;
			break;
		}

	} while (1);

	do
	{
		printf("Unesite prezime lidera stranke: ");
		char* input = citaj_unos();
		input[strcspn(input, "\r\n")] = '\0';
		if (strlen(input) <= 0 || strlen(input) > 29 || string_sadrzi_brojeve_i_specijalne_karaktere(input, true))
		{
			ocisti_ekran();
			printf("Svi podaci su obavezni: \n\n");
			printf("Naziv stranke:  %s\n", kandidat->punNazivStranke);
			printf("Skracenica: %s\n", kandidat->skracenica);
			printf("Ime: %s\n", kandidat->imeLidera);
			printf("Greska pri unosu, prezime mora imati od 1 do 29 karaktera i ne sme sadrzati brojeve i specijalne karaktere\n");
			free(input);
			input = NULL;
		}
		else if (strlen(input) > 0 && strlen(input) <= 29 && !string_sadrzi_brojeve_i_specijalne_karaktere(input, true))
		{
			strncpy(kandidat->prezimeLidera, input, strlen(input));
			kandidat->prezimeLidera[strlen(input)] = '\0';
			formatiraj_string(kandidat->prezimeLidera, 1);
			free(input);
			input = NULL;
			break;
		}

	} while (1);

	printf("Unesite redni broj stranke na listi: ");
	int ret;
	do {
		ret = scanf_s("%d", &kandidat->redniBroj);
		if (ret != 1) {
			ocisti_ekran();
			printf("Svi podaci su obavezni: \n\n");
			printf("Naziv stranke:  %s\n", kandidat->punNazivStranke);
			printf("Skracenica: %s\n", kandidat->skracenica);
			printf("Ime: %s\n", kandidat->imeLidera);
			printf("Prezime: %s\n", kandidat->prezimeLidera);
			printf("Greska: unos mora biti broj.\n");
			// ocistimo ulazni bafer tako sto uklanjamo karaktere iz ulaza dok ne dodjemo do \n ili EOF
			int c;
			while ((c = getchar()) != '\n' && c != EOF) {}
		}
	} while (ret != 1);

	kandidat->brojGlasova = 0;
}

void ocisti_podatke(Kandidat* kandidat)
{
	kandidat->brojGlasova = 0;
	kandidat->redniBroj = 0;
	kandidat->imeLidera[0] = '\0';
	kandidat->punNazivStranke[0] = '\0';
	kandidat->skracenica[0] = '\0';
}

void kreiranje_kandidata(Kandidat* kandidat)
{
	bool uspesno_zavrseno = false;
	while (!uspesno_zavrseno)
	{
		printf("Dodajte kandidata popunjavanjem svih podataka, svi podaci su obavezni: \n\n");
		ocisti_podatke(kandidat);
		unesi_podatke(kandidat);
		bool registrovan_kandidat = posalji_podatke_za_upis(kandidat);
		if (registrovan_kandidat)
		{
			printf("\nRegistracija uspesna, podaci:\n");
			printf("Naziv stranke: %s\n", kandidat->punNazivStranke);
			printf("Skracenica: %s\n", kandidat->skracenica);
			printf("Ime lidera: %s\n", kandidat->imeLidera);
			printf("Prezime lidera: %s\n", kandidat->prezimeLidera);
			printf("Redni broj na listi: %d\n", kandidat->redniBroj);
			printf("Broj glasova: %d\n", kandidat->brojGlasova);
			printf("\nKreiranje kandidata, molimo sacekajte...\n");
			Sleep(3000); //Simulacija kreiranja kandidata, moze da se obrise zbog performansi, cisto je tu zbog izgleda :)
			printf("Kandidat uspesno kreiran, redirektovanje na pocetnu stranicu...\n");
			Sleep(2000);
			ocisti_ekran();
			uspesno_zavrseno = true;
			return;
		}
		else
		{
			printf("Greska prilikom kreiranja kandidata, proverite da li kandidat vec postoji, molimo pokusajte opet\n");
			Sleep(2000);
			ocisti_ekran();
			ocisti_podatke(kandidat);
			continue;
		}
	}
}

void pocetni_ekran(Kandidat* kandidat)
{
	int izbor, ret;
	nacrtaj_pocetni_ekran();
	do {
		printf("Odaberite broj 1 i pritisnite Enter: ");

		ret = scanf_s("%d", &izbor);
		if (ret != 1 || izbor != 1) {
			ocisti_ekran();
			nacrtaj_pocetni_ekran();
			printf("Greska: unos mora biti broj 1\n");
			// ocistimo ulazni bafer tako sto uklanjamo karaktere iz ulaza dok ne dodjemo do \n ili EOF
			int c;
			while ((c = getchar()) != '\n' && c != EOF) {}
		}
	} while (ret != 1 || izbor != 1);
	if (izbor == 1)
	{
		ocisti_ekran();
		kreiranje_kandidata(&kandidat);
	}
}

void nacrtaj_pocetni_ekran()
{
	printf("\t\tDobrodosli u elektronski sistem za glasanje i kontrolu glasanja u Republici Srbiji\n\n");
	printf("\t\t\t -----------------------------------------------\n");
	printf("\t\t\t|\t\t\t\t\t\t|\n\t\t\t| 1) Kreirajte kandidata  \t\t\t|\n\t\t\t|\t\t\t\t\t\t|\n");
	printf("\t\t\t -----------------------------------------------\n\n");
}

void procitaj_podatke_iz_fajla()
{
	Kandidat kandidat;
	FILE* fajl;
	fajl = fopen("registrovani_kandidati.bin", "rb");
	if (fajl == NULL)
	{
		fprintf(stderr, "Greska prilikom otvaranja fajla!\n");
		return;
	}
	while (fread(&kandidat, sizeof(kandidat), 1, fajl) == 1)
	{
		printf("Naziv stranke: %s\n", kandidat.punNazivStranke);
		printf("Skracenica: %s\n", kandidat.skracenica);
		printf("Ime lidera: %s\n", kandidat.imeLidera);
		printf("Prezime lidera: %s\n", kandidat.prezimeLidera);
		printf("Redni broj: %d\n", kandidat.redniBroj);
		printf("Broj glasova: %d\n", kandidat.brojGlasova);
		printf("\n");
	}
	fclose(fajl);
}

int main() 
{
	Kandidat kandidat;
	pocetni_ekran(&kandidat);
	return 0;
}