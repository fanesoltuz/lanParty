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

Consider ca tema este utila pentru a invata operatii cu pointeri.

Consider ca aceasta implementare este una eficienta, arborii AVL trebuie rescrisi.

## Implementare

Intregul enunt al temei este implementat in afara de cerinta 5, care este implementata cu probleme.

\*Arborele AVL are anumite probleme ce trebuie remediate. 
Balansarea arborelui AVL nu se realizeaza corespunzator.

Am identificat cu valgrind memory leak-uri si au fost remediate.
Makefile-ul genereaza executabile cu simboluri de debug.
Nu exista warning-uri sau erori la compilare (se folosesc -Wall si -Wpedantic).
