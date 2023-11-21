#include <stdio.h>
#include <stdlib.h>

typedef struct pro{ double x1, y1, x2, y2 } pro;
typedef struct kol{ double x, y, r } kol;
typedef struct zgi{ double x1, y1, x2, y2; int k } zgi;
typedef enum { K, P, Z } typ_kartki;
typedef struct kartka { typ_kartki typ; void* kartka } kartka;

int ileWkartce(kartka* kartki, int k, int x, int y){
    switch(kartki[k].typ){
        
        case K:
            // jeśli w kole, wyrzuć 1, jeśli nie wyrzuć 0
            break;

        case P:
            // jeśli w prostokącie, wyrzuć 1, jeśli nie wyrzuć 0
            break;
        
        case Z:
            // jeśli po złej stronie, wyrzuć 0

            // jeśli po dobrej, daj dwa rekursywne zapytania niżej,
            // i zsumuj, wyrzuć wynik
            break;
        
        default:
            return 0;
        
    }
    return 0;
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
    scanf("%d %d", n, q);
    kartka* kartki = malloc((unsigned) n * sizeof(kartki));
    
    for(int i = 0; i < n; i++){
        char typ = 0;
        scanf(" %c", &typ);
        switch(typ){
        case 'P':
            kartki[i].typ = P;
            pro* new_pro = malloc(sizeof(new_pro));
            scanf("%lf %lf %lf %lf", new_pro->x1, new_pro->y1, 
                                     new_pro->x2, new_pro->y2);
            kartki[i].kartka = (void*) new_pro;
            break;

        case 'K':
            kartki[i].typ = K;
            kol* new_kol = malloc(sizeof(new_kol));
            scanf("%lf %lf %lf", new_kol->x, new_kol->y, new_kol->r);
            kartki[i].kartka = (void*) new_kol;
            break;

        case 'Z':
            kartki[i].typ = Z;
            zgi* new_zgi = malloc(sizeof(new_zgi));
            scanf("%d %lf %lf %lf", new_zgi->k, new_zgi->x1, new_zgi->y1,
                                                new_zgi->x2, new_zgi->y2);
            kartki[i].kartka = (void*) new_zgi;
            break;
        
        default:
            break;
        }
    }

    for(int i = 0; i < q; i++){
        int k = 0;
        double x = 0, y = 0;
        scanf("%d %lf %lf", &k, &x, &y);
        printf("%d\n", ileWkartce(kartki, k, x, y));
    }

    return 0;
}