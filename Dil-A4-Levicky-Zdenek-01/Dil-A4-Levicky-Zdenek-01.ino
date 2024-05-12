#include <avr/io.h>         // Knihovna pro vstupně/výstupní operace
#include <avr/interrupt.h>  // Knihovna pro obsluhu přerušení
#include <util/delay.h>     // Knihovna pro zpoždění

// Definice pinů pro maticovou klávesnici
#define Radky 3
#define Sloupce 3


// Definice symbolů pro každé tlačítko
char klavesnice_symboly[Radky][Sloupce] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'}
};

// Proměnné pro uchování stisknutých kláves
char klavesa[9];
int klavesa_count = 0;

// Funkce pro čtení stisknuté klávesy
char klavesnice()
  {
    for (int i = 0; i < Sloupce; i++) {
        PORTF = 0x0F | (1 << (i + 4)); // Nastavíme PORTF, aby aktivně snímal řádky maticové klávesnice
        _delay_ms(1);

        for (int j = 0; j < Radky; j++) {
            if (!(PINF & (1 << j))) {    // Kontrola, zda je nějaké tlačítko stisknuto
                while (!(PINF & (1 << j))); // Čekej, dokud se tlačítko nepustí
                return klavesnice_symboly[j][i];  // Vrátíme příslušný symbol tlačítka
            }
        }
    }
    return 0; // Žádná klávesa nebyla stisknuta
  }

// Funkce pro ovládání maticového displeje
void displej_symboly(char symbol)  // Zde jsou definovány matice pro zobrazení čísel 1 až 9
                                   // Každá matice reprezentuje jedno číslo na displeji
{
    int cislo_jedna[7][8] = {
    {1,1,1,1,1,1,1,1},
    {0,1,1,1,0,1,1,1},
    {1,0,1,1,0,1,1,1},
    {0,0,1,1,0,1,1,1},
    {1,1,0,1,0,1,1,1},
    {0,1,0,1,0,1,1,1},
    {1,0,0,1,1,1,1,1},
  };

    int cislo_dva[7][8] = {
    {1,1,1,1,1,1,1,1},
    {0,1,1,1,0,0,0,1},
    {1,0,1,1,0,1,1,1},
    {0,0,1,1,0,0,0,1},
    {1,1,0,1,1,1,0,1},
    {0,1,0,1,0,0,0,1},
    {1,0,0,1,1,1,1,1},
  };

    int cislo_tri[7][8] = {
    {1,1,1,1,1,1,1,1},
    {0,1,1,1,0,0,0,1},
    {1,0,1,1,0,1,1,1},
    {0,0,1,1,0,0,0,1},
    {1,1,0,1,0,1,1,1},
    {0,1,0,1,0,0,0,1},
    {1,0,0,1,1,1,1,1},
  };

    int cislo_ctyri[7][8] = {
    {1,1,1,1,1,1,1,1},
    {0,1,1,1,0,1,0,1},
    {1,0,1,1,0,1,0,1},
    {0,0,1,1,0,0,0,1},
   {1,1,0,1,0,1,1,1},
    {0,1,0,1,0,1,1,1},
   {1,0,0,1,1,1,1,1},
  };

    int cislo_pet[7][8] = {
    {1,1,1,1,1,1,1,1},
    {0,1,1,1,0,0,0,1},
    {1,0,1,1,1,1,0,1},
    {0,0,1,1,0,0,0,1},
    {1,1,0,1,0,1,1,1},
    {0,1,0,1,0,0,0,1},
    {1,0,0,1,1,1,1,1},
  };

    int cislo_sest[7][8] = {
    {1,1,1,1,1,1,1,1},
    {0,1,1,1,0,0,0,1},
    {1,0,1,1,1,1,0,1},
    {0,0,1,1,0,0,0,1},
    {1,1,0,1,0,1,0,1},
    {0,1,0,1,0,0,0,1},
   {1,0,0,1,1,1,1,1},
  };

    int cislo_sedm[7][8] = {
    {1,1,1,1,1,1,1,1},
    {0,1,1,1,0,0,0,1},
    {1,0,1,1,0,1,1,1},
    {0,0,1,1,1,0,1,1},
    {1,1,0,1,1,1,0,1},
    {0,1,0,1,1,1,0,1},
   {1,0,0,1,1,1,1,1},
  };

    int cislo_osm[7][8] = {
    {1,1,1,1,1,1,1,1},
    {0,1,1,1,0,0,0,1},
    {1,0,1,1,0,1,0,1},
    {0,0,1,1,0,0,0,1},
    {1,1,0,1,0,1,0,1},
    {0,1,0,1,0,1,0,1},
    {1,0,0,1,1,1,1,1},
  };

    int cislo_devet[7][8] = {
    {1,1,1,1,1,1,1,1},
    {0,1,1,1,0,0,0,1},
    {1,0,1,1,0,1,0,1},
    {0,0,1,1,0,0,0,1},
    {1,1,0,1,0,1,1,1},
    {0,1,0,1,0,1,1,1},
    {1,0,0,1,1,1,1,1},
  };

}

// Hlavní funkce
int main() 
  {
    DDRB = 0xFF;   // Nastavíme port B jako výstupní
    DDRE = 0x0F;   // Nastavíme port E, aby dolní 4 bity byly výstupní pro ovládání maticového displeje
    DDRF = 0x0F;   // Nastavíme port F, aby dolní 4 bity byly výstupní pro ovládání maticové klávesnice
    while (1) {
        char a = klavesnice();  // Zavoláte funkci pro čtení stisknutých kláves
        if (a != 0 && klavesa_count < 9) {
            klavesa[klavesa_count++] = a; // Uložíte stisknutý symbol
            displej_symboly(a);          // Zobrazíte stisknutý symbol na displeji
        }
     }

    return 0;
  }
