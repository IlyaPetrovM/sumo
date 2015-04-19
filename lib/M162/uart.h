/*
  Работа с USART0 и USART1
  Реализация функций для работы с RS485
  Version 1.8
  09.12.2008
  LP: 01.11.2010
  Chip type : ATmega162
  Program type        : Application
  Clock frequency     : 7,372800 MHz
*/

#ifndef uart_h
#define uart_h

#include <mega162.h>

#pragma used+

#define USART_DISABLE_INTERRUPT UCSR0B=0x18
#define USART_ENABLE_INTERRUPT  UCSR0B=0x98

// Эти переменные нужны для ловли байта из последовательного порта                                                   
extern char USART_CHAR0;
extern char USART_CHAR1;

#define U485CTL PORTB.0   //ef_B0

#define U485READ  0
#define U485WRITE 1

interrupt [USART0_RXC] void usart0_rx_isr(void);
// USART0 Receiver interrupt service routine

interrupt [USART1_RXC] void usart1_rx_isr(void);
// USART1 Receiver interrupt service routine

#define BR9600  0x2F   // 9600
#define BR38400 0x0B   // 38400
#define BR57600 0x07   // 57600

void USART0_Init(unsigned char BR);
// USART0 initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART1 Receiver: On
// USART1 Transmitter: On
// USART1 Mode: Asynchronous
// USART1 Baud rate: BR
// BR: 0x2F - 9600, 0x0B - 38400, 0x07 - 57600 

void USART1_Init(unsigned char BR);
// USART1 initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART1 Receiver: On
// USART1 Transmitter: On
// USART1 Mode: Asynchronous
// USART1 Baud rate: BR
// BR: 0x0B - 38400, 0x07 - 57600 

char Getch0(void);
// Чтение символа

//----------------------------------------------------------
// RS-485
//----------------------------------------------------------

inline int UWasChar0(void);
inline char Ugetchar0(void);
char Getch0();

inline int UWasChar1(void);
inline char Ugetchar1(void);
char Getch1();
char getchar1(void);

void putchar1(char c);

#pragma used-

#endif
