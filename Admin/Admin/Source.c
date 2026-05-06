#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define SERVER_ADDRESS "127.0.0.1"

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<stdbool.h>
#include<string.h>
#include<Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include<conio.h>

typedef struct
{
	char punNazivStranke[101];
	char skracenica[7];
	char imeLidera[16];
	char prezimeLidera[31];
	int redniBroj;
	int brojGlasova;
}Kandidat;

typedef enum 
{
	kreiranjeKandidata = 1,
	procitajSveKandidate = 2,
	imeIliPrezime = 3, 
	skracenica = 4, 
	nazivStranke = 5
}Izbor;

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
SOCKET kreiraj_soket(const char* port);
void citanje_svih_kandidata();
void vrati_se_nazad(Kandidat* kandidat);

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

		if (ch == EOF || ch == '\n')
			ch = 0;
		if (ch == 27) 
		{

			free(line);
			line = NULL;
			return line;
		}

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
	SOCKET ConnectSocket = kreiraj_soket("27016");
	if (ConnectSocket == INVALID_SOCKET) 
	{
		printf("Greska prilikom kreiranja soketa: %ld\n", WSAGetLastError());
		WSACleanup();
		return false;
	}
	int iResult;

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

	// Signalizacija serveru da je slanje zavrseno
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown neuspesan sa greskom: %d\n", WSAGetLastError());
	}

	// Cekam i primam odgovor: 1 bajt
	char resp = 0;
	total = 0;
	expected = 1; // ocekujem 1 bajt
	char* resp_ptr = &resp;
	while (total < expected) {
		iResult = recv(ConnectSocket, resp_ptr + total, expected - total, 0);
		if (iResult > 0) {
			total += iResult;
		}
		else if (iResult == 0) {
			printf("Konekcija zatvorena od strane servera pre nego sto je poslao odgovor\n");
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
		return false;
	}
}

char* formatiraj_string(char* s, int izbor)
{
	if (izbor == imeIliPrezime) //ime ili prezime
	{
		s[0] = toupper(s[0]);
		for (int i = 1; s[i] != '\0'; i++)
			s[i] = tolower(s[i]);
		return s;
	}
	else if (izbor == skracenica) //skracenica 
	{
		for (int i = 0; s[i] != '\0'; i++)
			s[i] = toupper(s[i]);
		return s;
	}
	else if (izbor == nazivStranke) //naziv stranke
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
		if (strlen(input) <= 0 || strlen(input) > 100 || string_sadrzi_brojeve_i_specijalne_karaktere(input, false))
		{
			ocisti_ekran();
			printf("Svi podaci su obavezni: \n\n");
			printf("Greska pri unosu, ime mora imati od 1 do 100 karaktera i ne sme sadrzati brojeve i specijalne karaktere\n");
			free(input);
			input = NULL;
		}
		else if (strlen(input) > 0 && strlen(input) <= 100 && !string_sadrzi_brojeve_i_specijalne_karaktere(input, false))
		{
			strncpy(kandidat->punNazivStranke, input, strlen(input));
			kandidat->punNazivStranke[strlen(input)] = '\0';
			formatiraj_string(kandidat->punNazivStranke, nazivStranke);
			free(input);
			input = NULL;
			break;
		}

	} while (1);

	do
	{
		printf("Unesite skracenicu stranke: ");
		char* input = citaj_unos();
		input[strcspn(input, "\r\n")] = '\0';
		if (strlen(input) <= 0 || strlen(input) > 6 || string_sadrzi_brojeve_i_specijalne_karaktere(input, false))
		{
			ocisti_ekran();
			printf("Svi podaci su obavezni: \n\n");
			printf("Naziv stranke: %s", kandidat->punNazivStranke);
			printf("\nGreska pri unosu, ime mora imati od 1 do 6 karaktera i ne sme sadrzati brojeve i specijalne karaktere\n");
			free(input);
			input = NULL;
		}
		else if (strlen(input) > 0 && strlen(input) <= 6 && !string_sadrzi_brojeve_i_specijalne_karaktere(input, false))
		{
			strncpy(kandidat->skracenica, input, strlen(input));
			kandidat->skracenica[strlen(input)] = '\0';
			formatiraj_string(kandidat->skracenica, skracenica);
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
		if (strlen(input) <= 0 || strlen(input) > 15 || string_sadrzi_brojeve_i_specijalne_karaktere(input, true))
		{
			ocisti_ekran();
			printf("Svi podaci su obavezni: \n\n");
			printf("Naziv stranke: %s\n", kandidat->punNazivStranke);
			printf("Skracenica: %s\n", kandidat->skracenica);
			printf("\nGreska pri unosu, ime mora imati od 1 do 15 karaktera i ne sme sadrzati brojeve i specijalne karaktere\n");
			free(input);
			input = NULL;
		}
		else if (strlen(input) > 0 && strlen(input) <= 15 && !string_sadrzi_brojeve_i_specijalne_karaktere(input, true))
		{
			strncpy(kandidat->imeLidera, input, strlen(input));
			kandidat->imeLidera[strlen(input)] = '\0';
			formatiraj_string(kandidat->imeLidera, imeIliPrezime);
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
		if (strlen(input) <= 0 || strlen(input) > 30 || string_sadrzi_brojeve_i_specijalne_karaktere(input, true))
		{
			ocisti_ekran();
			printf("Svi podaci su obavezni: \n\n");
			printf("Naziv stranke:  %s\n", kandidat->punNazivStranke);
			printf("Skracenica: %s\n", kandidat->skracenica);
			printf("Ime: %s\n", kandidat->imeLidera);
			printf("\nGreska pri unosu, prezime mora imati od 1 do 30 karaktera i ne sme sadrzati brojeve i specijalne karaktere\n");
			free(input);
			input = NULL;
		}
		else if (strlen(input) > 0 && strlen(input) <= 30 && !string_sadrzi_brojeve_i_specijalne_karaktere(input, true))
		{
			strncpy(kandidat->prezimeLidera, input, strlen(input));
			kandidat->prezimeLidera[strlen(input)] = '\0';
			formatiraj_string(kandidat->prezimeLidera, imeIliPrezime);
			free(input);
			input = NULL;
			break;
		}

	} while (1);

	printf("Unesite redni broj stranke na listi: ");
	int ret;
	do {
		ret = scanf_s("%d", &kandidat->redniBroj);
		if (ret != 1) 
		{
			ocisti_ekran();
			printf("Svi podaci su obavezni: \n\n");
			printf("Naziv stranke:  %s\n", kandidat->punNazivStranke);
			printf("Skracenica: %s\n", kandidat->skracenica);
			printf("Ime: %s\n", kandidat->imeLidera);
			printf("Prezime: %s\n", kandidat->prezimeLidera);
			printf("\nGreska: unos mora biti broj.\n");
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
			ocisti_podatke(kandidat);
			pocetni_ekran(kandidat);
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
		printf("Odaberite broj 1 ili 2 i pritisnite Enter: ");

		ret = scanf_s("%d", &izbor);
		if (ret != 1 || (izbor != kreiranjeKandidata && izbor != procitajSveKandidate)) {
			ocisti_ekran();
			nacrtaj_pocetni_ekran();
			printf("Greska: unos mora biti broj 1 ili 2\n");
			int c;
			while ((c = getchar()) != '\n' && c != EOF) {}
		}
	} while (ret != 1 || (izbor != kreiranjeKandidata && izbor != procitajSveKandidate));
	if (izbor == kreiranjeKandidata)
	{
		ocisti_ekran();
		kreiranje_kandidata(kandidat);
	}
	if (izbor == procitajSveKandidate) 
	{
		ocisti_ekran();
		printf("Svi kandidati: \n\n");
		citanje_svih_kandidata();
		vrati_se_nazad(kandidat);
	}
}

void nacrtaj_pocetni_ekran()
{
	printf("\t\tDobrodosli u elektronski sistem za glasanje i kontrolu glasanja u Republici Srbiji\n\n");
	printf("\t\t\t -----------------------------------------------\n");
	printf("\t\t\t|\t\t\t\t\t\t|\n\t\t\t| 1) Kreirajte kandidata  \t\t\t|\n\t\t\t|\t\t\t\t\t\t|\n");
	printf("\t\t\t|\t\t\t\t\t\t|\n\t\t\t|                                            \t|\n\t\t\t|\t\t\t\t\t\t|\n");
	printf("\t\t\t|\t\t\t\t\t\t|\n\t\t\t| 2) Procitajte sve kandidate \t\t\t|\n\t\t\t|\t\t\t\t\t\t|\n");
	printf("\t\t\t -----------------------------------------------\n\n");
}

SOCKET kreiraj_soket(const char* port) 
{
	WSADATA wsaData;
	SOCKET ServerSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL, hints;
	const char* imeServera = "127.0.0.1";   // ili IP/adresa servera
	const char* portServera = port;       // ili port na kojem server slusa

	// 1) Inicijalizacija Winsock-a
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		fprintf(stderr, "WSAStartup neuspesan\n");
		return INVALID_SOCKET;
	}

	// 2) Priprema getaddrinfo
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(imeServera, portServera, &hints, &result) != 0) {
		fprintf(stderr, "getaddrinfo neuspesan\n");
		WSACleanup();
		return INVALID_SOCKET;
	}

	// 3) Kreiranje socket-a
	ServerSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ServerSocket == INVALID_SOCKET) {
		fprintf(stderr, "socket neuspesan: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return INVALID_SOCKET;
	}

	// 4) Povezivanje na server
	if (connect(ServerSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
		fprintf(stderr, "connect neuspesan: %d\n", WSAGetLastError());
		closesocket(ServerSocket);
		freeaddrinfo(result);
		WSACleanup();
		return INVALID_SOCKET;
	}
	freeaddrinfo(result);

	return ServerSocket;
}

void citanje_svih_kandidata()
{
	//Primanje tacno sizeof(Kandidat) bajtova
	SOCKET ServerSocket = kreiraj_soket("27017");
	Kandidat primljeni;
	int total = 0;
	int expected = sizeof(Kandidat);
	char* bufptr;
	while (1)
	{
		total = 0;
		bufptr = (char*)&primljeni;
		while (total < expected)
		{
			int iResult = recv(ServerSocket, bufptr + total, expected - total, 0);
			if (iResult > 0)
			{
				total += iResult;
			}
			else if (iResult == 0)
			{
				closesocket(ServerSocket);
				WSACleanup();
				return;
			}
			else
			{
				closesocket(ServerSocket);
				WSACleanup();
				printf("recv neuspesan, greska: %d\n", WSAGetLastError());
				return;
			}
		}
		// Null-terminate polja
		primljeni.punNazivStranke[sizeof primljeni.punNazivStranke - 1] = '\0';
		primljeni.skracenica[sizeof primljeni.skracenica - 1] = '\0';
		primljeni.imeLidera[sizeof primljeni.imeLidera - 1] = '\0';
		primljeni.prezimeLidera[sizeof primljeni.prezimeLidera - 1] = '\0';

		printf("Naziv stranke: %s\n", primljeni.punNazivStranke);
		printf("Skracenica: %s\n", primljeni.skracenica);
		printf("Ime lidera: %s\n", primljeni.imeLidera);
		printf("Prezime lidera: %s\n", primljeni.prezimeLidera);
		printf("Redni broj: %d\n", primljeni.redniBroj);
		printf("Broj glasova: %d\n\n", primljeni.brojGlasova);
	}
	// 6) Zatvori konekciju i ocisti Winsock
	closesocket(ServerSocket);
	WSACleanup();
}

void vrati_se_nazad(Kandidat* kandidat)
{
	int vrati_se_nazad = 0, ret = 0;
	do
	{
		printf("Da bi se vratili na prethodnu stranicu, unesite 0 i pritisnite 'enter': ");
		ret = scanf_s("%d", &vrati_se_nazad);
		if (ret != 1 || vrati_se_nazad != 0)
		{
			ocisti_ekran();
			citanje_svih_kandidata();
			printf("Greska: unos mora biti broj 0\n");
			int c;
			while ((c = getchar()) != '\n' && c != EOF) {}
		}
	} while (vrati_se_nazad != 0 || ret != 1);
	if (vrati_se_nazad == 0)
	{
		ocisti_ekran();
		pocetni_ekran(kandidat);
		return;
	}
}

int main() 
{
	Kandidat kandidat;
	pocetni_ekran(&kandidat);
	return 0;
}