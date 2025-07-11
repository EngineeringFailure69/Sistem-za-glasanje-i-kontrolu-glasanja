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

#define DEFAULT_PORT "27015"
#define DRUGI_PORT "27016"

// Definicija strukture User – ista na klijentu i serveru
typedef struct
{
    char jmbg[14];
    char imeKorisnika[15];
    char prezimeKorisnika[30];
    char brojTelefona[11];
    char glasackiBroj[7];
} User;

typedef struct 
{
    char punNazivStranke[100];
    char skracenica[6];
    char imeLidera[30];
    char prezimeLidera[30];
    int redniBroj;
    int brojGlasova;
}Kandidat;

bool birac_postoji(User korisnik_koji_se_registruje)
{
    User user;
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
            && strcmp(korisnik_koji_se_registruje.glasackiBroj, user.glasackiBroj) == 0)
        {
            fclose(fajl);
            return true;
        }
    }
    fclose(fajl);
    return false;
}

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

void obradi_usera(SOCKET ClientSocket) 
{
    // 1. Primanje tacno sizeof(User) bajtova
    User primljeni;
    int total = 0;
    int expected = sizeof(User);
    char* bufptr = (char*)&primljeni;
    while (total < expected) {
        int iResult = recv(ClientSocket, bufptr + total, expected - total, 0);
        if (iResult > 0) {
            total += iResult;
        }
        else if (iResult == 0) {
            // Klijent je zatvorio vezu prerano
            printf("Klijent je zatvorio vezu prerano (recv returned 0)\n");
            break;
        }
        else {
            printf("recv neuspesan sa greskom: %d\n", WSAGetLastError());
            break;
        }
    }

    printf("Server sizeof(User)=%zu\n", sizeof(User));
    printf("Server total=%d\n", total);


    if (total == expected) {
        // Null-terminate polja
        primljeni.jmbg[sizeof primljeni.jmbg - 1] = '\0';
        primljeni.imeKorisnika[sizeof primljeni.imeKorisnika - 1] = '\0';
        primljeni.prezimeKorisnika[sizeof primljeni.prezimeKorisnika - 1] = '\0';
        primljeni.brojTelefona[sizeof primljeni.brojTelefona - 1] = '\0';
        primljeni.glasackiBroj[sizeof primljeni.glasackiBroj - 1] = '\0';

        // 2.Provera iz fajla
        bool postoji = birac_postoji(primljeni);
        printf("Provera korisnika: %s\n", postoji ? "POSTOJI" : "NE POSTOJI");

        // 3. odgovor – jedan bajt
        char resp = postoji ? 1 : 0;

        // 4. Slanje jednog bajta
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
            // Ispis vrednosti koju saljem (0 ili 1)
            printf("Odgovor poslat klijentu: %d\n", (int)resp);
        }
    }

    // 5. Zatvoranje konekcije sa tim klijentom
    closesocket(ClientSocket);
    printf("Konekcija sa klijentom zatvorena\n");
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
    // 1. Primanje tacno sizeof(Kandidat) bajtova
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
            // Klijent je zatvorio vezu prerano
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
        // Null-terminate polja
        primljeni.punNazivStranke[sizeof primljeni.punNazivStranke - 1] = '\0';
        primljeni.skracenica[sizeof primljeni.skracenica - 1] = '\0';
        primljeni.imeLidera[sizeof primljeni.imeLidera - 1] = '\0';
        primljeni.prezimeLidera[sizeof primljeni.prezimeLidera - 1] = '\0';
        primljeni.brojGlasova = 0;

        // 2.Provera iz fajla
        bool upisan = upisi_podatke_u_fajl(primljeni);
        printf("Provera korisnika: %s\n", upisan ? "UPISAN" : "NIJE UPISAN");

        // 3. odgovor – jedan bajt
        char resp = upisan ? 1 : 0;

        // 4. Slanje jednog bajta
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
            // Ispis vrednosti koju saljem (0 ili 1)
            printf("Odgovor poslat klijentu: %d\n", (int)resp);
        }
    }

    // 5. Zatvoranje konekcije sa tim klijentom
    closesocket(ClientSocket);
    printf("Konekcija sa klijentom zatvorena\n");
}

int __cdecl main(void)
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket1 = INVALID_SOCKET;
    SOCKET ListenSocket2 = INVALID_SOCKET;

    struct addrinfo *result1 = NULL, *result2 = NULL;
    struct addrinfo hints;

    // 1. Inicijalizacija Winsock-a
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup neuspesan sa greskom: %d\n", iResult);
        return 1;
    }

    // 2. Hintovi za getaddrinfo
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;         // IPv4
    hints.ai_socktype = SOCK_STREAM;   // TCP
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;       // za bind

    //-------------------Port za user------------------------------------
    //Ovako se svaki put otvara drugi port za slanje podataka, ovo iznad uvek ostaje isto i radi se samo jednom

    // 3. Resolve server adrese i port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result1);
    if (iResult != 0) {
        printf("getaddrinfo neuspesan sa greskom: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // 4. Socket za slusanje
    ListenSocket1 = socket(result1->ai_family, result1->ai_socktype, result1->ai_protocol);
    if (ListenSocket1 == INVALID_SOCKET) {
        printf("socket1 user neuspesan sa greskom: %ld\n", WSAGetLastError());
        freeaddrinfo(result1);
        WSACleanup();
        return 1;
    }

    // 5. Bind socket na port
    iResult = bind(ListenSocket1, result1->ai_addr, (int)result1->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind1 neuspesan sa greskom: %d\n", WSAGetLastError());
        freeaddrinfo(result1);
        closesocket(ListenSocket1);
        WSACleanup();
        return 1;
    }
    freeaddrinfo(result1);

    // 6. Listen
    iResult = listen(ListenSocket1, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen neuspesan sa greskom: %d\n", WSAGetLastError());
        closesocket(ListenSocket1);
        WSACleanup();
        return 1;
    }

    // Provera na kom portu slusa server
    printf("Server slusa na portu %s.\n", DEFAULT_PORT);

    //----------------------------------Drugi port za admin-------------------------------------

    //1.
    iResult = getaddrinfo(NULL, DRUGI_PORT, &hints, &result2);
    if (iResult != 0) 
    {
        printf("getaddrinfo neuspesan sa greskom: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    //2.
    ListenSocket2 = socket(result2->ai_family, result2->ai_socktype, result2->ai_protocol);
    if (ListenSocket2 == INVALID_SOCKET) 
    {
        printf("socket2 user neuspesan sa greskom: %ld\n", WSAGetLastError());
        freeaddrinfo(result2);
        WSACleanup();
        return 1;
    }

    // 3. 
    iResult = bind(ListenSocket2, result2->ai_addr, (int)result2->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind2 neuspesan sa greskom: %d\n", WSAGetLastError());
        freeaddrinfo(result2);
        closesocket(ListenSocket2);
        WSACleanup();
        return 1;
    }
    freeaddrinfo(result2);

    //4.
    iResult = listen(ListenSocket2, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("liste2 neuspesan sa greskom: %d\n", WSAGetLastError());
        closesocket(ListenSocket2);
        WSACleanup();
        return 1;
    }

    // Provera na kom portu slusa server
    printf("Server slusa na portu %s.\n", DRUGI_PORT);

    //Ovako se obradjuje slanje i prijem podataka/odgovora 
    fd_set readSet;
    SOCKET maxSock = (ListenSocket1 > ListenSocket2 ? ListenSocket1 : ListenSocket2);

    while (1) {
        FD_ZERO(&readSet);
        FD_SET(ListenSocket1, &readSet);
        FD_SET(ListenSocket2, &readSet);

        int rc = select((int)maxSock + 1, &readSet, NULL, NULL, NULL);
        if (rc == SOCKET_ERROR) {
            printf("select neuspesan: %d\n", WSAGetLastError());
            break;
        }

        // nova konekcija na prvom portu
        if (FD_ISSET(ListenSocket1, &readSet)) {
            SOCKET ClientSocket = accept(ListenSocket1, NULL, NULL);
            if (ClientSocket != INVALID_SOCKET) {
                printf("Prihvacena konekcija na portu %s (user aplikacija)\n", DRUGI_PORT);
                obradi_usera(ClientSocket);
            }
        }
        // nova konekcija na drugom portu
        if (FD_ISSET(ListenSocket2, &readSet)) {
            SOCKET ClientSocket = accept(ListenSocket2, NULL, NULL);
            if (ClientSocket != INVALID_SOCKET) {
                printf("Prihvacena konekcija na portu %s (admin aplikacija)\n", DRUGI_PORT);
                //Ovde pozovite funkciju za obradu druge aplikacije,
                obradi_admina(ClientSocket);
                closesocket(ClientSocket);  // za sada samo zatvaramo
            }
        }
    }

    // 5. Cleanup
    closesocket(ListenSocket1);
    closesocket(ListenSocket2);
    WSACleanup();
    return 0;
}