#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define rozmiar_planszy 8
#define rozmiar_pola_planszy 55
#define szerokosc_menu_programu 135
#define liczba_rzedow_pionkow 3
#define JASNE_POLE 0
#define CIEMNE_POLE 1
#define WHITE_PION 2
#define RED_PION 3
#define WHITE_DAMA 4
#define RED_DAMA 5

typedef struct wspolrzedna
{
  int posX, posY;
} Wspolrzedna;

Wspolrzedna BRAK_POZYCJI;

bool rowne_wspolrzedne(Wspolrzedna A, Wspolrzedna B);

Wspolrzedna odejmij_wspolrzedne(Wspolrzedna A, Wspolrzedna B);

Wspolrzedna dodaj_wspolrzedne(Wspolrzedna A, Wspolrzedna B);

typedef struct pole_planszy
{
  GtkWidget * button;
  int typ_pola;
  bool zbita;
  int przynaleznosc;
  Wspolrzedna pozycja;
} PolePlanszy;

typedef struct figura
{
  bool pion;
  bool zbita;
  Wspolrzedna pozycja;
  struct figura * next;
} Figura;

typedef struct stan_planszy
{
  Figura * F[2];
  struct stan_planszy * next;
} Stan_planszy;

typedef struct mozliwe_ruchy
{
  Stan_planszy * lista;
} Mozliwe_ruchy;

Wspolrzedna stworz_wspolrzedna(int posX, int posY);

Figura * stworz_figure(bool pion, Wspolrzedna pozycja, Figura * next);

bool nalezy_do_planszy(Wspolrzedna pozycja);

Wspolrzedna znak_przemieszczenia(Wspolrzedna startowa, Wspolrzedna docelowa);

GtkWidget * wczytaj_obraz_pola_planszy(int typ_pola);

void zmien_typ_pola_planszy(PolePlanszy * rozpatrywane_pole_planszy, int nowy_typ);

void zmien_wyswietlanie_pola_planszy(PolePlanszy * rozpatrywane_pole_planszy, int nowy_wyswietlany_typ);

Figura * figury_gracza[2];
PolePlanszy plansza[rozmiar_planszy][rozmiar_planszy];
int dlugosc_sekwencji_wcisniec;
int najdluzsza_mozliwa_sekwencja;
PolePlanszy * aktualnie_wybrana_figura;
int aktualny_gracz;
bool czy_mozliwe_bicie;
int brak_bic;

Figura * kopia_figur_gracza[2];
PolePlanszy kopia_planszy[rozmiar_planszy][rozmiar_planszy];
PolePlanszy * kopia_aktualnie_wybranej_figury;
bool czy_komputer[2];
bool obracanie_planszy;

////////////////////////////////////////////////////////////

GtkWidget * menu_glowne_programu;
GtkWidget * okno_glowne_programu;
GtkWidget * okienko_informacyjne;
GtkWidget * wyswietlacz_planszy;

void oglos_wygrana(int numer_gracza);

void oglos_remis();

void zmien_obracanie();

void obroc_plansze();

#include "mechanika.c"

void display_new_game();

void czlowiek_vs_czlowiek();

void czlowiek_vs_komputer();

void komputer_vs_czlowiek();

void komputer_vs_komputer();

void wroc_do_menu_glowne();
