#define INF 1000
#define PUNKT_ZA_PIONKA 5
#define PUNKT_ZA_DAMKE 50
#define PUNKT_ZA_POZIOM_I 0
#define PUNKT_ZA_POZIOM_II 1
#define PUNKT_ZA_POZIOM_III 3
#define PUNKT_ZA_POZIOM_IV 20
#define PUNKT_ZA_CHRONIONA_BAZE 50

Mozliwe_ruchy pobierz_mozliwe_ruchy(Stan_planszy*,int);

void wypisz_stan_planszy(Stan_planszy*);

int punkty_za_figury_gracza(Stan_planszy s, int gracz);

int ile_figur_ma_gracz(Stan_planszy s, int gracz);

int punkty_za_ustawienie_figur(Stan_planszy s, int gracz);

int czy_chroni_baze(Stan_planszy s, int gracz);

bool stan_koncowy(Stan_planszy s);

int uzytecznosc_stanu(Stan_planszy s);

int heurystyczna_ocena_stanu(Stan_planszy s);
