#undef UNICODE
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Potrebno je linkovati Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

// Definicija strukture User ista na klijentu i serveru
typedef struct
{
    char jmbg[14];
    char imeKorisnika[16];
    char prezimeKorisnika[31];
    char sifra[1025];
    char email[51];
    char brojTelefona[11];
    char glasackiBroj[7];
} Korisnik;

typedef struct 
{
    char jmbg[14];
    char imeKorisnika[16];
    char prezimeKorisnika[31];
    char brojTelefona[11];
    char glasackiBroj[7];
} Korisnik2;

typedef struct 
{
    char punNazivStranke[101];
    char skracenica[7];
    char imeLidera[16];
    char prezimeLidera[31];
    int redniBroj;
    int brojGlasova;
}Kandidat;

#pragma region Obradjivanje_biraca_klijent_dela

bool birac_postoji(Korisnik korisnik_koji_se_registruje)
{
    Korisnik2 user;
    FILE* fajl;
    bool ima = false;
    fajl = fopen("registrovani_biraci.bin", "rb");
    if (fajl == NULL)
    {
        return false;
    }
    while (fread(&user, sizeof(user), 1, fajl) == 1)
    {
        if (strcmp(korisnik_koji_se_registruje.jmbg, user.jmbg) == 0 && strcmp(korisnik_koji_se_registruje.brojTelefona, user.brojTelefona) == 0
            && strcmp(korisnik_koji_se_registruje.glasackiBroj, user.glasackiBroj) == 0 && strcmp(korisnik_koji_se_registruje.imeKorisnika, user.imeKorisnika) == 0
            && strcmp(korisnik_koji_se_registruje.prezimeKorisnika, user.prezimeKorisnika) == 0)
        {
            fclose(fajl);
            return true;
        }
    }
    fclose(fajl);
    return false;
}

bool korisnik_vec_postoji_kao_registrovan_nalog(Korisnik korisnik_koji_se_registruje, bool prijavljivanje)
{
    Korisnik korisnik;
    FILE* fajl;
    fajl = fopen("registrovani_korisnici.bin", "rb");
    if (fajl == NULL)
    {
        return false;
    }
    while (fread(&korisnik, sizeof(korisnik), 1, fajl) == 1)
    {
#pragma region Login_pogledaj_posle

        if (prijavljivanje)
        {
            if (strcmp(korisnik_koji_se_registruje.jmbg, korisnik.jmbg) == 0 && strcmp(korisnik_koji_se_registruje.imeKorisnika, korisnik.imeKorisnika) == 0
                && strcmp(korisnik_koji_se_registruje.prezimeKorisnika, korisnik.prezimeKorisnika) == 0 && strcmp(korisnik_koji_se_registruje.sifra, korisnik.sifra) == 0
                && strcmp(korisnik_koji_se_registruje.email, korisnik.email) == 0 && strcmp(korisnik_koji_se_registruje.brojTelefona, korisnik.brojTelefona) == 0 &&
                strcmp(korisnik_koji_se_registruje.glasackiBroj, korisnik.glasackiBroj) == 0)
            {
                fclose(fajl);
                return true;
            }
            else if (strcmp(korisnik_koji_se_registruje.jmbg, korisnik.jmbg) != 0)
            {
                fclose(fajl);
                printf("Uneli ste pogresan jmbg\n");
                return false;
            }
            else if (strcmp(korisnik_koji_se_registruje.imeKorisnika, korisnik.imeKorisnika) != 0)
            {
                fclose(fajl);
                printf("Uneli ste pogresano ime\n");
                return false;
            }
            else if (strcmp(korisnik_koji_se_registruje.prezimeKorisnika, korisnik.prezimeKorisnika) != 0)
            {
                fclose(fajl);
                printf("Uneli ste pogresano prezime\n");
                return false;
            }
            else if (strcmp(korisnik_koji_se_registruje.sifra, korisnik.sifra) != 0)
            {
                fclose(fajl);
                printf("Uneli ste pogresnu sifru\n");
                return false;
            }
            else if (strcmp(korisnik_koji_se_registruje.email, korisnik.email) != 0)
            {
                fclose(fajl);
                printf("Uneli ste pogresan email\n");
                return false;
            }
            else if (strcmp(korisnik_koji_se_registruje.brojTelefona, korisnik.brojTelefona) != 0)
            {
                fclose(fajl);
                printf("Uneli ste pogresan broj telefona\n");
                return false;
            }
            else if (strcmp(korisnik_koji_se_registruje.glasackiBroj, korisnik.glasackiBroj) != 0)
            {
                fclose(fajl);
                printf("Uneli ste pogresan glasacki broj\n");
                return false;
            }
        }
#pragma endregion
        else
        {
            if (strcmp(korisnik_koji_se_registruje.jmbg, korisnik.jmbg) == 0)
            {
                fclose(fajl);
                return true;
            }
            if (strcmp(korisnik_koji_se_registruje.brojTelefona, korisnik.brojTelefona) == 0)
            {
                fclose(fajl);
                return true;
            }
            if (strcmp(korisnik_koji_se_registruje.glasackiBroj, korisnik.glasackiBroj) == 0)
            {
                fclose(fajl);
                return true;
            }
            if (strcmp(korisnik_koji_se_registruje.email, korisnik.email) == 0)
            {
                fclose(fajl);
                return true;
            }

        }
    }
    fclose(fajl);
    return false;
}

bool upisi_podatke_u_fajl_birac(Korisnik korisnik)
{
    int upisano = 0;
    if (!korisnik_vec_postoji_kao_registrovan_nalog(korisnik, false))
    {
        FILE* fajl;
        fajl = fopen("registrovani_korisnici.bin", "ab");
        if (fajl == NULL)
        {
            return false;
        }
        upisano = fwrite(&korisnik, sizeof(Korisnik), 1, fajl);
        fclose(fajl);
    }
    else
        printf("Korisnik vec postoji u bazi, ne mozete da se registrujete dva puta kao isti korisnik!\n");

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

void obradi_usera(SOCKET ClientSocket)
{
    //Primanje tacno sizeof(User) bajtova
    Korisnik primljeni;
    int total = 0;
    int expected = sizeof(Korisnik);
    char* bufptr = (char*)&primljeni;
    while (total < expected) {
        int iResult = recv(ClientSocket, bufptr + total, expected - total, 0);
        if (iResult > 0) {
            total += iResult;
        }
        else if (iResult == 0) {
            //Klijent je zatvorio vezu prerano
            printf("Klijent je zatvorio vezu prerano (recv returned 0)\n");
            break;
        }
        else {
            printf("recv neuspesan sa greskom: %d\n", WSAGetLastError());
            break;
        }
    }

    printf("Server sizeof(User)=%zu\n", sizeof(Korisnik));
    printf("Server total=%d\n", total);


    if (total == expected) {
        //Null-terminate polja
        primljeni.jmbg[sizeof primljeni.jmbg - 1] = '\0';
        primljeni.imeKorisnika[sizeof primljeni.imeKorisnika - 1] = '\0';
        primljeni.prezimeKorisnika[sizeof primljeni.prezimeKorisnika - 1] = '\0';
        primljeni.brojTelefona[sizeof primljeni.brojTelefona - 1] = '\0';
        primljeni.glasackiBroj[sizeof primljeni.glasackiBroj - 1] = '\0';
        primljeni.sifra[sizeof primljeni.sifra - 1] == '\0';
        primljeni.email[sizeof primljeni.email - 1] == '\0';

        //Provera iz fajla registrovani_biraci
        bool postoji = birac_postoji(primljeni);
        printf("Provera korisnika: %s\n", postoji ? "POSTOJI" : "NE POSTOJI");

        //Odgovor jedan bajt
        char resp;
        //Biram koji odgovor saljem
        if (postoji) //ako birac postoji proveravam da li je vec kreirao nalog
        {
            bool uspesna_registracija = upisi_podatke_u_fajl_birac(primljeni);
            resp = uspesna_registracija ? 1 : 2; // ako je uspesno registrovan nalog, i birac je na spisku, vracam 1, ako je birac vec 
            //kreirao nalog, pa pokusava opet, onda vracam 2
        }
        else //ako birac ne postoji na spisku kao registrovan, jednostavno vracam 0 kao i do sada 
        {
            resp = 0;
        }

        //Slanje jednog bajta
        int sent = 0;
        while (sent < 1) {
            int iResult = send(ClientSocket, (const char*)&resp + sent, 1 - sent, 0);
            if (iResult == SOCKET_ERROR) {
                printf("send neuspesan sa greskom: %d\n", WSAGetLastError());
                break;
            }
            sent += iResult;
        }
        if (sent == 1) {
            // Ispis vrednosti koju saljem (0 ili 1 ili 2)
            printf("Odgovor poslat klijentu: %d\n", (int)resp);
        }
    }

    //Zatvoranje konekcije sa tim klijentom
    closesocket(ClientSocket);
    printf("Konekcija sa klijentom zatvorena\n");
}

#pragma endregion

#pragma region Obradjivanje_kandidata_admin_dela

bool kandidat_je_vec_upisan(Kandidat kandidat_koji_se_registruje) 
{
    Kandidat kandidat;
    FILE* fajl;
    bool ima = false;
    fajl = fopen("registrovani_kandidati.bin", "rb");
    if (fajl == NULL)
    {
        return false;
    }
    while (fread(&kandidat, sizeof(kandidat), 1, fajl) == 1)
    {
        if (strcmp(kandidat_koji_se_registruje.punNazivStranke, kandidat.punNazivStranke) == 0
                || kandidat_koji_se_registruje.redniBroj == kandidat.redniBroj)
        {
            fclose(fajl);
            return true;
        }
    }
    fclose(fajl);
    return false;
}

bool upisi_podatke_u_fajl(Kandidat kandidat)
{
    int upisano = 0;
    if (!kandidat_je_vec_upisan(kandidat))
    {
        FILE* fajl;
        fajl = fopen("registrovani_kandidati.bin", "ab");
        if (fajl == NULL)
        {
            return false;
        }
        upisano = fwrite(&kandidat, sizeof(Kandidat), 1, fajl);
        fclose(fajl);
    }
    else
        printf("Kandidat vec postoji u bazi, ne mozete da se registrujete dva puta kao isti kandidat!\n\n");

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

void obradi_admina(SOCKET ClientSocket)
{
    //Primanje tacno sizeof(Kandidat) bajtova
    Kandidat primljeni;
    int total = 0;
    int expected = sizeof(Kandidat);
    char* bufptr = (char*)&primljeni;
    while (total < expected) {
        int iResult = recv(ClientSocket, bufptr + total, expected - total, 0);
        if (iResult > 0) {
            total += iResult;
        }
        else if (iResult == 0) {
            //Klijent je zatvorio vezu prerano
            printf("Klijent je zatvorio vezu prerano (recv returned 0)\n");
            break;
        }
        else {
            printf("recv neuspesan sa greskom: %d\n", WSAGetLastError());
            break;
        }
    }

    printf("Server sizeof(Kandidat)=%zu\n", sizeof(Kandidat));
    printf("Server total=%d\n", total);


    if (total == expected) {
        //Null-terminate polja
        primljeni.punNazivStranke[sizeof primljeni.punNazivStranke - 1] = '\0';
        primljeni.skracenica[sizeof primljeni.skracenica - 1] = '\0';
        primljeni.imeLidera[sizeof primljeni.imeLidera - 1] = '\0';
        primljeni.prezimeLidera[sizeof primljeni.prezimeLidera - 1] = '\0';
        primljeni.brojGlasova = 0;

        //Provera iz fajla
        bool upisan = upisi_podatke_u_fajl(primljeni);
        printf("Provera kandidata: %s\n", upisan ? "UPISAN" : "NIJE UPISAN");

        //Odgovor jedan bajt
        char resp = upisan ? 1 : 0;

        //Slanje jednog bajta
        int sent = 0;
        while (sent < 1) {
            int iResult = send(ClientSocket, (const char*)&resp + sent, 1 - sent, 0);
            if (iResult == SOCKET_ERROR) {
                printf("send neuspesan sa greskom: %d\n", WSAGetLastError());
                break;
            }
            sent += iResult;
        }
        if (sent == 1) {
            //Ispis vrednosti koju saljem (0 ili 1)
            printf("Odgovor poslat klijentu: %d\n", (int)resp);
        }
    }

    //Zatvoranje konekcije sa tim klijentom
    closesocket(ClientSocket);
    printf("Konekcija sa klijentom zatvorena\n");
}

bool salji_sve_kandidate(SOCKET ConnectSocket)
{
    Kandidat kandidat;
    FILE* fajl;
    fajl = fopen("registrovani_kandidati.bin", "rb");
    if (fajl == NULL)
    {
        fprintf(stderr, "Greska prilikom otvaranja fajla!\n");
        return false;
    }
    while (fread(&kandidat, sizeof(kandidat), 1, fajl) == 1)
    {
        // Saljem celu strukturu Kandidat
        int sent = 0;
        int to_send = sizeof(Kandidat);
        const char* bufptr = (const char*)&kandidat;
        while (sent < to_send) 
        {
            int iResult = send(ConnectSocket, bufptr + sent, to_send - sent, 0);
            if (iResult == SOCKET_ERROR) 
            {
                printf("send neuspesan sa greskom: %d\n", WSAGetLastError());
                fclose(fajl);
                return false;
            }
            sent += iResult;
        }
    }
    fclose(fajl);
    // signal kraj podataka
    if (shutdown(ConnectSocket, SD_SEND) == SOCKET_ERROR)
        fprintf(stderr, "shutdown failed: %d\n", WSAGetLastError());
    return true;
}

#pragma endregion

SOCKET kreiraj_soket(const char* port) 
{
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    // 2. Hintovi za getaddrinfo
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;         // IPv4
    hints.ai_socktype = SOCK_STREAM;   // TCP
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;       // za bind

    //Ovako se svaki put otvara drugi port za slanje podataka, ovo iznad uvek ostaje isto i radi se samo jednom

    // 3. Resolve server adrese i port
    iResult = getaddrinfo(NULL, port, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo neuspesan sa greskom: %d\n", iResult);
        WSACleanup();
        return INVALID_SOCKET;
    }

    // 4. Socket za slusanje
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket1 user neuspesan sa greskom: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return INVALID_SOCKET;
    }

    // 5. Bind socket na port
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind1 neuspesan sa greskom: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }
    freeaddrinfo(result);

    // 6. Listen
    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen neuspesan sa greskom: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }

    // Provera na kom portu slusa server
    printf("Server slusa na portu %s.\n", port);
    return ListenSocket;
}
   
int __cdecl main(void)
{
    // 1. Inicijalizacija Winsock-a
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
    {
        fprintf(stderr, "WSAStartup neuspesan\n");
        return 1;
    }

    const char* portovi[] = {"27015", "27016", "27017"};

    SOCKET ListenSocket1 = kreiraj_soket(portovi[0]);
    SOCKET ListenSocket2 = kreiraj_soket(portovi[1]);
    SOCKET ListenSocket3 = kreiraj_soket(portovi[2]);
    if (ListenSocket1 == INVALID_SOCKET || ListenSocket2 == INVALID_SOCKET || ListenSocket3 == INVALID_SOCKET)
    {
        printf("Greska na nekom od soketa: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    //Ovako se obradjuje slanje i prijem podataka/odgovora 
    fd_set readSet;
    SOCKET maxSock = ListenSocket1;
    if (ListenSocket2 > maxSock) maxSock = ListenSocket2;
    if (ListenSocket3 > maxSock) maxSock = ListenSocket3;

    while (1) {
        FD_ZERO(&readSet);
        FD_SET(ListenSocket1, &readSet);
        FD_SET(ListenSocket2, &readSet);
        FD_SET(ListenSocket3, &readSet);

        int rc = select((int)maxSock + 1, &readSet, NULL, NULL, NULL);
        if (rc == SOCKET_ERROR) {
            printf("select neuspesan: %d\n", WSAGetLastError());
            break;
        }

        //Nova konekcija na prvom portu
        if (FD_ISSET(ListenSocket1, &readSet)) 
        {
            SOCKET ClientSocket = accept(ListenSocket1, NULL, NULL);
            if (ClientSocket != INVALID_SOCKET) 
            {
                printf("\nPrihvacena konekcija na portu %s (user aplikacija)\n", portovi[0]);
                obradi_usera(ClientSocket);
            }
        }
        //Nova konekcija na drugom portu
        if (FD_ISSET(ListenSocket2, &readSet)) 
        {
            SOCKET ClientSocket = accept(ListenSocket2, NULL, NULL);
            if (ClientSocket != INVALID_SOCKET) 
            {
                printf("\nPrihvacena konekcija na portu %s (admin aplikacija)\n", portovi[1]);
                obradi_admina(ClientSocket);
                closesocket(ClientSocket);  //Za sada samo zatvaram, moze da se obrise
            }
        }
        //Nova konekcija na trecem portu
        if (FD_ISSET(ListenSocket3, &readSet))
        {
            SOCKET ClientSocket = accept(ListenSocket3, NULL, NULL);
            if (ClientSocket != INVALID_SOCKET)
            {
                printf("\nPrihvacena konekcija na portu %s (admin ili user aplikacija)\n", portovi[2]);
                if(!salji_sve_kandidate(ClientSocket))
                    fprintf(stderr, "Greska pri slanju kandidata\n");
                closesocket(ClientSocket);
            }
        }
    }

    //Cistim sve
    closesocket(ListenSocket1);
    closesocket(ListenSocket2);
    closesocket(ListenSocket3);
    WSACleanup();
    return 0;
}