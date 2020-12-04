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
/*
    nacitam delimiter                               DONE
    skontrolujem ci su argumenty korektne
    nacitam tabulku                                 DONE
    while cyklus ktory prebehne všetky argumenty    DONE
    každou iteraciou urobim upravu tabulky
    vypíšem tabulku
*/

//todo spravit list vypisu chyb
//todo DELIM nesmí obsahovat uvozovky ani zpětné lomítko.
//todo nakodit kontrolu argumentov pretoze akoze fuuu
//todo čo sa stane ked vstup je prazdna bunka
//todo pretypovat cely program zatial funguje iba 0 - 9
int nacitaj_delimiter(int argc, char *argv[], char *delimiter, char delimiter_array[]);

RIADOK *nacitaj_tabulku(char meno_suboru[], char delimiter_array[]);

int nacitaj_prikazy(int argc, char *argv[], char prikazy[][1000], int i);

RIADOK *spracuj_prikazy(RIADOK *zaciatok, char prikazy[][1000], int pocet_prikazov);

void vypis_tabulku(char delimiter, RIADOK *zaciatok);

RIADOK *uvolni(RIADOK *zaciatok);

RIADOK *irow(RIADOK *zaciatok, int vybrany_riadok);

RIADOK *arow(RIADOK *zaciatok, int vybrany_riadok);

RIADOK *drow(RIADOK *zaciatok, int vybrany_riadok);

RIADOK *icol(RIADOK *zaciatok, int vybrany_riadok, int vybrany_stlpec);

RIADOK *acol(RIADOK *zaciatok, int vybrany_riadok, int vybrany_stlpec);

RIADOK *dcol(RIADOK *zaciatok, int vybrany_riadok, int vybrany_stlpec);

int main(int argc, char *argv[]) {
    RIADOK *zaciatok = NULL;
    char delimiter = ' ', delimiter_array[MAX] = {0}, meno_suboru[MAX] = {0}, prikazy[1000][1000];
    int i = 1, pocet_prikazov = 0;
    strcpy(meno_suboru, argv[argc - 1]);
    //if podmienka ktora skontroluje ci sa realne nacital delimeter, ak ano i = 3(dalsi argument v poradi)
    if ((nacitaj_delimiter(argc, argv, &delimiter, delimiter_array))) {
        i = 3;
    }
    zaciatok = nacitaj_tabulku(meno_suboru, delimiter_array);
    pocet_prikazov = nacitaj_prikazy(argc, argv, prikazy, i);

//    i = 0; int j = 0;
//    while(i < pocet_prikazov){
//        while(prikazy[i][j] != '\0'){
//            printf("%c", prikazy[i][j++]);
//        }
//        printf("\n");
//        i++, j = 0;
//    }

    zaciatok = spracuj_prikazy(zaciatok, prikazy, pocet_prikazov);
    vypis_tabulku(delimiter, zaciatok);
    zaciatok = uvolni(zaciatok);
    return 0;
}

int nacitaj_delimiter(int argc, char *argv[], char *delimiter, char delimiter_array[]) {
    if (argc > 2 && (strcmp(argv[1], "-d")) == 0) {
        // Nakopirujeme si delimitere do char array-u
        strcpy(delimiter_array, argv[2]);
        *delimiter = delimiter_array[0];
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
                if (p_stlpec ==
                    NULL) {                                      //ak načítaveme prvého herca, uložíme jeho adresu do temp_film->herec
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
                if (zaciatok == NULL) {                                    // ak načítavame prvý film
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
                    NULL) {                                      //ak načítaveme prvého herca, uložíme jeho adresu do temp_film->herec
                    p_stlpec = temp_stlpec;
                    temp_riadok->stlpec = p_stlpec;
                } else {
                    p_stlpec->p_dalsi_stlpec = temp_stlpec;
                    p_stlpec = temp_stlpec;
                }
                temp_riadok->pocet_stlpcov = pomocna_stlpec;
                if (zaciatok == NULL) {                                    // ak načítavame prvý film
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
                printf("Koniec!\n");
                //todo zatvorit subor
                return zaciatok;
            }
            while (j < (int) strlen(delimiter_array)) {
                if (c ==
                    delimiter_array[j]) {            // Skontrolujeme znak ktory sme prave nacitali na vyskyt v znakoch ktore uzivatel zadal ako delimitre, ak ano, nastavime dany znak na hlavny delimiter
                    temp_stlpec->bunka[i] = '\0';
                    if (p_stlpec ==
                        NULL) {                                      //ak načítaveme prvého herca, uložíme jeho adresu do temp_film->herec
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
    int j = 0, r = 0, s = 0, kontrola = 0, kontrola_medzery = 0;
    char pomocny_array[1000] = {0};
    while (i < argc - 1) {
        strcpy(pomocny_array, argv[i++]);
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
    int i = 0, j = 0, k = 0, vybrany_riadok = 1000, vybrany_stlpec = 1000;
    char pomocny_array[100] = {0};
    while (i < pocet_prikazov) {
        if (prikazy[i][0] == '[') {
            if (prikazy[i][1] == '_') {
                vybrany_riadok = '_';
            } else {
                vybrany_riadok = prikazy[i][1] - '0';
            }
            if (prikazy[i][3] == '_') {
                vybrany_stlpec = '_';
            } else {
                vybrany_stlpec = prikazy[i][3] - '0';
            }
//            printf("%c %c\n", vybrany_riadok, vybrany_stlpec);
        } else {
            while (prikazy[i][j] != ' ' && prikazy[i][j] != '\0') {
                pomocny_array[k++] = prikazy[i][j++];
            }
            if (strcmp(pomocny_array, "irow") == 0) {
                zaciatok = irow(zaciatok, vybrany_riadok);
            } else if (strcmp(pomocny_array, "arow") == 0) {
                zaciatok = arow(zaciatok, vybrany_riadok);
            } else if (strcmp(pomocny_array, "drow") == 0) {
                zaciatok = drow(zaciatok, vybrany_riadok);
            } else if (strcmp(pomocny_array, "icol") == 0) {
                zaciatok = icol(zaciatok, vybrany_riadok, vybrany_stlpec);
            } else if (strcmp(pomocny_array, "acol") == 0) {
                zaciatok = acol(zaciatok, vybrany_riadok, vybrany_stlpec);
            } else if (strcmp(pomocny_array, "dcol") == 0) {
                zaciatok = dcol(zaciatok, vybrany_riadok, vybrany_stlpec);
            }
            //porovna ktory prikaz chceme robit
            // while cyklus po [ zistime ktory prikaz, spustime
        }
//        printf("%s\n", pomocny_array);
        i++, j = 0, k = 0;
        while (pomocny_array[j] != '\0') {
            pomocny_array[j++] = '\0';
        }
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
            do {
                printf("%s", f->bunka);
                if (f->p_dalsi_stlpec != NULL) {
                    putchar(delimiter);
                }
                f = f->p_dalsi_stlpec;
            } while (f != NULL);                                // vypisujeme dokial je čo vypisovať
            printf("\n");
            p = p->p_dalsi_riadok;
            if (p != NULL) {
                f = p->stlpec;                                   // nastavíme F na nového prvého herca nového filmu
            }
        } while (p != NULL);
    }
}

RIADOK *irow(RIADOK *zaciatok, int vybrany_riadok) {
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
        if (vybrany_riadok == 1 || vybrany_riadok == '_') {
            pomocny_pointer_riadok->p_dalsi_riadok = zaciatok;
            zaciatok = pomocny_pointer_riadok;
        } else {
            i = 1;
            pointer_riadok = zaciatok;
            while (pointer_riadok->p_dalsi_riadok != NULL && i++ < vybrany_riadok) {
                pointer_riadok = pointer_riadok->p_dalsi_riadok;
            }
            pomocny_pointer_riadok->p_dalsi_riadok = pointer_riadok->p_dalsi_riadok;
            pointer_riadok->p_dalsi_riadok = pomocny_pointer_riadok;
        }
    }
    return zaciatok;
}

RIADOK *arow(RIADOK *zaciatok, int vybrany_riadok) {
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
        while (pointer_riadok->p_dalsi_riadok != NULL && i++ < vybrany_riadok) {
            pointer_riadok = pointer_riadok->p_dalsi_riadok;
        }
        pomocny_pointer_riadok->p_dalsi_riadok = pointer_riadok->p_dalsi_riadok;
        pointer_riadok->p_dalsi_riadok = pomocny_pointer_riadok;
    }
    return zaciatok;
}

RIADOK *drow(RIADOK *zaciatok, int vybrany_riadok) {
    RIADOK *pomocny_pointer_riadok = NULL;
    RIADOK *p_p_riadok_2 = NULL;
    STLPEC *pomocny_pointer_stlpec = NULL;
    STLPEC *stlpec = NULL;
    int spravny_riadok = 1;
    if (vybrany_riadok == '_') {
        while (zaciatok != NULL) {
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
        }
    } else if (vybrany_riadok == 1) {
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
    } else {
        pomocny_pointer_riadok = zaciatok;
        while (spravny_riadok++ != vybrany_riadok - 1) {
            pomocny_pointer_riadok = pomocny_pointer_riadok->p_dalsi_riadok;
            if (pomocny_pointer_riadok == NULL) {
                return zaciatok;
            }
        }
        //nastavime p p riadok 2 na riadok ktory chceme odstranit
        p_p_riadok_2 = pomocny_pointer_riadok->p_dalsi_riadok;
        //nastavime prepojime 2 pointre s tym ze vynehame zrovna riadok ktory ideme premazat ..+++-+++.. = ..++++++..
        pomocny_pointer_riadok->p_dalsi_riadok = p_p_riadok_2->p_dalsi_riadok;

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
    printf("Success");
    return zaciatok;
}

RIADOK *icol(RIADOK *zaciatok, int vybrany_riadok, int vybrany_stlpec) {
    if (zaciatok != NULL) {
        int i = 1, j = 1;
        RIADOK *pointer_riadok = zaciatok;
//TODO TATO FUNKCIA SA DA NEJAKYM SIKOVNYM RIESENIM ZJEDNODUSIT
        if (vybrany_riadok == '_') {
            do {
                if (pointer_riadok->stlpec != NULL) {
                    STLPEC *pomocny_pointer_stlpec = (STLPEC *) malloc(sizeof(STLPEC));
                    STLPEC *pointer_stlpec = pointer_riadok->stlpec;
                    pointer_riadok->pocet_stlpcov++;

                    if (vybrany_stlpec == 1 || vybrany_stlpec == '_') {
                        pomocny_pointer_stlpec->p_dalsi_stlpec = pointer_stlpec;
                        pointer_riadok->stlpec = pomocny_pointer_stlpec;
                    } else {
                        j = 1;
                        while (pointer_stlpec->p_dalsi_stlpec != NULL && j++ < vybrany_stlpec - 1) {
                            pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                        }
                        pomocny_pointer_stlpec->p_dalsi_stlpec = pointer_stlpec->p_dalsi_stlpec;
                        pointer_stlpec->p_dalsi_stlpec = pomocny_pointer_stlpec;
                        pomocny_pointer_stlpec->bunka[0] = '\0';
                    }
                    pomocny_pointer_stlpec->bunka[0] = '\0';
                }
                pointer_riadok = pointer_riadok->p_dalsi_riadok;
            } while (pointer_riadok != NULL && i++ < vybrany_riadok);
        } else {
            i = 1;
            while (pointer_riadok->p_dalsi_riadok != NULL && i++ < vybrany_riadok) {
                pointer_riadok = pointer_riadok->p_dalsi_riadok;
            }
            if (pointer_riadok->stlpec != NULL) {
                STLPEC *pomocny_pointer_stlpec = (STLPEC *) malloc(sizeof(STLPEC));
                STLPEC *pointer_stlpec = pointer_riadok->stlpec;
                pointer_riadok->pocet_stlpcov++;
                if (vybrany_stlpec == 1 || vybrany_stlpec == '_') {
                    pomocny_pointer_stlpec->p_dalsi_stlpec = pointer_stlpec;
                    pointer_riadok->stlpec = pomocny_pointer_stlpec;
                } else {
                    j = 1;
                    while (pointer_stlpec->p_dalsi_stlpec != NULL && j++ < vybrany_stlpec - 1) {
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

RIADOK *acol(RIADOK *zaciatok, int vybrany_riadok, int vybrany_stlpec) {
    if (zaciatok != NULL) {
        int i = 1, j = 1;
        RIADOK *pointer_riadok = zaciatok;
//TODO TATO FUNKCIA SA DA NEJAKYM SIKOVNYM RIESENIM ZJEDNODUSIT
        if (vybrany_riadok == '_') {
            do {
                if (pointer_riadok->stlpec != NULL) {
                    STLPEC *pomocny_pointer_stlpec = (STLPEC *) malloc(sizeof(STLPEC));
                    STLPEC *pointer_stlpec = pointer_riadok->stlpec;
                    pointer_riadok->pocet_stlpcov++;

                    j = 1;
                    while (pointer_stlpec->p_dalsi_stlpec != NULL && j++ < vybrany_stlpec) {
                        pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                    }
                    pomocny_pointer_stlpec->p_dalsi_stlpec = pointer_stlpec->p_dalsi_stlpec;
                    pointer_stlpec->p_dalsi_stlpec = pomocny_pointer_stlpec;

                    pomocny_pointer_stlpec->bunka[0] = '\0';
                }
                pointer_riadok = pointer_riadok->p_dalsi_riadok;
            } while (pointer_riadok != NULL && i++ < vybrany_riadok);
        } else {
            i = 1;
            while (pointer_riadok->p_dalsi_riadok != NULL && i++ < vybrany_riadok) {
                pointer_riadok = pointer_riadok->p_dalsi_riadok;
            }
            if (pointer_riadok->stlpec != NULL) {
                STLPEC *pomocny_pointer_stlpec = (STLPEC *) malloc(sizeof(STLPEC));
                STLPEC *pointer_stlpec = pointer_riadok->stlpec;
                pointer_riadok->pocet_stlpcov++;

                j = 1;
                while (pointer_stlpec->p_dalsi_stlpec != NULL && j++ < vybrany_stlpec) {
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

RIADOK *dcol(RIADOK *zaciatok, int vybrany_riadok, int vybrany_stlpec) {
    if (zaciatok != NULL) {
        int i = 1, j = 1;
        RIADOK *pointer_riadok = zaciatok;
//TODO TATO FUNKCIA SA DA NEJAKYM SIKOVNYM RIESENIM ZJEDNODUSIT
        if (vybrany_riadok == '_') {
            do {
                if (pointer_riadok->stlpec != NULL) {
                    STLPEC *pomocny_pointer_stlpec = NULL;
                    STLPEC *pointer_stlpec = pointer_riadok->stlpec;
                    if (vybrany_stlpec == '_') {
                        zaciatok = uvolni(zaciatok);
                        return zaciatok;
                    } else {
                        if (vybrany_stlpec == 1) {
                            pomocny_pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                            free(pointer_stlpec);
                            pointer_stlpec = NULL;
                            pointer_riadok->stlpec = pomocny_pointer_stlpec;
                        } else {
                            j = 1;
                            while (j++ < vybrany_stlpec - 1) {
                                pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                                // zadany stlpec na vymazanie je mimo rozsahu tabulky
                                if (pointer_stlpec == NULL) {
                                    return zaciatok;
                                }
                            }

                            pomocny_pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                            pointer_stlpec->p_dalsi_stlpec = pomocny_pointer_stlpec->p_dalsi_stlpec;
                            free(pomocny_pointer_stlpec);
                            pomocny_pointer_stlpec = NULL;
                        }
                    }
                }
                pointer_riadok = pointer_riadok->p_dalsi_riadok;
            } while (pointer_riadok != NULL && i++ < vybrany_riadok);
        } else {
            i = 1;
            while (pointer_riadok->p_dalsi_riadok != NULL && i++ < vybrany_riadok) {
                pointer_riadok = pointer_riadok->p_dalsi_riadok;
            }
            if (pointer_riadok->stlpec != NULL) {
                STLPEC *pomocny_pointer_stlpec = NULL;
                STLPEC *pointer_stlpec = pointer_riadok->stlpec;
                if (vybrany_stlpec == '_') {
                    while (pointer_stlpec != NULL) {
                        pomocny_pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                        free(pointer_stlpec);
                        pointer_stlpec = pomocny_pointer_stlpec;
                    }
                    //todo potrebujem vymazat cely riadok
                    //TODO ERROR [5,_];dcol
                    pointer_riadok->stlpec = NULL;
                    return zaciatok;
                } else {
                    if (vybrany_stlpec == 1) {
                        pomocny_pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                        free(pointer_stlpec);
                        pointer_stlpec = NULL;
                        pointer_riadok->stlpec = pomocny_pointer_stlpec;
                    } else {
                        j = 1;
                        while (j++ < vybrany_stlpec - 1) {
                            pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                            // zadany stlpec na vymazanie je mimo rozsahu tabulky
                            if (pointer_stlpec == NULL) {
                                return zaciatok;
                            }
                        }
                        pomocny_pointer_stlpec = pointer_stlpec->p_dalsi_stlpec;
                        pointer_stlpec->p_dalsi_stlpec = pomocny_pointer_stlpec->p_dalsi_stlpec;
                        free(pomocny_pointer_stlpec);
                        pomocny_pointer_stlpec = NULL;
                    }
                }
            }
        }
    }
    return zaciatok;
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
        return zaciatok;
    }
}
