#include <stdio.h>
#include <conio.h>
#include <dos.h>

// Deklarace funkcí
void send_bit(int bit);
void send_startbit(void);
void send_mux(int x);
void send_num(int num);
void send_alpha(int c);
void display_number(char input);
void check_last_bit(void); // Funkce pro kontrolu posledního bitu posuvného registru

int main(void) 
{
    char input;
    while (1)
    {
        input = getch(); // Přečte vstup z klávesnice
        if (input >= '1' && input <= '9')
        {
            display_number(input); // Zobrazí číslo na 7-segmentovém displeji
        }
        else if ((input >= 'A' && input <= 'Z'))
        {
            send_mux(0); // Nastaví 14-segmentový displej
            send_startbit(); // Startovací bit
            send_alpha(input - 'A' + 10); // Zobrazí písmeno na 14-segmentovém displeji
        }
    }
 return 0;
}

void display_number(char input)
{
    int numbers[10] = {0x0C3F, 0x0406, 0x00DB, 0x008F, 0x00E6, 0x2069, 0x00FD, 0x0007, 0x00FF, 0x00EF};
    send_mux(1); // Nastaví 7-segmentový displej
    send_startbit(); // Startovací bit
    send_num(input - '0'); // Zobrazí číslo na 7-segmentovém displeji
}

void delay (int x)
{
    int i=0;
    for (i=0;i<x;i++);
}

void send_bit(int i)
{
    if (i == 1) 
    {
        outportb(0x301,0x01);
        delay(5);
        outportb(0x301,0x03);
        delay(5);
        outportb(0x301,0x01);
        delay(5);
    }
    else
    {
        outportb(0x301,0x00);
        delay(5);
        outportb(0x301,0x02);
        delay(5);
        outportb(0x301,0x00);
        delay(5);
    }
}

void send_startbit(void)
{
    send_bit(1); // Odeslání startovacího bitu
}

void send_mux(int x)
{
    int mux_a[5] = {1, 0, 0, 0, 0}; // mux A + padding
    int mux_b[5] = {0, 1, 0, 0, 0}; // mux B + padding
    int i = 0;
    if (x == 0)
        for (i = 0; i < 5; i++)
            send_bit(mux_a[i]);
    if (x == 1)
        for (i = 0; i < 5; i++)
            send_bit(mux_b[i]);
}

void send_num(int num)
{
    int idx;
    for (idx = 0; idx < 15; idx++)
    {
        if ((num & (1 << idx)) == 0)
            send_bit(0); // Pokud je bit nula, odeslat nulu
        else send_bit(1); // Pokud je bit jedna, odeslat jedničku
    }
}

void send_alpha(int c)
{
    int characters[26] = {0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x3D, 0x76, 0x30, 0x1E, 
                          0x75, 0x38, 0x15, 0x37, 0x3F, 0x73, 0x6B, 0x33, 0x6D, 0x78, 
                          0x3E, 0x3E, 0x2A, 0x76, 0x6E, 0x5B};

    int idx;
    for (idx = 0; idx < 14; idx++)
    {
        if ((characters[c] & (1 << idx)) == 0)
            send_bit(0); // Pokud je bit nula, odeslat nulu
        else send_bit(1); // Pokud je bit jedna, odeslat jedničku
    }
}

void check_last_bit(void)
{
    // Čteme poslední bit posuvného registru
    int last_bit = (inportb(0x300) >> 35) & 1;

    // Pokud je poslední bit 1, vykopírovat předchozích 35 bitů do vyrovnávacího registru a vygenerovat signál RESET
    if (last_bit == 1)
    {
        // Zkopírovat předchozích 35 bitů do vyrovnávacího registru
        for (int i = 0; i < 35; i++)
        {
            int bit_to_copy = (inportb(0x300) >> i) & 1; // Čteme bit z posuvného registru
            if (bit_to_copy == 1)
                outportb(0x301, 0x0F);
            else
                outportb(0x301, 0x00);
            delay(5);
        }
        // Generovat signál RESET
        outportb(0x301, 0x04); // Nastavit výstup pro RESET
        delay(5);
        outportb(0x301, 0x00); // Nastavení zpět na nulu
        delay(5);
    }
}

