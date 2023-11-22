#include <stdio.h>
#include <stdlib.h>
#define EPS 1e-6
#define SQ(x) (x)*(x)

// Deklaracje rónych typów kartek, i pojemnika "kartka", 
// przechowującego kartkę i jej typ
typedef struct pro{ double x1, y1, x2, y2; } pro;
typedef struct kol{ double x, y, r; } kol;
typedef struct zgi{ double x1, y1, x2, y2; int k; } zgi;
typedef enum { K, P, Z } typ_kartki;
typedef struct kartka { 
    typ_kartki typ; 
    union miejsce_na_kartke { 
        pro kartka_pro; 
        kol kartka_kol; 
        zgi kartka_zgi; 
        } kartka;
} kartka;


float q_rsqrt(float number)
{
        long i;
        float x2, y;
        const float threehalfs = 1.5F;

        x2 = number * 0.5F;
        y  = number;
        i  = * ( long * ) &y;                       // evil floating point bit level hacking
        i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
        y  = * ( float * ) &i;
        y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
        y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

        return y;
    }

// Sprawdzamy ile razy dany punkt znajduje się w danej kartce.
int ileWkartce(kartka* kartki, int k, double x, double y){
    switch(kartki[k].typ){
        case P: { // nawiasy ponieważ deklarujemy zmienne w case'ach
            pro prostokat = kartki[k].kartka.kartka_pro;
            // jeśli w prostokącie, wyrzuć 1, jeśli nie wyrzuć 0
            if(prostokat.x1 <= x + EPS && x - EPS <= prostokat.x2 &&
               prostokat.y1 <= y + EPS && y - EPS <= prostokat.y2)
                return 1;
            return 0;
        }
        case K: {
            kol kolo = kartki[k].kartka.kartka_kol;
            // jeśli w kole, wyrzuć 1, jeśli nie wyrzuć 0
            if(SQ(x - kolo.x) + SQ(y - kolo.y) <= SQ(kolo.r) + EPS)
                return 1;
            return 0;
        }
        case Z: {
            zgi zgiecie = kartki[k].kartka.kartka_zgi;
            // jeśli po złej stronie, wyrzuć 0
            if((x - zgiecie.x1)*(zgiecie.y2 - zgiecie.y1) >= 
               (zgiecie.x2 - zgiecie.x1)*(y - zgiecie.y1))
                return 0;
            // jeśli po dobrej, odbij punkt według odcinka P1-P2 zrób dwa 
            // rekursywne zapytania niżej, zsumuj, i wyrzuć wynik

            double x_norm = (zgiecie.x1 - zgiecie.x2) * q_rsqrt((SQ(zgiecie.x1 - zgiecie.x2) + SQ(zgiecie.y1 - zgiecie.y2)));
            double y_norm = (zgiecie.y1 - zgiecie.y2) * q_rsqrt((SQ(zgiecie.x1 - zgiecie.x2) + SQ(zgiecie.y1 - zgiecie.y2)));
            
            x -= zgiecie.x2;
            y -= zgiecie.y2;

            double odbite_x = x * SQ(x_norm) - y * SQ(y_norm) - x_norm * y_norm * (x - y);
            double odbite_y = y * SQ(y_norm) - x * SQ(x_norm) - x_norm * y_norm * (x + y);

            x += zgiecie.x2;
            y += zgiecie.y2;
            odbite_x += zgiecie.x2;
            odbite_y += zgiecie.y2;

            return ileWkartce(kartki, zgiecie.k-1, odbite_x, odbite_y) + ileWkartce(kartki, zgiecie.k-1, x, y); 
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
                pro* nowy_pro = &kartki[i].kartka.kartka_pro;
                scanf("%lf %lf %lf %lf", &nowy_pro->x1, &nowy_pro->y1, 
                                         &nowy_pro->x2, &nowy_pro->y2);
                break;
            }
            case 'K':{
                kartki[i].typ = K;
                kol* nowy_kol = &kartki[i].kartka.kartka_kol;
                scanf("%lf %lf %lf", &nowy_kol->x, &nowy_kol->y, &nowy_kol->r);
                break;
            }
            case 'Z': {
                kartki[i].typ = Z;
                zgi* nowy_zgi = &kartki[i].kartka.kartka_zgi;
                scanf("%d %lf %lf %lf %lf", &nowy_zgi->k, &nowy_zgi->x1, 
                             &nowy_zgi->y1, &nowy_zgi->x2, &nowy_zgi->y2);
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
    free(kartki);
    return 0;
}