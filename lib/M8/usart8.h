/*
  Работа с USART
  Реализация функций для работы с RS485
  Version 1.05
  09.12.2008
  LP 09.12.2008
  Chip type : ATmega8
  Program type        : Application
  Clock frequency     : 7,372800 MHz
*/

#ifndef usart8_h
#define usart8_h

#include <mega8.h>
#include "m8lib.h"

typedef unsigned char BYTE;

interrupt [USART_RXC] void usart_rx_isr(void);

#define U485CTL PORTB.0 //ef_7
#define U485READ  0
#define U485WRITE 1

#define BR9600  0x2F   // 9600
#define BR38400 0x0B   // 38400
#define BR57600 0x07   // 57600

void USART_Init(BYTE BR);
// BR: 0x2F - 9600, 0x0B - 38400, 0x07 - 57600 


int UWasChar1(void);
char Ugetchar1(void);

void putchar1(char c);

#endif
