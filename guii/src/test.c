#include <16F877A.h>
#fuses NOWDT, PUT, XT, NOPROTECT
#use delay(clock = 4000000)
#use fast_io(b)

#byte intcon = 0x000B
#define RB4 PIN_B4
#define RB5 PIN_B5
#define RB6 PIN_B6
#define RB7 PIN_B7
#bit RBIF = intcon .0 //dinh nghia co ngat RB
#bit RBIE = intcon .3 //dinh nghia bit cho phep ngat RB
#define tb1 PIN_B0
#define tb2 PIN_B1
#define tb3 PIN_B2
#define tb4 PIN_B3

#use rs232(baud = 9600, xmit = pin_c6, rcv = pin_c7) // khai bao uart
int btn, btn2, btn3, btn4 = 0;

// Chuong trinh ngat
#int_RB
void ngat_RB()
{
    if ((RBIF) && (RBIE))
    {
        //Kiem tra sw1
        {
            if (input(RB4) == 0)
            {
                btn++;
                output_high(tb1);
                putc('A');
                if (btn % 2 == 0)
                {
                    output_low(tb1);
                    btn = 0;
                    putc('a');
                }
            }
        }
        //Kiem tra sw2
        {
            if (input(RB5) == 0)
            {
                btn2++;
                output_high(tb2);
                putc('B');
            }
            if (btn2 % 2 == 0)
            {
                output_low(tb2);
                btn2 = 0;
                putc('b');
            }
        }
        //Kiem tra sw3
        {
            if (input(RB6) == 0)
            {
                btn3++;
                output_high(tb3);
                putc('c');
            }
            if (btn3 % 2 == 0)
            {
                output_low(tb3);
                btn3 = 0;
                putc('c');
            }
        }
        //Kiem tra sw4
        {
            if (input(RB7) == 0)
            {
                btn4++;
                output_high(tb4);
                putc('d');
            }
            if (btn4 % 2 == 0)
            {
                output_low(tb4);
                btn4 = 0;
                putc('d');
            }
        }
        RBIF = 0; //Xoa co ngat RB
    }
}

void main()
{
    //PORT_B_PULLUPS(0XFF);
    set_tris_b(0b11110000); // rb_4-rb7 input
    output_b(0b00001111);
    enable_interrupts(global);
    enable_interrupts(int_RB);
    ext_int_edge(H_to_L);

    while (true)
    {
        Unsigned char c = getch();
        if (c == 'h')
        {
            output_high(tb1);
        }
        if (c == 'j')
        {
            output_high(tb2);
        }
        if (c == 'k')
        {
            output_high(tb3);
        }
        if (c == 'l')
        {
            output_high(tb4);
        }
        if (c == '1')
        {
            output_low(tb1);
        }
        if (c == '2')
        {
            output_low(tb2);
        }
        if (c == '2')
        {
            output_low(tb3);
        }
        if (c == '3')
        {
            output_low(tb4);
        }
    }
}