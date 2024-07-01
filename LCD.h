#ifndef __LCD_H_
#define __LCD_H_


#include <msp430.h>
#include "string.h"

#define MAX_DISP_X   128
#define MAX_DISP_Y   8

#define SIZE_WORD   5

void InitLCD(int flag, int Volume_set);
void WriteLCD(int, char);
void PageWorkLCD(unsigned int page, char data);
void WriteWordEn(unsigned int x, unsigned int y, char Word);
void WriteWord(unsigned int x, unsigned int y, char const *Word);
unsigned char reverse (unsigned char d);
void dispGotoXY (unsigned char x, unsigned char y);
void dispClear(void);

int CheckWord(char Word);

void ConvertShort(short *src, char *dest);

#endif
