#include "funkcjaocen.h"

int punkty_za_figury_gracza(Stan_planszy s, int gracz)
{
  int result = 0;
  Figura * it = s.F[gracz];
  while(it != NULL)
  {
    if(it->pion) result+=PUNKT_ZA_PIONKA;
    else result+=PUNKT_ZA_DAMKE;
    it = it->next;
  }
  return result;
}

int ile_figur_ma_gracz(Stan_planszy s, int gracz)
{
  int result = 0;
  Figura * it = s.F[gracz];
  while(it != NULL)
  {
    result++;
    it = it->next;
  }
  return result;
}

int punkty_za_ustawienie_figur(Stan_planszy s, int gracz)
{
  int result = 0;
  Figura * it = s.F[gracz];
  int cwiartka = rozmiar_planszy/4;
  while(it != NULL)
  {
    if(it->pion)
    {
      if(gracz == 0)
      {
        if(it->pozycja.posY < 1*cwiartka) result += PUNKT_ZA_POZIOM_I;
        else if(it->pozycja.posY < 2*cwiartka) result += PUNKT_ZA_POZIOM_II;
        else if(it->pozycja.posY < 3*cwiartka) result += PUNKT_ZA_POZIOM_III;
        else result += PUNKT_ZA_POZIOM_IV;
      }
      else
      {
        if(it->pozycja.posY >= 3*cwiartka) result += PUNKT_ZA_POZIOM_I;
        else if(it->pozycja.posY >= 2*cwiartka) result += PUNKT_ZA_POZIOM_II;
        else if(it->pozycja.posY >= 1*cwiartka) result += PUNKT_ZA_POZIOM_III;
        else result += PUNKT_ZA_POZIOM_IV;
      }
    }
    it = it->next;
  }
  return result;
}

int czy_chroni_baze(Stan_planszy s, int gracz)
{
  int result = 0;
  Figura * it = s.F[gracz];
  while(it != NULL)
  {
    if(gracz == 0 && it->pozycja.posY == rozmiar_planszy-1) result++;
    if(gracz == 1 && it->pozycja.posY == 0) result++;
    it = it->next;
  }
  if(result == 4) return 1;
  else return 0;
}

bool stan_koncowy(Stan_planszy s)
{
  if(ile_figur_ma_gracz(s,0) == 0) return true;
  if(ile_figur_ma_gracz(s,1) == 0) return true;
  Mozliwe_ruchy gracza_A = pobierz_mozliwe_ruchy(&s,0);
  if(gracza_A.lista == NULL) return true;
  Mozliwe_ruchy gracza_B = pobierz_mozliwe_ruchy(&s,1);
  if(gracza_B.lista == NULL) return true;
  return false;
}

int uzytecznosc_stanu(Stan_planszy s)
{
  if(ile_figur_ma_gracz(s,0) == 0) return -INF;
  if(ile_figur_ma_gracz(s,1) == 0) return INF;
  Mozliwe_ruchy gracza_A = pobierz_mozliwe_ruchy(&s,0);
  if(gracza_A.lista == NULL) return -INF;
  Mozliwe_ruchy gracza_B = pobierz_mozliwe_ruchy(&s,1);
  if(gracza_B.lista == NULL) return INF;
}

int heurystyczna_ocena_stanu(Stan_planszy s)
{
  //printf("Oceniam heurystycznie ten stan planszy:\n");
  //wypisz_stan_planszy(&s);
  int A = (punkty_za_figury_gracza(s,0) - punkty_za_figury_gracza(s,1));
  int B = (punkty_za_ustawienie_figur(s,0) - punkty_za_ustawienie_figur(s,1));
  int C = (czy_chroni_baze(s,0) - czy_chroni_baze(s,1));
  int result = A + B + C;
  //printf("Ocena to %d\n\n", result);
  return result;
}
