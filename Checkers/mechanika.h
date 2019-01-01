#include "ai.c"

int liczba_figur_gracza(int numer_gracza);

void dodaj_do_listy_figur_gracza(int numer_gracza, Figura * dodawana_figura);

void usun_z_listy_figur_gracza(int numer_gracza, Wspolrzedna pozycja);

void zmien_pozycje_na_liscie_figur_gracza(int numer_gracza, Wspolrzedna stara, Wspolrzedna nowa);

void zmien_typ_na_liscie_figur_gracza(int numer_gracza, Wspolrzedna pozycja, bool czy_pion);

void wczytaj_pionki();

Wspolrzedna pierwsza_wspolrzedna_po_wykonaniu_bicia(Figura * rozpatrywana_figura, Figura * figura_przeciwnika);

int czyja_pozycja(Wspolrzedna pozycja);

int liczba_przeciwnych_figur_na_drodze(int numer_gracza, Wspolrzedna start, Wspolrzedna koniec);

bool mozliwe_bicie(Figura * rozpatrywana_figura, Figura * figura_przeciwnika);

void wykonaj_bicie_pionem(Figura * rozpatrywana_figura, Figura * figura_przeciwnika);

void wykonaj_bicie_damka(Figura * rozpatrywana_figura, Figura * figura_przeciwnika, Wspolrzedna pozycja);

int dfs_najdluzsze_mozliwe_bicie(Figura * rozpatrywana_figura);

int najdluzsze_mozliwe_bicie();

void wykonaj_kopie_stanu_gry();

void zaznacz_pole_planszy(PolePlanszy * pole_planszy_do_zaznaczenia);

bool czy_sie_cofa(int numer_gracza, Wspolrzedna kierunek_ruchu);

bool mozliwy_ruch(PolePlanszy * rozpatrywana_figura, PolePlanszy * nastepne_pole);

void odznacz_pole_planszy(PolePlanszy * pole_planszy_do_odznaczenia);

void wykonaj_ruch(PolePlanszy * rozpatrywana_figura, PolePlanszy * nastepne_pole);

void przywroc_kopie_stanu_gry();

Stan_planszy pobierz_aktualny_stan();

void aktualizuj_plansze(Stan_planszy nowy);

bool same_damki();

bool partia_zostala_rozstrzygnieta();

void zakoncz_partie();

bool nastapila_blokada();

bool wykonaj_ruch_komputerem();

bool zakoncz_ruch();

void wcisniecie_pola_planszy(GtkWidget * button, PolePlanszy * wcisniete_pole_planszy);
