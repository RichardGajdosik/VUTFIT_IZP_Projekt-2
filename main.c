#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 10241

typedef struct stlpec {
    char bunka[MAX];
    struct stlpec *p_dalsi_stlpec;
} STLPEC;
typedef struct riadok {
    int dlzka;
    int pocet_stlpcov;
    STLPEC *stlpec;
    struct riadok *p_dalsi_riadok;
} RIADOK;
/*
    nacitam delimiter                               DONE
    skontrolujem ci su argumenty korektne
    nacitam tabulku
    while cyklus ktory prebehne všetky argumenty
    každou iteraciou urobim upravu tabulky
    vypíšem tabulka
*/

//todo spravit list vypisu chyb
//todo DELIM nesmí obsahovat uvozovky ani zpětné lomítko.

int nacitaj_delimiter(int argc, char *argv[], char *delimiter, char delimiter_array[]);
RIADOK* nacitaj_tabulku(char meno_suboru[], char delimiter_array[]);
void vypis_tabulku(char delimiter, RIADOK *zaciatok);
int main(int argc, char *argv[]) {
    RIADOK *zaciatok = NULL;
    char delimiter = ' ', delimiter_array[MAX] = {0}, meno_suboru[MAX] = {0};
    int i = 1;
    strcpy(meno_suboru,argv[argc-1]);
    strcat(meno_suboru, ".txt");
    //if podmienka ktora skontroluje ci sa realne nacital delimeter, ak ano i = 3(dalsi argument v poradi)
    if ((nacitaj_delimiter(argc, argv, &delimiter, delimiter_array))){
        i = 3;
    }
    zaciatok = nacitaj_tabulku(meno_suboru, delimiter_array);
    vypis_tabulku(delimiter, zaciatok);
//    printf("i: %d delimiter: %c array: %s\n", i, delimiter, delimiter_array);
    return 0;
}
int nacitaj_delimiter(int argc, char *argv[], char *delimiter, char delimiter_array[]){
    if(argc > 2 && (strcmp(argv[1], "-d")) == 0){
        // Nakopirujeme si delimitere do char array-u
        strcpy(delimiter_array, argv[2]);
        *delimiter = delimiter_array[0];
        return 1;
    }
    return 0;
}
RIADOK* nacitaj_tabulku(char meno_suboru[], char delimiter_array[]){
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
        while ((c = getc(fr)) != EOF) {                              // while cyklus sa vykonava dokym nenarazi na koniec suboru
            if (c == '\n' && (c = getc(fr)) != EOF) {
                ungetc(c, fr);
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
                j = 0, i = 0;
                //BUGTEST
                if (zaciatok == NULL) {                                    // ak načítavame prvý film
                    zaciatok = temp_riadok;
                } else {
                    p_riadok = zaciatok;
                    while (p_riadok->p_dalsi_riadok != NULL) {                      // pripojíme struct na koniec linked listu
                        p_riadok = p_riadok->p_dalsi_riadok;
                    }
                    p_riadok->p_dalsi_riadok = temp_riadok;
                }
                p_stlpec->p_dalsi_stlpec = NULL;                                  // vynulovanie, aby pri výpise vedel while cyklus kedy skončiť
                p_stlpec = NULL;
                temp_riadok = (RIADOK *) malloc(sizeof(RIADOK));
            } else if (c == EOF) {
                temp_stlpec->bunka[i] = '\0';
                if (p_stlpec == NULL) {                                      //ak načítaveme prvého herca, uložíme jeho adresu do temp_film->herec
                    p_stlpec = temp_stlpec;
                    temp_riadok->stlpec = p_stlpec;
                } else {
                    p_stlpec->p_dalsi_stlpec = temp_stlpec;
                    p_stlpec = temp_stlpec;
                }
                pomocna_stlpec += 1;
                if (zaciatok == NULL) {                                    // ak načítavame prvý film
                    zaciatok = temp_riadok;
                } else {
                    p_riadok = zaciatok;
                    while (p_riadok->p_dalsi_riadok != NULL) {                      // pripojíme struct na koniec linked listu
                        p_riadok = p_riadok->p_dalsi_riadok;
                    }
                    p_riadok->p_dalsi_riadok = temp_riadok;
                }
                p_stlpec->p_dalsi_stlpec = NULL;                                  // vynulovanie, aby pri výpise vedel while cyklus kedy skončiť
                p_stlpec = NULL;
                printf("Koniec!\n");
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
            if(kontrola == 1){
                kontrola = 0;
            } else {
                temp_stlpec->bunka[i++] = c;
            }
            j = 0;
            pocet_znakov++;
        }
    }
    return zaciatok;
}
void vypis_tabulku(char delimiter, RIADOK *zaciatok){
    if (zaciatok != NULL) {                                                 // kvoli stabilite programu sa spýtame či máme vôbec čo vypisovať
        RIADOK *p = zaciatok;
        STLPEC *f = p->stlpec;
        do {
            do {
                printf("%s", f->bunka);
                if(f->p_dalsi_stlpec != NULL){
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