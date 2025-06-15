#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
#include<string.h>

typedef struct
{
    char jmbg[14];
    char imeKorisnika[15];
    char prezimeKorisnika[30];
    char brojTelefona[11];
    char glasackiBroj[7];
}User;

bool upisi_podatke_u_fajl(User user);
void procitaj_podatke_iz_fajla(); //Vise je kao test funkcija
bool korisnik_vec_postoji(User korisnik_koji_se_registruje);

void generisi_rand_13_cifara(char jmbg_str[14]) {
    for (int i = 0; i < 13; i++) 
        jmbg_str[i] = '0' + rand() % 10;
    jmbg_str[13] = '\0';
}

void generisi_broj_telefona(char br_tel[11]) 
{
    for (int i = 0; i < 10; i++)
        br_tel[i] = '0' + rand() % 10;
    br_tel[10] = '\0';
}

void generisi_glasacki_broj(char glasacki_broj[7]) 
{
    for (int i = 0; i < 6; i++)
        glasacki_broj[i] = '0' + rand() % 10;
    glasacki_broj[6] = '\0';
}

void izaberi_imena(char ime[15]) 
{
    const char* imena[] = {
        "Aleksandar", "Luka", "Nikola", "Stefan", "Marko", "Jovan", "Nemanja", "Milos", "Petar", "Filip",
        "Dusan", "Lazar", "Vuk", "Uros", "Teodor", "Mihajlo", "Pavle", "Bojan", "Goran", "Igor",
        "Dejan", "Dragan", "Darko", "Davor", "Mladen", "Radovan", "Radomir", "Radoslav", "Slobodan", "Zoran",
        "Zdravko", "Bogdan", "Ljubomir", "Miroslav", "Vladimir", "Vladislav", "Predrag", "Ognjen", "Nebojsa", "Nenad",
        "Srdjan", "Zeljko", "Vasilije", "Oliver", "Obren", "Janko", "Milorad", "Miomir", "Momcilo", "Mirko",
        "Marija", "Milica", "Jelena", "Ana", "Sara", "Sofija", "Teodora", "Lena", "Dunja", "Nina",
        "Tara", "Mila", "Jovana", "Kristina", "Katarina", "Ivana", "Andjela", "Masa", "Hana", "Ema",
        "Elena", "Nevena", "Snezana", "Dragana", "Gordana", "Vesna", "Marina", "Natalija", "Tijana", "Ljiljana",
        "Danica", "Ljubica", "Nada", "Vera", "Zorica", "Radmila", "Biljana", "Mirjana", "Milena", "Anastasija",
        "Isidora", "Dijana", "Olivera", "Sladjana", "Bojana", "Dragica", "Svetlana", "Veselina", "Ljilja", "Kornelija",
        "Emil", "Emilijan", "Erak", "Evgenije", "Filimon", "Gavrilo", "Gerasim", "Gligor", "Gligorije", "Gojko",
        "Grgur", "Hristofor", "Hvalimir", "Ignjat", "Ignjatije", "Ilarion", "Ilija", "Isaija", "Isidor", "Ivica",
        "Jeremija", "Jevrem", "Julijan", "Kalinik", "Konstantin", "Kosta", "Kostadin", "Kuzman", "Milutin", "Mojsije",
        "Ognjan", "Petronije", "Predislav", "Radosav", "Radisa", "Rastko", "Ratimir", "Radoje", "Savatije", "Savo",
        "Sergej", "Simeon", "Simo", "Spasoje", "Sreten", "Tadija", "Tanasko", "Tihomir", "Todor", "Velibor",
        "Aksinja", "Angelina", "Anica", "Anka", "Bozena", "Cveta", "Dusanka", "Emilija", "Evica", "Fatima",
        "Ivanka", "Julijana", "Katica", "Ksenija", "Lada", "Leposava", "Lidija", "Ljubinka", "Ljupka", "Magdalena",
        "Marijana", "Milanka", "Milka", "Miona", "Mirasa", "Mira", "Natasa", "Olja", "Paja", "Rajka",
        "Rastislava", "Ruzana", "Savka", "Senka", "Slavomira", "Smilja", "Sonja", "Stanislava", "Stojanka", "Tatjana",
        "Tihana", "Valerija", "Vanja", "Veselinka", "Viktorija", "Vladana", "Zdenka", "Zeljana", "Zivka", "Zivoslava",
        "Arsenije", "Vladeta", "Slavisa", "Sinisa", "Vid", "Slavomir", "Slavoljub", "Zaharije", "Zarko", "Tomislav",
        "Ranko", "Radivoj", "Stojan", "Strahinja", "Vlastimir", "Dobrosav", "Cvijan", "Cvjetko", "Budisav", "Bozidar",
        "Damjan", "Danijel", "Vojin", "Caslav", "Mutimir", "Prvoslav", "Miladin", "Obrad", "Radun", "Sasa",
        "Zvonimir", "Toma", "Ugljesa", "Vukan", "Zlatibor", "Pero", "Mladenko", "Milojan", "Rajislav", "Borivoj",
        "Gvozden", "Radivoje", "Rastislav", "Strahimir", "Zivan", "Arsenija", "Zorislava", "Bozidarka", "Svetislava", "Milisavka",
        "Dragoslava", "Slobodanka", "Ratka", "Rajna", "Branimira", "Biserka", "Cvijeta", "Marislava", "Teofana", "Teodosija",
        "Jelisaveta", "Milusa", "Stana", "Vida", "Radojka", "Slavica", "Velimira", "Vidosava", "Videnka", "Simeona",
        "Ognjanka", "Jadranka", "Danijela", "Slavena", "Zdravka", "Mirislava", "Draginja", "Bozica", "Slavomila", "Dobrila",
        "Ljuboslava", "Vukosava", "Prodanica", "Ratimira", "Milomira", "Radmira", "Milodara", "Veselka", "Srbijanka", "Draga",
        "Olgica", "Slaveta", "Tekla", "Jovanka", "Milunka", "Agapije", "Alce", "Andreja", "Berilo", "Bogdasa",
        "Bogoje", "Bogoslav", "Bozic", "Bojko", "Boljeslav", "Boruj", "Braten", "Bratoslav", "Bratuj", "Bratus",
        "Budoje", "Velko", "Veloje", "Voislav", "Vojko", "Vojsil", "Vremac", "Goislav", "Gojak", "Dabiziv",
        "Desislav", "Dmitar", "Dobren", "Dragcuj", "Drazilo", "Dudal", "Djurdjic", "Odoje", "Ovrinja", "Ogmic",
        "Ocinja", "Preljub", "Premil", "Priban", "Pribil", "Priboje", "Pribislav", "Prodan", "Slavoje", "Sladj",
        "Smilj", "Srdan", "Srdilo", "Srdj", "Stajko", "Agnija", "Agnica", "Belosava", "Vojislava", "Gavrila",
        "Gavrilka", "Despina", "Djuka", "Evdokija", "Dobrina", "Velizarka", "Dositeja", "Miloslava", "Sladja", "Slavojka",
        "Vidoja", "Svetoslava", "Zvonimira", "Ognjenka", "Predislava", "Hranislava", "Saveta", "Preljuba", "Kosara", "Milijana",
        "Zdravislava", "Ljubana", "Darislava", "Bogdanka", "Gojka", "Dragomirka", "Jerka", "Teofila", "Ljubomirka", "Slavomirka",
        "Hristina", "Vlastimira", "Hvalimira", "Radanika", "Srebrenka", "Zlatimira", "Zivomirka", "Vidislava", "Kresimira", "Nenadija",
        "Acim", "Andjelko", "Arandjel", "Atanasije", "Bajceta", "Balsa", "Berislav", "Berisav", "Biljan", "Blagomir",
        "Bogisa", "Bogoboj", "Bogomir", "Bogosav", "Bozur", "Boran", "Branivoje", "Branivoj", "Bratimir", "Bratovan",
        "Bratoljub", "Brnca", "Budislav", "Vekoslav", "Velisav", "Velisa", "Vidosav", "Vitomir", "Vojdrag", "Vugdrag",
        "Vujadin", "Drazeta", "Javorka", "Jasan", "Jasenka", "Jelka", "Jevta", "Jevrosima", "Jevremija", "Kaliopa",
        "Ljubislava", "Majda", "Marcela", "Milobriga", "Miroslava", "Mojsija", "Nadan", "Najdana", "Njegoslava", "Perunika",
        "Ratmila", "Ratomira", "Savatija", "Sladjana", "Slavinka", "Snezinka", "Smiljka", "Srecoslava", "Stracimira", "Tihomira",
        "Urosa", "Vekoslava", "Velislava", "Zivislava", "Ilijana", "Jefimija", "Kiriliana", "Kosirina", "Lubomirka", "Svetomila",
        "Vidoslava", "Vlcenka", "Zupanija", "Jerina", "Jeremir"
    };

    int count = 465;

    int idx = rand() % count;
    strcpy(ime, imena[idx]);
}

void izaberi_prezimena(char prezime[30]) 
{
    const char* prezimena[] = {
        "Jovanovic", "Petrovic", "Nikolic", "Ilic", "Djordjevic", "Pavlovic", "Markovic", "Popovic", "Stojanovic", "Zivkovic",
        "Jankovic", "Todorovic", "Stankovic", "Ristic", "Kostic", "Milosevic", "Cvetkovic", "Kovacevic", "Dimitrijevic", "Tomic",
        "Krstic", "Ivanovic", "Lukic", "Filipovic", "Savic", "Mitrovic", "Lazic", "Petkovic", "Obradovic", "Aleksic",
        "Radovanovic", "Lazarevic", "Vasic", "Milovanovic", "Jovic", "Simic", "Djuric", "Nedeljkovic", "Novakovic", "Marinkovic",
        "Bogdanovic", "Knezevic", "Radosavljevic", "Mihajlovic", "Gajic", "Mitic", "Stefanovic", "Blagojevic", "Antic", "Vasiljevic",
        "Jevtic", "Djokic", "Stojkovic", "Vukovic", "Rakic", "Stanojevic", "Pesic", "Tasic", "Milic", "Milanovic",
        "Zdravkovic", "Grujic", "Babic", "Vuckovic", "Matic", "Peric", "Ciric", "Paunovic", "Marjanovic", "Maksimovic",
        "Andjelkovic", "Jakovljevic", "Gavrilovic", "Veljkovic", "Tosic", "Trajkovic", "Ivkovic", "Arsic", "Miletic", "Velickovic",
        "Radovic", "Miljkovic", "Nesic", "Jeremic", "Radulovic", "Djurdjevic", "Milojevic", "Urosevic", "Boskovic", "Trifunovic",
        "Bozic", "Radivojevic", "Djukic", "Milutinovic", "Stamenkovic", "Abadzic", "Abramovic", "Acimovic", "Adamovic", "Adzic",
        "Agbaba", "Ahmetovic", "Alimpic", "Anastasijevic", "Andric", "Anic", "Arandjelovic", "Babovic", "Balokovic", "Banjac",
        "Barovic", "Begovic", "Beric", "Bjelic", "Bobic", "Bojic", "Bulatovic", "Cvetanovic", "Cvitanovic", "Dakic",
        "Dapcevic", "Dedic", "Detic", "Djurkovic", "Drajic", "Drobnjak", "Dukic", "Filipovicic", "Furtula", "Gavranovic",
        "Georgievic", "Gvozdenovic", "Grbic", "Grubac", "Hadzic", "Ivetic", "Jelic", "Joksimovic", "Jovanovicic", "Kalinic",
        "Karadzic", "Kilibarda", "Krstovic", "Kuzmanovic", "Labovic", "Laketic", "Lalic", "Lebanovic", "Ljubinkovic", "Majstorovic",
        "Marjanovicic", "Milovanovicic", "Miskovic", "Naskovic", "Nikolajevic", "Obradovicic", "Pantovic", "Papovicic", "Pavlovicic", "Perovic",
        "Petricevic", "Petrinovic", "Pobric", "Popovicic", "Preradovic", "Ristovic", "Rolovic", "Rubinovic", "Savicic", "Sejdinovic",
        "Simovic", "Skoric", "Srdanovic", "Stavric", "Stojanovicic", "Subotic", "Sudic", "Todorov", "Todorovicic", "Tomicic",
        "Tomasevic", "Trbic", "Trbovic", "Veselinovic", "Vesovic", "Vesovicic", "Vidovic", "Vidovicic", "Vojinovic", "Vranic",
        "Vujadinovic", "Zarkovic", "Zekovic", "Zivanovicic", "Cakic", "Caric", "Cekic", "Cincarevic", "Cerovic", "Cirjak",
        "Crnjanski", "Crncevic", "Crnojevic", "Cvejin", "Cvetic", "Cvetkovic", "Cvejic", "Cvetojevic", "Cvijanovic", "Cvijic",
        "Cvijovic", "Cvijetic", "Cvijeticanin", "Cabirlo", "Cablinovic", "Cavoski", "Cacic", "Celebic", "Cermericic", "Cetnik",
        "Cicic", "Ckonjevic", "Ckrebic", "Cubrilovic", "Cuckovic", "Cumic", "Cupic", "Curovic", "Cvorovic", "Damjanovic",
        "Damljanovic", "Damnjanovic", "Danilovic", "Danicic", "Davidovic", "Dejanovic", "Delibasic", "Delic", "Despotovic", "Dimic",
        "Dimitrijevic", "Dinic", "Divac", "Dobricic", "Dokic", "Dragic", "Dragisic", "Dragomanovic", "Dragovic", "Dragutinovic",
        "Drakulic", "Draskovic", "Drljaca", "Drljacic", "Drulovic", "Dudukovic", "Dujkovic", "Dunjic", "Durkovic", "Dzadzic",
        "Dzajic", "Dzokic", "Djoric", "Djorovic", "Djukanovic", "Djokovic", "Djurasevic", "Djurdjevic", "Djuretic", "Djurica",
        "Djuric", "Djuricic", "Djurisic", "Durasevic", "Duric", "Erak", "Ercegovac", "Eric", "Femic", "Filic",
        "Filipevic", "Garasanin", "Garic", "Gasic", "Gavrilovic", "Gavric", "Gligorijevic", "Glogovac", "Gojkovic", "Goranovic",
        "Gorkic", "Govedarica", "Grahovac", "Gracanin", "Grlic", "Grkovic", "Grubic", "Grujic", "Grujicic", "Gvozdenovic",
        "Gucevac", "Guzina", "Guzvic", "Jaksic", "Janic", "Janjic", "Jaramaz", "Jeftanovic", "Jeftic", "Jerinic",
        "Jerkov", "Jerkovic", "Jevtic", "Jevtovic", "Cabarkapa", "Corluka", "Cubric", "Cvjeticanin", "Damjanac", "Dere",
        "Dinkic", "Dujic", "Ekmecic", "Fajfric", "Gacesa", "Gegic", "Golubic", "Guduras", "Gvozdic", "Harambasic",
        "Junuzovic", "Jurisic", "Kacar", "Krulj", "Mandich", "Puzigaca", "Baljci", "Brnovic", "Brkovic", "Deljkovic",
        "Djendjic", "Dragunovic", "Drobac", "Gligor", "Grubor", "Ivezic", "Jocic", "Jokanovic", "Kordic", "Kosutic",
        "Leskovic", "Lukacevic", "Macura", "Majic", "Maricanin", "Martinovic", "Mastilovic", "Mladjenovic", "Niketic", "Orlovic",
        "Plavsic", "Prijovic", "Radisic", "Sekulic", "Sikimic", "Stamatovic", "Tabakovic", "Vlahovic", "Vranjes", "Zaric",
        "Zec", "Zivadinovic", "Zlatanovic", "Vitorovic", "Vukasinovic", "Zecevic", "Bakic", "Barjaktarevic", "Bocic", "Borovic",
        "Bukvic", "Cestic", "Cilic", "Dapcevic", "Divljan", "Dolicanin", "Dragasevic", "Dragoljevic", "Draskovic", "Dzolic",
        "Gabric", "Glisic", "Golinac", "Griskovic", "Ilicevic", "Jakovic", "Jovcic", "Krajisnik", "Krkic", "Labudovic",
        "Lapcevic", "Lazareski", "Macanovic", "Markinovic", "Mekic", "Mihaljevic", "Milicic", "Mirkovic", "Mitranic", "Nojkovic",
        "Radulic", "Ocokoljic", "Opric", "Oreskovic", "Orlic", "Ostojic", "Otovic", "Pacic", "Paspalj", "Pejanovic",
        "Pecelj", "Pekic", "Perisic", "Perunicic", "Pevac", "Plakalovic", "Plicanic", "Potic", "Potpopovic", "Pribicevic",
        "Pric", "Princip", "Prodic", "Prstojevic", "Psilovic", "Pucar", "Radinovic", "Radisavljevic", "Radmilovic", "Radosevic",
        "Radunovic", "Rakovic", "Ralovic", "Rasovic", "Reljic", "Risticic", "Rnic", "Roginovic", "Rokvic", "Rosljevic",
        "Rudic", "Ruzic", "Sabljic", "Sajic", "Saljic", "Samurovic", "Saranovic", "Sedlakovic", "Sekulovic", "Semjenovic",
        "Senjanovic", "Sevajic", "Sipetic", "Sklizovic", "Sljivic", "Smiljkovic", "Snimic", "Sobotic", "Solakov", "Somunovic",
        "Sotic", "Spahic", "Spasojevic", "Stajkovic", "Starcevic", "Stasovic", "Stevanac", "Stoiljkovic", "Strbac", "Sudovic",
        "Sutic", "Svilar", "Svoboda", "Svetinovic", "Tadic", "Tanaskovic", "Tatomirovic", "Tepavcevic", "Tica", "Todorinovic",
        "Tomasevic", "Tomicic", "Topalovic", "Tosevic", "Trbovic", "Trifunovic", "Trtic", "Ugrinic", "Udovicic", "Ukropina",
        "Uzunovic", "Vadurina", "Vajdelic", "Vasicic", "Vebrovic", "Veljkovic", "Vidakovic", "Banovic", "Baralic", "Bartulovic",
        "Batanovic", "Beljic", "Besic", "Bijelic", "Bisetovic", "Borislavljevic", "Brankovic", "Bratic", "Brzakovic", "Bukurov",
        "Carapic", "Dasovic", "Dobrosavljevic", "Dokmanovic", "Dosenovic", "Drasic", "Drinjakovic", "Drozdek", "Dubravcic", "Dujmov",
        "Dulovic", "Durdevic", "Dzafic", "Dzananovic", "Elezovic", "Florijanovic", "Galic", "Gelic", "Gigic", "Glavonjak",
        "Gnjatovic", "Gorup", "Grbovic", "Grgurevic", "Hadziabdic", "Hanumic", "Ivancevic", "Janicijevic", "Jukic", "Jurkovic",
        "Kadric", "Kajtezovic", "Kalic", "Kisic", "Kolundzija", "Korac", "Kosarac", "Kozul", "Krajnovic", "Kukic",
        "Kurtovic", "Lalovic", "Lekovic", "Ljubankovic", "Maljkovic", "Maslovara", "Mijajlovic", "Miladinovic", "Mirjanic", "Miskin",
        "Mrdak"
    };

    int count = 566;

    int idx = rand() % count;
    strcpy(prezime, prezimena[idx]);
}

bool upisi_podatke_u_fajl(User user)
{
    int upisano = 0;
    if (!korisnik_vec_postoji(user))
    {
        FILE* fajl;
        fajl = fopen("registrovani_biraci.bin", "ab");
        if (fajl == NULL)
        {
            fprintf(stderr, "Greska prilikom otvaranja fajla!\n");
            exit(1);
        }
        upisano = fwrite(&user, sizeof(User), 1, fajl);
        fclose(fajl);
    }
    else
        printf("Korisnik vec postoji u bazi, ne mozete da se registrujete dva puta kao isti korisnik!\n\n");

    if (upisano == 1)
    {
        //printf("Uspesna registracija, podaci zabelezeni!\n");
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
    fajl = fopen("registrovani_biraci.bin", "rb");
    int counter = 0;
    if (fajl == NULL)
    {
        fprintf(stderr, "Greska prilikom otvaranja fajla!\n");
        exit(1);
    }
    while (fread(&user, sizeof(user), 1, fajl) == 1)
    {
        counter++;
    }
    printf("Broj podataka u fajlu: %d\n", counter);
    fclose(fajl);
}

bool korisnik_vec_postoji(User korisnik_koji_se_registruje)
{
    User user;
    FILE* fajl;
    fajl = fopen("postojeci_korisnici.bin", "rb");
    if (fajl == NULL)
    {
        //fprintf(stderr, "Greska prilikom otvaranja fajla!\n");
        //exit(1);
        return false;
    }
    while (fread(&user, sizeof(user), 1, fajl) == 1)
    {
        if (strcmp(korisnik_koji_se_registruje.jmbg, user.jmbg) == 0)
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
    fclose(fajl);
    return false;
}

int main(void) {
    User user;
    // Inicijalizuj generator slucajnih brojeva jednom
    srand((unsigned)time(NULL));
    int counter = 0;
    bool upisano = false;
    while (counter < 100000) 
    {
        char jmbg[14], ime[15], prezime[30], brt_tel[11], glasacki_br[7];
        generisi_rand_13_cifara(user.jmbg);
        izaberi_imena(user.imeKorisnika);
        izaberi_prezimena(user.prezimeKorisnika);
        generisi_broj_telefona(user.brojTelefona);
        generisi_glasacki_broj(user.glasackiBroj);
        upisano = upisi_podatke_u_fajl(user);
        if (upisano)
            counter++;
        else
            continue;
    }
    printf("Zavrseno generisanje podataka!\n");
    procitaj_podatke_iz_fajla();
    return 0;
}