
#include "interfejs.h"

bool rowne_wspolrzedne(Wspolrzedna A, Wspolrzedna B)
{
  return A.posX == B.posX && A.posY == B.posY;
}

Wspolrzedna odejmij_wspolrzedne(Wspolrzedna A, Wspolrzedna B)
{
  Wspolrzedna wynik_odejmowania;
  wynik_odejmowania.posX = A.posX - B.posX;
  wynik_odejmowania.posY = A.posY - B.posY;
  return wynik_odejmowania;
}

Wspolrzedna dodaj_wspolrzedne(Wspolrzedna A, Wspolrzedna B)
{
  Wspolrzedna wynik_dodawania;
  wynik_dodawania.posX = A.posX + B.posX;
  wynik_dodawania.posY = A.posY + B.posY;
  return wynik_dodawania;
}

Wspolrzedna stworz_wspolrzedna(int posX, int posY)
{
  Wspolrzedna nowa;
  nowa.posX = posX;
  nowa.posY = posY;
  return nowa;
}

Figura * stworz_figure(bool pion, Wspolrzedna pozycja, Figura * next)
{
  Figura * nowa = (Figura*) malloc((sizeof(Figura)));
  nowa->pion = pion;
  nowa->zbita = false;
  nowa->pozycja = pozycja;
  nowa->next = next;
  return nowa;
}

bool nalezy_do_planszy(Wspolrzedna pozycja)
{
  return pozycja.posX >=0 && pozycja.posX < rozmiar_planszy && pozycja.posY >=0 && pozycja.posY < rozmiar_planszy;
}

Wspolrzedna znak_przemieszczenia(Wspolrzedna startowa, Wspolrzedna docelowa)
{
  Wspolrzedna wektor_przemieszczenia = odejmij_wspolrzedne(docelowa, startowa);
  if(abs(wektor_przemieszczenia.posX) != abs(wektor_przemieszczenia.posY)) return BRAK_POZYCJI;
  Wspolrzedna znak;
  znak.posX = sign(wektor_przemieszczenia.posX);
  znak.posY = sign(wektor_przemieszczenia.posY);
  return znak;
}

GtkWidget * wczytaj_obraz_pola_planszy(int typ_pola)
{
  char str[10];
  sprintf(str, "%d", typ_pola);
  strcat(str,".jpg");
  GtkWidget * image = gtk_image_new_from_file(str);
}

void zmien_typ_pola_planszy(PolePlanszy * rozpatrywane_pole_planszy, int nowy_typ)
{
  rozpatrywane_pole_planszy->typ_pola = nowy_typ;
  gtk_button_set_image(GTK_BUTTON(rozpatrywane_pole_planszy->button), wczytaj_obraz_pola_planszy(rozpatrywane_pole_planszy->typ_pola));
}

void zmien_wyswietlanie_pola_planszy(PolePlanszy * rozpatrywane_pole_planszy, int nowy_wyswietlany_typ)
{
  gtk_button_set_image(GTK_BUTTON(rozpatrywane_pole_planszy->button), wczytaj_obraz_pola_planszy(nowy_wyswietlany_typ));
}

void oglos_wygrana(int numer_gracza)
{
  GtkWidget *dialog;
  if(numer_gracza == 0)
  {
    dialog = gtk_message_dialog_new(GTK_WINDOW(okno_glowne_programu), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Wygrał gracz BIAŁY!");
  }
  else
  {
    dialog = gtk_message_dialog_new(GTK_WINDOW(okno_glowne_programu), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Wygrał gracz CZERWONY!");
  }
  gtk_window_set_title(GTK_WINDOW(dialog), "Partia rozstrzygnięta");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void oglos_remis()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(okno_glowne_programu), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Remis!");
  gtk_window_set_title(GTK_WINDOW(dialog), "Partia nierozstrzygnięta");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void zmien_obracanie()
{
  if(obracanie_planszy) obracanie_planszy = false;
  else obracanie_planszy = true;
}

void obroc_plansze()
{
  if(!obracanie_planszy) return;
  printf("Obrocilem plansze, aktualny gracz to %d\n", aktualny_gracz);
  int i, j;
  for(i=0; i<rozmiar_planszy; i++)
  {
    for(j=0; j<rozmiar_planszy; j++)
    {
      if(aktualny_gracz == 0) gtk_fixed_move(GTK_FIXED(wyswietlacz_planszy), plansza[j][i].button, j*rozmiar_pola_planszy, i*rozmiar_pola_planszy);
      else gtk_fixed_move(GTK_FIXED(wyswietlacz_planszy), plansza[rozmiar_planszy-1-j][rozmiar_planszy-1-i].button, j*rozmiar_pola_planszy, i*rozmiar_pola_planszy);
    }
  }
}

void display_new_game()
{
  obracanie_planszy = false;
  int i, j;
  for(i=0; i<rozmiar_planszy; i++)
  {
    for(j=0; j<rozmiar_planszy; j++)
    {
      gtk_fixed_put(GTK_FIXED(wyswietlacz_planszy), plansza[j][i].button, j*rozmiar_pola_planszy, i*rozmiar_pola_planszy);
      if((i+j)%2) zmien_typ_pola_planszy(&plansza[j][i], CIEMNE_POLE);
      else zmien_typ_pola_planszy(&plansza[j][i], JASNE_POLE);
      plansza[j][i].przynaleznosc = 2;
      plansza[j][i].zbita = false;
    }
  }
  gtk_label_set_markup(GTK_LABEL(okienko_informacyjne), "\n<span font='12'>Zaczyna gracz</span>\n<span bgcolor='#FFFFFF' font='20'>BIAŁY</span>");
  aktualny_gracz = 0;
  brak_bic = 0;
  czy_mozliwe_bicie = false;
  dlugosc_sekwencji_wcisniec = 0;
  najdluzsza_mozliwa_sekwencja = 0;
  Figura * it = figury_gracza[0], *nastepna;
  while(it != NULL)
  {
    nastepna = it->next;
    free(it);
    it = nastepna;
  }
  it = figury_gracza[1];
  while(it != NULL)
  {
    nastepna = it->next;
    free(it);
    it = nastepna;
  }
  wczytaj_pionki();
  if(czy_komputer[aktualny_gracz]) wykonaj_ruch_komputerem();
}

void czlowiek_vs_czlowiek()
{
  czy_komputer[0] = false;
  czy_komputer[1] = false;
  display_new_game();
  gtk_widget_show_all(okno_glowne_programu);
  gtk_widget_hide(menu_glowne_programu);
}

void czlowiek_vs_komputer()
{
  czy_komputer[0] = false;
  czy_komputer[1] = true;
  display_new_game();
  gtk_widget_show_all(okno_glowne_programu);
  gtk_widget_hide(menu_glowne_programu);
}

void komputer_vs_czlowiek()
{
  czy_komputer[0] = true;
  czy_komputer[1] = false;
  display_new_game();
  gtk_widget_show_all(okno_glowne_programu);
  gtk_widget_hide(menu_glowne_programu);
}

void komputer_vs_komputer()
{
  czy_komputer[0] = true;
  czy_komputer[1] = true;
  display_new_game();
  gtk_widget_show_all(okno_glowne_programu);
  gtk_widget_hide(menu_glowne_programu);
}

void wroc_do_menu_glowne()
{
  gtk_widget_hide(okno_glowne_programu);
  gtk_widget_show_all(menu_glowne_programu);
}

int main (int argc, char *argv[])
{
  BRAK_POZYCJI.posX = -5;
  BRAK_POZYCJI.posY = -5;

  gtk_init(&argc, &argv);

  menu_glowne_programu = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  GtkWidget * kontener_menu_glownego_programu = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add(GTK_CONTAINER(menu_glowne_programu), kontener_menu_glownego_programu);
  gtk_window_set_default_size(GTK_WINDOW(menu_glowne_programu), 200, 100);
  gtk_window_set_position(GTK_WINDOW(menu_glowne_programu), GTK_WIN_POS_CENTER);
  gtk_window_set_title (GTK_WINDOW(menu_glowne_programu), "Warcaby");
  g_signal_connect(G_OBJECT(menu_glowne_programu), "destroy", G_CALLBACK(gtk_main_quit), NULL);

  GtkWidget * human_vs_human = gtk_button_new_with_label("HUMAN vs HUMAN");
  GtkWidget * human_vs_ai = gtk_button_new_with_label("HUMAN vs AI");
  GtkWidget * ai_vs_human = gtk_button_new_with_label("AI vs HUMAN");
  GtkWidget * zakoncz_program = gtk_button_new_with_label("EXIT");
  gtk_container_add(GTK_CONTAINER(kontener_menu_glownego_programu), human_vs_human);
  gtk_container_add(GTK_CONTAINER(kontener_menu_glownego_programu), human_vs_ai);
  gtk_container_add(GTK_CONTAINER(kontener_menu_glownego_programu), ai_vs_human);
  gtk_container_add(GTK_CONTAINER(kontener_menu_glownego_programu), zakoncz_program);

  g_signal_connect(G_OBJECT(human_vs_human), "clicked", G_CALLBACK(czlowiek_vs_czlowiek), NULL);
  g_signal_connect(G_OBJECT(human_vs_ai), "clicked", G_CALLBACK(czlowiek_vs_komputer), NULL);
  g_signal_connect(G_OBJECT(ai_vs_human), "clicked", G_CALLBACK(komputer_vs_czlowiek), NULL);
  g_signal_connect(G_OBJECT(zakoncz_program), "clicked", G_CALLBACK(gtk_main_quit), NULL);

  okno_glowne_programu = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(okno_glowne_programu), rozmiar_planszy*rozmiar_pola_planszy+szerokosc_menu_programu, rozmiar_planszy*rozmiar_pola_planszy);
  gtk_window_set_position(GTK_WINDOW(okno_glowne_programu), GTK_WIN_POS_CENTER);
  gtk_window_set_title (GTK_WINDOW(okno_glowne_programu), "Warcaby");
  g_signal_connect(G_OBJECT(okno_glowne_programu), "destroy", G_CALLBACK(gtk_main_quit), NULL);

  GtkWidget * kontener_glowny_programu = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_container_add(GTK_CONTAINER(okno_glowne_programu), kontener_glowny_programu);

  wyswietlacz_planszy = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(kontener_glowny_programu), wyswietlacz_planszy);

  GtkWidget * vseparator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
  gtk_container_add(GTK_CONTAINER(kontener_glowny_programu), vseparator);

  GtkWidget * menu_programu = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add(GTK_CONTAINER(kontener_glowny_programu), menu_programu);

  int i, j;
  for(i=0; i<rozmiar_planszy; i++)
  {
    for(j=0; j<rozmiar_planszy; j++)
    {
      plansza[j][i].button = gtk_button_new();
      plansza[j][i].pozycja = stworz_wspolrzedna(j,i);
      gtk_widget_set_size_request(plansza[j][i].button, rozmiar_pola_planszy, rozmiar_pola_planszy);
      g_signal_connect(G_OBJECT(plansza[j][i].button), "clicked",G_CALLBACK(wcisniecie_pola_planszy),&plansza[j][i]);
    }
  }

  GtkWidget * new_game_button = gtk_button_new_with_label("Nowa gra");
  GtkWidget * turn_button = gtk_button_new_with_label("Obracanie\nszachownicy");
  GtkWidget * give_up_button = gtk_button_new_with_label("Rezygnuję");

  gtk_container_add(GTK_CONTAINER(menu_programu), new_game_button);
  g_signal_connect(G_OBJECT(new_game_button), "clicked",G_CALLBACK(display_new_game),NULL);
  gtk_container_add(GTK_CONTAINER(menu_programu), turn_button);
  g_signal_connect(G_OBJECT(turn_button), "clicked",G_CALLBACK(zmien_obracanie),NULL);
  gtk_container_add(GTK_CONTAINER(menu_programu), give_up_button);
  g_signal_connect(G_OBJECT(give_up_button), "clicked",G_CALLBACK(wroc_do_menu_glowne),NULL);

  GtkWidget * hseparator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_container_add(GTK_CONTAINER(menu_programu), hseparator);

  okienko_informacyjne = gtk_label_new(NULL);
  gtk_container_add(GTK_CONTAINER(menu_programu), okienko_informacyjne);

  gtk_widget_show_all(menu_glowne_programu);
  gtk_main();
  return 0;
}
