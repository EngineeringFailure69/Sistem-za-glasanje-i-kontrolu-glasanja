#include "Include.h"

void kreiranje_naloga(Korisnik* korisnik)
{
	bool uspesno_zavrseno = false;
	while (!uspesno_zavrseno)
	{
		printf("Kreirajte nalog popunjavanjem svih podataka, svi podaci su obavezni: \n\n");
		ocisti_podatke(korisnik);
		unesi_podatke(korisnik);
		// Ispis unetih vrednosti radi provere
		Korisnik2 korisnik2;
		strncpy(korisnik2.jmbg, korisnik->jmbg, 13);
		korisnik2.jmbg[13] = '\0';
		strncpy(korisnik2.imeKorisnika, korisnik->imeKorisnika, strlen(korisnik->imeKorisnika));
		korisnik2.imeKorisnika[strlen(korisnik->imeKorisnika)] = '\0';
		strncpy(korisnik2.prezimeKorisnika, korisnik->prezimeKorisnika, strlen(korisnik->prezimeKorisnika));
		korisnik2.prezimeKorisnika[strlen(korisnik->prezimeKorisnika)] = '\0';
		strncpy(korisnik2.brojTelefona, korisnik->brojTelefona, 10);
		korisnik2.brojTelefona[10] = '\0';
		strncpy(korisnik2.glasackiBroj, korisnik->glasackiBroj, 6);
		korisnik2.glasackiBroj[6] = '\0';
		strncpy(korisnik2.email, korisnik->email, strlen(korisnik->email));
		korisnik2.email[strlen(korisnik->email)] = '\0';
		strncpy(korisnik2.sifra, korisnik->sifra, strlen(korisnik->sifra));
		korisnik2.sifra[strlen(korisnik->sifra)] = '\0';
		korisnik2.tipOperacije = kreiranjeNaloga;
		printf("\nProvera podataka, molimo sacekajte...\n\n");
		char registrovan_birac = proveri_da_li_je_korisnik_registrovan_ili_se_ulogujte(&korisnik2);
		if ((int)registrovan_birac == 1)
		{
			printf("\nRegistracija uspesna!\n");
			printf("\nKreiranje naloga, molimo sacekajte...\n");
			Sleep(3000); //Simulacija kreiranja naloga, moze da se obrise zbog performansi, cisto je tu zbog izgleda :)
			printf("Nalog uspesno kreiran, redirektovanje na pocetnu stranicu...\n");
			Sleep(2000);
			ocisti_ekran();
			korisnicki_ekran(korisnik);
			uspesno_zavrseno = true;
			return;
		}
		else if ((int)registrovan_birac == 2)
		{
			printf("Birac je vec registrovan, vracamo vas na pocetni ekran\n");
			Sleep(2000);
			ocisti_ekran();
			ocisti_podatke(korisnik);
			pocetni_ekran(korisnik);
			return;
		}
		else if ((int)registrovan_birac == 5) 
		{
			printf("Greska prilikom kreiranja naloga, zapoceta operacija nije prepoznata, molimo pokusajte opet\n");
			Sleep(2000);
			ocisti_ekran();
			ocisti_podatke(korisnik);
			continue;
		}
		else
		{
			printf("Greska prilikom kreiranja naloga, uneti podaci ne postoje kao registrovani, molimo pokusajte opet\n");
			Sleep(2000);
			ocisti_ekran();
			ocisti_podatke(korisnik);
			continue;
		}
	}
}

void prijavite_se(Korisnik* korisnik)
{
	bool uspesno_zavrseno = false;
	while (!uspesno_zavrseno)
	{
		printf("Prijavite se na vas nalog popunjavanjem svih podataka, svi podaci su obavezni: \n\n");
		ocisti_podatke(korisnik);
		unesi_podatke(korisnik);
		// Ispis unetih vrednosti radi provere
		Korisnik2 korisnik2;
		strncpy(korisnik2.jmbg, korisnik->jmbg, 13);
		korisnik2.jmbg[13] = '\0';
		strncpy(korisnik2.imeKorisnika, korisnik->imeKorisnika, strlen(korisnik->imeKorisnika));
		korisnik2.imeKorisnika[strlen(korisnik->imeKorisnika)] = '\0';
		strncpy(korisnik2.prezimeKorisnika, korisnik->prezimeKorisnika, strlen(korisnik->prezimeKorisnika));
		korisnik2.prezimeKorisnika[strlen(korisnik->prezimeKorisnika)] = '\0';
		strncpy(korisnik2.brojTelefona, korisnik->brojTelefona, 10);
		korisnik2.brojTelefona[10] = '\0';
		strncpy(korisnik2.glasackiBroj, korisnik->glasackiBroj, 6);
		korisnik2.glasackiBroj[6] = '\0';
		strncpy(korisnik2.email, korisnik->email, strlen(korisnik->email));
		korisnik2.email[strlen(korisnik->email)] = '\0';
		strncpy(korisnik2.sifra, korisnik->sifra, strlen(korisnik->sifra));
		korisnik2.sifra[strlen(korisnik->sifra)] = '\0';
		korisnik2.tipOperacije = prijavljivanjeNaNalog;
		char uspesno_logovanje = proveri_da_li_je_korisnik_registrovan_ili_se_ulogujte(&korisnik2);
		if ((int)uspesno_logovanje == 3)
		{
			printf("\nPrijavljivanje uspesno!\n");
			printf("Redirektovanje na pocetnu stranicu...\n");
			Sleep(2000);
			ocisti_ekran();
			korisnicki_ekran(korisnik);
			uspesno_zavrseno = true;
			return;
		}
		else if((int)uspesno_logovanje == 4)
		{
			printf("Greska prilikom prijavljivanja na vas nalog, uneti podaci ne postoje kao registrovani, molimo pokusajte opet\n");
			Sleep(2000);
			ocisti_ekran();
			ocisti_podatke(korisnik);
			continue;
		}
		else if ((int)uspesno_logovanje == 5)
		{
			printf("Greska prilikom kreiranja naloga, zapoceta operacija nije prepoznata, molimo pokusajte opet\n");
			Sleep(2000);
			ocisti_ekran();
			ocisti_podatke(korisnik);
			continue;
		}
	}
}

char proveri_da_li_je_korisnik_registrovan_ili_se_ulogujte(const Korisnik2* korisnik)
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL, * ptr = NULL, hints;
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
	iResult = getaddrinfo(SERVER_ADDRESS, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) 
	{
		printf("getaddrinfo neuspesan sa greskom: %d\n", iResult);
		WSACleanup();
		return 0;
	}

	// Pokusaj povezivanja 
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) 
	{
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) 
		{
			printf("socket neuspesan sa greskom: %ld\n", WSAGetLastError());
			WSACleanup();
			freeaddrinfo(result);
			return 0;
		}
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) 
		{
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) 
	{
		printf("Neuspesna konekcija na server!\n");
		WSACleanup();
		return 0;
	}

	// Saljem celu strukturu Korisnik2
	int total = 0;
	int expected = sizeof(Korisnik2);
	const char* bufptr = (const char*)korisnik;
	while (total < expected) 
	{
		iResult = send(ConnectSocket, bufptr + total, expected - total, 0);
		if (iResult == SOCKET_ERROR) 
		{
			printf("send neuspesan sa greskom: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 0;
		}
		total += iResult;
	}
	//printf("Poslata Korisnik2 struktura, ukupno bajtova: %d\n", total);

	// Signaliziram serveru da je slanje zavrseno
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) 
	{
		printf("shutdown neuspesan sa greskom: %d\n", WSAGetLastError());
	}

	// Cekamo i primamo odgovor: 1 bajt
	char resp = 0;
	total = 0;
	expected = 1; // ocekujem 1 bajt
	char* resp_ptr = &resp;
	while (total < expected) 
	{
		iResult = recv(ConnectSocket, resp_ptr + total, expected - total, 0);
		if (iResult > 0) 
		{
			total += iResult;
		}
		else if (iResult == 0) 
		{
			printf("Konekcija zatvorena od strane servera pre nego sto je poslao odgovor\n");
			break;
		}
		else 
		{
			printf("recv neuspesno sa greskom: %d\n", WSAGetLastError());
			break;
		}
	}

	closesocket(ConnectSocket);
	WSACleanup();

	if (total == 1) 
	{
		//printf("Primljen odgovor od servera: %d\n", (int)resp);//0 za gresku, 1 za ispravno, 2 za pokusaj ponovnog registrovanja, 
		// 3 za uspesan login, 4 za gresku kod login-a
		return resp;
	}
	else 
	{
		return 0;
	}
}