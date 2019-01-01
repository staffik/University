#include "ai.h"

int abs(int a)
{
  return a > 0 ? a:(-a);
}

int sign(int a)
{
  return a > 0 ? 1:(-1);
}

int przeciwnik_gracza(int gracz)
{
  return gracz ^ 1;
}

int min(int a, int b)
{
  return a < b ? a:b;
}

int max(int a, int b)
{
  return a > b ? a:b;
}

int minimax_alfa(Stan_planszy,int,int,int);
int minimax_beta(Stan_planszy,int,int,int);

void usun_stan_planszy(Stan_planszy usuwany)
{
  Figura *it = usuwany.F[0], *next;
  while(it!=NULL)
  {
    next = it->next;
    free(it);
    it = next;
  }
  it = usuwany.F[1];
  while(it!=NULL)
  {
    next = it->next;
    free(it);
    it = next;
  }
}

void usun_mozliwe_ruchy(Mozliwe_ruchy usuwane)
{
  Stan_planszy *it = usuwane.lista, *next;
  while(it!=NULL)
  {
    next = it->next;
    usun_stan_planszy(*it);
    free(it);
    it = next;
  }
}

void wypisz_stan_planszy(Stan_planszy * X)
{
  Figura * it = X->F[0];
  printf("Wypisuje figury gracza A:\n");
  while(it!=NULL)
  {
    if(it->pion) printf("Pion");
    else printf("Dama");
    it = it->next;
  }
  printf("Wypisuje figury gracza B:\n");
  it = X->F[1];
  while(it!=NULL)
  {
    if(it->pion) printf("Pion");
    else printf("Dama");
    printf(" na pozycji (%d,%d)\n", it->pozycja.posX, it->pozycja.posY);
    it = it->next;
  }
  printf("\n");
}

void wypisz_mozliwe_ruchy(Mozliwe_ruchy * X)
{
  Stan_planszy * it = X->lista;
  if(it == NULL) printf("Mozliwe ruchy - pusty obiekt\n");
  if(it != NULL) while(it != NULL)
  {
    wypisz_stan_planszy(it);
    it = it->next;
  }
  printf("\n");
}


void dodaj_mozliwe_ruchy(Mozliwe_ruchy * A, Mozliwe_ruchy B)
{
  Stan_planszy * it = A->lista;
  if(it != NULL)
  {
    while(it->next != NULL)
    {
      it = it->next;
    }
    it->next = B.lista;
  }
  else
  {
    A->lista = B.lista;
  }
}

void dodaj_do_mozliwych_ruchow(Mozliwe_ruchy * A, Stan_planszy * B)
{
  if(A->lista == NULL)
  {
    A->lista = B;
  }
  else
  {
    B->next = A->lista;
    A->lista = B;
  }
}

int licz_figury(Stan_planszy s, int gracz)
{
  int wynik = 0;
  Figura * it = s.F[gracz];
  while(it != NULL)
  {
    if(!(it->zbita)) wynik++;
    it = it->next;
  }
  return wynik;
}

int najmniej_figur(Mozliwe_ruchy mozliwosci, int gracz)
{
  int minimum = INF;
  Stan_planszy * it = mozliwosci.lista;
  while(it != NULL)
  {
    minimum = min(minimum, licz_figury(*it,gracz));
    it = it->next;
  }
  return minimum;
}

void usun_zbite(Stan_planszy * S)
{
  Figura *it=S->F[0], *prev=NULL;
  while(it!=NULL)
  {
    if(it->zbita)
    {
      if(prev == NULL)
      {
        S->F[0] = it->next;
        free(it);
        it = S->F[0];
      }
      else
      {
        Figura * nastepna = it->next;
        prev->next = nastepna;
        free(it);
        it = nastepna;
      }
    }
    else
    {
      prev = it;
      it = it->next;
    }
  }
  it=S->F[1];
  prev=NULL;
  while(it!=NULL)
  {
    if(it->zbita)
    {
      if(prev == NULL)
      {
        S->F[1] = it->next;
        free(it);
        it = S->F[1];
      }
      else
      {
        Figura * nastepna = it->next;
        prev->next = nastepna;
        free(it);
        it = nastepna;
      }
    }
    else
    {
      prev = it;
      it = it->next;
    }
  }
}

void zrob_damki(Stan_planszy * X)
{
  Figura * it = X->F[0];
  while(it != NULL)
  {
    if(it->pozycja.posY == 0) it->pion = false;
    it = it->next;
  }
  it = X->F[1];
  while(it != NULL)
  {
    if(it->pozycja.posY == rozmiar_planszy-1) it->pion = false;
    it = it->next;
  }
}

Mozliwe_ruchy utnij_do_minimum(Mozliwe_ruchy mozliwosci, int minimum, int gracz)
{
  Stan_planszy * it = mozliwosci.lista, *prev = NULL;
  while(it != NULL)
  {
    if(licz_figury(*it,gracz) != minimum)
    {
      if(prev == NULL)
      {
        mozliwosci.lista = it->next;
        usun_stan_planszy(*it);
        free(it);
        it = mozliwosci.lista;
      }
      else
      {
        prev->next = it->next;
        usun_stan_planszy(*it);
        free(it);
        it = prev->next;
      }
    }
    else
    {
      prev = it;
      it = it->next;
    }
  }
  it = mozliwosci.lista;
  while(it!=NULL)
  {
    usun_zbite(it);
    zrob_damki(it);
    it = it->next;
  }
  return mozliwosci;
}

Figura * kopiuj_liste_figur(Figura * first)
{
  Figura * wynik = NULL, *it = first;
  while(it != NULL)
  {
    if(wynik == NULL) wynik = stworz_figure(it->pion,it->pozycja,NULL);
    else
    {
      wynik = stworz_figure(it->pion,it->pozycja,wynik);
    }
    wynik->zbita = it->zbita;
    it = it->next;
  }
  return wynik;
}

Stan_planszy * kopiuj_stan_planszy(Stan_planszy * X)
{
  Stan_planszy * kopia = (Stan_planszy*) malloc(sizeof(Stan_planszy));
  kopia->F[0] = kopiuj_liste_figur(X->F[0]);
  kopia->F[1] = kopiuj_liste_figur(X->F[1]);
  kopia->next = NULL;
  return kopia;
}

Figura * znajdz_figure(Figura * szukana, Stan_planszy * gdzie, int gracz)
{
  Figura * it = gdzie->F[gracz];
  while(it != NULL)
  {
    if(rowne_wspolrzedne(it->pozycja,szukana->pozycja)) return it;
    it = it->next;
  }
}

Wspolrzedna jaka_wspolrzedna_po_zbiciu(Wspolrzedna start, Wspolrzedna koniec)
{
  Wspolrzedna kierunek = znak_przemieszczenia(start,koniec);
  return dodaj_wspolrzedne(koniec,kierunek);
}

bool czy_zajeta_pozycja(Wspolrzedna cel, Stan_planszy * from)
{
  Figura * it = from->F[0];
  while(it != NULL)
  {
    if(rowne_wspolrzedne(cel,it->pozycja)) return true;
    it = it->next;
  }
  it = from->F[1];
  while(it != NULL)
  {
    if(rowne_wspolrzedne(cel,it->pozycja)) return true;
    it = it->next;
  }
  return false;
}

int liczba_figur_po_drodze(Wspolrzedna start, Wspolrzedna koniec, Wspolrzedna kierunek, Stan_planszy * from)
{
  int result = 0;
  Wspolrzedna aktualna = start;
  koniec = dodaj_wspolrzedne(koniec,kierunek);
  while(!rowne_wspolrzedne(aktualna,koniec))
  {
    if(czy_zajeta_pozycja(aktualna,from)) result++;
    aktualna = dodaj_wspolrzedne(aktualna,kierunek);
  }
  return result;
}

bool mozna_sie_ruszyc(Figura * ruszana, Wspolrzedna cel, Stan_planszy * from, int gracz)
{
  Wspolrzedna kierunek = znak_przemieszczenia(ruszana->pozycja,cel);
  if(rowne_wspolrzedne(kierunek,BRAK_POZYCJI)) return false;
  if(czy_zajeta_pozycja(cel,from)) return false;
  if(liczba_figur_po_drodze(dodaj_wspolrzedne(ruszana->pozycja,kierunek),cel,kierunek,from) > 0) return false;
  if(ruszana->pion)
  {
    if(gracz == 0 && kierunek.posY == 1) return false;
    if(gracz == 1 && kierunek.posY == -1) return false;
    if(!rowne_wspolrzedne(dodaj_wspolrzedne(ruszana->pozycja, kierunek),cel)) return false;
    return true;
  }
  else return true;
}

Mozliwe_ruchy nie_bijacy(Figura * ruszana, Stan_planszy * from, int gracz)
{
  Mozliwe_ruchy wynik;
  wynik.lista = NULL;
  int i, j;
  for(i=0; i<rozmiar_planszy; i++) for(j=0; j<rozmiar_planszy; j++)
  {
    if(mozna_sie_ruszyc(ruszana,stworz_wspolrzedna(i,j),from,gracz))
    {
      Stan_planszy * kopia = kopiuj_stan_planszy(from);
      Figura * modyfikowana = znajdz_figure(ruszana,kopia,gracz);
      (*modyfikowana).pozycja = stworz_wspolrzedna(i,j);
      dodaj_do_mozliwych_ruchow(&wynik,kopia);
    }
  }
  return wynik;
}

bool mozna_bic(Figura * bijaca, Figura * bita, Stan_planszy * s, int gracz)
{
  if(bita->zbita) return false;
  Wspolrzedna kierunek = znak_przemieszczenia(bijaca->pozycja,bita->pozycja);
  if(rowne_wspolrzedne(kierunek,BRAK_POZYCJI)) return false;
  Wspolrzedna koncowa = jaka_wspolrzedna_po_zbiciu(bijaca->pozycja, bita->pozycja);
  if(!nalezy_do_planszy(koncowa)) return false;
  if(czy_zajeta_pozycja(koncowa,s)) return false;
  int liczba_napotkanych = liczba_figur_po_drodze(dodaj_wspolrzedne(bijaca->pozycja,kierunek),koncowa,kierunek,s);
  if(liczba_napotkanych != 1) return false;
  if(bijaca->pion && !rowne_wspolrzedne(bita->pozycja,dodaj_wspolrzedne(bijaca->pozycja,kierunek))) return false;
  return true;
}

Mozliwe_ruchy ruch_bijacy(Figura * ruszana, Stan_planszy * from, int gracz)
{
  Mozliwe_ruchy wynik;
  wynik.lista = NULL;
  if(!pierwszy_ruch_bijacy) dodaj_do_mozliwych_ruchow(&wynik, from);
  else pierwszy_ruch_bijacy = false;
  int i, j;
  Figura * it = from->F[przeciwnik_gracza(gracz)];
  while(it != NULL)
  {
    if(mozna_bic(ruszana,it,from,gracz))
    {
      if(ruszana->pion)
      {
        Stan_planszy * kopia = kopiuj_stan_planszy(from);
        Figura * modyfikowana = znajdz_figure(ruszana,kopia,gracz);
        (*modyfikowana).pozycja = jaka_wspolrzedna_po_zbiciu(ruszana->pozycja,it->pozycja);
        Figura *zniszczona = znajdz_figure(it,kopia,przeciwnik_gracza(gracz));
        zniszczona->zbita = true;
        dodaj_mozliwe_ruchy(&wynik,ruch_bijacy(modyfikowana,kopia,gracz));
      }
      else
      {
        Wspolrzedna pozycja = jaka_wspolrzedna_po_zbiciu(ruszana->pozycja,it->pozycja);
        Wspolrzedna kierunek = znak_przemieszczenia(ruszana->pozycja,it->pozycja);
        while(true)
        {
          if(!nalezy_do_planszy(pozycja)) break;
          if(czy_zajeta_pozycja(pozycja,from)) break;
          Stan_planszy * kopia = kopiuj_stan_planszy(from);
          Figura * modyfikowana = znajdz_figure(ruszana,kopia,gracz);
          (*modyfikowana).pozycja = pozycja;
          Figura *zniszczona = znajdz_figure(it,kopia,przeciwnik_gracza(gracz));
          zniszczona->zbita = true;
          dodaj_mozliwe_ruchy(&wynik,ruch_bijacy(modyfikowana,kopia,gracz));
          pozycja = dodaj_wspolrzedne(pozycja, kierunek);
        }
      }
    }
    it = it->next;
  }
  return wynik;
}

Mozliwe_ruchy pobierz_mozliwe_ruchy(Stan_planszy * from, int gracz)
{
  Mozliwe_ruchy mozliwosci;
  mozliwosci.lista = NULL;
  Figura * it = from->F[gracz];
  while(it != NULL)
  {
    dodaj_mozliwe_ruchy(&mozliwosci,nie_bijacy(it,from,gracz));
    Stan_planszy * kopia = kopiuj_stan_planszy(from);
    pierwszy_ruch_bijacy = true;
    Figura * jt = znajdz_figure(it,kopia,gracz);
    dodaj_mozliwe_ruchy(&mozliwosci,ruch_bijacy(jt,kopia,gracz));
    it = it->next;
  }
  int minimum = najmniej_figur(mozliwosci,przeciwnik_gracza(aktualny_gracz));
  return utnij_do_minimum(mozliwosci,minimum,przeciwnik_gracza(aktualny_gracz));
}

int minimax_alfa(Stan_planszy s,int poziom, int alfa, int beta)
{
  if(stan_koncowy(s)) return uzytecznosc_stanu(s);
  if(poziom>=max_poziom) return heurystyczna_ocena_stanu(s);
  Mozliwe_ruchy stany_nastepne = pobierz_mozliwe_ruchy(&s,0);
  Stan_planszy * stan_nastepny = stany_nastepne.lista;
  while(stan_nastepny != NULL)
  {
    alfa = max(alfa,minimax_beta(*stan_nastepny,poziom+1,alfa,beta));
    if(alfa >= beta)
    {
      usun_mozliwe_ruchy(stany_nastepne);
      return beta;
    }
    stan_nastepny = stan_nastepny->next;
  }
  usun_mozliwe_ruchy(stany_nastepne);
  return alfa;
}

int minimax_beta(Stan_planszy s,int poziom, int alfa, int beta)
{
  if(stan_koncowy(s)) return uzytecznosc_stanu(s);
  if(poziom>=max_poziom) return heurystyczna_ocena_stanu(s);
  Mozliwe_ruchy stany_nastepne = pobierz_mozliwe_ruchy(&s,1);
  Stan_planszy * stan_nastepny = stany_nastepne.lista;
  while(stan_nastepny != NULL)
  {
    beta = min(beta,minimax_alfa(*stan_nastepny,poziom+1,alfa,beta));
    if(beta <= alfa)
    {
      usun_mozliwe_ruchy(stany_nastepne);
      return alfa;
    }
    stan_nastepny = stan_nastepny->next;
  }
  usun_mozliwe_ruchy(stany_nastepne);
  return beta;
}

void wypisz_stan_planszy_do_pliku(Stan_planszy * X, FILE * plik)
{
  Figura * it = X->F[0];
  fprintf(plik, "Figury gracza A: \n");
  while(it!=NULL)
  {
    if(it->pion) fprintf(plik,"Pion");
    else fprintf(plik,"Dama");
    fprintf(plik, " na pozycji (%d,%d) ", it->pozycja.posX, it->pozycja.posY);
    if(it->zbita) fprintf(plik, "zbite\n");
    else fprintf(plik, "niezbite\n");
    it = it->next;
  }
  fprintf(plik,"Figury gracza B:\n");
  it = X->F[1];
  while(it!=NULL)
  {
    if(it->pion) fprintf(plik,"Pion");
    else fprintf(plik,"Dama");
    fprintf(plik, " na pozycji (%d,%d) ", it->pozycja.posX, it->pozycja.posY);
    if(it->zbita) fprintf(plik, "zbite\n");
    else fprintf(plik, "niezbite\n");
    it = it->next;
  }
  fprintf(plik,"\n");
}

void wypisz_mozliwe_ruchy_do_pliku(Mozliwe_ruchy * X, FILE * plik)
{
  Stan_planszy * it = X->lista;
  if(it == NULL) fprintf(plik, "Mozliwe ruchy - pusty obiekt\n");
  else while(it != NULL)
  {
    wypisz_stan_planszy_do_pliku(it,plik);
    it = it->next;
  }
  fprintf(plik,"\n");
  printf("WYPISALEM MOZLIWE RUCHY DO PLIKU\n");
}

void kreska()
{
  int i, j;
  for(i=0; i<3; i++, printf("\n")) for(j=0; j<70; j++) printf("|");
}

Stan_planszy * ruch_komputera(Stan_planszy aktualny)
{
  Mozliwe_ruchy S = pobierz_mozliwe_ruchy(&aktualny,aktualny_gracz);
  int nr_najlepszego = -1;
  int wartosc_najlepszego;
  if(aktualny_gracz == 0) wartosc_najlepszego = -INF-1;
  else wartosc_najlepszego = INF+1;
  int nr_przegladanego = -1;
  Stan_planszy * it = S.lista;
  while(it != NULL)
  {
    nr_przegladanego++;
    int biezaca_wartosc;
    if(aktualny_gracz == 0)
    {
      biezaca_wartosc = minimax_beta(*it,0,-INF,INF);
      if(biezaca_wartosc > wartosc_najlepszego)
      {
        nr_najlepszego = nr_przegladanego;
        wartosc_najlepszego = biezaca_wartosc;
      }
    }
    else
    {
      biezaca_wartosc = minimax_alfa(*it,0,-INF,INF);
      if(biezaca_wartosc < wartosc_najlepszego)
      {
        nr_najlepszego = nr_przegladanego;
        wartosc_najlepszego = biezaca_wartosc;
      }
    }
    it = it->next;
  }
  it = S.lista;
  nr_przegladanego = -1;
  while(it != NULL)
  {
    nr_przegladanego++;
    if(nr_przegladanego == nr_najlepszego)
    {
      Stan_planszy * optymalny = kopiuj_stan_planszy(it);
      usun_mozliwe_ruchy(S);
      return optymalny;
    }
    it = it->next;
  }
}
