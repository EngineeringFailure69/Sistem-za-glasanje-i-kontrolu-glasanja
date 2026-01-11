#define _CRT_SECURE_NO_WARNINGS

#include"Include.h"

bool string_sadrzi_slova_i_specijalne_karaktere(const char* s)
{
	for (int i = 0; s[i] != '\0'; i++)
	{
		if (s[i] < '0' || s[i] > '9')
			return true;
	}
	return false;
}

bool string_sadrzi_brojeve_i_specijalne_karaktere(const char* s) 
{
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
	while (ch) 
	{
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

void unesi_podatke(Korisnik* korisnik)
{
	ocisti_podatke(korisnik);
	
	do
	{
		printf("Unesite vas JMBG (13 cifara): ");
		char* input = citaj_unos();
		input[strcspn(input, "\r\n")] = '\0';
		if (strlen(input) != 13 || string_sadrzi_slova_i_specijalne_karaktere(input))
		{
			ocisti_ekran();
			printf("Svi podaci su obavezni: \n\n");
			printf("Greska pri unosu, jmbg mora imati 13 cifara, i ne sme sadrzati slova i specijalne karaktere\n");
			free(input);
			input = NULL;
		}
		else if (strlen(input) == 13 && !string_sadrzi_slova_i_specijalne_karaktere(input))
		{
			strncpy(korisnik->jmbg, input, 13);
			korisnik->jmbg[13] = '\0';
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
		if (strlen(input) <= 0 || strlen(input) > 15 || string_sadrzi_brojeve_i_specijalne_karaktere(input))
		{
			ocisti_ekran();
			printf("Svi podaci su obavezni: \n\n");
			printf("JMBG: %s\n", korisnik->jmbg);
			printf("Greska pri unosu, ime mora imati od 1 do 15 karaktera i ne sme sadrzati brojeve i specijalne karaktere\n");
			free(input);
			input = NULL;
		}
		else if (strlen(input) > 0 && strlen(input) <= 15 && !string_sadrzi_brojeve_i_specijalne_karaktere(input))
		{
			strncpy(korisnik->imeKorisnika, input, strlen(input));
			korisnik->imeKorisnika[strlen(input)] = '\0';
			formatiraj_string(korisnik->imeKorisnika);
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
		if (strlen(input) <= 0 || strlen(input) > 30 || string_sadrzi_brojeve_i_specijalne_karaktere(input))
		{
			ocisti_ekran();
			printf("Svi podaci su obavezni: \n\n");
			printf("JMBG: %s\n", korisnik->jmbg);
			printf("Ime: %s\n", korisnik->imeKorisnika);
			printf("Greska pri unosu, prezime mora imati od 1 do 30 karaktera i ne sme sadrzati brojeve i specijalne karaktere\n");
			free(input);
			input = NULL;
		}
		else if (strlen(input) > 0 && strlen(input) <= 30 && !string_sadrzi_brojeve_i_specijalne_karaktere(input))
		{
			strncpy(korisnik->prezimeKorisnika, input, strlen(input));
			korisnik->prezimeKorisnika[strlen(input)] = '\0';
			formatiraj_string(korisnik->prezimeKorisnika);
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
		if (strlen(input) <= 0 || strlen(input) > 50 || !email_je_ispravno_formatiran(input))
		{
			ocisti_ekran();
			printf("Svi podaci su obavezni: \n\n");
			printf("JMBG: %s\n", korisnik->jmbg);
			printf("Ime: %s\n", korisnik->imeKorisnika);
			printf("Prezime: %s\n", korisnik->prezimeKorisnika);
			printf("Greska pri unosu, email mora imati od 1 do 50 karaktera, i mora biti u formatu primer@gmail.com\n");
			free(input);
			input = NULL;
	
		}
		else if (strlen(input) > 0 && strlen(input) <= 50 && email_je_ispravno_formatiran(input))
		{
			strncpy(korisnik->email, input, strlen(input));
			korisnik->email[strlen(input)] = '\0';
			formatiraj_string(korisnik->email);
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
			printf("Svi podaci su obavezni: \n\n");
			printf("JMBG: %s\n", korisnik->jmbg);
			printf("Ime: %s\n", korisnik->imeKorisnika);
			printf("Prezime: %s\n", korisnik->prezimeKorisnika);
			printf("Email: %s\n", korisnik->email);
			printf("Greska pri unosu, broj telefona mora imati 10 cifara, i ne sme sadrzati slova  i specijalne karaktere\n");
			free(input);
			input = NULL;
		}
		else if (strlen(input) == 10 && !string_sadrzi_slova_i_specijalne_karaktere(input))
		{
			strncpy(korisnik->brojTelefona, input, 10);
			korisnik->brojTelefona[10] = '\0';
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
		if (strlen(input) < 10 || strlen(input) > 1024 || !string_sadrzi_slova_i_specijalne_karaktere(input))
		{
			ocisti_ekran();
			printf("Svi podaci su obavezni: \n\n");
			printf("JMBG: %s\n", korisnik->jmbg);
			printf("Ime: %s\n", korisnik->imeKorisnika);
			printf("Prezime: %s\n", korisnik->prezimeKorisnika);
			printf("Email: %s\n", korisnik->email);
			printf("Broj telefona: %s\n", korisnik->brojTelefona);
			printf("Greska pri unosu, sifra mora imati minimum duzinu 10, a maksimum 1023, i mora sadrzati bar jedno slovo sadrzati ili specijalni karakter\n");
			free(input);
			input = NULL;
		}
		else if (strlen(input) >= 10 && strlen(input) <= 1024 && string_sadrzi_slova_i_specijalne_karaktere(input))
		{
			strncpy(korisnik->sifra, input, strlen(input));
			korisnik->sifra[strlen(input)] = '\0';
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
			printf("Svi podaci su obavezni: \n\n");
			printf("JMBG: %s\n", korisnik->jmbg);
			printf("Ime: %s\n", korisnik->imeKorisnika);
			printf("Prezime: %s\n", korisnik->prezimeKorisnika);
			printf("Email: %s\n", korisnik->email);
			printf("Broj telefona: %s\n", korisnik->brojTelefona);
			printf("Greska pri unosu, glasacki broj mora imati duzinu 6 i nesme da sadrzi slova i specijalne karaktere\n");
			free(input);
			input = NULL;
		}
		else if (strlen(input) == 6 && !string_sadrzi_slova_i_specijalne_karaktere(input))
		{
			strncpy(korisnik->glasackiBroj, input, 6);
			korisnik->glasackiBroj[6] = '\0';
			free(input);
			input = NULL;
			break;
		}
	} while (1);
}

void ocisti_podatke(Korisnik* korisnik)
{
	korisnik->jmbg[0] = '\0';
	korisnik->imeKorisnika[0] = '\0';
	korisnik->prezimeKorisnika[0] = '\0';
	korisnik->sifra[0] = '\0';
	korisnik->email[0] = '\0';
	korisnik->brojTelefona[0] = '\0';
	korisnik->glasackiBroj[0] = '\0';
	korisnik->glasao = false;
}

SOCKET kreiraj_soket(char* portServera)
{
	WSADATA wsaData;
	SOCKET ServerSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL, hints, *ptr = NULL;
	const char* adresaServera = SERVER_ADDRESS;   // ili IP/adresa servera
	const char* portServeraZaKonektovanje = portServera;       // ili port na kojem server slusa
	int iResult;

	// Inicijalizacija Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup neuspesan sa greskom: %d\n", iResult);
		return 0;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Adresa servera i port
	iResult = getaddrinfo(adresaServera, portServeraZaKonektovanje, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo neuspesan sa greskom: %d\n", iResult);
		WSACleanup();
		return 0;
	}

	// Pokusaj povezivanja 
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		ServerSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ServerSocket == INVALID_SOCKET)
		{
			printf("socket neuspesan sa greskom: %ld\n", WSAGetLastError());
			WSACleanup();
			freeaddrinfo(result);
			return 0;
		}
		iResult = connect(ServerSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			closesocket(ServerSocket);
			ServerSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(result);

	if (ServerSocket == INVALID_SOCKET)
	{
		printf("Neuspesna konekcija na server!\n");
		WSACleanup();
		return 0;
	}

	return ServerSocket;
}

void citanje_svih_kandidata(int izbor)
{
	//Primanje tacno sizeof(Kandidat) bajtova
	char* portServera = "27017";
	SOCKET ServerSocket = kreiraj_soket(portServera);
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
				// server je pozvao shutdown(SD_SEND) ili zatvorio socket
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

		printf("Redni broj: %d\n", primljeni.redniBroj);
		printf("Naziv stranke: %s\n", primljeni.punNazivStranke);
		printf("Skracenica: %s\n", primljeni.skracenica);
		printf("Ime lidera: %s\n", primljeni.imeLidera);
		printf("Prezime lidera: %s\n", primljeni.prezimeLidera);
		if (izbor == 3) 
		{
			printf("Broj glasova: %d\n", primljeni.brojGlasova);
			float procenti = 0;
			procenti = ((float)primljeni.brojGlasova/100000.0) * 100.0;
			printf("Procenti glasova: %.2f\n", procenti);
		}
		printf("\n");
	}
	// 6) Zatvaram konekciju i cistim Winsock
	closesocket(ServerSocket);
	WSACleanup();
}

void vrati_se_nazad(Korisnik* korisnik)
{
	int vrati_se_nazad = 0, ret = 0;
	do
	{
		printf("Da bi se vratili na prethodnu stranicu, unesite 0 i pritisnite 'enter': ");
		ret = scanf_s("%d", &vrati_se_nazad);
		if (ret != 1 || vrati_se_nazad != 0)
		{
			ocisti_ekran();
			citanje_svih_kandidata(1);
			printf("Greska: unos mora biti broj 0\n");
			int c;
			while ((c = getchar()) != '\n' && c != EOF) {}
		}
	} while (vrati_se_nazad != 0 || ret != 1);
	if (vrati_se_nazad == 0)
	{
		ocisti_ekran();
		korisnicki_ekran(korisnik);
		return;
	}
}

void kopiraj_strukture(Korisnik* korisnik, Korisnik2* korisnik2, int tipOperacije)
{
	strncpy(korisnik2->jmbg, korisnik->jmbg, 13);
	korisnik2->jmbg[13] = '\0';
	strncpy(korisnik2->imeKorisnika, korisnik->imeKorisnika, strlen(korisnik->imeKorisnika));
	korisnik2->imeKorisnika[strlen(korisnik->imeKorisnika)] = '\0';
	strncpy(korisnik2->prezimeKorisnika, korisnik->prezimeKorisnika, strlen(korisnik->prezimeKorisnika));
	korisnik2->prezimeKorisnika[strlen(korisnik->prezimeKorisnika)] = '\0';
	strncpy(korisnik2->brojTelefona, korisnik->brojTelefona, 10);
	korisnik2->brojTelefona[10] = '\0';
	strncpy(korisnik2->glasackiBroj, korisnik->glasackiBroj, 6);
	korisnik2->glasackiBroj[6] = '\0';
	strncpy(korisnik2->email, korisnik->email, strlen(korisnik->email));
	korisnik2->email[strlen(korisnik->email)] = '\0';
	strncpy(korisnik2->sifra, korisnik->sifra, strlen(korisnik->sifra));
	korisnik2->sifra[strlen(korisnik->sifra)] = '\0';
	if (tipOperacije == kreiranjeNaloga)
		korisnik2->tipOperacije = kreiranjeNaloga;
	else if (tipOperacije == prijavljivanjeNaNalog)
		korisnik2->tipOperacije = prijavljivanjeNaNalog;
	else
		printf("Tip operacije nepoznat");
}

void uspesna_verifikacija_koriscenjem_email_koda(SOCKET serverSocket)
{
	int ret = 0, verifikacioniKod, brojacPokusaja = 0; //brojac pokusaja broji koliko puta je kod pogresno unet, i ako je to vece od 5, onda se ponistava kreiranje naloga
	do 
	{
		printf("\nUnesite verifikacioni kod koji ste dobili na vasu email adresu: ");
		ret = scanf_s("%d", &verifikacioniKod);
		if (ret != 1 || verifikacioniKod > 9999999) //9999999 je najveci sedmocifreni broj, mrzi me da pisem funkciju koja ce da broji cifre, pa cu ovako
		{
			ocisti_ekran();
			printf("Greska: unos mora biti broj, i mora biti manji od 9999999\n");
			int c;
			while ((c = getchar()) != '\n' && c != EOF) {}
		}
		brojacPokusaja++;

	} while ((ret!=1 || verifikacioniKod > 9999999) && brojacPokusaja < 5);

	int iResult;

	// Saljem kod
	int total = 0;
	int expected = sizeof(verifikacioniKod);
	const char* bufptr = (const char*)&verifikacioniKod;
	while (total < expected)
	{
		iResult = send(serverSocket, bufptr + total, expected - total, 0);
		if (iResult == SOCKET_ERROR)
		{
			printf("send neuspesan sa greskom: %d\n", WSAGetLastError());
			closesocket(serverSocket);
			WSACleanup();
			//return 0;
			break;
		}
		total += iResult;
	}
}