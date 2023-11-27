/* Autor: Wiktor Kotala
 * Peer review: Konrad Kaczmarczyk */
#include <assert.h>
#include <complex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const double EPS = 1e-8;

typedef enum { RECTANGLE, CIRCLE, FOLD } sheetType;

// Reprezentacja punktu: taka unia sprawia, że point.x == creal(point.value) i point.y == cimag(point.value)
/// Tak właściwie to nie mam pewności, że to tak działa, ale na testach działa xd i imo poprawia czytelność.. jak wiesz to daj znać
typedef union {
    struct {
        double x;
        double y;
    };
    double complex value;
} point;

// Reprezentacja kartki papieru
typedef struct sheet sheet;
struct sheet {
    sheetType type;

    union {
        struct {
            point p1, p2;
        } rect;

        struct {
            point p;
            double r;
        } circ;

        struct {
            point p1, p2;
            sheet *which;
        } fold;
    };
};

bool less(double x, double y) {
    return x < y - EPS;
}

bool lessOrEqual(double x, double y) {
    return x < y + EPS;
}

// Zwraca odległość między punktami p, q
double dist(point p, point q) {
    return cabs(p.value - q.value);
}

// Zwraca iloczyn wektorowy wektorów a->b i a->c
double crossProduct(point a, point b, point c) {
    return cimag(conj(b.value - a.value) * (c.value - a.value));
}

// Zwraca odbicie punktu p względem prostej a->b
point reflection(point p, point a, point b) {
    // Ustalenie a jako środka układu współrzędnych
    p.value -= a.value; // przesunięcie p
    b.value -= a.value; // przesunięcie b

    // Ustalenie b jako osi X
    p.value /= b.value; // obrócenie p

    // Odbicie p względem prostej a->b (osi X)
    p.value = conj(p.value);

    return (point) {.value = p.value * b.value + a.value}; // odwrócenie przekształceń
}

// Wczytuje i zwraca punkt
point readPoint() {
    point p;
    assert(scanf("%lf %lf", &p.x, &p.y) == 2);
    return p;
}

// Wczytuje i zwraca tablicę kartek
sheet *readSheets(int n) {
    sheet *sheets = (sheet *) malloc(sizeof(sheet) * (size_t) n);

    for (int i = 0; i < n; i++) {
        char type;
        assert(scanf(" %c", &type) == 1);

        switch (type) {
            case 'P':
                sheets[i].type = RECTANGLE;
                sheets[i].rect.p1 = readPoint();
                sheets[i].rect.p2 = readPoint();
                break;
            case 'K':
                sheets[i].type = CIRCLE;
                sheets[i].circ.p = readPoint();
                assert(scanf("%lf", &sheets[i].circ.r) == 1);
                break;
            case 'Z':
                sheets[i].type = FOLD;
                int k;
                assert(scanf("%d", &k) == 1);
                sheets[i].fold.p1 = readPoint();
                sheets[i].fold.p2 = readPoint();
                sheets[i].fold.which = &sheets[k - 1];
                break;
        }
    }
    return sheets;
}

/* Zwraca, czy punkt p należy do kartki shape (w kształcie prostokąta lub koła)
 * Warunek początkowy: shape -> type != FOLD */
bool isInSheet(point p, const sheet *shape) {
    if (shape->type == RECTANGLE) {
        return lessOrEqual(shape->rect.p1.x, p.x) && lessOrEqual(p.x, shape->rect.p2.x) &&
               lessOrEqual(shape->rect.p1.y, p.y) && lessOrEqual(p.y, shape->rect.p2.y);
    }
    else if (shape->type == CIRCLE) {
        return lessOrEqual(dist(shape->circ.p, p), shape->circ.r);
    }
    __builtin_unreachable(); // shape -> type powinien być RECTANGLE lub CIRCLE!
}

/* Zwraca tablicę punktów sprzed złożenia foldSheet odpowiadających punktom z points
 * Aktualizuje pointsCount na rozmiar zwracanej tablicy
 * Warunek początkowy: foldSheet -> type == FOLD */
point *foldPoints(point *points, int *pointsCount, const sheet *foldSheet) {
    point *newPoints = (point *) malloc(sizeof(point) * (size_t) (*pointsCount) * 2);
    int newPointsCount = 0;

    for (int i = 0; i < *pointsCount; i++) {
        double crossProd = crossProduct(foldSheet->fold.p1, foldSheet->fold.p2, points[i]);
        if (lessOrEqual(0, crossProd)) { // punkt leży na prostej lub po jej lewej stronie
            newPoints[newPointsCount++] = points[i];
        }
        if (less(0, crossProd)) { // punkt leży po lewej stronie prostej
            newPoints[newPointsCount++] = reflection(points[i], foldSheet->fold.p1, foldSheet->fold.p2);
        }
    }

    newPoints = (point *) realloc(newPoints, sizeof(point) * (size_t) (newPointsCount));
    *pointsCount = newPointsCount;
    free(points);
    return newPoints;
}

int main() {
    int n, q;
    assert(scanf("%d %d", &n, &q) == 2);
    sheet *sheets = readSheets(n);

    while (q--) {
        int k;
        assert(scanf("%d", &k) == 1);
        point pin = readPoint();
        sheet *currSheet = &sheets[k - 1];

        int pointsCount = 1;
        point *points = (point *) malloc(sizeof(point) * (size_t) pointsCount);
        points[0] = pin;

        while (currSheet->type == FOLD) {
            points = foldPoints(points, &pointsCount, currSheet);
            currSheet = currSheet->fold.which;
        }

        int ans = 0;
        for (int i = 0; i < pointsCount; i++) {
            ans += (int) isInSheet(points[i], currSheet);
        }

        free(points);
        printf("%d\n", ans);
    }

    free(sheets);
    return 0;
}