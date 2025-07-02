#include "Include.h"

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