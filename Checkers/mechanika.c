#include "mechanika.h"

int liczba_figur_gracza(int numer_gracza)
{
  Figura * it = figury_gracza[numer_gracza];
  int wynik = 0;
  while(it != NULL)
  {
    wynik++;
    it = it->next;
  }
  return wynik;
}

void dodaj_do_listy_figur_gracza(int numer_gracza, Figura * dodawana_figura)
{
  dodawana_figura->next = figury_gracza[numer_gracza];
  figury_gracza[numer_gracza] = dodawana_figura;
}

void usun_z_listy_figur_gracza(int numer_gracza, Wspolrzedna pozycja)
{
  Figura * przegladana_figura = figury_gracza[numer_gracza];
  Figura * poprzednio_przegladana = NULL;
  while(!rowne_wspolrzedne(przegladana_figura->pozycja, pozycja))
  {
    poprzednio_przegladana = przegladana_figura;
    przegladana_figura = przegladana_figura->next;
  }
  if(poprzednio_przegladana == NULL)
  {
    figury_gracza[numer_gracza] = przegladana_figura->next;
  }
  else
  {
    poprzednio_przegladana->next = przegladana_figura->next;
  }
  free(przegladana_figura);
}

void zmien_pozycje_na_liscie_figur_gracza(int numer_gracza, Wspolrzedna stara, Wspolrzedna nowa)
{
  Figura * przegladana_figura = figury_gracza[numer_gracza];
  while(!rowne_wspolrzedne(przegladana_figura->pozycja, stara))
  {
    przegladana_figura = przegladana_figura->next;
  }
  przegladana_figura->pozycja = nowa;
}

void zmien_typ_na_liscie_figur_gracza(int numer_gracza, Wspolrzedna pozycja, bool czy_pion)
{
  Figura * przegladana_figura = figury_gracza[numer_gracza];
  while(!rowne_wspolrzedne(przegladana_figura->pozycja, pozycja))
  {
    przegladana_figura = przegladana_figura->next;
  }
  przegladana_figura->pion = czy_pion;
}

void wczytaj_pionki()
{
  figury_gracza[0] = NULL;
  figury_gracza[1] = NULL;
  int i, j;

  for(i=rozmiar_planszy-liczba_rzedow_pionkow; i<rozmiar_planszy; i++)
  {
    for(j=0; j<rozmiar_planszy; j++)
    {
      if(plansza[j][i].typ_pola == CIEMNE_POLE)
      {
        zmien_typ_pola_planszy(&plansza[j][i], WHITE_PION);
        plansza[j][i].pozycja = stworz_wspolrzedna(j,i);
        plansza[j][i].przynaleznosc = 0;
        Figura * nowy_pionek = stworz_figure(true,stworz_wspolrzedna(j,i),NULL);
        dodaj_do_listy_figur_gracza(0,nowy_pionek);
      }
    }
  }

  for(i=0; i<liczba_rzedow_pionkow; i++)
  {
    for(j=0; j<rozmiar_planszy; j++)
    {
      if(plansza[j][i].typ_pola == CIEMNE_POLE)
      {
        zmien_typ_pola_planszy(&plansza[j][i], RED_PION);
        plansza[j][i].pozycja = stworz_wspolrzedna(j,i);
        plansza[j][i].przynaleznosc = 1;
        Figura * nowy_pionek = stworz_figure(true,stworz_wspolrzedna(j,i),NULL);
        dodaj_do_listy_figur_gracza(1,nowy_pionek);
      }
    }
  }
}

Wspolrzedna pierwsza_wspolrzedna_po_wykonaniu_bicia(Figura * rozpatrywana_figura, Figura * figura_przeciwnika)
{
  Wspolrzedna wektor_przemieszczenia_rozpatrywanej_figury = odejmij_wspolrzedne(figura_przeciwnika->pozycja, rozpatrywana_figura->pozycja);
  Wspolrzedna znak_przemieszczenia_rozpatrywanej_figury = znak_przemieszczenia(rozpatrywana_figura->pozycja, figura_przeciwnika->pozycja);
  if(rowne_wspolrzedne(znak_przemieszczenia_rozpatrywanej_figury, BRAK_POZYCJI)) return BRAK_POZYCJI;
  wektor_przemieszczenia_rozpatrywanej_figury = dodaj_wspolrzedne(wektor_przemieszczenia_rozpatrywanej_figury, znak_przemieszczenia_rozpatrywanej_figury);
  if(!nalezy_do_planszy(dodaj_wspolrzedne(rozpatrywana_figura->pozycja, wektor_przemieszczenia_rozpatrywanej_figury))) return BRAK_POZYCJI;
  return dodaj_wspolrzedne(rozpatrywana_figura->pozycja, wektor_przemieszczenia_rozpatrywanej_figury);
}

int czyja_pozycja(Wspolrzedna pozycja)
{
  Figura * przegladana_figura;
  przegladana_figura = figury_gracza[0];
  while(przegladana_figura != NULL)
  {
    if(rowne_wspolrzedne(przegladana_figura->pozycja, pozycja)) return 0;
    przegladana_figura = przegladana_figura->next;
  }
  przegladana_figura = figury_gracza[1];
  while(przegladana_figura != NULL)
  {
    if(rowne_wspolrzedne(przegladana_figura->pozycja, pozycja)) return 1;
    przegladana_figura = przegladana_figura->next;
  }
  return 2;
}

int liczba_przeciwnych_figur_na_drodze(int numer_gracza, Wspolrzedna start, Wspolrzedna koniec)
{
  int wynik = 0;
  Wspolrzedna kierunek_bicia = znak_przemieszczenia(start,koniec);
  Wspolrzedna aktualna_pozycja = dodaj_wspolrzedne(start,kierunek_bicia);
  while(!rowne_wspolrzedne(aktualna_pozycja,koniec))
  {
    if(czyja_pozycja(aktualna_pozycja) == numer_gracza) return -1;
    if(czyja_pozycja(aktualna_pozycja) == przeciwnik_gracza(numer_gracza)) wynik++;
    aktualna_pozycja = dodaj_wspolrzedne(aktualna_pozycja, kierunek_bicia);
  }
  return wynik;
}

bool mozliwe_bicie(Figura * rozpatrywana_figura, Figura * figura_przeciwnika)
{
  if(figura_przeciwnika->zbita) return false;
  Wspolrzedna pozycja = pierwsza_wspolrzedna_po_wykonaniu_bicia(rozpatrywana_figura, figura_przeciwnika);
  if(rowne_wspolrzedne(pozycja, BRAK_POZYCJI)) return false;
  if(czyja_pozycja(pozycja) != 2) return false;
  Wspolrzedna kierunek_bicia = znak_przemieszczenia(rozpatrywana_figura->pozycja, figura_przeciwnika->pozycja);
  if(rozpatrywana_figura->pion)
  {
    if(!rowne_wspolrzedne(dodaj_wspolrzedne(rozpatrywana_figura->pozycja,kierunek_bicia),figura_przeciwnika->pozycja)) return false;
  }
  else
  {
    int ile_wrogow = liczba_przeciwnych_figur_na_drodze(aktualny_gracz,rozpatrywana_figura->pozycja,pozycja);
    if(ile_wrogow == -1) return false;
    if(ile_wrogow > 1) return false;
  }
  return true;
}

void wykonaj_bicie_pionem(Figura * rozpatrywana_figura, Figura * figura_przeciwnika)
{
  Wspolrzedna pozycja = pierwsza_wspolrzedna_po_wykonaniu_bicia(rozpatrywana_figura, figura_przeciwnika);
  rozpatrywana_figura->pozycja = pozycja;
  figura_przeciwnika->zbita = true;
}

void wykonaj_bicie_damka(Figura * rozpatrywana_figura, Figura * figura_przeciwnika, Wspolrzedna pozycja)
{
  rozpatrywana_figura->pozycja = pozycja;
  figura_przeciwnika->zbita = true;
}

int dfs_najdluzsze_mozliwe_bicie(Figura * rozpatrywana_figura)
{
  int maksimum = 0;
  Figura * figura_przeciwnika = figury_gracza[przeciwnik_gracza(aktualny_gracz)];
  while(figura_przeciwnika != NULL)
  {
    if(mozliwe_bicie(rozpatrywana_figura, figura_przeciwnika))
    {
      if(rozpatrywana_figura->pion)
      {
        Figura * kopia_rozpatrywanej_figury = stworz_figure(rozpatrywana_figura->pion, rozpatrywana_figura->pozycja, rozpatrywana_figura->next);
        Figura * kopia_figury_przeciwnika = stworz_figure(figura_przeciwnika->pion, figura_przeciwnika->pozycja, figura_przeciwnika->next);
        wykonaj_bicie_pionem(rozpatrywana_figura, figura_przeciwnika);
        maksimum = max(maksimum, 1+dfs_najdluzsze_mozliwe_bicie(rozpatrywana_figura));
        *rozpatrywana_figura = *kopia_rozpatrywanej_figury;
        free(kopia_rozpatrywanej_figury);
        figura_przeciwnika->zbita = false;
        figura_przeciwnika = kopia_figury_przeciwnika->next;
        free(kopia_figury_przeciwnika);
      }
      else
      {
        Wspolrzedna gdzie_staje_po_biciu = pierwsza_wspolrzedna_po_wykonaniu_bicia(rozpatrywana_figura,figura_przeciwnika);
        Wspolrzedna kierunek_bicia = znak_przemieszczenia(rozpatrywana_figura->pozycja, figura_przeciwnika->pozycja);
        Figura * kopia_rozpatrywanej_figury = stworz_figure(rozpatrywana_figura->pion, rozpatrywana_figura->pozycja, rozpatrywana_figura->next);
        Figura * kopia_figury_przeciwnika = stworz_figure(figura_przeciwnika->pion, figura_przeciwnika->pozycja, figura_przeciwnika->next);
        while(nalezy_do_planszy(gdzie_staje_po_biciu) && czyja_pozycja(gdzie_staje_po_biciu) == 2)
        {

          wykonaj_bicie_damka(rozpatrywana_figura, figura_przeciwnika, gdzie_staje_po_biciu);
          maksimum = max(maksimum, 1+dfs_najdluzsze_mozliwe_bicie(rozpatrywana_figura));
          gdzie_staje_po_biciu = dodaj_wspolrzedne(gdzie_staje_po_biciu,kierunek_bicia);
        }
        *rozpatrywana_figura = *kopia_rozpatrywanej_figury;
        free(kopia_rozpatrywanej_figury);
        figura_przeciwnika->zbita = false;
        figura_przeciwnika = kopia_figury_przeciwnika->next;
        free(kopia_figury_przeciwnika);
      }
    }
    else
    {
      figura_przeciwnika = figura_przeciwnika->next;
    }
  }
  return maksimum;
}

int najdluzsze_mozliwe_bicie()
{
  int maksimum = 0;
  Figura * rozpatrywana_figura = figury_gracza[aktualny_gracz];
  while(rozpatrywana_figura != NULL)
  {
    maksimum = max(maksimum, dfs_najdluzsze_mozliwe_bicie(rozpatrywana_figura));
    rozpatrywana_figura = rozpatrywana_figura->next;
  }
  return maksimum;
}

void wykonaj_kopie_stanu_gry()
{
  kopia_aktualnie_wybranej_figury = aktualnie_wybrana_figura;
  Figura * kopiowana_figura = figury_gracza[0];
  kopia_figur_gracza[0] = NULL;
  while(kopiowana_figura != NULL)
  {
    Figura * kopia_figury = (Figura*) malloc(sizeof(Figura));
    *kopia_figury = *kopiowana_figura;
    kopia_figury->next = kopia_figur_gracza[0];
    kopia_figur_gracza[0] = kopia_figury;
    kopiowana_figura = kopiowana_figura->next;
  }
  kopiowana_figura = figury_gracza[1];
  kopia_figur_gracza[1] = NULL;
  while(kopiowana_figura != NULL)
  {
    Figura * kopia_figury = (Figura*) malloc(sizeof(Figura));
    *kopia_figury = *kopiowana_figura;
    kopia_figury->next = kopia_figur_gracza[1];
    kopia_figur_gracza[1] = kopia_figury;
    kopiowana_figura = kopiowana_figura->next;
  }
  int i, j;
  for(i=0; i<rozmiar_planszy; i++)
  {
    for(j=0; j<rozmiar_planszy; j++)
    {
      kopia_planszy[j][i] = plansza[j][i];
    }
  }
}

void zaznacz_pole_planszy(PolePlanszy * pole_planszy_do_zaznaczenia)
{
  zmien_wyswietlanie_pola_planszy(pole_planszy_do_zaznaczenia, pole_planszy_do_zaznaczenia->typ_pola+RED_DAMA);
}

bool czy_sie_cofa(int numer_gracza, Wspolrzedna kierunek_ruchu)
{
  if(numer_gracza == 0 && kierunek_ruchu.posY == 1) return true;
  if(numer_gracza == 1 && kierunek_ruchu.posY == -1) return true;
  return false;
}

bool mozliwy_ruch(PolePlanszy * rozpatrywana_figura, PolePlanszy * nastepne_pole)
{
  Wspolrzedna kierunek_ruchu = znak_przemieszczenia(rozpatrywana_figura->pozycja, nastepne_pole->pozycja);
  if(rowne_wspolrzedne(kierunek_ruchu, BRAK_POZYCJI)) return false;
  int liczba_figur_przeciwnika_po_drodze_ruchu = 0;
  Wspolrzedna aktualnie_rozpatrywana_wspolrzedna = dodaj_wspolrzedne(rozpatrywana_figura->pozycja, kierunek_ruchu);
  while(!rowne_wspolrzedne(aktualnie_rozpatrywana_wspolrzedna, nastepne_pole->pozycja))
  {
    if(plansza[aktualnie_rozpatrywana_wspolrzedna.posX][aktualnie_rozpatrywana_wspolrzedna.posY].przynaleznosc == aktualny_gracz) return false;
    if(plansza[aktualnie_rozpatrywana_wspolrzedna.posX][aktualnie_rozpatrywana_wspolrzedna.posY].zbita) return false;
    if(plansza[aktualnie_rozpatrywana_wspolrzedna.posX][aktualnie_rozpatrywana_wspolrzedna.posY].przynaleznosc == przeciwnik_gracza(aktualny_gracz)) liczba_figur_przeciwnika_po_drodze_ruchu++;
    aktualnie_rozpatrywana_wspolrzedna = dodaj_wspolrzedne(aktualnie_rozpatrywana_wspolrzedna, kierunek_ruchu);
  }
  if(liczba_figur_przeciwnika_po_drodze_ruchu > 1) return false;
  bool czy_pion = rozpatrywana_figura->typ_pola == WHITE_PION || rozpatrywana_figura->typ_pola == RED_PION;
  if(liczba_figur_przeciwnika_po_drodze_ruchu == 1 && czy_pion && !rowne_wspolrzedne(dodaj_wspolrzedne(kierunek_ruchu, dodaj_wspolrzedne(rozpatrywana_figura->pozycja, kierunek_ruchu)), nastepne_pole->pozycja)) return false;
  if(liczba_figur_przeciwnika_po_drodze_ruchu == 0 && czy_pion && !rowne_wspolrzedne(dodaj_wspolrzedne(rozpatrywana_figura->pozycja, kierunek_ruchu), nastepne_pole->pozycja)) return false;
  if(liczba_figur_przeciwnika_po_drodze_ruchu == 0 && czy_pion && czy_sie_cofa(aktualny_gracz,kierunek_ruchu)) return false;
  if(czy_mozliwe_bicie && liczba_figur_przeciwnika_po_drodze_ruchu == 0) return false;
  return true;
}

void odznacz_pole_planszy(PolePlanszy * pole_planszy_do_odznaczenia)
{
  zmien_wyswietlanie_pola_planszy(pole_planszy_do_odznaczenia, pole_planszy_do_odznaczenia->typ_pola);
}

void wykonaj_ruch(PolePlanszy * rozpatrywana_figura, PolePlanszy * nastepne_pole)
{
  Wspolrzedna pozycja_figury_przeciwnika = BRAK_POZYCJI;
  Wspolrzedna kierunek_ruchu = znak_przemieszczenia(rozpatrywana_figura->pozycja, nastepne_pole->pozycja);
  Wspolrzedna aktualnie_rozpatrywana_wspolrzedna = dodaj_wspolrzedne(rozpatrywana_figura->pozycja, kierunek_ruchu);
  while(!rowne_wspolrzedne(aktualnie_rozpatrywana_wspolrzedna, nastepne_pole->pozycja))
  {
    if(plansza[aktualnie_rozpatrywana_wspolrzedna.posX][aktualnie_rozpatrywana_wspolrzedna.posY].przynaleznosc == przeciwnik_gracza(aktualny_gracz)) pozycja_figury_przeciwnika = aktualnie_rozpatrywana_wspolrzedna;
    aktualnie_rozpatrywana_wspolrzedna = dodaj_wspolrzedne(aktualnie_rozpatrywana_wspolrzedna, kierunek_ruchu);
  }
  if(!rowne_wspolrzedne(pozycja_figury_przeciwnika, BRAK_POZYCJI))
  {
    plansza[pozycja_figury_przeciwnika.posX][pozycja_figury_przeciwnika.posY].zbita = true;
  }
  zmien_pozycje_na_liscie_figur_gracza(aktualny_gracz,rozpatrywana_figura->pozycja, nastepne_pole->pozycja);
  nastepne_pole->przynaleznosc = aktualny_gracz;
  rozpatrywana_figura->przynaleznosc = 2;
  zmien_typ_pola_planszy(nastepne_pole, rozpatrywana_figura->typ_pola);
  zmien_typ_pola_planszy(rozpatrywana_figura,CIEMNE_POLE);
  zaznacz_pole_planszy(nastepne_pole);
}

void przywroc_kopie_stanu_gry()
{
  aktualnie_wybrana_figura = kopia_aktualnie_wybranej_figury;
  dlugosc_sekwencji_wcisniec = 0;
  int i, j;
  for(i=0; i<rozmiar_planszy; i++)
  {
    for(j=0; j<rozmiar_planszy; j++)
    {
      plansza[j][i] = kopia_planszy[j][i];
      zmien_typ_pola_planszy(&plansza[j][i], plansza[j][i].typ_pola);
    }
  }
  Figura * rozpatrywana_figura = figury_gracza[0];
  Figura * nastepna_do_usuniecia;
  while(rozpatrywana_figura != NULL)
  {
    nastepna_do_usuniecia = rozpatrywana_figura->next;
    free(rozpatrywana_figura);
    rozpatrywana_figura = nastepna_do_usuniecia;
  }
  rozpatrywana_figura = figury_gracza[1];
  while(rozpatrywana_figura != NULL)
  {
    nastepna_do_usuniecia = rozpatrywana_figura->next;
    free(rozpatrywana_figura);
    rozpatrywana_figura = nastepna_do_usuniecia;
  }
  figury_gracza[0] = kopia_figur_gracza[0];
  figury_gracza[1] = kopia_figur_gracza[1];
}

Stan_planszy pobierz_aktualny_stan()
{
  Stan_planszy aktualny;
  aktualny.F[0] = NULL;
  aktualny.F[1] = NULL;
  aktualny.next = NULL;
  Figura * it = figury_gracza[0];
  while(it!=NULL)
  {
    Figura * kopia = stworz_figure(it->pion,it->pozycja,aktualny.F[0]);
    aktualny.F[0] = kopia;
    it = it->next;
  }
  it = figury_gracza[1];
  while(it!=NULL)
  {
    Figura * kopia = stworz_figure(it->pion,it->pozycja,aktualny.F[1]);
    aktualny.F[1] = kopia;
    it = it->next;
  }
  return aktualny;
}

void aktualizuj_plansze(Stan_planszy nowy)
{
  int i, j;
  for(i=0; i<rozmiar_planszy; i++)
  {
    for(j=0; j<rozmiar_planszy; j++)
    {
      if((i+j)%2)
      {
        zmien_typ_pola_planszy(&plansza[i][j],CIEMNE_POLE);
        //gtk_widget_queue_draw(plansza[i][j].button);
        plansza[i][j].przynaleznosc = 2;
      }
    }
  }
  Figura * it = nowy.F[0];
  while(it != NULL)
  {
    if(it->pion) zmien_typ_pola_planszy(&plansza[it->pozycja.posX][it->pozycja.posY],WHITE_PION);
    else zmien_typ_pola_planszy(&plansza[it->pozycja.posX][it->pozycja.posY],WHITE_DAMA);
    plansza[it->pozycja.posX][it->pozycja.posY].przynaleznosc = 0;
    it = it->next;
  }
  it = nowy.F[1];
  while(it != NULL)
  {
    if(it->pion) zmien_typ_pola_planszy(&plansza[it->pozycja.posX][it->pozycja.posY],RED_PION);
    else zmien_typ_pola_planszy(&plansza[it->pozycja.posX][it->pozycja.posY],RED_DAMA);
    plansza[it->pozycja.posX][it->pozycja.posY].przynaleznosc = 1;
    it = it->next;
  }
  Figura * nastepna;
  it = figury_gracza[0];
  while(it != NULL)
  {
    nastepna = it->next;
    free(it);
    it = nastepna;
  }
  figury_gracza[0] = nowy.F[0];
  it = figury_gracza[1];
  while(it != NULL)
  {
    nastepna = it->next;
    free(it);
    it = nastepna;
  }
  figury_gracza[1] = nowy.F[1];
}

bool same_damki()
{
  Figura * it = figury_gracza[0];
  while(it != NULL)
  {
    if(it->pion) return false;
    it = it->next;
  }
  it = figury_gracza[1];
  while(it != NULL)
  {
    if(it->pion) return false;
    it = it->next;
  }
  return true;
}

bool partia_zostala_rozstrzygnieta()
{
  if(najdluzsza_mozliwa_sekwencja == 1) return true;
  if(liczba_figur_gracza(aktualny_gracz) == 0) return true;
  if(liczba_figur_gracza(przeciwnik_gracza(aktualny_gracz)) == 0) return true;
  if(same_damki() && brak_bic == 30) return true;
  return false;
}

void zakoncz_partie()
{
  if(najdluzsza_mozliwa_sekwencja == 1) oglos_wygrana(przeciwnik_gracza(aktualny_gracz));
  else if(liczba_figur_gracza(aktualny_gracz) == 0) oglos_wygrana(przeciwnik_gracza(aktualny_gracz));
  else if(liczba_figur_gracza(przeciwnik_gracza(aktualny_gracz)) == 0) oglos_wygrana(aktualny_gracz);
  else if(same_damki() && brak_bic == 30) oglos_remis();
}

bool nastapila_blokada()
{
  Figura * rozpatrywana = figury_gracza[aktualny_gracz];
  while(rozpatrywana != NULL)
  {
    int i, j;
    for(i=-1; i<3; i+=2) for(j=-1; j<3; j+=2)
    {
      Wspolrzedna nowa_pozycja = dodaj_wspolrzedne(rozpatrywana->pozycja, stworz_wspolrzedna(i,j));
      if(!nalezy_do_planszy(nowa_pozycja)) continue;
      if(rozpatrywana->pion && czy_sie_cofa(aktualny_gracz, stworz_wspolrzedna(i,j))) continue;
      if(czyja_pozycja(nowa_pozycja) == 2) return false;
    }
    rozpatrywana = rozpatrywana->next;
  }
  return true;
}

bool wykonaj_ruch_komputerem()
{
  obroc_plansze();
  najdluzsza_mozliwa_sekwencja = najdluzsze_mozliwe_bicie()+1;
  czy_mozliwe_bicie = najdluzsza_mozliwa_sekwencja > 1;
  if(czy_mozliwe_bicie) brak_bic = 0;
  else brak_bic++;
  if(najdluzsza_mozliwa_sekwencja == 1 && !nastapila_blokada()) najdluzsza_mozliwa_sekwencja = 2;
  if(partia_zostala_rozstrzygnieta())
  {
    zakoncz_partie();
    return false;
  }
  if(aktualny_gracz==0) gtk_label_set_markup(GTK_LABEL(okienko_informacyjne), "\n<span font='12'>Ruch ma gracz</span>\n<span bgcolor='#FFFFFF' font='20'>BIAŁY</span>");
  else gtk_label_set_markup(GTK_LABEL(okienko_informacyjne), "\n<span font='12'>Ruch ma gracz</span>\n<span bgcolor='#FF0000' font='16'>CZERWONY</span>");
  Stan_planszy aktualny_stan = pobierz_aktualny_stan();
  Stan_planszy * nastepny_stan = ruch_komputera(aktualny_stan);
  aktualizuj_plansze(*nastepny_stan);
  usun_stan_planszy(aktualny_stan);
  free(nastepny_stan);
  if(partia_zostala_rozstrzygnieta()) zakoncz_partie();
  aktualny_gracz = przeciwnik_gracza(aktualny_gracz);
  if(aktualny_gracz==0) gtk_label_set_markup(GTK_LABEL(okienko_informacyjne), "\n<span font='12'>Ruch ma gracz</span>\n<span bgcolor='#FFFFFF' font='20'>BIAŁY</span>");
  else gtk_label_set_markup(GTK_LABEL(okienko_informacyjne), "\n<span font='12'>Ruch ma gracz</span>\n<span bgcolor='#FF0000' font='16'>CZERWONY</span>");
  obroc_plansze();
  if(czy_komputer[aktualny_gracz]) return wykonaj_ruch_komputerem();
  return true;
}

bool zakoncz_ruch()
{
  int i, j;
  for(i=0; i<rozmiar_planszy; i++) for(j=0; j<rozmiar_planszy; j++)
  {
    if(plansza[i][j].zbita)
    {
      plansza[i][j].zbita = false;
      plansza[i][j].przynaleznosc = 2;
      zmien_typ_pola_planszy(&plansza[i][j], CIEMNE_POLE);
      usun_z_listy_figur_gracza(przeciwnik_gracza(aktualny_gracz),plansza[i][j].pozycja);
    }
  }
  dlugosc_sekwencji_wcisniec = 0;
  odznacz_pole_planszy(aktualnie_wybrana_figura);
  if(aktualnie_wybrana_figura->typ_pola == WHITE_PION && aktualnie_wybrana_figura->pozycja.posY == 0)
  {
    zmien_typ_pola_planszy(aktualnie_wybrana_figura,WHITE_DAMA);
    zmien_typ_na_liscie_figur_gracza(aktualny_gracz,aktualnie_wybrana_figura->pozycja,false);
  }
  if(aktualnie_wybrana_figura->typ_pola == RED_PION && aktualnie_wybrana_figura->pozycja.posY == rozmiar_planszy-1)
  {
    zmien_typ_pola_planszy(aktualnie_wybrana_figura,RED_DAMA);
    zmien_typ_na_liscie_figur_gracza(aktualny_gracz,aktualnie_wybrana_figura->pozycja,false);
  }
  aktualny_gracz = przeciwnik_gracza(aktualny_gracz);
  obroc_plansze();

  if(czy_komputer[aktualny_gracz]) return wykonaj_ruch_komputerem();
  return true;
}

void wcisniecie_pola_planszy(GtkWidget * button, PolePlanszy * wcisniete_pole_planszy)
{
  if(aktualny_gracz == 0) gtk_label_set_markup(GTK_LABEL(okienko_informacyjne), "\n<span font='12'>Ruch ma gracz</span>\n<span bgcolor='#FFFFFF' font='20'>BIAŁY</span>");
  else gtk_label_set_markup(GTK_LABEL(okienko_informacyjne), "\n<span font='12'>Ruch ma gracz</span>\n<span bgcolor='#FF0000' font='16'>CZERWONY</span>");
  obroc_plansze();
  if(czy_komputer[aktualny_gracz]) return;
  if(dlugosc_sekwencji_wcisniec == 0)
  {
    if(wcisniete_pole_planszy->przynaleznosc != aktualny_gracz) return;
    najdluzsza_mozliwa_sekwencja = najdluzsze_mozliwe_bicie()+1;
    czy_mozliwe_bicie = najdluzsza_mozliwa_sekwencja > 1;
    if(czy_mozliwe_bicie) brak_bic = 0;
    else brak_bic++;
    if(najdluzsza_mozliwa_sekwencja == 1 && !nastapila_blokada()) najdluzsza_mozliwa_sekwencja = 2;
    if(najdluzsza_mozliwa_sekwencja != 1)
    {
      aktualnie_wybrana_figura = wcisniete_pole_planszy;
      wykonaj_kopie_stanu_gry();
      zaznacz_pole_planszy(wcisniete_pole_planszy);
    }
  }
  else
  {
    if(dlugosc_sekwencji_wcisniec > 0 && wcisniete_pole_planszy == aktualnie_wybrana_figura)
    {
      przywroc_kopie_stanu_gry();
      return;
    }
    if(wcisniete_pole_planszy->typ_pola != CIEMNE_POLE) return;
    if(mozliwy_ruch(aktualnie_wybrana_figura, wcisniete_pole_planszy))
    {
      wykonaj_ruch(aktualnie_wybrana_figura, wcisniete_pole_planszy);
      aktualnie_wybrana_figura = wcisniete_pole_planszy;
    }
    else
    {
      przywroc_kopie_stanu_gry();
      return;
    }
  }
  dlugosc_sekwencji_wcisniec++;
  if(najdluzsza_mozliwa_sekwencja != 1 && dlugosc_sekwencji_wcisniec == najdluzsza_mozliwa_sekwencja) if(!zakoncz_ruch()) return;
  if(aktualny_gracz==0) gtk_label_set_markup(GTK_LABEL(okienko_informacyjne), "\n<span font='12'>Ruch ma gracz</span>\n<span bgcolor='#FFFFFF' font='20'>BIAŁY</span>");
  else gtk_label_set_markup(GTK_LABEL(okienko_informacyjne), "\n<span font='12'>Ruch ma gracz</span>\n<span bgcolor='#FF0000' font='16'>CZERWONY</span>");
  if(partia_zostala_rozstrzygnieta())
  {
    zakoncz_partie();
  }
}
