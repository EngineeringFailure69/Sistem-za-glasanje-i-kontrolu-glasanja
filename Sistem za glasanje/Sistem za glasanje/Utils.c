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
	if (!korisnik_vec_postoji(user, false))
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

bool korisnik_vec_postoji(User korisnik_koji_se_registruje, bool prijavljivanje)
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
		if (prijavljivanje)
		{
			if (strcmp(korisnik_koji_se_registruje.jmbg, user.jmbg) == 0 && strcmp(korisnik_koji_se_registruje.imeKorisnika, user.imeKorisnika) == 0
				&& strcmp(korisnik_koji_se_registruje.prezimeKorisnika, user.prezimeKorisnika) == 0 && strcmp(korisnik_koji_se_registruje.sifra, user.sifra) == 0
				&& strcmp(korisnik_koji_se_registruje.email, user.email) == 0 && strcmp(korisnik_koji_se_registruje.brojTelefona, user.brojTelefona) == 0 &&
				strcmp(korisnik_koji_se_registruje.glasackiBroj, user.glasackiBroj) == 0) 
			{
				fclose(fajl);
				return true;
			}
			else if (strcmp(korisnik_koji_se_registruje.jmbg, user.jmbg) != 0) 
			{
				fclose(fajl);
				printf("Uneli ste pogresan jmbg\n");
				return false;
			}
			else if (strcmp(korisnik_koji_se_registruje.imeKorisnika, user.imeKorisnika) != 0)
			{
				fclose(fajl);
				printf("Uneli ste pogresano ime\n");
				return false;
			}
			else if (strcmp(korisnik_koji_se_registruje.prezimeKorisnika, user.prezimeKorisnika) != 0)
			{
				fclose(fajl);
				printf("Uneli ste pogresano prezime\n");
				return false;
			}
			else if (strcmp(korisnik_koji_se_registruje.sifra, user.sifra) != 0)
			{
				fclose(fajl);
				printf("Uneli ste pogresnu sifru\n");
				return false;
			}
			else if (strcmp(korisnik_koji_se_registruje.email, user.email) != 0)
			{
				fclose(fajl);
				printf("Uneli ste pogresan email\n");
				return false;
			}
			else if (strcmp(korisnik_koji_se_registruje.brojTelefona, user.brojTelefona) != 0)
			{
				fclose(fajl);
				printf("Uneli ste pogresan broj telefona\n");
				return false;
			}
			else if (strcmp(korisnik_koji_se_registruje.glasackiBroj, user.glasackiBroj) != 0)
			{
				fclose(fajl);
				printf("Uneli ste pogresan glasacki broj\n");
				return false;
			}
		}
		else 
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
	}
	fclose(fajl);
	return false;
}

void unesi_podatke(User* user) 
{
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
	user->glasao = false;
	
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
			printf("Svi podaci su obavezni: \n\n");
			printf("JMBG: %s\n", user->jmbg);
			printf("Greska pri unosu, ime mora imati od 1 do 14 karaktera i ne sme sadrzati brojeve i specijalne karaktere\n");
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
			printf("Svi podaci su obavezni: \n\n");
			printf("JMBG: %s\n", user->jmbg);
			printf("Ime: %s\n", user->imeKorisnika);
			printf("Greska pri unosu, prezime mora imati od 1 do 29 karaktera i ne sme sadrzati brojeve i specijalne karaktere\n");
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
			printf("Svi podaci su obavezni: \n\n");
			printf("JMBG: %s\n", user->jmbg);
			printf("Ime: %s\n", user->imeKorisnika);
			printf("Prezime: %s\n", user->prezimeKorisnika);
			printf("Greska pri unosu, email mora imati od 1 do 49 karaktera, i mora biti u formatu primer@gmail.com\n");
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
			printf("Svi podaci su obavezni: \n\n");
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
			printf("Svi podaci su obavezni: \n\n");
			printf("JMBG: %s\n", user->jmbg);
			printf("Ime: %s\n", user->imeKorisnika);
			printf("Prezime: %s\n", user->prezimeKorisnika);
			printf("Email: %s\n", user->email);
			printf("Broj telefona: %s\n", user->brojTelefona);
			printf("Greska pri unosu, sifra mora imati minimum duzinu 10, a maksimum 1023, i mora sadrzati bar jedno slovo sadrzati ili specijalni karakter\n");
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
			printf("Svi podaci su obavezni: \n\n");
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
}

void ocisti_podatke(User* user) 
{
	user->jmbg[0] = '\0';
	user->imeKorisnika[0] = '\0';
	user->prezimeKorisnika[0] = '\0';
	user->sifra[0] = '\0';
	user->email[0] = '\0';
	user->brojTelefona[0] = '\0';
	user->glasackiBroj[0] = '\0';
	user->tipKorisnika[0] = '\0';
}

SOCKET kreiraj_soket()
{
	WSADATA wsaData;
	SOCKET ServerSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL, hints;
	const char* imeServera = "127.0.0.1";   // ili IP/adresa tvog servera
	const char* portServera = "27017";       // ili port na kojem server slusa

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
	SOCKET ServerSocket = kreiraj_soket();
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
				return;
			}
			else
			{
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