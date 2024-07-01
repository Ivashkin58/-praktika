#include "LCD.h"

void main(void)
{
    unsigned char TempBufText[256];
    unsigned int i = 0;
    //  Остановка сторожевого таймера
    WDTCTL = WDTPW + WDTHOLD;

    //  Конфигурирование работы от внешнего резонатора
    //  SR &= ~OSCOFF;
    BCSCTL1 |= XTS;
    do
    {
        IFG1 &= ~OFIFG;                         // Clear OSCFault flag
        for (i = 0xFF; i > 0; i--);         // Time for flag to set
    }
    while (IFG1 & OFIFG);                     // OSCFault flag still set?
    BCSCTL2 |= SELM1 + SELM0;                    // Select LFXT1CLK


    P6SEL &= 0xF0; //A0, E, Res, CS1
    P6DIR |= 0x0F; // ����� P6.0-P6.3 - ������

    InitLCD(1, 10);

    memset(TempBufText, 0x00, 256);
    ConvertShort((short *)"ТЕСТОВЫЙ  ЗАПУСК", TempBufText);
    WriteWord(20, 3, TempBufText);

    do
    {

    }while(1);
}
