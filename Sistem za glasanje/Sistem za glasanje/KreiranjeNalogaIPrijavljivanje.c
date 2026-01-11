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
		kopiraj_strukture(korisnik, &korisnik2, kreiranjeNaloga);
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
		else if ((int)registrovan_birac == 3) 
		{
			printf("Greska prilikom kreiranja naloga, zapoceta operacija nije prepoznata, molimo pokusajte opet\n");
			Sleep(2000);
			ocisti_ekran();
			ocisti_podatke(korisnik);
			continue;
		}
		else if ((int)registrovan_birac == 4) 
		{
			printf("Greska prilikom kreiranja naloga, unet verifikacioni kod nije ispravan\n");
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
		kopiraj_strukture(korisnik, &korisnik2, prijavljivanjeNaNalog);
		printf("\nProvera podataka, molimo sacekajte...\n\n");
		char uspesno_logovanje = proveri_da_li_je_korisnik_registrovan_ili_se_ulogujte(&korisnik2);
		if ((int)uspesno_logovanje == 1)
		{
			printf("\nPrijavljivanje uspesno!\n");
			printf("Redirektovanje na pocetnu stranicu...\n");
			Sleep(2000);
			ocisti_ekran();
			korisnicki_ekran(korisnik);
			uspesno_zavrseno = true;
			return;
		}
		else if((int)uspesno_logovanje == 2)
		{
			printf("Greska prilikom prijavljivanja na vas nalog, uneti podaci ne postoje kao registrovani, molimo pokusajte opet\n");
			Sleep(2000);
			ocisti_ekran();
			ocisti_podatke(korisnik);
			continue;
		}
		else if ((int)uspesno_logovanje == 3)
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
	SOCKET serverSocket = kreiraj_soket(DEFAULT_PORT);
	int iResult;

	// Saljem celu strukturu Korisnik2
	int total = 0;
	int expected = sizeof(Korisnik2);
	const char* bufptr = (const char*)korisnik;
	while (total < expected) 
	{
		iResult = send(serverSocket, bufptr + total, expected - total, 0);
		if (iResult == SOCKET_ERROR) 
		{
			printf("send neuspesan sa greskom: %d\n", WSAGetLastError());
			closesocket(serverSocket);
			WSACleanup();
			return 0;
		}
		total += iResult;
	}
	//printf("Poslata Korisnik2 struktura, ukupno bajtova: %d\n", total);
	//Verifikacioni kod koji se salje klijentu
	if(korisnik->tipOperacije == kreiranjeNaloga) //Ovo se radi samo kada kreiramo nalog, jer kad se prijavljujemo na kreirani nalog, vec smo prosli verifikaciju
		uspesna_verifikacija_koriscenjem_email_koda(serverSocket);
	// Signaliziram serveru da je slanje zavrseno
	iResult = shutdown(serverSocket, SD_SEND);
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
		iResult = recv(serverSocket, resp_ptr + total, expected - total, 0);
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

	closesocket(serverSocket);
	WSACleanup();

	if (total == 1) 
	{
		//printf("Primljen odgovor od servera: %d\n", (int)resp);
		/*
		* Kodovi za povratni rezultat:
		* 
		* Za kreiranje naloga: 1 ako je uspesno registrovan nalog, 2 ako pokusava opet da kreira nalog, 3 ako je nepoznata operacija, 4 ako je 
		* pogresan verifikacioni kod, sve ostalo je greska ako podaci ne postoje kao registrovani
		* 
		* Za prijavljivanje na nalog: 1 uspesno, 2 greska prilikom unosa podataka, 3 ako je nepoznata operacija
		*/
		return resp;
	}
	else 
	{
		return 0;
	}
}