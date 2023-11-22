#include <stdio.h>
#include <stdlib.h>
#define EPS 1e-10
#define SQ(x) (x)*(x)

// Deklaracje rónych typów kartek, i pojemnika "kartka", 
// przechowującego kartkę i jej typ
typedef struct pro{ double x1, y1, x2, y2; } pro;
typedef struct kol{ double x, y, r; } kol;
typedef struct zgi{ double x1, y1, x2, y2; int k; } zgi;
typedef enum { K, P, Z } typ_kartki;
typedef struct kartka { typ_kartki typ; void* kartka; } kartka;

// Sprawdzamy ile razy dany punkt znajduje się w danej kartce.
int ileWkartce(kartka* kartki, int k, double x, double y){
    switch(kartki[k].typ){
        case P: { // nawiasy ponieważ deklarujemy zmienne w case'ach
            pro prostokat = *((pro*) kartki[k].kartka);
            // jeśli w prostokącie, wyrzuć 1, jeśli nie wyrzuć 0
            if(prostokat.x1 <= x + EPS && x - EPS <= prostokat.x2 &&
               prostokat.y1 <= y + EPS && y - EPS <= prostokat.y2)
                return 1;
            return 0;
        }
        case K: {
            kol kolo = *((kol*) kartki[k].kartka);
            // jeśli w kole, wyrzuć 1, jeśli nie wyrzuć 0
            if(SQ(x - kolo.x) + SQ(y - kolo.y) <= SQ(kolo.r) + EPS)
                return 1;
            return 0;
        }
        case Z: {
            zgi zgiecie = *((zgi*) kartki[k].kartka);
            // jeśli po złej stronie, wyrzuć 0
            if((x - zgiecie.x1)*(zgiecie.y2 - zgiecie.y1) >= 
               (zgiecie.x2 - zgiecie.x1)*(y - zgiecie.y1))
                return 0;
            // jeśli po dobrej, odbij punkt według odcinka P1-P2 zrób dwa 
            // rekursywne zapytania niżej, zsumuj, i wyrzuć wynik

            // TO DO: napisz to solidnie
            double b = (zgiecie.y2 - zgiecie.y1) / (zgiecie.x1 - zgiecie.x2);
            double c = (zgiecie.y1 * zgiecie.x2 - zgiecie.x1 * zgiecie.y2)
                         / (zgiecie.x1 - zgiecie.x2);
            double x_mirrored = (x*(1 - SQ(b)) - 2*b*(y + c)) / (1 + SQ(b));
            double y_mirrored = (y*(SQ(b) - 1) - 2*(b*x + c)) / (1 + SQ(b));

            return ileWkartce(kartki, zgiecie.k-1, x, y)
                 + ileWkartce(kartki, zgiecie.k-1, x_mirrored, y_mirrored); 
        }
        default:
            return 0;
    }
}

/*
                              |b
                               ||
                               ||,
                               |'\
                                |`|
                                | `|
                                '| |.
                                 | `L
                                 |  |,
                                 `|  |.                              _,/7
                                  |  `|                       __,---'' /
                                  |   `|              __,---'"   ..- ./
                                  ||   |      _.--=/'',--=-,,  -    ,/
                                  `|   \,/'''`\,     /'   >  \     /'
                                   | ,/'  `. .  `\ ,/'   / \  \   /'
                           ___,----Y-'      Y     7'   .'.' `\ \ /'
                       _-/''        `\.     .   ,/'    /.'    `\\ 
                   _,-''          ---  \    `  /'    ./.'   __-' "
                _/''     .--            `\    /'     /,'_--'
             ,/''  ..                     `"\_     ./ /='etb
         .,/'                             .--"\    / / 
        <------------------,_____      ,/~'  \ | ./ .'
                                `"""""""      `|_/\ /

*/

int main(){
    // wczytujemy n q, i alokujemy miejsce na kartki
    int n = 0, q = 0;
    scanf("%d %d", &n, &q);
    kartka* kartki = malloc((unsigned) n * sizeof(kartki));
    // Wczytywanie kolejnych kartek, i zapisywanie ich w tablicy
    for(int i = 0; i < n; i++){
        // po pierwszym znaku (P, K, lub Z) wybieramy rodzaj i zapisujemy 
        // kartkę w pamięci
        char typ = 0;
        while ((getchar()) != '\n');
        scanf("%c", &typ);

        switch(typ){
            case 'P':{
                kartki[i].typ = P;
                pro* nowy_pro = malloc(sizeof(*nowy_pro));
                scanf("%lf %lf %lf %lf", &nowy_pro->x1, &nowy_pro->y1, 
                                         &nowy_pro->x2, &nowy_pro->y2);
                kartki[i].kartka = (void*) nowy_pro;
                break;
            }
            case 'K':{
                kartki[i].typ = K;
                kol* nowy_kol = malloc(sizeof(*nowy_kol));
                scanf("%lf %lf %lf", &nowy_kol->x, &nowy_kol->y, &nowy_kol->r);
                kartki[i].kartka = (void*) nowy_kol;
                break;
            }
            case 'Z': {
                kartki[i].typ = Z;
                zgi* nowy_zgi = malloc(sizeof(*nowy_zgi));
                scanf("%d %lf %lf %lf %lf", &nowy_zgi->k, &nowy_zgi->x1, 
                             &nowy_zgi->y1, &nowy_zgi->x2, &nowy_zgi->y2);
                kartki[i].kartka = (void*) nowy_zgi;
                break;
            }
            default:
                break;
        }
    }
    // Wczytywanie zapytań i odpowiadanie na nie
    for(int i = 0; i < q; i++){
        int k = 0;
        double x = 0, y = 0;
        scanf("%d %lf %lf", &k, &x, &y);
        printf("%d\n", ileWkartce(kartki, k-1, x, y));
    }
    // Zwalniamy pamięć
    /*
    for(int i = 0; i < n; i++){
        switch(kartki[i].typ){
            case P:
                free((pro*)(kartki[i].kartka));
                break;
            case K:
                free((kol*)(kartki[i].kartka));
                break;
            case Z:
                free((zgi*)(kartki[i].kartka));
                break;
            default:
                break;   
        }
    }
    */
    // free(kartki);
    return 0;
}