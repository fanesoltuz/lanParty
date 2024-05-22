# Tema 2 lanParty

## Organizare

Am optat pentru modularizarea codului in functie de componentele necesare:
- player.c pentru operatii cu jucatori si liste inlantuite de jucatori.
- team.c pentru operatii cu echipe si lista inlantuita de echipe.
- match.c pentru operatii legate de meciuri.
- bst.c pentru arbori binari de cautare.
- avl.c pentru arbori AVL.
S-a ales un design pattern pseudo-oop cu functii constructor/destructor pentru structurile dinamice.
Scriptul checker.sh este inclus in repozitoriu.

## Implementare

Am identificat cu valgrind memory leak-uri si au fost rezolvate.
Makefile-ul genereaza executabile cu simboluri de debug.
Nu exista warning-uri sau errori la compilare (se folosesc -Wall si -Wpedantic).
