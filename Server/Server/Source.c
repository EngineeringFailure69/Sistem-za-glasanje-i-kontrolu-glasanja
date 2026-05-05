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
#include<curl/curl.h>

// Potrebno je linkovati Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

// Definicija strukture Korisnik ista na klijentu i serveru
typedef struct
{
    char jmbg[14];
    char imeKorisnika[16];
    char prezimeKorisnika[31];
    char sifra[1025];
    char email[51];
    char brojTelefona[11];
    char glasackiBroj[7];
    int tipOperacije;
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

typedef enum 
{
    kreiranjeNaloga = 1,
    prijavljivanjeNaNalog = 2, 
    glasanje = 3
}TipOperacije;

typedef enum 
{
    uspesnoGlasanje = 1,
    vecGlasao = 2,
    serverskaGreska = 3,
    kandidatNePostoji = 4
}KodoviGresaka;

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

bool uspesna_verifikacija_koriscenjem_koda_poslatog_na_email(SOCKET ClientSocket, int verifikacioniKod)
{
    int kod;
    int total = 0;
    int expected = sizeof(verifikacioniKod);
    char* bufptr = (char*)&kod;

    while (total < expected)
    {
        int iResult = recv(ClientSocket, bufptr + total, expected - total, 0);
        if (iResult > 0)
        {
            total += iResult;
        }
        else if (iResult == 0)
        {
            //Klijent je zatvorio vezu prerano
            printf("Klijent je zatvorio vezu prerano (recv returned 0)\n");
            break;
        }
        else
        {
            printf("recv neuspesan sa greskom: %d\n", WSAGetLastError());
            break;
        }
    }
    printf("Server generisan i poslat kod: %d i kod poslat sa klijenta: %d\n", verifikacioniKod, kod);
    if (total == expected)
    {
        if (kod == verifikacioniKod) 
        {
            printf("Verifikacioni kod je ispravan!\n");
            return true;
        }
        else 
        {
            printf("Greska, verifikacioni kod je pogresan");
            return false;
        }
    }
    return false;
}

bool posalji_email_i_proveri(const char* email, SOCKET ClientSocket)
{
    FILE* fajl = fopen("credentials.txt", "r");
    if (fajl == NULL)
    {
        fprintf(stderr, "Greska prilikom otvaranja fajla!\n");
        return false;
    }
    char email_fajl[50];
    char sifra_fajl[256];
    if (fgets(email_fajl, sizeof(email_fajl), fajl) != NULL)
        email_fajl[strcspn(email_fajl, "\r\n")] = '\0';
    else
    {
        fprintf(stderr, "Greska pri citanju email linije\n");
        fclose(fajl);
        return false;
    }

    if (fgets(sifra_fajl, sizeof(sifra_fajl), fajl) != NULL)
        sifra_fajl[strcspn(sifra_fajl, "\r\n")] = '\0';
    else
    {
        fprintf(stderr, "Greska pri citanju sifra linije\n");
        fclose(fajl);
        return false;
    }

    fclose(fajl);

    printf("Procitan email: %s\n", email_fajl);
    printf("Procitana sifra: %s\n", sifra_fajl);

    //Ovde generisem kod koji saljem 
    srand((unsigned)time(NULL));
    int verifikacioni_kod = rand() % 9000000 + 1000000;

    FILE* poruka = fopen("msg.txt", "w");
    if (poruka == NULL)
    {
        fprintf(stderr, "Greska prilikom otvaranja fajla!\n");
        return false;
    }

    //Poruka koja se salje se pamti u txt fajlu 
    fprintf(poruka,
        "From: \"<sistemzaglasanje@gmail.com>\"\r\n"
        "To: <%s>\r\n"
        "Subject: Verifikacioni kod za glasanje\r\n"
        "\r\n"
        "Vas kod je: %d\r\n",
        email,
        verifikacioni_kod
    );
    fclose(poruka);

    //Inicijalizacija libcurl 
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curl = curl_easy_init();
    if (!curl) return false;
    FILE* porukacitanje = fopen("msg.txt", "r");

    //Slanje email-a
    curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_USERNAME, email_fajl);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, sifra_fajl);
    char mail_from[128];
    snprintf(mail_from, sizeof(mail_from), "<%s>", email_fajl);
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, mail_from);
    struct curl_slist* rcpts = NULL;
    rcpts = curl_slist_append(rcpts, email);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, rcpts);
    curl_easy_setopt(curl, CURLOPT_READDATA, porukacitanje);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    CURLcode res = curl_easy_perform(curl);

    //Ciscenje
    fclose(porukacitanje);
    curl_slist_free_all(rcpts);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    remove("msg.txt");

    bool proveraVerifikacije = uspesna_verifikacija_koriscenjem_koda_poslatog_na_email(ClientSocket, verifikacioni_kod);
    if (proveraVerifikacije)
        return true;
    else
        return false;
}

int korisnik_glasao(SOCKET ClientSocket, Korisnik korisnik) 
{
    int brojListe;
    int total = 0;
    int expected = sizeof(int);
    char* bufptr = (char*)&brojListe;

    while (total < expected)
    {
        int iResult = recv(ClientSocket, bufptr + total, expected - total, 0);
        if (iResult > 0)
        {
            total += iResult;
        }
        else if (iResult == 0)
        {
            //Klijent je zatvorio vezu prerano
            printf("Klijent je zatvorio vezu prerano (recv returned 0)\n");
            break;
        }
        else
        {
            printf("recv neuspesan sa greskom: %d\n", WSAGetLastError());
            break;
        }
    }
    if (total == expected)
    {
        bool kandidatPostoji = false;
        Kandidat kandidat;
        FILE* fajl;
        fajl = fopen("registrovani_kandidati.bin", "rb+");
        if (fajl == NULL)
        {
            fprintf(stderr, "Greska prilikom otvaranja fajla!\n");
            return serverskaGreska;
        }
        while (fread(&kandidat, sizeof(kandidat), 1, fajl) == 1)
        {
            if (kandidat.redniBroj == brojListe) 
            {
                kandidatPostoji = true;
                //fclose(fajl);
                break;
            }
        }
        if (kandidatPostoji) 
        {
            Korisnik user;
            int upisano = 0;
            FILE* fajl2;
            fajl2 = fopen("Biraci_koji_su_glasali.bin", "ab+");
            fseek(fajl2, 0, SEEK_SET);
            if (fajl2 == NULL)
            {
                fprintf(stderr, "Greska prilikom otvaranja fajla!\n");
                return serverskaGreska;
            }
            while (fread(&user, sizeof(user), 1, fajl2) == 1)
            {
                if (strcmp(korisnik.jmbg, user.jmbg) == 0 && strcmp(korisnik.brojTelefona, user.brojTelefona) == 0
                    && strcmp(korisnik.glasackiBroj, user.glasackiBroj) == 0 && strcmp(korisnik.imeKorisnika, user.imeKorisnika) == 0
                    && strcmp(korisnik.prezimeKorisnika, user.prezimeKorisnika) == 0 && strcmp(korisnik.email, user.email) == 0)
                {
                    printf("Greska prilikom glasanja i upisa podataka, korisnik je vec glasao!\n");
                    fclose(fajl2);
                    return vecGlasao;
                }
            }
            upisano = fwrite(&korisnik, sizeof(Korisnik), 1, fajl2);
            fclose(fajl2);
            if (upisano == 1)
            {
                printf("Uspesno glasanje, podaci zabelezeni!\n");
                //rewind(fajl); //vracam ga na pocetak fajla
                fseek(fajl, -(long)sizeof(kandidat), SEEK_CUR); //Vracam se na pocetak strukture kandidata za kojeg glasa korisnik u fajlu
                kandidat.brojGlasova = kandidat.brojGlasova + 1;
                printf("Test broj glasova: %d\n", kandidat.brojGlasova);
                fwrite(&kandidat, sizeof(kandidat), 1, fajl);
                fclose(fajl);
                return uspesnoGlasanje;
            }
            else
            {
                fclose(fajl);
                printf("Greska prilikom glasanja i upisa podataka!\n");
                return serverskaGreska;
            }
        }
        else 
        {
            fclose(fajl);
            printf("Greska, kandidat za koga glasate ne postoji!\n");
            return kandidatNePostoji;
        }
    }
}

void obradi_usera(SOCKET ClientSocket)
{
    //Primanje tacno sizeof(User) bajtova
    Korisnik primljeni;
    int total = 0;
    int expected = sizeof(Korisnik);
    char* bufptr = (char*)&primljeni;
    while (total < expected) 
    {
        int iResult = recv(ClientSocket, bufptr + total, expected - total, 0);
        if (iResult > 0) 
        {
            total += iResult;
        }
        else if (iResult == 0) 
        {
            //Klijent je zatvorio vezu prerano
            printf("Klijent je zatvorio vezu prerano (recv returned 0)\n");
            break;
        }
        else 
        {
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
        primljeni.sifra[sizeof primljeni.sifra - 1] = '\0';
        primljeni.email[sizeof primljeni.email - 1] = '\0';

        char resp;
        //bool postoji;
        //Provera da li radim registrovanje ili login
        if (primljeni.tipOperacije == kreiranjeNaloga) 
        {
            printf("Pokrenuta operacija kreiranja naloga za korisnika.\n");
            //Provera iz fajla registrovani_biraci
            bool postoji = birac_postoji(primljeni);
            bool emailKodVerifikovan = posalji_email_i_proveri(primljeni.email, ClientSocket);
            printf("Provera korisnika: %s\n", postoji ? "POSTOJI" : "NE POSTOJI");

            //Biram koji odgovor saljem
            if (postoji && emailKodVerifikovan) //ako birac postoji proveravam da li je vec kreirao nalog
            {
                bool uspesna_registracija = upisi_podatke_u_fajl_birac(primljeni);
                resp = uspesna_registracija ? 1 : 2; //1 ako je uspesno registrovan nalog, i birac je na spisku, ako je birac vec 
                //kreirao nalog, pa pokusava opet, onda vracam 2
            }
            else if(!emailKodVerifikovan)
            {
                resp = 4;
            }
            else //ako birac ne postoji na spisku kao registrovan, jednostavno vracam 0 kao i do sada 
            {
                resp = 0;
            }
        }
        else if (primljeni.tipOperacije == prijavljivanjeNaNalog)
        {
            printf("Pokrenuta operacija prijavljivanja na nalog korisnika.\n");
            //Provera da li korisnik postoji
            bool uspesno_logovanje = korisnik_vec_postoji_kao_registrovan_nalog(primljeni, true);
            resp = uspesno_logovanje ? 1 : 2; //1 ako uspe, 2 ako ne
        }
        else if (primljeni.tipOperacije == glasanje) 
        {
            printf("Pokrenuta operacija glasanja\n");
            int uspesno_glasanje = korisnik_glasao(ClientSocket, primljeni);
            resp = uspesno_glasanje; // vracam kod odgovora
        }
        else 
        {
            resp = 3; // nije definisana operacija, ovo cu da prosirim da se salje odgovarajuca poruka posle 
        }

        //Slanje jednog bajta
        int sent = 0;
        while (sent < 1) 
        {
            int iResult = send(ClientSocket, (const char*)&resp + sent, 1 - sent, 0);
            if (iResult == SOCKET_ERROR) 
            {
                printf("send neuspesan sa greskom: %d\n", WSAGetLastError());
                break;
            }
            sent += iResult;
        }
        if (sent == 1) 
        {
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