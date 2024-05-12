#include<stdio.h>
#include<conio.h>
#include<dos.h>

int Vstup=0xFFFF; 
int Vystup=0xFFFF;   

void displej (int cislo)
  {
    Vystup=Vystup & 0xFFF8; // bity 0-2 do nuly
    Vystup=Vystup+cislo;    // nastaveni bcd kombinace
    outport(0x300,Vystup);  // odeslani na port
  }

void led_nahoru (void)
  {
    Vystup=Vystup & 0xFFF7; // bit 3 do nuly
    outport(0x300,Vystup);
  }

void led_dolu (void)
  {
    Vystup=Vystup & 0xFFEF; // bit 4 do nuly
    outport(0x300,Vystup);
  }

void led_vypnuta(void)
  {
    Vystup=Vystup | 0x0018; // bity 3 a 4 do jednicky
    outport(0x300,Vystup);
  }

void svetlo_zapnuto(void)
  {
    Vystup=Vystup & 0xFFDF; // bit 5 do nuly
    outport(0x300,Vystup);
  }

void svetlo_vypnuto(void)
  {
    Vystup=Vystup | 0x0020; // bit 5 do jednicky
    outport(0x300,Vystup);
  }

void motor_zastavit(void)
  {
    Vystup=Vystup | 0x00C0; // bit 6 a 7 do jednicky
    outport(0x300,Vystup);
  }

void motor_nahoru(void)
  {
    Vystup=Vystup & 0xFFBF; // bit 6 do nuly
    outport(0x300,Vystup);
  }

void motor_dolu(void)
  {
    Vystup=Vystup & 0xFF3F; // bit 6 a 7 do nuly
    outport(0x300,Vystup);
  }

int jake_patro(void)
  {
    Vstup=inport(0x300);
    if (Vstup==0) return(0);
    if ((Vstup & 0x0001)==0) return(1); // 1. patro
    if ((Vstup & 0x0002)==0) return(2); // 2. patro
    if ((Vstup & 0x0004)==0) return(3); // 3. patro
    if ((Vstup & 0x0008)==0) return(4); // 4. patro
    return(0);
  }

int tlacitko_na_patre(void)
  {
    Vstup=inport(0x300);
    if ((Vstup & 0x0100)==0) return(1);
    if ((Vstup & 0x0200)==0) return(2);
    if ((Vstup & 0x0400)==0) return(3);
    if ((Vstup & 0x0800)==0) return(4);
    return(0);
  }

int tlacitko_v_kabine(void)
  {
    Vstup=inport(0x300);
    if ((Vstup & 0x1000)==0) return(1);
    if ((Vstup & 0x2000)==0) return(2);
    if ((Vstup & 0x4000)==0) return(3);
    if ((Vstup & 0x8000)==0) return(4);
    return(0);
  }

int dvere_zavrene(void)
  {
    Vstup=inport(0x300);
    if ((Vstup & 0x0020)==0) return(0);
  }

void volani_z_patra(void)
{
    int akt_poloha;
    akt_poloha = jake_patro();
    if (dvere_zavrene()==1)
    {
      if (tlacitko_na_patre() > akt_poloha)
      {
        sipka_nahoru();
        motor_nahoru();
        svetlo_zapnuto();
        while (jake_patro() != tlacitko_na_patre());
        motor_zastavit();
        led_vypnuta();
        svetlo_vypnuto();
      } 
    
      else if (tlacitko_na_patre() < akt_poloha)
      {
        sipka_dolu();
        motor_dolu();
        svetlo_zapnuto();
        while (jake_patro() != tlacitko_na_patre());
        motor_zastavit();
        led_vypnuta();
        svetlo_vypnuto();
      }
    }
}

void volani_z_kabiny(void)
{
    int akt_poloha;
    akt_poloha = jake_patro();
    if (dvere_zavrene()==1)
    {
      if (tlacitko_v_kabine() > akt_poloha)
      {
        sipka_nahoru();
        motor_nahoru();
        svetlo_zapnuto();
        while (jake_patro() != tlacitko_v_kabine());
        motor_zastavit();
        led_vypnuta();
        svetlo_vypnuto();
      } 
    
      else if (tlacitko_v_kabine() < akt_poloha)
      {
        sipka_dolu();
        motor_dolu();
        svetlo_zapnuto();
        while (jake_patro() != tlacitko_v_kabine());
        motor_zastavit();
        led_vypnuta();
        svetlo_vypnuto();
      }
    }
}

while (1) // Neustále se opakuje
{
    volani_z_patra(); // Ovladač z patra
    volani_z_kabiny();// Ovladač z kabiny
}