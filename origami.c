#include <stdio.h>
#include <stdlib.h>
#define EPS 1e-10
#define SQ(x) (x)*(x)

typedef struct pro{ double x1, y1, x2, y2; } pro;
typedef struct kol{ double x, y, r; } kol;
typedef struct zgi{ double x1, y1, x2, y2; int k; } zgi;
typedef enum { K, P, Z } typ_kartki;
typedef struct kartka { typ_kartki typ; void* kartka; } kartka;

int ileWkartce(kartka* kartki, int k, double x, double y){
    switch(kartki[k].typ){
        
        case P: {
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
            // jeśli po dobrej, daj dwa rekursywne zapytania niżej,
            // i zsumuj, wyrzuć wynik
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
    int n = 0, q = 0;
    scanf("%d %d", &n, &q);
    kartka* kartki = malloc((unsigned) n * sizeof(kartki));
    // Wczytywanie kolejnych kartek, i zapisywanie ich w tablicy
    for(int i = 0; i < n; i++){
        char typ = 0;
        scanf(" %c", &typ);
        switch(typ){
            case 'P':{
                kartki[i].typ = P;
                pro* new_pro = malloc(sizeof(new_pro));
                scanf("%lf %lf %lf %lf", &new_pro->x1, &new_pro->y1, 
                                         &new_pro->x2, &new_pro->y2);
                kartki[i].kartka = (void*) new_pro;
                break;
            }
            case 'K':{
                kartki[i].typ = K;
                kol* new_kol = malloc(sizeof(new_kol));
                scanf("%lf %lf %lf", &new_kol->x, &new_kol->y, &new_kol->r);
                kartki[i].kartka = (void*) new_kol;
                break;
            }
            case 'Z': {
                kartki[i].typ = Z;
                zgi* new_zgi = malloc(sizeof(new_zgi));
                scanf("%d %lf %lf %lf %lf", &new_zgi->k, &new_zgi->x1, 
                             &new_zgi->y1, &new_zgi->x2, &new_zgi->y2);
                kartki[i].kartka = (void*) new_zgi;
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
        fflush(stdout);
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