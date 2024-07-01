#include "LCD.h"
#include "SystemRus5x7.h"


unsigned char reverse (unsigned char d)
{
    unsigned char result;
    static const unsigned char revTable[] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
    unsigned char a = d >> 4;
    unsigned char a1 = revTable[a];
    unsigned char b = d & 0x0F;
    unsigned char b1 = revTable[b];

    result = a1 |(b1<<4);

    return result;
}


void InitLCD(int flag, int Volume_set)
{
    unsigned int i;
    // IF flag = 1 -> Initilization When the built-in power is not being
    // used immediately after turning on the power

    P4SEL &= 0x7F;		// ���� �4.7 ����������� ��� GPIO //0x3f
    P4DIR |= 0x80;
    P4OUT |= 0x80;


    P1SEL &= 0x00;      // // ����� �1.0-P1.7 ����������� ��� GPIO
    P1DIR = 0xff;       // ����� �1.0-P1.7 ����������� ��� ������

    P6OUT &= (~0x04); //res = 0
    WriteLCD(0, reverse(0xAE));
    for(i = 0; i < 50000; i++)
        asm(" nop");

    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");

    P6OUT |= 0x04; //res = 1
    WriteLCD(0, reverse(0xe2)); // reset//

    WriteLCD(0, reverse(0xAF));
    WriteLCD(0, reverse(0xA2));   // 1/9 bias
    WriteLCD(0, reverse(0xA0));   // ADC = normal
    WriteLCD(0, reverse(0xC0));   // common output direction normal
    WriteLCD(0, reverse(0x26));   // V5 ratio //26
    WriteLCD(0, reverse(0x81)); // Electronic volume control //default
    WriteLCD(0,reverse(Volume_set));

    WriteLCD(0, reverse(0x2F));   // Power Control Set Command(page 34): booster+voltage regulator+voltage follower

    WriteLCD(0, reverse(0xe3)); // NOP

    WriteLCD(0, reverse(0xa4));
    WriteLCD(0, reverse(0xa6)); // display normal
    WriteLCD(0, reverse(0xac));

    WriteLCD(0, reverse(0x40));

    WriteLCD(0, reverse(0xe3)); // NOP


    dispClear();
}
//////////////

void dispClear(void)
{
  unsigned char x, y;

  for (y = 0; y < MAX_DISP_Y; y++)
  {
    dispGotoXY (0, y);
    for (x = 0; x < MAX_DISP_X; x++)
    {
        WriteLCD(1, 0x00);
    }
  }
  dispGotoXY (0, 0);
}

unsigned char reverse_Y(unsigned char y)
{
    switch(y)
    {
    case 0: return 7;
    case 1: return 6;
    case 2: return 5;
    case 3: return 4;
    case 4: return 3;
    case 5: return 2;
    case 6: return 1;
    case 7: return 0;
    default:
        return y;
    }
}

void dispGotoXY(unsigned char x, unsigned char y)
{


    y = reverse_Y(y);
    if(y >= MAX_DISP_Y)
        y = 0;
    if(x >= MAX_DISP_X)
        x = 0;

    WriteLCD(0, reverse(0xB0|y));
    WriteLCD(0, reverse(0x10|(x >> 4)));
    WriteLCD(0, reverse(x & 0x0f));
}


//////////////

void WriteLCD(int A0, char command)
{
    int i;

    if (A0 == 1)
        P6OUT |= (0x01); // A0 = 1
    else
        P6OUT &= ~(0x01); // A0 = 0

    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    P6OUT &= (~(0x02)); // E = 0
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    P1OUT = command; // write

    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    P6OUT &=  (~(0x08)); // cs1 = 0
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");

    P4OUT &= 0x7F; // R/W = 0
    P1DIR = 0xFF;
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    P6OUT |= 0x02; // E = 1
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");

    asm(" nop");
    asm(" nop");

    asm(" nop");
    asm(" nop");
    P6OUT &= (~(0x02)); // E = 0
    asm(" nop");
    asm(" nop");
    asm(" nop");

    asm(" nop");
       asm(" nop");
       asm(" nop");
    asm(" nop");
    P4OUT |= ~(0x7F); // R/W = 1
    asm(" nop");
    asm(" nop");
    P6OUT |=  (0x08); // cs1 = 1

    for(i = 0; i < 3; i++)
        asm(" nop");
}


void WriteWord(unsigned int x, unsigned int y, char const *Word)
{
    int size = strlen(Word);
    unsigned int i;//,j;

    for(i = 0; i < size; i++)
    {
        WriteWordEn(x, y, Word[i]);
        x+=SIZE_WORD+1;
    }
    WriteLCD(0, reverse(0xAF));
}

// Перевод русских символов (0xd0 0xXX) в нормальный вид (0xXX)
void ConvertShort(short *src, char *dest)
{
    if((src == NULL)||(dest == NULL)) return;
    int i, size = strlen(src);

    for(i = 0; i < (size); i++)
    {
        dest[i] = (src[i] & 0xFF00) >> 8;
    }
    dest[size] = '\0';

}

void PageWorkLCD(unsigned int page, char data){
    unsigned int i;
    dispGotoXY(0, page);
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    for(i = 0; i < MAX_DISP_X; i++)
    {
        WriteLCD(1, data);
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
    }
}
//
void WriteWordEn(unsigned int x, unsigned int y,char Word){
    int NumberWord = 0;
    unsigned int i = 0;
    NumberWord = CheckWord(Word);
    if (NumberWord != -1)
    {
        dispGotoXY(x, y);
        for(i = 0; i < 5; i++)
        {
            WriteLCD(1, SystemRus[NumberWord][i]);
        }
        WriteLCD(1, 0x00);
    }
}


int CheckWord(char Word){
    int number = 0;
    ///// A(ru) -> A(en) /////
            if ((Word == 0x90)||(Word == 0xC0)) Word = 'A';
            if ((Word == 0x92)||(Word == 0xC2)) Word = 'B';
            if ((Word == 0x95)||(Word == 0xC5)) Word = 'E';
            if ((Word == 0x9A)||(Word == 0xCA)) Word = 'K';
            if ((Word == 0x9C)||(Word == 0xCC)) Word = 'M';
            if ((Word == 0x9D)||(Word == 0xCD)) Word = 'H';
            if (((Word == 0x9E)||(Word == '0')||(Word == 0xCE))) Word = 'O';
            if ((Word == 0xA0)||(Word == 0xD0)) Word = 'P';
            if ((Word == 0xA1)||(Word == 0xD1)) Word = 'C';
            if ((Word == 0xA2)||(Word == 0xD2)) Word = 'T';
            if ((Word == 0xA5)||(Word == 0xD5)) Word = 'X';

            switch (Word){
            case ' ':
                number = 0;
                break;
            case '!':
                number = 1;
                break;
            case '#':
                number = 2;
                break;
            case '$':
                number = 3;
                break;
            case '%':
                number = 4;
                break;
            case '&':
                number = 5;
                break;
            case '(':
                number = 6;
                break;
            case ')':
                number = 7;
                break;
            case '*':
                number = 8;
                break;
            case '+':
                number = 9;
                break;
            case ',':
                number = 10;
                break;
            case '-':
                number = 11;
                break;
            case '.':
                number = 12;
                break;
            case '/':
                number = 13;
                break;
            case '1':
                number = 14;
                break;
            case '2':
                number = 15;
                break;
            case '3':
                number = 16;
                break;
            case '4':
                number = 17;
                break;
            case '5':
                number = 18;
                break;
            case '6':
                number = 19;
                break;
            case '7':
                number = 20;
                break;
            case '8':
                number = 21;
                break;
            case '9':
                number = 22;
                break;
            case ':':
                number = 23;
                break;
            case ';':
                number = 24;
                break;
            case '<':
                number = 25;
                break;
            case '=':
                number = 26;
                break;
            case '>':
                number = 27;
                break;
            case '?':
                number = 28;
                break;
            case '@':
                number = 29;
                break;
            case 'A':
                number = 30;
                break;
            case 'B':
                number = 31;
                break;
            case 'C':
                number = 32;
                break;
            case 'D':
                number = 33;
                break;
            case 'E':
                number = 34;
                break;
            case 'F':
                number = 35;
                break;
            case 'G':
                number = 36;
                break;
            case 'H':
                number = 37;
                break;
            case 'I':
                number = 38;
                break;
            case 'J':
                number = 39;
                break;
            case 'K':
                number = 40;
                break;
            case 'L':
                number = 41;
                break;
            case 'M':
                number = 42;
                break;
            case 'N':
                number = 43;
                break;
            case 'O':
                number = 44;
                break;
            case 'P':
                number = 45;
                break;
            case 'Q':
                number = 46;
                break;
            case 'R':
                number = 47;
                break;
            case 'S':
                number = 48;
                break;
            case 'T':
                number = 49;
                break;
            case 'U':
                number = 50;
                break;
            case 'V':
                number = 51;
                break;
            case 'W':
                number = 52;
                break;
            case 'X':
                number = 53;
                break;
            case 'Y':
                number = 54;
                break;
            case 'Z':
                number = 55;
                break;
            case '[':
                number = 56;
                break;
            case ']':
                number = 57;
                break;
            case '^':
                number = 58;
                break;
            case '_':
                number = 59;
                break;
            case '{':
                number = 60;
                break;
            case '|':
                number = 61;
                break;
            case '}':
                number = 62;
                break;
            case 0xC1:
            case 0x91:
                number = 63;
                break;
            case 0xC3:
            case 0x93:
                number = 64;
                break;
            case 0xC4:
            case 0x94:
                number = 65;
                break;
            case 0xC6:
            case 0x96:
                number = 66;
                break;
            case 0xC7:
            case 0x97:
                number = 67;
                break;
            case 0xC8:
            case 0x98:
                number = 68;
                break;
            case 0xC9:
            case 0x99:
                number = 69;
                break;
            case 0xCB:
            case 0x9B:
                number = 70;
                break;
            case 0xCF:
            case 0x9F:
                number = 71;
                break;
            case 0xD3:
            case 0xA3:
                number = 72;
                break;
            case 0xD4:
            case 0xA4:
                number = 73;
                break;
            case 0xD6:
            case 0xA6:
                number = 74;
                break;
            case 0xD7:
            case 0xA7:
                number = 75;
                break;
            case 0xD8:
            case 0xA8:
                number = 76;
                break;
            case 0xD9:
            case 0xA9:
                number = 77;
                break;
            case 0xDA:
            case 0xAA:
                number = 78;
                break;
            case 0xDB:
            case 0xAB:
                number = 79;
                break;
            case 0xDC:
            case 0xAC:
                number = 80;
                break;
            case 0xDD:
            case 0xAD:
                number = 81;
                break;
            case 0xDE:
            case 0xAE:
                number = 82;
                break;
            case 0xDF:
            case 0xAF:
                number = 83;
                break;
            case '~':
                number = 84;
                break;
            default:
                number = -1;
                break;
            }
            return number;
}
