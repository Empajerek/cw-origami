# Zadanie 3: Origami

Napisz program dla fanów origami do badania, ile warstw ma w danym punkcie sprytnie poskładana kartka papieru. Program powinien wczytać opis tworzenia kolejnych kartek origami i pytań o ilość warstw w zadanych punktach zadanych kartek z `stdin`. W pierwszym wierszu wejścia są dwie liczby `n` i `q` oznaczające odpowiednio liczbę kartek i liczbę pytań. Następnie na wejściu jest n wierszy opisujących kolejne kartki i q pytań o liczbę warstw zadanych kartek w zadanych punktach.

Opis i-tej kartki (1 ≤ i ≤ n) znajduje się w wierszu i + 1. Opis ten ma jedną z trzech postaci:

```
    P x1 y1 x2 y2
    K x y r
    Z k x1 y1 x2 y2
```

Pierwsza postać reprezentuje domknięty prostokąt o bokach równoległych do osi układu współrzędnych o lewym dolnym rogu w punkcie P<sub>1</sub> = (x<sub>1</sub>, y<sub>1</sub>) a prawym górnym w punkcie P<sub>2</sub> = (x<sub>2</sub>, y<sub>2</sub>). Punkt P<sub>1</sub> musi więc być nieostro w lewo i w dół od punktu P<sub>2</sub>. Gdy w kartkę tę wbije się szpilkę wewnątrz (lub na krawędziach) prostokąta, kartka zostanie przebita 1 raz, w pozostałych przypadkach 0 razy.

Druga postać reprezentuje koło domknięte o środku w punkcie (x, y) i promieniu r.

Trzecia postać reprezentuje kartkę powstałą przez zgięcie k-tej kartki (1 ≤ k < i) wzdłuż prostej przechodzącej przez punkty P<sub>1</sub> = (x<sub>1</sub>, y<sub>1</sub>) i P<sub>2</sub> = (x<sub>2</sub>, y<sub>2</sub>) - muszą to być różne punkty. Papier jest składany w ten sposób, że z prawej strony prostej (patrząc w kierunku od P<sub>1</sub> do P<sub>2</sub>) jest przekładany na lewą. Dla tak złożonej kartki przebicie po prawej strony prostej daje 0 warstw; przebicie dokładnie na prostej powinno zwrócić tyle samo, co przebicie kartki przed złożeniem; po stronie lewej - tyle co przed złożeniem plus przebicie rozłożonej kartki w punkcie, który nałożył się na punkt przebicia.

j-te zapytanie (1 ≤ j ≤ q) znajduje się w wierszu n + j + 1 i ma postać `k x y`, gdzie k jest numerem kartki (1 ≤ k ≤ n), a (x, y) punktem, w którym wbijamy szpilkę, aby określić liczbę warstw k-tej kartki w tym punkcie.

Program powinien wypisać na `stdout` q wierszy - odpowiedzi na zapytania o liczbę warstw.

## Uwagi

Liczby oznaczające współrzędne punktów bądź promień koła są liczbami zmiennopozycyjnymi. Nie ma ograniczeń na złożoność czasową rozwiązania. W szczególności będą dopuszczane rozwiązania wykładnicze. Nasze testy nie są złośliwe ze względu na precyzję obliczeń.

## Przykładowe wejście

```
4 5
P 0 0 2.5 1
Z 1 0.5 0 0.5 1
K 0 1 5
Z 3 0 1 -1 0
1 1.5 0.5
2 1.5 0.5
2 0 0.5
3 4 4
4 4 4

```

## Przykładowe wyjście

```
1
0
2
1
2
```
