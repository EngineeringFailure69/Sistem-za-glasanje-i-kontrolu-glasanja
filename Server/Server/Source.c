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

// Definicija strukture User – ista na klijentu i serveru
typedef struct
{
    char jmbg[14];
    char imeKorisnika[15];
    char prezimeKorisnika[30];
    char brojTelefona[11];
    char glasackiBroj[7];
} User;

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

int __cdecl main(void)
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL;
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

    // 3. Resolve server adrese i port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo neuspesan sa greskom: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // 4. Socket za slusanje
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket neuspesan sa greskom: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // 5. Bind socket na port
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind neuspesan sa greskom: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    freeaddrinfo(result);

    // 6. Listen
    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen neuspesan sa greskom: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Provera na kom portu slusa server
    printf("Server slusa na portu %s.\n", DEFAULT_PORT);

    // 7. Petlja prihvatanja klijenata
    while (1) {
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept neuspesan sa greskom: %d\n", WSAGetLastError());
            // nastavak prihvatanja
            continue;
        }
        printf("Prihvacena konekcija od klijenta\n");

        // 8. Primanje tacno sizeof(User) bajtova
        User primljeni;
        int total = 0;
        int expected = sizeof(User);
        char* bufptr = (char*)&primljeni;
        while (total < expected) {
            iResult = recv(ClientSocket, bufptr + total, expected - total, 0);
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

            // 9.Provera iz fajla
            bool postoji = birac_postoji(primljeni);
            printf("Provera korisnika: %s\n", postoji ? "POSTOJI" : "NE POSTOJI");

            // 10. odgovor – jedan bajt
            char resp = postoji ? 1 : 0;

            // 11. Slanje jednog bajta
            int sent = 0;
            while (sent < 1) {
                iResult = send(ClientSocket, (const char*)&resp + sent, 1 - sent, 0);
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

        // 12. Zatvoranje konekcije sa tim klijentom
        closesocket(ClientSocket);
        printf("Konekcija sa klijentom zatvorena\n");
    }

    // 13. Cleanup
    closesocket(ListenSocket);
    WSACleanup();
    return 0;
}