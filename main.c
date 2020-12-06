#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10241

typedef struct stlpec {
    char bunka[MAX];
    struct stlpec *p_dalsi_stlpec;
} STLPEC;
typedef struct riadok {
    int pocet_stlpcov;
    STLPEC *stlpec;
    struct riadok *p_dalsi_riadok;
} RIADOK;

//todo pretypovat cely program zatial funguje iba 0 - 9
//todo napisat funkciu ktora mi skontroluje cisla ci su vacsie ako nula
//todo pred kazdy exit daj este funkciu na vynulovanie pamate
// najst pouzivanie \0 a pouzit funkciu vynuluj
//todo vsade pridat podmienku if zaciatok null
//todo skontrolovat strtod na minusove cisla
//todo funkcia ktora vstup zarovna
//todo escapovat v prikazoch
//todo escapovat v texte
//todo Výběr buněk může přesahovat hranice vstupní tabulky. V takovém případě bude tabulka rozšířena o požadovaný počet řádků a sloupců obsahující prázdné buňky až do aktuálního výběru.
void vynuluj(char pole[]);

void zarovnaj(RIADOK *zaciatok);

int nacitaj_delimiter(int argc, char *argv[], char *delimiter, char delimiter_array[]);

RIADOK *nacitaj_tabulku(char meno_suboru[], char delimiter_array[]);

int nacitaj_prikazy(int argc, char *argv[], char prikazy[][1000], int i);

RIADOK *spracuj_prikazy(RIADOK *zaciatok, char prikazy[][1000], int pocet_prikazov);

void vypis_tabulku(char delimiter, RIADOK *zaciatok);

RIADOK *uvolni(RIADOK *zaciatok);

RIADOK *irow(RIADOK *zaciatok, int vybrany_riadok_do);

RIADOK *arow(RIADOK *zaciatok, int vybrany_riadok_do);

RIADOK *drow(RIADOK *zaciatok, int vybrany_riadok_od, int vybrany_riadok_do);

RIADOK *icol(RIADOK *zaciatok, int vybrany_riadok_do, int vybrany_stlpec_do);

RIADOK *acol(RIADOK *zaciatok, int vybrany_riadok_do, int vybrany_stlpec_do);

RIADOK *
dcol(RIADOK *zaciatok, int vybrany_riadok_od, int vybrany_riadok_do, int vybrany_stlpec_od, int vybrany_stlpec_do);

void funkcia_set(RIADOK *zaciatok, int vybrany_riadok_od, int vybrany_riadok_do, int vybrany_stlpec_od,
                 int vybrany_stlpec_do,
                 char *set);

void funkcia_swap(RIADOK *zaciatok, int vybrany_riadok_od, int vybrany_riadok_do, int vybrany_stlpec_od,
                  int vybrany_stlpec_do, int riadok, int stlpec);

void funkcia_sum(RIADOK *zaciatok, int vybrany_riadok_od, int vybrany_riadok_do, int vybrany_stlpec_od,
                 int vybrany_stlpec_do, int riadok, int stlpec);

void funkcia_avg(RIADOK *zaciatok, int vybrany_riadok_od, int vybrany_riadok_do, int vybrany_stlpec_od,
                 int vybrany_stlpec_do, int riadok, int stlpec);

void funkcia_count(RIADOK *zaciatok, int vybrany_riadok_od, int vybrany_riadok_do, int vybrany_stlpec_od,
                   int vybrany_stlpec_do, int riadok, int stlpec);

void funkcia_len(RIADOK *zaciatok, int vybrany_riadok_od, int vybrany_riadok_do, int vybrany_stlpec_od,
                 int vybrany_stlpec_do, int riadok, int stlpec);

void funkcia_min(RIADOK *zaciatok, int *vybrany_riadok_od, int *vybrany_riadok_do, int *vybrany_stlpec_od,
                 int *vybrany_stlpec_do);

void funkcia_max(RIADOK *zaciatok, int *vybrany_riadok_od, int *vybrany_riadok_do, int *vybrany_stlpec_od,
                 int *vybrany_stlpec_do);

void funkcia_find(RIADOK *zaciatok, int *vybrany_riadok_od, int *vybrany_riadok_do, int *vybrany_stlpec_od,
                  int *vybrany_stlpec_do, char pomocny_array_2[]);


int main(int argc, char *argv[]) {
    RIADOK *zaciatok = NULL;
    char delimiter = ' ', delimiter_array[MAX] = {0}, meno_suboru[MAX] = {0}, prikazy[1000][1000];
    int i = 1, pocet_prikazov = 0;
    strcpy(meno_suboru, argv[argc - 1]);
    //if podmienka ktora skontroluje ci sa realne nacital delimeter, ak ano i = 3(dalsi argument v poradi)
    if (strcmp(argv[1], "-d") == 0){
        if ((nacitaj_delimiter(argc, argv, &delimiter, delimiter_array))) {
            i = 3;
        }
    } else {
        delimiter_array[0] = ' ';
    }
    zaciatok = nacitaj_tabulku(meno_suboru, delimiter_array);
    pocet_prikazov = nacitaj_prikazy(argc, argv, prikazy, i);
    zarovnaj(zaciatok);

//    i = 0; int j = 0;
//    while(i < pocet_prikazov){
//        while(prikazy[i][j] != '\0'){
//            printf("%c", prikazy[i][j++]);
//        }
//        printf("\n");
//        i++, j = 0;
//    }

//    zaciatok = spracuj_prikazy(zaciatok, prikazy, pocet_prikazov);
    vypis_tabulku(delimiter, zaciatok);
    zaciatok = uvolni(zaciatok);
    return 0;
}

void zarovnaj(RIADOK *zaciatok){
    int max_pocet_stlpcov = -1, pocitadlo = 0;
    if(zaciatok != NULL){
        RIADOK* pomocny_pointer_riadok = zaciatok;
        STLPEC* pomocny_pointer_stlpec = NULL;
        STLPEC* p_stlpec = NULL;
        while(pomocny_pointer_riadok != NULL){
            pomocny_pointer_stlpec = pomocny_pointer_riadok->stlpec;
            while(pomocny_pointer_stlpec != NULL){
                pocitadlo++;
                pomocny_pointer_stlpec = pomocny_pointer_stlpec->p_dalsi_stlpec;
            }
            if(max_pocet_stlpcov == -1){
                max_pocet_stlpcov = pocitadlo;
            } else {
                if(max_pocet_stlpcov < pocitadlo){
                    max_pocet_stlpcov = pocitadlo;
                }
            }
            pocitadlo = 0;
            pomocny_pointer_riadok = pomocny_pointer_riadok->p_dalsi_riadok;
        }
//        printf("%d\n", max_pocet_stlpcov);
         pomocny_pointer_riadok = zaciatok;
        pomocny_pointer_stlpec = NULL;

        //zarovname
        while(pomocny_pointer_riadok != NULL){
            int i = 0;
            pomocny_pointer_riadok->pocet_stlpcov = max_pocet_stlpcov;
            pomocny_pointer_stlpec = pomocny_pointer_riadok->stlpec;
            while(i < max_pocet_stlpcov){
                if(pomocny_pointer_stlpec == NULL){
                    pomocny_pointer_stlpec = (STLPEC *) malloc(sizeof(STLPEC));
                    pomocny_pointer_stlpec->bunka[0] = '\0';
                    pomocny_pointer_stlpec->p_dalsi_stlpec = NULL;
                    p_stlpec->p_dalsi_stlpec = pomocny_pointer_stlpec;
                }
                i++;
                p_stlpec = pomocny_pointer_stlpec;
                pomocny_pointer_stlpec = pomocny_pointer_stlpec->p_dalsi_stlpec;
            }
            pomocny_pointer_riadok = pomocny_pointer_riadok->p_dalsi_riadok;
        }
    }
}

void vynuluj(char *pole) {
    int i = 0;
    while (pole[i] != '\0') {
        pole[i] = '\0';
        i++;
    }
}

int nacitaj_delimiter(int argc, char *argv[], char *delimiter, char delimiter_array[]) {
    if (argc > 2 && (strcmp(argv[1], "-d")) == 0 && argv[2][0] != '\'') {
        int i = 0;
        // Nakopirujeme si delimitere do char array-u
        strcpy(delimiter_array, argv[2]);
        *delimiter = delimiter_array[0];
        while(delimiter_array[i] != '\0'){
            if(delimiter_array[i] == '"' || delimiter_array[i] == '\\' ){
                delimiter_array[i] = delimiter_array[0];
            }
            i++;
        }
        return 1;
    }
    return 0;
}

RIADOK *nacitaj_tabulku(char meno_suboru[], char delimiter_array[]) {
    FILE *fr;

    RIADOK *zaciatok = NULL;
    RIADOK *temp_riadok = NULL;
    RIADOK *p_riadok = NULL;

    STLPEC *temp_stlpec = NULL;
    STLPEC *p_stlpec = NULL;

    if ((fr = fopen(meno_suboru, "r")) == NULL) {
        fprintf(stderr, "%s", "Subor sa nepodarilo otvorit!");
        zaciatok = uvolni(zaciatok);
        exit(-1);
    } else {
        char c = 0;
        int pocet_znakov = 0, i = 0, j = 0, pomocna_stlpec = 1, kontrola = 0;
        temp_riadok = (RIADOK *) malloc(sizeof(RIADOK));
        temp_riadok->p_dalsi_riadok = NULL;
        temp_stlpec = (STLPEC *) malloc(sizeof(STLPEC));
        do {                              // while cyklus sa vykonava dokym nenarazi na koniec suboru
            c = getc(fr);
            if (c == '\n') {
                c = getc(fr);
                temp_stlpec->bunka[i] = '\0';
                if (p_stlpec == NULL) {
                    p_stlpec = temp_stlpec;
                    temp_riadok->stlpec = p_stlpec;
                } else {
                    p_stlpec->p_dalsi_stlpec = temp_stlpec;
                    p_stlpec = temp_stlpec;
                }
                temp_stlpec = (STLPEC *) malloc(sizeof(STLPEC));
                temp_riadok->pocet_stlpcov = pomocna_stlpec;
                pomocna_stlpec = 1;
                j = 0, i = 0;
                if (zaciatok == NULL) {                                    // ak načítavame prvý riadok
                    zaciatok = temp_riadok;
                } else {
                    p_riadok = zaciatok;
                    while (p_riadok->p_dalsi_riadok !=
                           NULL) {                      // pripojíme struct na koniec linked listu
                        p_riadok = p_riadok->p_dalsi_riadok;
                    }
                    p_riadok->p_dalsi_riadok = temp_riadok;
                }
                p_stlpec->p_dalsi_stlpec = NULL;                                  // vynulovanie, aby pri výpise vedel while cyklus kedy skončiť
                p_stlpec = NULL;
                temp_riadok = (RIADOK *) malloc(sizeof(RIADOK));
                temp_riadok->p_dalsi_riadok = NULL;
            } else if (c == EOF) {
                temp_stlpec->bunka[i] = '\0';
                if (p_stlpec ==
                    NULL) {                                      //ak načítaveme prvý riadok, uložíme jeho adresu
                    p_stlpec = temp_stlpec;
                    temp_riadok->stlpec = p_stlpec;
                } else {
                    p_stlpec->p_dalsi_stlpec = temp_stlpec;
                    p_stlpec = temp_stlpec;
                }
                temp_riadok->pocet_stlpcov = pomocna_stlpec;
                if (zaciatok == NULL) {                                    // ak načítavame prvý riadok
                    zaciatok = temp_riadok;
                } else {
                    p_riadok = zaciatok;
                    while (p_riadok->p_dalsi_riadok !=
                           NULL) {                      // pripojíme struct na koniec linked listu
                        p_riadok = p_riadok->p_dalsi_riadok;
                    }
                    p_riadok->p_dalsi_riadok = temp_riadok;
                }
                p_stlpec->p_dalsi_stlpec = NULL;                                  // vynulovanie, aby pri výpise vedel while cyklus kedy skončiť
                p_stlpec = NULL;
//                printf("Koniec!\n");
                //todo zatvorit subor
                return zaciatok;
            }
            while (j < (int) strlen(delimiter_array)) {
                if (c == delimiter_array[j]) {            // Skontrolujeme znak ktory sme prave nacitali na vyskyt v znakoch ktore uzivatel zadal ako delimitre, ak ano, nastavime dany znak na hlavny delimiter
                    temp_stlpec->bunka[i] = '\0';
                    if (p_stlpec == NULL) {                                      //ak načítaveme prvého herca, uložíme jeho adresu do temp_film->herec
                        p_stlpec = temp_stlpec;
                        temp_riadok->stlpec = p_stlpec;
                    } else {
                        p_stlpec->p_dalsi_stlpec = temp_stlpec;
                        p_stlpec = temp_stlpec;
                    }
                    temp_stlpec = (STLPEC *) malloc(sizeof(STLPEC));
                    pomocna_stlpec += 1;
                    i = 0, kontrola = 1;
                    break;
                }
                j++;
            }
            if (kontrola == 1) {
                kontrola = 0;
            } else {
                temp_stlpec->bunka[i++] = c;
            }
            j = 0;
            pocet_znakov++;
        } while (c != EOF);
    }
    return zaciatok;
}

int nacitaj_prikazy(int argc, char *argv[], char prikazy[][1000], int i) {
    int j = 0, r = 0, s = 0, kontrola = 0;
    char pomocny_array[1001] = {0};
    if(argc > 1000){
        fprintf(stderr, "%s", "Prilis vela prikazov!""\n");
        exit(-1);
    }
    while (i < argc - 1) {
        strcpy(pomocny_array, argv[i++]);
        if((int) strlen(pomocny_array) > 1000){
            fprintf(stderr, "%s", "Prilis velky prikaz!""\n");
            exit(-1);
        }
        while (j < (int) strlen(pomocny_array)) {
            if (pomocny_array[j] == 39) {
                kontrola++, j++;
            }
            if (pomocny_array[j] == ';') {
                r++, j++, s = 0;
            }
            prikazy[r][s++] = pomocny_array[j++];
        }
        if (s != 0) {
            prikazy[r][s++] = ' ';
        }
        if (kontrola == 2) {
            r++;
            break;
        }
        j = 0;
    }
    return r;
}

RIADOK *spracuj_prikazy(RIADOK *zaciatok, char prikazy[][1000], int pocet_prikazov) {
    int i = 0, j = 0, k = 0, vybrany_riadok_od_int = 1, vybrany_riadok_do_int = 1000, vybrany_stlpec_od_int = 1, vybrany_stlpec_do_int = 1000, riadok = 1, stlpec = 1;
    int predchadzajuci_vybrany_riadok_od_int = 1, predchadzajuci_vybrany_riadok_do_int = 1000, predchadzajuci_vybrany_stlpec_od_int = 1,
            predchadzajuci_vybrany_stlpec_do_int = 1000;
    char pomocny_array[1000] = {0}, pomocny_array_2[1000] = {0}, *ptr;
    char set[1000] = {0};
    int kontrola_R1_C1_R2_C2 = 0;
    while (i < pocet_prikazov) {
        if (prikazy[i][0] == '[') {
            int kontrola_ciarok = 0;
            j = 1, predchadzajuci_vybrany_riadok_od_int = vybrany_riadok_od_int, predchadzajuci_vybrany_riadok_do_int = vybrany_riadok_do_int,
            predchadzajuci_vybrany_stlpec_od_int = vybrany_stlpec_od_int,
            predchadzajuci_vybrany_stlpec_do_int = vybrany_stlpec_do_int;
            // skontrolujeme ci nemame argument typu [string]
            while (prikazy[i][j] != ']' && prikazy[i][j] != ' ' && prikazy[i][j] != '\0') {
                pomocny_array_2[k++] = prikazy[i][j++];
            }
            k = 0, j = 1;
            // skontrolujeme ci nemame argument typu [R1,C1,R2,C2]
            while (prikazy[i][j] != ']') {
                kontrola_R1_C1_R2_C2 = 1;
                if (prikazy[i][j] == '\0' || (prikazy[i][j] < '0' && prikazy[i][j] != ',') ||
                    (prikazy[i][j] > '9' && prikazy[i][j] != '_')) {
                    kontrola_R1_C1_R2_C2 = 0;
                    break;
                }
                if (prikazy[i][j] == ',') {
                    if (kontrola_ciarok == 0) {
                        if (strtod(pomocny_array, &ptr) == 0) {
                            vybrany_riadok_od_int = 1;
                        } else {
                            vybrany_riadok_od_int = strtod(pomocny_array, &ptr);
                        }
                    } else if (kontrola_ciarok == 1) {
                        if (strtod(pomocny_array, &ptr) == 0) {
                            vybrany_stlpec_od_int = 1;
                        } else {
                            vybrany_stlpec_od_int = strtod(pomocny_array, &ptr);
                        }
                    } else if (kontrola_ciarok == 2) {
                        if (strtod(pomocny_array, &ptr) == 0) {
                            vybrany_riadok_do_int = '_';
                        } else {
                            vybrany_riadok_do_int = strtod(pomocny_array, &ptr);
                        }
                    }
                    kontrola_ciarok++;
                    j++, k = 0;
                    vynuluj(pomocny_array);
                    if (prikazy[i][j] == ',') {
                        kontrola_R1_C1_R2_C2 = 0;
                        break;
                    }
                }
                //todo bugtest pravdepodobne zle
                if (kontrola_ciarok > 3 || kontrola_ciarok < 3) {
                    kontrola_R1_C1_R2_C2 = 0;
                }
                pomocny_array[k++] = prikazy[i][j++];
            }
            j = 0, k = 0;
            //zabezpecime aby sme si nevymazali ulozene cislo predtym nez ho prepiseme
            if (kontrola_R1_C1_R2_C2 == 0) {
                vynuluj(pomocny_array);
                vybrany_riadok_od_int = predchadzajuci_vybrany_riadok_od_int, vybrany_riadok_do_int = predchadzajuci_vybrany_riadok_do_int,
                vybrany_stlpec_od_int = predchadzajuci_vybrany_stlpec_od_int,
                vybrany_stlpec_do_int = predchadzajuci_vybrany_stlpec_do_int;
            }
            if (strtod(pomocny_array, &ptr) == 0) {
                vybrany_stlpec_do_int = '_';
            } else {
                vybrany_stlpec_do_int = strtod(pomocny_array, &ptr);
            }
            vynuluj(pomocny_array);
            // pokial mame argument typu [R1,C1,R2,C2]
            if(vybrany_riadok_do_int <= 0 || vybrany_riadok_od_int <= 0 ||
               vybrany_stlpec_do_int <= 0 || vybrany_stlpec_od_int <= 0){
                fprintf(stderr, "%s", "Vyber riadku alebo stlpcu je nula!""\n");
                exit(-1);
            }
            if (kontrola_R1_C1_R2_C2 == 1 && vybrany_riadok_do_int > vybrany_riadok_od_int &&
                vybrany_stlpec_do_int > vybrany_stlpec_od_int) {

            } else if (prikazy[i][1] == '_' && prikazy[i][2] == ',' && prikazy[i][3] == '_' &&
                       prikazy[i][4] == ']') { // pokial mame argument typu [_,_]
                vybrany_riadok_od_int = 1;
                vybrany_riadok_do_int = '_';
                vybrany_stlpec_od_int = 1;
                vybrany_stlpec_do_int = '_';
                // pokial mame argument typu [_,int]
            } else if (prikazy[i][1] == '_' && prikazy[i][2] == ',') {
                int p = 3;
                while (prikazy[i][p] != ']') {
                    if (prikazy[i][p] == '\0' || !(prikazy[i][p] >= '0' && prikazy[i][p] <= '9')) {
                        fprintf(stderr, "%s", "Zly argument!""\n");
                        zaciatok = uvolni(zaciatok);
                        exit(1);
                    }
                    pomocny_array[k++] = prikazy[i][p++];
                }
                vybrany_stlpec_od_int = strtod(pomocny_array, &ptr);
                vybrany_stlpec_do_int = strtod(pomocny_array, &ptr);
                k = 0;
                vynuluj(pomocny_array);
                // pokial mame argument typu [int,_] alebo [int,int]
            } else if (prikazy[i][1] >= '0' && prikazy[i][1] <= '9') {
                int p = 1;
                while (prikazy[i][p] != ',') {
                    if (prikazy[i][p] == '\0' || !(prikazy[i][p] >= '0' && prikazy[i][p] <= '9')) {
                        fprintf(stderr, "%s", "Zly argument!""\n");
                        zaciatok = uvolni(zaciatok);
                        exit(1);
                    }
                    pomocny_array[k++] = prikazy[i][p++];
                }
                vybrany_riadok_od_int = strtod(pomocny_array, &ptr);
                vybrany_riadok_do_int = strtod(pomocny_array, &ptr);
                k = 0;
                vynuluj(pomocny_array);
                p++;
                // pokial mame argument typu [int,_]
                if (prikazy[i][p] == '_') {
                    vybrany_stlpec_od_int = 1;
                    vybrany_stlpec_do_int = '_';

                    // pokial mame argument typu [int,int]
                } else {
                    while (prikazy[i][p] != ']') {
                        if (prikazy[i][p] == '\0' || !(prikazy[i][p] >= '0' && prikazy[i][p] <= '9')) {
                            fprintf(stderr, "%s", "Zly argument!""\n");
                            zaciatok = uvolni(zaciatok);
                            exit(1);
                        }
                        pomocny_array[k++] = prikazy[i][p++];
                    }
                    vybrany_stlpec_od_int = strtod(pomocny_array, &ptr);
                    vybrany_stlpec_do_int = strtod(pomocny_array, &ptr);

                    k = 0;
                    vynuluj(pomocny_array);
                }
            } else if (strcmp(pomocny_array_2, "min") == 0) {
                funkcia_min(zaciatok, &vybrany_riadok_od_int, &vybrany_riadok_do_int, &vybrany_stlpec_od_int,
                            &vybrany_stlpec_do_int);
            } else if (strcmp(pomocny_array_2, "max") == 0) {
                funkcia_max(zaciatok, &vybrany_riadok_od_int, &vybrany_riadok_do_int, &vybrany_stlpec_od_int,
                            &vybrany_stlpec_do_int);
            } else if (strcmp(pomocny_array_2, "find") == 0) {
                //plus dva nakolko chcem skocit z posledneho znaku, cez medzeru na string ktory chceme nacitat a vyhladat v tabulke
                int f = (int) strlen(pomocny_array_2) + 2;
                k = 0;
                vynuluj(pomocny_array_2);
                while (prikazy[i][f] != ']') {
                    if (prikazy[i][f] == '\0') {
                        fprintf(stderr, "%s", "Zly argument!""\n");
                        zaciatok = uvolni(zaciatok);
                        exit(1);
                    }
                    pomocny_array_2[k++] = prikazy[i][f++];
                }
                k = 0;
                funkcia_find(zaciatok, &vybrany_riadok_od_int, &vybrany_riadok_do_int, &vybrany_stlpec_od_int,
                             &vybrany_stlpec_do_int, pomocny_array_2);
                vynuluj(pomocny_array_2);
            } else {
                fprintf(stderr, "%s", "Zly argument!""\n");
                zaciatok = uvolni(zaciatok);
                exit(1);
            }
//            printf("riadok_od: %d do: %d ... stlpec_od: %d do: %d\n", vybrany_riadok_od_int, vybrany_riadok_do_int,
//                   vybrany_stlpec_od_int, vybrany_stlpec_do_int);
            vynuluj(pomocny_array_2);
        } else {
            while (prikazy[i][j] != ' ' && prikazy[i][j] != '\0') {
                pomocny_array[k++] = prikazy[i][j++];
            }
            if (strcmp(pomocny_array, "irow") == 0) {
                zaciatok = irow(zaciatok, vybrany_riadok_od_int);
            } else if (strcmp(pomocny_array, "arow") == 0) {
                zaciatok = arow(zaciatok, vybrany_riadok_do_int);
            } else if (strcmp(pomocny_array, "drow") == 0) {
                zaciatok = drow(zaciatok, vybrany_riadok_od_int, vybrany_riadok_do_int);
            } else if (strcmp(pomocny_array, "icol") == 0) {
                zaciatok = icol(zaciatok, vybrany_riadok_do_int, vybrany_stlpec_do_int);
            } else if (strcmp(pomocny_array, "acol") == 0) {
                zaciatok = acol(zaciatok, vybrany_riadok_od_int, vybrany_stlpec_od_int);
            } else if (strcmp(pomocny_array, "dcol") == 0) {
                zaciatok = dcol(zaciatok, vybrany_riadok_od_int, vybrany_riadok_do_int, vybrany_stlpec_od_int,
                                vybrany_stlpec_do_int);
            } else if (strcmp(pomocny_array, "set") == 0 || strcmp(pomocny_array, "clear") == 0) {
                if (strcmp(pomocny_array, "set") == 0) {
                    k = j, j = 0;
                    while (pomocny_array[j] != '\0') {
                        pomocny_array[j++] = '\0';
                    }
                    j = k, k = 0, j++;
                    while (prikazy[i][j] != ' ' && prikazy[i][j] != '\0') {
                        pomocny_array[k++] = prikazy[i][j++];
                    }
                    strcpy(set, pomocny_array);
                } else {
                    int o = 0;
                    while (set[o] != '\0') {
                        set[o++] = '\0';
                    }
                }
                funkcia_set(zaciatok, vybrany_riadok_od_int, vybrany_riadok_do_int, vybrany_stlpec_od_int,
                            vybrany_stlpec_do_int, set);
            } else if (strcmp(pomocny_array, "swap") == 0) {
                k = j, j = 0;
                while (pomocny_array[j] != '\0') {
                    pomocny_array[j++] = '\0';
                }
                j = k, k = 0, j++;
                while (prikazy[i][j] != ' ' && prikazy[i][j] != '\0') {
                    pomocny_array[k++] = prikazy[i][j++];
                }
                riadok = pomocny_array[1] - '0';
                stlpec = pomocny_array[3] - '0';
                funkcia_swap(zaciatok, vybrany_riadok_od_int, vybrany_riadok_do_int, vybrany_stlpec_od_int,
                             vybrany_stlpec_do_int, riadok, stlpec);
            } else if (strcmp(pomocny_array, "sum") == 0) {
                k = j, j = 0;
                while (pomocny_array[j] != '\0') {
                    pomocny_array[j++] = '\0';
                }
                j = k, k = 0, j++;
                while (prikazy[i][j] != ' ' && prikazy[i][j] != '\0') {
                    pomocny_array[k++] = prikazy[i][j++];
                }
                riadok = pomocny_array[1] - '0';
                stlpec = pomocny_array[3] - '0';
                funkcia_sum(zaciatok, vybrany_riadok_od_int, vybrany_riadok_do_int, vybrany_stlpec_od_int,
                            vybrany_stlpec_do_int,
                            riadok, stlpec);
            } else if (strcmp(pomocny_array, "avg") == 0) {
                k = j, j = 0;
                while (pomocny_array[j] != '\0') {
                    pomocny_array[j++] = '\0';
                }
                j = k, k = 0, j++;
                while (prikazy[i][j] != ' ' && prikazy[i][j] != '\0') {
                    pomocny_array[k++] = prikazy[i][j++];
                }
                riadok = pomocny_array[1] - '0';
                stlpec = pomocny_array[3] - '0';
                funkcia_avg(zaciatok, vybrany_riadok_od_int, vybrany_riadok_do_int, vybrany_stlpec_od_int,
                            vybrany_stlpec_do_int,
                            riadok, stlpec);
            } else if (strcmp(pomocny_array, "count") == 0) {
                k = j, j = 0;
                while (pomocny_array[j] != '\0') {
                    pomocny_array[j++] = '\0';
                }
                j = k, k = 0, j++;
                while (prikazy[i][j] != ' ' && prikazy[i][j] != '\0') {
                    pomocny_array[k++] = prikazy[i][j++];
                }
                riadok = pomocny_array[1] - '0';
                stlpec = pomocny_array[3] - '0';
                funkcia_count(zaciatok, vybrany_riadok_od_int, vybrany_riadok_do_int, vybrany_stlpec_od_int,
                              vybrany_stlpec_do_int,
                              riadok, stlpec);
            } else if (strcmp(pomocny_array, "len") == 0) {
                k = j, j = 0;
                while (pomocny_array[j] != '\0') {
                    pomocny_array[j++] = '\0';
                }
                j = k, k = 0, j++;
                while (prikazy[i][j] != ' ' && prikazy[i][j] != '\0') {
                    pomocny_array[k++] = prikazy[i][j++];
                }
                riadok = pomocny_array[1] - '0';
                stlpec = pomocny_array[3] - '0';
                funkcia_len(zaciatok, vybrany_riadok_od_int, vybrany_riadok_do_int, vybrany_stlpec_od_int,
                            vybrany_stlpec_do_int,
                            riadok, stlpec);
            } else {
                fprintf(stderr, "%s", "Zly argument!""\n");
                zaciatok = uvolni(zaciatok);
                exit(1);
            }

        }
//        printf("%s\n", pomocny_array);
        i++, j = 0, k = 0;
        vynuluj(pomocny_array);
        j = 0;
    }
    return zaciatok;
}

void vypis_tabulku(char delimiter, RIADOK *zaciatok) {
    if (zaciatok !=
        NULL) {                                                 // kvoli stabilite programu sa spýtame či máme vôbec čo vypisovať
        RIADOK *p = zaciatok;
        STLPEC *f = p->stlpec;
        do {
            if (f != NULL) {
                do {
                    printf("%s", f->bunka);
                    if (f->p_dalsi_stlpec != NULL) {
                        putchar(delimiter);
                    }
                    f = f->p_dalsi_stlpec;
                } while (f != NULL);                                // vypisujeme dokial je čo vypisovať
            }
            printf("\n");
            p = p->p_dalsi_riadok;
            if (p != NULL) {
                f = p->stlpec;                                   // nastavíme F na nového prvého herca nového filmu
            }
        } while (p != NULL);
    }
}

RIADOK *irow(RIADOK *zaciatok, int vybrany_riadok_do) {
    if (zaciatok != NULL) {
        int i = 1;
        RIADOK *pomocny_pointer_riadok = (RIADOK *) malloc(sizeof(RIADOK));
        RIADOK *pointer_riadok = NULL;
        pomocny_pointer_riadok->p_dalsi_riadok = NULL;
        STLPEC *pomocny_pointer_stlpec = (STLPEC *) malloc(sizeof(STLPEC));
        STLPEC *pointer_stlpec = pomocny_pointer_stlpec;
        pomocny_pointer_riadok->stlpec = pomocny_pointer_stlpec;
        pomocny_pointer_riadok->pocet_stlpcov = zaciatok->pocet_stlpcov;

        pomocny_pointer_stlpec->bunka[0] = '\0';
        pomocny_pointer_stlpec->p_dalsi_stlpec = NULL;
        pointer_stlpec = pomocny_pointer_stlpec;
        pomocny_pointer_stlpec = pomocny_pointer_stlpec->p_dalsi_stlpec;
        while (i++ < zaciatok->pocet_stlpcov) {
            pomocny_pointer_stlpec = (STLPEC *) malloc(sizeof(STLPEC));
            pomocny_pointer_stlpec->bunka[0] = '\0';
            pomocny_pointer_stlpec->p_dalsi_stlpec = NULL;
            pointer_stlpec->p_dalsi_stlpec = pomocny_pointer_stlpec;
            pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
            pointer_stlpec->p_dalsi_stlpec = NULL;
        }
        if (vybrany_riadok_do == 1 || vybrany_riadok_do == '_') {
            pomocny_pointer_riadok->p_dalsi_riadok = zaciatok;
            zaciatok = pomocny_pointer_riadok;
        } else {
            i = 1;
            pointer_riadok = zaciatok;
            while (pointer_riadok->p_dalsi_riadok != NULL && i++ < vybrany_riadok_do) {
                pointer_riadok = pointer_riadok->p_dalsi_riadok;
            }
            pomocny_pointer_riadok->p_dalsi_riadok = pointer_riadok->p_dalsi_riadok;
            pointer_riadok->p_dalsi_riadok = pomocny_pointer_riadok;
        }
    }
    return zaciatok;
}

RIADOK *arow(RIADOK *zaciatok, int vybrany_riadok_do) {
    if (zaciatok != NULL) {
        int i = 1;
        RIADOK *pomocny_pointer_riadok = (RIADOK *) malloc(sizeof(RIADOK));
        RIADOK *pointer_riadok = NULL;
        pomocny_pointer_riadok->p_dalsi_riadok = NULL;
        STLPEC *pomocny_pointer_stlpec = (STLPEC *) malloc(sizeof(STLPEC));
        STLPEC *pointer_stlpec = pomocny_pointer_stlpec;
        pomocny_pointer_riadok->stlpec = pomocny_pointer_stlpec;
        pomocny_pointer_riadok->pocet_stlpcov = zaciatok->pocet_stlpcov;

        pomocny_pointer_stlpec->bunka[0] = '\0';
        pomocny_pointer_stlpec->p_dalsi_stlpec = NULL;
        pointer_stlpec = pomocny_pointer_stlpec;
        pomocny_pointer_stlpec = pomocny_pointer_stlpec->p_dalsi_stlpec;
        while (i++ < zaciatok->pocet_stlpcov) {
            pomocny_pointer_stlpec = (STLPEC *) malloc(sizeof(STLPEC));
            pomocny_pointer_stlpec->bunka[0] = '\0';
            pomocny_pointer_stlpec->p_dalsi_stlpec = NULL;
            pointer_stlpec->p_dalsi_stlpec = pomocny_pointer_stlpec;
            pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
            pointer_stlpec->p_dalsi_stlpec = NULL;
        }
        pointer_riadok = zaciatok;
        i = 1;
        //todo ked je vzbrany riadok _ nejak vymysliet ako dam funkciam vediet
        while (pointer_riadok->p_dalsi_riadok != NULL && i++ < vybrany_riadok_do) {
            pointer_riadok = pointer_riadok->p_dalsi_riadok;
        }
        pomocny_pointer_riadok->p_dalsi_riadok = pointer_riadok->p_dalsi_riadok;
        pointer_riadok->p_dalsi_riadok = pomocny_pointer_riadok;
    }
    return zaciatok;
}

RIADOK *drow(RIADOK *zaciatok, int vybrany_riadok_od, int vybrany_riadok_do) {
    RIADOK *pomocny_pointer_riadok = NULL;
    RIADOK *p_p_riadok_2 = NULL;
    RIADOK *pointer_riadok = zaciatok;
    STLPEC *pomocny_pointer_stlpec = NULL;
    STLPEC *stlpec = NULL;
    int i = 1;
    //nastavime pointer_riadok o jeden pointer pred ten ktory chceme vymazat
    while (i++ < vybrany_riadok_od - 1) {
        pointer_riadok = pointer_riadok->p_dalsi_riadok;
        if (pointer_riadok == NULL) {
            //mimo rozsah
            return zaciatok;
        }
    }
    i = vybrany_riadok_od;
    if (vybrany_riadok_od == 1 && vybrany_riadok_do == '_') {
        uvolni(zaciatok);
    } else if (vybrany_riadok_do == 1) {
        pomocny_pointer_riadok = zaciatok->p_dalsi_riadok;
        stlpec = zaciatok->stlpec;
        while (stlpec->p_dalsi_stlpec != NULL) {
            pomocny_pointer_stlpec = stlpec;
            stlpec = stlpec->p_dalsi_stlpec;
            free(pomocny_pointer_stlpec);
            pomocny_pointer_stlpec = NULL;
        }
        free(zaciatok);
        zaciatok = pomocny_pointer_riadok;
    } else if (vybrany_riadok_od == 1) {
        i = 1;
        pomocny_pointer_riadok = zaciatok;
        while (i++ <= vybrany_riadok_do && pomocny_pointer_riadok != NULL) {
            //nastavime p p riadok 2 na riadok ktory chceme odstranit
            p_p_riadok_2 = pomocny_pointer_riadok;
            pomocny_pointer_riadok = pomocny_pointer_riadok->p_dalsi_riadok;
            stlpec = p_p_riadok_2->stlpec;
            while (stlpec->p_dalsi_stlpec != NULL) {
                pomocny_pointer_stlpec = stlpec;
                stlpec = stlpec->p_dalsi_stlpec;
                free(pomocny_pointer_stlpec);
                pomocny_pointer_stlpec = NULL;
            }
            free(p_p_riadok_2);
            p_p_riadok_2 = NULL;
        }
        zaciatok = pomocny_pointer_riadok;
    } else {
        i = vybrany_riadok_od - 1;
        //vzdy sa viem nastavit na riadok predtym
        pomocny_pointer_riadok = pointer_riadok;
        //nastavime p p riadok 2 na riadok ktory chceme odstranit
        p_p_riadok_2 = pomocny_pointer_riadok->p_dalsi_riadok;
        //prepojime 2 pointre s tym ze vynehame zrovna riadok ktory ideme premazat ..+++-+++.. = ..++++++..
//        pomocny_pointer_riadok->p_dalsi_riadok = p_p_riadok_2->p_dalsi_riadok;
        while (i++ <= vybrany_riadok_do && pomocny_pointer_riadok != NULL) {
            stlpec = p_p_riadok_2->stlpec;
            while (stlpec->p_dalsi_stlpec != NULL) {
                pomocny_pointer_stlpec = stlpec;
                stlpec = stlpec->p_dalsi_stlpec;
                free(pomocny_pointer_stlpec);
                pomocny_pointer_stlpec = NULL;
            }
            free(p_p_riadok_2);
            p_p_riadok_2 = NULL;
            pomocny_pointer_riadok = pomocny_pointer_riadok->p_dalsi_riadok;
            p_p_riadok_2 = pomocny_pointer_riadok;
        }
        pointer_riadok->p_dalsi_riadok = pomocny_pointer_riadok;
    }
//    printf("Success");
    return zaciatok;
}

RIADOK *icol(RIADOK *zaciatok, int vybrany_riadok_do, int vybrany_stlpec_do) {
    if (zaciatok != NULL) {
        int i = 1, j = 1;
        RIADOK *pointer_riadok = zaciatok;
//TODO TATO FUNKCIA SA DA NEJAKYM SIKOVNYM RIESENIM ZJEDNODUSIT
        if (vybrany_riadok_do == '_') {
            do {
                if (pointer_riadok->stlpec != NULL) {
                    STLPEC *pomocny_pointer_stlpec = (STLPEC *) malloc(sizeof(STLPEC));
                    STLPEC *pointer_stlpec = pointer_riadok->stlpec;
                    pointer_riadok->pocet_stlpcov++;

                    if (vybrany_stlpec_do == 1 || vybrany_stlpec_do == '_') {
                        pomocny_pointer_stlpec->p_dalsi_stlpec = pointer_stlpec;
                        pointer_riadok->stlpec = pomocny_pointer_stlpec;
                    } else {
                        j = 1;
                        while (pointer_stlpec->p_dalsi_stlpec != NULL && j++ < vybrany_stlpec_do - 1) {
                            pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                        }
                        pomocny_pointer_stlpec->p_dalsi_stlpec = pointer_stlpec->p_dalsi_stlpec;
                        pointer_stlpec->p_dalsi_stlpec = pomocny_pointer_stlpec;
                        pomocny_pointer_stlpec->bunka[0] = '\0';
                    }
                    pomocny_pointer_stlpec->bunka[0] = '\0';
                }
                pointer_riadok = pointer_riadok->p_dalsi_riadok;
            } while (pointer_riadok != NULL && i++ < vybrany_riadok_do);
        } else {
            i = 1;
            while (pointer_riadok->p_dalsi_riadok != NULL && i++ < vybrany_riadok_do) {
                pointer_riadok = pointer_riadok->p_dalsi_riadok;
            }
            if (pointer_riadok->stlpec != NULL) {
                STLPEC *pomocny_pointer_stlpec = (STLPEC *) malloc(sizeof(STLPEC));
                STLPEC *pointer_stlpec = pointer_riadok->stlpec;
                pointer_riadok->pocet_stlpcov++;
                if (vybrany_stlpec_do == 1 || vybrany_stlpec_do == '_') {
                    pomocny_pointer_stlpec->p_dalsi_stlpec = pointer_stlpec;
                    pointer_riadok->stlpec = pomocny_pointer_stlpec;
                } else {
                    j = 1;
                    while (pointer_stlpec->p_dalsi_stlpec != NULL && j++ < vybrany_stlpec_do - 1) {
                        pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                    }
                    pomocny_pointer_stlpec->p_dalsi_stlpec = pointer_stlpec->p_dalsi_stlpec;
                    pointer_stlpec->p_dalsi_stlpec = pomocny_pointer_stlpec;
                }
                pomocny_pointer_stlpec->bunka[0] = '\0';
            }
        }
    }
    return zaciatok;
}

RIADOK *acol(RIADOK *zaciatok, int vybrany_riadok_do, int vybrany_stlpec_do) {
    if (zaciatok != NULL) {
        int i = 1, j = 1;
        RIADOK *pointer_riadok = zaciatok;
//TODO TATO FUNKCIA SA DA NEJAKYM SIKOVNYM RIESENIM ZJEDNODUSIT
        if (vybrany_riadok_do == '_') {
            do {
                if (pointer_riadok->stlpec != NULL) {
                    STLPEC *pomocny_pointer_stlpec = (STLPEC *) malloc(sizeof(STLPEC));
                    STLPEC *pointer_stlpec = pointer_riadok->stlpec;
                    pointer_riadok->pocet_stlpcov++;

                    j = 1;
                    while (pointer_stlpec->p_dalsi_stlpec != NULL && j++ < vybrany_stlpec_do) {
                        pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                    }
                    pomocny_pointer_stlpec->p_dalsi_stlpec = pointer_stlpec->p_dalsi_stlpec;
                    pointer_stlpec->p_dalsi_stlpec = pomocny_pointer_stlpec;

                    pomocny_pointer_stlpec->bunka[0] = '\0';
                }
                pointer_riadok = pointer_riadok->p_dalsi_riadok;
            } while (pointer_riadok != NULL && i++ < vybrany_riadok_do);
        } else {
            i = 1;
            while (pointer_riadok->p_dalsi_riadok != NULL && i++ < vybrany_riadok_do) {
                pointer_riadok = pointer_riadok->p_dalsi_riadok;
            }
            if (pointer_riadok->stlpec != NULL) {
                STLPEC *pomocny_pointer_stlpec = (STLPEC *) malloc(sizeof(STLPEC));
                STLPEC *pointer_stlpec = pointer_riadok->stlpec;
                pointer_riadok->pocet_stlpcov++;

                j = 1;
                while (pointer_stlpec->p_dalsi_stlpec != NULL && j++ < vybrany_stlpec_do) {
                    pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                }
                pomocny_pointer_stlpec->p_dalsi_stlpec = pointer_stlpec->p_dalsi_stlpec;
                pointer_stlpec->p_dalsi_stlpec = pomocny_pointer_stlpec;

                pomocny_pointer_stlpec->bunka[0] = '\0';
            }
        }
    }
    return zaciatok;
}

RIADOK *
dcol(RIADOK *zaciatok, int vybrany_riadok_od, int vybrany_riadok_do, int vybrany_stlpec_od, int vybrany_stlpec_do) {
    if (zaciatok != NULL) {
        int i = 1, j = 1;
        RIADOK *pointer_riadok = zaciatok;
        //nastavime pointer_riadok o jeden pointer pred ten ktory chceme vymazat
        while (i++ < vybrany_riadok_od) {
            pointer_riadok = pointer_riadok->p_dalsi_riadok;
            if (pointer_riadok == NULL) {
                //mimo rozsah
                return zaciatok;
            }
        }
        if (vybrany_riadok_od == 1 && vybrany_riadok_do == '_') {
            do {
                if (pointer_riadok->stlpec != NULL) {
                    STLPEC *pomocny_pointer_stlpec = NULL;
                    STLPEC *pointer_stlpec = pointer_riadok->stlpec;
                    if (vybrany_stlpec_od == 1 && vybrany_stlpec_do == '_') {
                        zaciatok = uvolni(zaciatok);
                        return zaciatok;
                    } else {
                        if (vybrany_stlpec_do == 1) {
                            pomocny_pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                            free(pointer_stlpec);
                            pointer_stlpec = NULL;
                            pointer_riadok->stlpec = pomocny_pointer_stlpec;
                        } else if (vybrany_stlpec_od == 1) {
                            j = 1;
                            while (pointer_stlpec != NULL && j++ <= vybrany_stlpec_do) {
                                pomocny_pointer_stlpec = pointer_stlpec;
                                pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                                free(pomocny_pointer_stlpec);
                                pomocny_pointer_stlpec = NULL;
                            }
                            //novy prvy stlpec
                            pointer_riadok->stlpec = pointer_stlpec;
                        } else {
                            j = 1;
                            // nastavime sa na o jeden pred stlpec ktory chceme odstranovat
                            while (j++ < vybrany_stlpec_od - 1) {
                                pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                                // zadany stlpec na vymazanie je mimo rozsahu tabulky
                                if (pointer_stlpec == NULL) {
                                    return zaciatok;
                                }
                            }
                            j = vybrany_stlpec_od;
                            STLPEC *p_p_stlpec = NULL;
                            pomocny_pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                            while (pomocny_pointer_stlpec != NULL && j++ <= vybrany_stlpec_do) {
                                p_p_stlpec = pomocny_pointer_stlpec;
                                pomocny_pointer_stlpec = pomocny_pointer_stlpec->p_dalsi_stlpec;
                                free(p_p_stlpec);
                                p_p_stlpec = NULL;
                            }
                            pointer_stlpec->p_dalsi_stlpec = pomocny_pointer_stlpec;
                        }
                    }
                }
                pointer_riadok = pointer_riadok->p_dalsi_riadok;
            } while (pointer_riadok != NULL && i++ < vybrany_riadok_do);
        } else {
            i = vybrany_riadok_od;
            while (pointer_riadok != NULL && i++ <= vybrany_riadok_do) {
                if (pointer_riadok->stlpec != NULL) {
                    STLPEC *pomocny_pointer_stlpec = NULL;
                    STLPEC *pointer_stlpec = pointer_riadok->stlpec;
                    if (vybrany_stlpec_od == 1 && vybrany_stlpec_do == '_') {
                        while (pointer_stlpec != NULL) {
                            pomocny_pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                            free(pointer_stlpec);
                            pointer_stlpec = pomocny_pointer_stlpec;
                        }
                        pointer_riadok->pocet_stlpcov = 0;
                        pointer_riadok->stlpec = NULL;
                    } else {
                        if (vybrany_stlpec_do == 1) {
                            pomocny_pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                            free(pointer_stlpec);
                            pointer_stlpec = NULL;
                            pointer_riadok->stlpec = pomocny_pointer_stlpec;
                        } else if (vybrany_stlpec_od == 1) {
                            j = 1;
                            while (pointer_stlpec != NULL && j++ <= vybrany_stlpec_do) {
                                pomocny_pointer_stlpec = pointer_stlpec;
                                pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                                free(pomocny_pointer_stlpec);
                                pomocny_pointer_stlpec = NULL;
                            }
                            //novy prvy stlpec
                            pointer_riadok->stlpec = pointer_stlpec;
                        } else {
                            j = 1;
                            // nastavime sa na o jeden pred stlpec ktory chceme odstranovat
                            while (j++ < vybrany_stlpec_od - 1) {
                                pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                                // zadany stlpec na vymazanie je mimo rozsahu tabulky
                                if (pointer_stlpec == NULL) {
                                    return zaciatok;
                                }
                            }
                            j = vybrany_stlpec_od;
                            STLPEC *p_p_stlpec = NULL;
                            pomocny_pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                            while (pomocny_pointer_stlpec != NULL && j++ <= vybrany_stlpec_do) {
                                p_p_stlpec = pomocny_pointer_stlpec;
                                pomocny_pointer_stlpec = pomocny_pointer_stlpec->p_dalsi_stlpec;
                                free(p_p_stlpec);
                                p_p_stlpec = NULL;
                            }
                            pointer_stlpec->p_dalsi_stlpec = pomocny_pointer_stlpec;
                        }
                    }
                }
                pointer_riadok = pointer_riadok->p_dalsi_riadok;
            }
        }
    }
    return zaciatok;
}

void funkcia_set(RIADOK *zaciatok, int vybrany_riadok_od, int vybrany_riadok_do, int vybrany_stlpec_od,
                 int vybrany_stlpec_do,
                 char *set) {
    int i = 1, j = 1;
    if (zaciatok != NULL) {
        RIADOK *pointer_riadok = zaciatok;
        STLPEC *pointer_stlpec = NULL;

        while (i++ < vybrany_riadok_od) {
            pointer_riadok = pointer_riadok->p_dalsi_riadok;
            if (pointer_riadok == NULL) {
                //mimo rozsah
                return;
            }
        }
        i = vybrany_riadok_od;

        // KVOLI TOMU, ZE KED PRI PRESIAHNUTI LIMITU STLPCA NECHCEM UKONCIT CELU FUNKCIU MUSIM ROBIT ZAUJIMAVE ROZHODNUTIA
        while (pointer_riadok != NULL && i++ <= vybrany_riadok_do) {
            pointer_stlpec = pointer_riadok->stlpec;
            while (j++ < vybrany_stlpec_od) {
                if (pointer_stlpec == NULL) {
                    if (pointer_riadok->p_dalsi_riadok != NULL) {
                        pointer_riadok = pointer_riadok->p_dalsi_riadok;
                        pointer_stlpec = pointer_riadok->stlpec;
                        j = 1, i++;
                        continue;
                    } else {
                        return;
                    }
                }
                pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
            }
            j = vybrany_stlpec_od;

            while (pointer_stlpec != NULL && j++ <= vybrany_stlpec_do) {
                strcpy(pointer_stlpec->bunka, set);
                pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
            }
            j = 1;
            pointer_riadok = pointer_riadok->p_dalsi_riadok;
        }
    }
}

void funkcia_swap(RIADOK *zaciatok, int vybrany_riadok_od, int vybrany_riadok_do, int vybrany_stlpec_od,
                  int vybrany_stlpec_do, int riadok, int stlpec) {
    int i = 1, j = 1, o = 1, t = 1, pomocny_vybrany_riadok_od = 1, pomocny_vybrany_riadok_do = riadok, pomocny_vybrany_stlpec_od = 1, pomocny_vybrany_stlpec_do = stlpec;
    //todo spravit cez malloc
    char STR[MAX] = {0};
    if (zaciatok != NULL) {
        RIADOK *pointer_riadok = zaciatok;
        RIADOK *POINTER_OD_KTOREHO_SPRACOVAVAM_SWAP = NULL;
        STLPEC *pointer_stlpec = NULL;
        RIADOK *pomocny_pointer_riadok = zaciatok;
        STLPEC *pomocny_pointer_stlpec = NULL;
        if (riadok != '_') {
            pomocny_vybrany_riadok_od = pomocny_vybrany_riadok_do;
        }
        if (stlpec != '_') {
            pomocny_vybrany_stlpec_od = pomocny_vybrany_stlpec_do;
        }
        while (o++ < pomocny_vybrany_riadok_od) {
            pomocny_pointer_riadok = pomocny_pointer_riadok->p_dalsi_riadok;
            if (pomocny_pointer_riadok == NULL) {
                //mimo rozsah
                return;
            }
        }
        while (i++ < vybrany_riadok_od) {
            pointer_riadok = pointer_riadok->p_dalsi_riadok;
            if (pointer_riadok == NULL) {
                //mimo rozsah
                return;
            }
        }
        POINTER_OD_KTOREHO_SPRACOVAVAM_SWAP = pointer_riadok;
        i = vybrany_riadok_od, o = pomocny_vybrany_riadok_od;

        // KVOLI TOMU, ZE KED PRI PRESIAHNUTI LIMITU STLPCA NECHCEM UKONCIT CELU FUNKCIU MUSIM ROBIT ZAUJIMAVE ROZHODNUTIA
        while (pomocny_pointer_riadok != NULL && o++ <= pomocny_vybrany_riadok_do) {
            pomocny_pointer_stlpec = pomocny_pointer_riadok->stlpec;
            while (t++ < pomocny_vybrany_stlpec_od) {
                if (pomocny_pointer_stlpec == NULL) {
                    if (pomocny_pointer_riadok->p_dalsi_riadok != NULL) {
                        pomocny_pointer_riadok = pomocny_pointer_riadok->p_dalsi_riadok;
                        pomocny_pointer_stlpec = pomocny_pointer_riadok->stlpec;
                        t = 1, o++;
                        continue;
                    } else {
                        return;
                    }
                }
                pomocny_pointer_stlpec = pomocny_pointer_stlpec->p_dalsi_stlpec;
            }
            t = pomocny_vybrany_stlpec_od;
            while (pomocny_pointer_stlpec != NULL && t++ <= pomocny_vybrany_stlpec_do) {
                while (pointer_riadok != NULL && i++ <= vybrany_riadok_do) {
                    pointer_stlpec = pointer_riadok->stlpec;
                    while (j++ < vybrany_stlpec_od) {
                        if (pointer_stlpec == NULL) {
                            if (pointer_riadok->p_dalsi_riadok != NULL) {
                                pointer_riadok = pointer_riadok->p_dalsi_riadok;
                                pointer_stlpec = pointer_riadok->stlpec;
                                j = 1, i++;
                                continue;
                            } else {
                                return;
                            }
                        }
                        pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                    }
                    j = vybrany_stlpec_od;

                    while (pointer_stlpec != NULL && j++ <= vybrany_stlpec_do) {
                        //TODO dve hodnoty vymenili
                        strcpy(STR, pomocny_pointer_stlpec->bunka);
                        strcpy(pomocny_pointer_stlpec->bunka, pointer_stlpec->bunka);
                        strcpy(pointer_stlpec->bunka, STR);
                        int h = 0;
                        while (STR[h] != '\0') {
                            STR[h++] = '\0';
                        }
                        pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                    }
                    j = 1;
                    pointer_riadok = pointer_riadok->p_dalsi_riadok;
                }
                pomocny_pointer_stlpec = pomocny_pointer_stlpec->p_dalsi_stlpec;
            }
            i = vybrany_riadok_od;
            pointer_riadok = POINTER_OD_KTOREHO_SPRACOVAVAM_SWAP;
            pomocny_pointer_riadok = pomocny_pointer_riadok->p_dalsi_riadok;
        }
    }
}

void funkcia_sum(RIADOK *zaciatok, int vybrany_riadok_od, int vybrany_riadok_do, int vybrany_stlpec_od,
                 int vybrany_stlpec_do, int riadok, int stlpec) {
    int i = 1, j = 1;
    //todo spravit cez malloc
    double cislo_double = 0, sucet = 0;
    char STR[MAX] = {0}, *ptr;
    if (zaciatok != NULL) {
        RIADOK *pointer_riadok = zaciatok;
        STLPEC *pointer_stlpec = NULL;

        while (i++ < vybrany_riadok_od) {
            pointer_riadok = pointer_riadok->p_dalsi_riadok;
            if (pointer_riadok == NULL) {
                //mimo rozsah
                return;
            }
        }
        i = vybrany_riadok_od;

        // KVOLI TOMU, ZE KED PRI PRESIAHNUTI LIMITU STLPCA NECHCEM UKONCIT CELU FUNKCIU MUSIM ROBIT ZAUJIMAVE ROZHODNUTIA
        while (pointer_riadok != NULL && i++ <= vybrany_riadok_do) {
            pointer_stlpec = pointer_riadok->stlpec;
            while (j++ < vybrany_stlpec_od) {
                if (pointer_stlpec == NULL) {
                    if (pointer_riadok->p_dalsi_riadok != NULL) {
                        pointer_riadok = pointer_riadok->p_dalsi_riadok;
                        pointer_stlpec = pointer_riadok->stlpec;
                        j = 1, i++;
                        continue;
                    } else {
                        return;
                    }
                }
                pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
            }
            j = vybrany_stlpec_od;

            while (pointer_stlpec != NULL && j++ <= vybrany_stlpec_do) {
                strcpy(STR, pointer_stlpec->bunka);
                // Prevedieme si string na float cislo
                cislo_double = strtod(STR, &ptr);
                // Pretypujeme na int
                STR[0] = '\0';
                //zapiseme sucet
                sucet += cislo_double;
                pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
            }
            j = 1;
            pointer_riadok = pointer_riadok->p_dalsi_riadok;
        }
        // Pretypujeme cislo na string
        sprintf(STR, "%g", sucet);
        // Cez cset prepiseme stare nezaokruhlene cislo novym
        funkcia_set(zaciatok, riadok, riadok, stlpec, stlpec, STR);
    }
}

void funkcia_avg(RIADOK *zaciatok, int vybrany_riadok_od, int vybrany_riadok_do, int vybrany_stlpec_od,
                 int vybrany_stlpec_do, int riadok, int stlpec) {
    int i = 1, j = 1, pocet_cisel = 0;
    //todo spravit cez malloc
    double cislo_double = 0, sucet = 0;
    char STR[MAX] = {0}, *ptr;
    if (zaciatok != NULL) {
        RIADOK *pointer_riadok = zaciatok;
        STLPEC *pointer_stlpec = NULL;

        while (i++ < vybrany_riadok_od) {
            pointer_riadok = pointer_riadok->p_dalsi_riadok;
            if (pointer_riadok == NULL) {
                //mimo rozsah
                return;
            }
        }
        i = vybrany_riadok_od;

        // KVOLI TOMU, ZE KED PRI PRESIAHNUTI LIMITU STLPCA NECHCEM UKONCIT CELU FUNKCIU MUSIM ROBIT ZAUJIMAVE ROZHODNUTIA
        while (pointer_riadok != NULL && i++ <= vybrany_riadok_do) {
            pointer_stlpec = pointer_riadok->stlpec;
            while (j++ < vybrany_stlpec_od) {
                if (pointer_stlpec == NULL) {
                    if (pointer_riadok->p_dalsi_riadok != NULL) {
                        pointer_riadok = pointer_riadok->p_dalsi_riadok;
                        pointer_stlpec = pointer_riadok->stlpec;
                        j = 1, i++;
                        continue;
                    } else {
                        return;
                    }
                }
                pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
            }
            j = vybrany_stlpec_od;

            while (pointer_stlpec != NULL && j++ <= vybrany_stlpec_do) {
                strcpy(STR, pointer_stlpec->bunka);
                // Prevedieme si string na float cislo
                cislo_double = strtod(STR, &ptr);
                // Pretypujeme na int
                STR[0] = '\0';
                //zapiseme sucet
                sucet += cislo_double;
                if (cislo_double != 0) {
                    pocet_cisel++;
                }
                pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
            }
            j = 1;
            pointer_riadok = pointer_riadok->p_dalsi_riadok;
        }
        // Pretypujeme cislo na string
        sucet = sucet / pocet_cisel;
        sprintf(STR, "%g", sucet);
        // Cez cset prepiseme stare nezaokruhlene cislo novym
        funkcia_set(zaciatok, riadok, riadok, stlpec, stlpec, STR);
    }
}

void funkcia_count(RIADOK *zaciatok, int vybrany_riadok_od, int vybrany_riadok_do, int vybrany_stlpec_od,
                   int vybrany_stlpec_do, int riadok, int stlpec) {
    int i = 1, j = 1, pocet_neprazdnych_buniek = 0;
    char STR[MAX] = {0};
    if (zaciatok != NULL) {
        RIADOK *pointer_riadok = zaciatok;
        STLPEC *pointer_stlpec = NULL;

        while (i++ < vybrany_riadok_od) {
            pointer_riadok = pointer_riadok->p_dalsi_riadok;
            if (pointer_riadok == NULL) {
                //mimo rozsah
                return;
            }
        }
        i = vybrany_riadok_od;

        // KVOLI TOMU, ZE KED PRI PRESIAHNUTI LIMITU STLPCA NECHCEM UKONCIT CELU FUNKCIU MUSIM ROBIT ZAUJIMAVE ROZHODNUTIA
        while (pointer_riadok != NULL && i++ <= vybrany_riadok_do) {
            pointer_stlpec = pointer_riadok->stlpec;
            while (j++ < vybrany_stlpec_od) {
                if (pointer_stlpec == NULL) {
                    if (pointer_riadok->p_dalsi_riadok != NULL) {
                        pointer_riadok = pointer_riadok->p_dalsi_riadok;
                        pointer_stlpec = pointer_riadok->stlpec;
                        j = 1, i++;
                        continue;
                    } else {
                        return;
                    }
                }
                pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
            }
            j = vybrany_stlpec_od;

            while (pointer_stlpec != NULL && j++ <= vybrany_stlpec_do) {
                if (pointer_stlpec->bunka[0] != '\0') {
                    pocet_neprazdnych_buniek++;
                }
                pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
            }
            j = 1;
            pointer_riadok = pointer_riadok->p_dalsi_riadok;
        }
        sprintf(STR, "%d", pocet_neprazdnych_buniek);
        // Cez set prepiseme
        funkcia_set(zaciatok, riadok, riadok, stlpec, stlpec, STR);
    }
}

void funkcia_len(RIADOK *zaciatok, int vybrany_riadok_od, int vybrany_riadok_do, int vybrany_stlpec_od,
                 int vybrany_stlpec_do, int riadok, int stlpec) {
    int i = 1, j = 1, pocet_neprazdnych_buniek = 0;
    char STR[MAX] = {0};
    if (zaciatok != NULL) {
        RIADOK *pointer_riadok = zaciatok;
        STLPEC *pointer_stlpec = NULL;

        while (i++ < vybrany_riadok_od) {
            pointer_riadok = pointer_riadok->p_dalsi_riadok;
            if (pointer_riadok == NULL) {
                //mimo rozsah
                return;
            }
        }
        i = vybrany_riadok_od;

        // KVOLI TOMU, ZE KED PRI PRESIAHNUTI LIMITU STLPCA NECHCEM UKONCIT CELU FUNKCIU MUSIM ROBIT ZAUJIMAVE ROZHODNUTIA
        while (pointer_riadok != NULL && i++ <= vybrany_riadok_do) {
            pointer_stlpec = pointer_riadok->stlpec;
            while (j++ < vybrany_stlpec_od) {
                if (pointer_stlpec == NULL) {
                    if (pointer_riadok->p_dalsi_riadok != NULL) {
                        pointer_riadok = pointer_riadok->p_dalsi_riadok;
                        pointer_stlpec = pointer_riadok->stlpec;
                        j = 1, i++;
                        continue;
                    } else {
                        return;
                    }
                }
                pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
            }
            j = vybrany_stlpec_od;

            while (pointer_stlpec != NULL && j++ <= vybrany_stlpec_do) {
                if (pointer_stlpec->bunka[0] != '\0') {
                    pocet_neprazdnych_buniek += (int) strlen(pointer_stlpec->bunka);
                }
                pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
            }
            j = 1;
            pointer_riadok = pointer_riadok->p_dalsi_riadok;
        }
        sprintf(STR, "%d", pocet_neprazdnych_buniek);
        // Cez set prepiseme
        funkcia_set(zaciatok, riadok, riadok, stlpec, stlpec, STR);
    }
}

void funkcia_min(RIADOK *zaciatok, int *vybrany_riadok_od, int *vybrany_riadok_do, int *vybrany_stlpec_od,
                 int *vybrany_stlpec_do) {
    int i = 1, j = 1, x = -1, y = -1;
    double pomocne_cislo = 0, min = -1;
    //todo spravit cez malloc
    char STR[MAX] = {0}, *ptr;
    if (zaciatok != NULL) {
        RIADOK *pointer_riadok = zaciatok;
        STLPEC *pointer_stlpec = NULL;

        while (i++ < *vybrany_riadok_od) {
            pointer_riadok = pointer_riadok->p_dalsi_riadok;
            if (pointer_riadok == NULL) {
                //mimo rozsah
                return;
            }
        }
        i = *vybrany_riadok_od;

        // KVOLI TOMU, ZE KED PRI PRESIAHNUTI LIMITU STLPCA NECHCEM UKONCIT CELU FUNKCIU MUSIM ROBIT ZAUJIMAVE ROZHODNUTIA
        while (pointer_riadok != NULL && i++ <= *vybrany_riadok_do) {
            pointer_stlpec = pointer_riadok->stlpec;
            while (j++ < *vybrany_stlpec_od) {
                if (pointer_stlpec == NULL) {
                    if (pointer_riadok->p_dalsi_riadok != NULL) {
                        pointer_riadok = pointer_riadok->p_dalsi_riadok;
                        pointer_stlpec = pointer_riadok->stlpec;
                        j = 1, i++;
                        continue;
                    } else {
                        return;
                    }
                }
                pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
            }
            j = *vybrany_stlpec_od;

            while (pointer_stlpec != NULL && j++ <= *vybrany_stlpec_do) {
                if (pointer_stlpec->bunka[0] != '\0') {
                    strcpy(STR, pointer_stlpec->bunka);
                    // Prevedieme si string na float cislo
                    pomocne_cislo = strtod(STR, &ptr);
                    // Pretypujeme na int
                    STR[0] = '\0';
                    //zapiseme najmensie cislo
                    if (min == -1 && pomocne_cislo != 0) {
                        min = pomocne_cislo;
                        x = i - 1;
                        y = j - 1;
                    } else if (pomocne_cislo != 0) {
                        if (min > pomocne_cislo) {
                            x = i - 1;
                            y = j - 1;
                            min = pomocne_cislo;
                            //zapiseme suradnice xy
                        }
                    }
                    pomocne_cislo = 0;
                }
                pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
            }
            j = 1;
            pointer_riadok = pointer_riadok->p_dalsi_riadok;
        }
        if (x != -1 && y != -1) {
            *vybrany_riadok_od = *vybrany_riadok_do = x;
            *vybrany_stlpec_od = *vybrany_stlpec_do = y;
        }
    }
}

void funkcia_max(RIADOK *zaciatok, int *vybrany_riadok_od, int *vybrany_riadok_do, int *vybrany_stlpec_od,
                 int *vybrany_stlpec_do) {
    int i = 1, j = 1, x = -1, y = -1;
    double pomocne_cislo = 0, max = -1;
    //todo spravit cez malloc
    char STR[MAX] = {0}, *ptr;
    if (zaciatok != NULL) {
        RIADOK *pointer_riadok = zaciatok;
        STLPEC *pointer_stlpec = NULL;

        while (i++ < *vybrany_riadok_od) {
            pointer_riadok = pointer_riadok->p_dalsi_riadok;
            if (pointer_riadok == NULL) {
                //mimo rozsah
                return;
            }
        }
        i = *vybrany_riadok_od;

        // KVOLI TOMU, ZE KED PRI PRESIAHNUTI LIMITU STLPCA NECHCEM UKONCIT CELU FUNKCIU MUSIM ROBIT ZAUJIMAVE ROZHODNUTIA
        while (pointer_riadok != NULL && i++ <= *vybrany_riadok_do) {
            pointer_stlpec = pointer_riadok->stlpec;
            while (j++ < *vybrany_stlpec_od) {
                if (pointer_stlpec == NULL) {
                    if (pointer_riadok->p_dalsi_riadok != NULL) {
                        pointer_riadok = pointer_riadok->p_dalsi_riadok;
                        pointer_stlpec = pointer_riadok->stlpec;
                        j = 1, i++;
                        continue;
                    } else {
                        return;
                    }
                }
                pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
            }
            j = *vybrany_stlpec_od;

            while (pointer_stlpec != NULL && j++ <= *vybrany_stlpec_do) {
                if (pointer_stlpec->bunka[0] != '\0') {
                    strcpy(STR, pointer_stlpec->bunka);
                    // Prevedieme si string na float cislo
                    pomocne_cislo = strtod(STR, &ptr);
                    // Pretypujeme na int
                    STR[0] = '\0';
                    //zapiseme najmensie cislo
                    if (max == -1 && pomocne_cislo != 0) {
                        max = pomocne_cislo;
                        x = i - 1;
                        y = j - 1;
                    } else if (pomocne_cislo != 0) {
                        if (max < pomocne_cislo) {
                            x = i - 1;
                            y = j - 1;
                            max = pomocne_cislo;
                            //zapiseme suradnice xy
                        }
                    }
                    pomocne_cislo = 0;
                }
                pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
            }
            j = 1;
            pointer_riadok = pointer_riadok->p_dalsi_riadok;
        }
        if (x != -1 && y != -1) {
            *vybrany_riadok_od = *vybrany_riadok_do = x;
            *vybrany_stlpec_od = *vybrany_stlpec_do = y;
        }
    }
}

void funkcia_find(RIADOK *zaciatok, int *vybrany_riadok_od, int *vybrany_riadok_do, int *vybrany_stlpec_od,
                  int *vybrany_stlpec_do, char pomocny_array_2[]) {
    int i = 1, j = 1, x = -1, y = -1;
    //todo spravit cez malloc
    if (zaciatok != NULL) {
        RIADOK *pointer_riadok = zaciatok;
        STLPEC *pointer_stlpec = NULL;

        while (i++ < *vybrany_riadok_od) {
            pointer_riadok = pointer_riadok->p_dalsi_riadok;
            if (pointer_riadok == NULL) {
                //mimo rozsah
                return;
            }
        }
        i = *vybrany_riadok_od;

        // KVOLI TOMU, ZE KED PRI PRESIAHNUTI LIMITU STLPCA NECHCEM UKONCIT CELU FUNKCIU MUSIM ROBIT ZAUJIMAVE ROZHODNUTIA
        while (pointer_riadok != NULL && i++ <= *vybrany_riadok_do) {
            pointer_stlpec = pointer_riadok->stlpec;
            while (j++ < *vybrany_stlpec_od) {
                if (pointer_stlpec == NULL) {
                    if (pointer_riadok->p_dalsi_riadok != NULL) {
                        pointer_riadok = pointer_riadok->p_dalsi_riadok;
                        pointer_stlpec = pointer_riadok->stlpec;
                        j = 1, i++;
                        continue;
                    } else {
                        return;
                    }
                }
                pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
            }
            j = *vybrany_stlpec_od;

            while (pointer_stlpec != NULL && j++ <= *vybrany_stlpec_do) {
                if (pointer_stlpec->bunka[0] != '\0') {
                    if (strcmp(pomocny_array_2, pointer_stlpec->bunka) == 0) {
                        x = i - 1;
                        y = j - 1;
                        *vybrany_riadok_od = *vybrany_riadok_do = x;
                        *vybrany_stlpec_od = *vybrany_stlpec_do = y;
                    }
                }
                pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
            }
            j = 1;
            pointer_riadok = pointer_riadok->p_dalsi_riadok;
        }
    }
}


RIADOK *uvolni(RIADOK *zaciatok) {
    if (zaciatok != NULL) {
        RIADOK *pomocny_pointer_riadok = NULL;
        RIADOK *pointer_riadok = zaciatok;
        STLPEC *pomocny_pointer_stlpec = NULL;
        STLPEC *pointer_stlpec = NULL;
        while (pointer_riadok != NULL) {
            pomocny_pointer_riadok = pointer_riadok->p_dalsi_riadok;
            pointer_stlpec = pointer_riadok->stlpec;
            while (pointer_stlpec != NULL) {
                pomocny_pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                free(pointer_stlpec);
                pointer_stlpec = pomocny_pointer_stlpec;
            }
            pointer_riadok->stlpec = NULL;
            free(pointer_riadok);
            pointer_riadok = pomocny_pointer_riadok;
        }
        zaciatok = NULL;
//        fprintf(stderr, "%s", "Vsetko uspesne vymazane!\n");
        exit(0);
        return zaciatok;
    }
    return zaciatok;
}
