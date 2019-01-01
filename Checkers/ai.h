#include "funkcjaocen.c"
#include <stdio.h>

#define max_poziom 4

int abs(int a);

int sign(int a);

int przeciwnik_gracza(int gracz);

int min(int a, int b);

int max(int a, int b);

int minimax_alfa(Stan_planszy,int,int,int);
int minimax_beta(Stan_planszy,int,int,int);

void usun_stan_planszy(Stan_planszy usuwany);

void usun_mozliwe_ruchy(Mozliwe_ruchy usuwane);

void wypisz_stan_planszy(Stan_planszy * X);

void wypisz_mozliwe_ruchy(Mozliwe_ruchy * X);

void dodaj_mozliwe_ruchy(Mozliwe_ruchy * A, Mozliwe_ruchy B);

void dodaj_do_mozliwych_ruchow(Mozliwe_ruchy * A, Stan_planszy * B);

int licz_figury(Stan_planszy s, int gracz);

int najmniej_figur(Mozliwe_ruchy mozliwosci, int gracz);

void usun_zbite(Stan_planszy * S);

void zrob_damki(Stan_planszy * X);

Mozliwe_ruchy utnij_do_minimum(Mozliwe_ruchy mozliwosci, int minimum, int gracz);

Figura * kopiuj_liste_figur(Figura * first);

Stan_planszy * kopiuj_stan_planszy(Stan_planszy * X);

Figura * znajdz_figure(Figura * szukana, Stan_planszy * gdzie, int gracz);

Wspolrzedna jaka_wspolrzedna_po_zbiciu(Wspolrzedna start, Wspolrzedna koniec);

bool czy_zajeta_pozycja(Wspolrzedna cel, Stan_planszy * from);

int iledebug = 0;

int liczba_figur_po_drodze(Wspolrzedna start, Wspolrzedna koniec, Wspolrzedna kierunek, Stan_planszy * from);

bool mozna_sie_ruszyc(Figura * ruszana, Wspolrzedna cel, Stan_planszy * from, int gracz);

Mozliwe_ruchy nie_bijacy(Figura * ruszana, Stan_planszy * from, int gracz);

bool mozna_bic(Figura * bijaca, Figura * bita, Stan_planszy * s, int gracz);

bool pierwszy_ruch_bijacy;
bool mmb = false;

Mozliwe_ruchy ruch_bijacy(Figura * ruszana, Stan_planszy * from, int gracz);

Mozliwe_ruchy pobierz_mozliwe_ruchy(Stan_planszy * from, int gracz);

int minimax_alfa(Stan_planszy s,int poziom, int alfa, int beta);

int minimax_beta(Stan_planszy s,int poziom, int alfa, int beta);

void wypisz_stan_planszy_do_pliku(Stan_planszy * X, FILE * plik);

void wypisz_mozliwe_ruchy_do_pliku(Mozliwe_ruchy * X, FILE * plik);

void kreska();

Stan_planszy * ruch_komputera(Stan_planszy aktualny);
