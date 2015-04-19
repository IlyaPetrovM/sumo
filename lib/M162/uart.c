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

#include <mega162.h>

#include "uart.h"

// Эти переменные нужны для ловли байта из последовательного порта                                                   
char USART_CHAR0 = 0;
char USART_CHAR1 = 0;

//----------------------------------------------------------

#pragma used+

char Getch0()
{
  USART_CHAR0 = 0;
  while(!USART_CHAR0);
  return USART_CHAR0;
}

char Getch1()
{
  USART_CHAR1 = 0;
  while(!USART_CHAR1);
  return USART_CHAR1;
}

#pragma used-

//----------------------------------------------------------

#define RXB8 1
#define TXB8 0
#define UPE 2
#define OVR 3
#define FE 4
#define UDRE 5
#define RXC 7

#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<OVR)
#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)

//----------------------------------------------------------
// USART0
//----------------------------------------------------------
// USART0 Receiver buffer
#define RX_BUFFER_SIZE0 32
char rx_buffer0[RX_BUFFER_SIZE0];

#if RX_BUFFER_SIZE0<256
unsigned char rx_wr_index0,rx_rd_index0,rx_counter0;
#else
unsigned int rx_wr_index0,rx_rd_index0,rx_counter0;
#endif

// This flag is set on USART0 Receiver buffer overflow
bit rx_buffer_overflow0;


// USART0 Receiver interrupt service routine
interrupt [USART0_RXC] void usart0_rx_isr(void)
{
  char status,data;
  status=UCSR0A;
  data=UDR0;
  if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
  {
    rx_buffer0[rx_wr_index0]=data;
    if (++rx_wr_index0 == RX_BUFFER_SIZE0) rx_wr_index0=0;
    if (++rx_counter0 == RX_BUFFER_SIZE0)
    {
      rx_counter0=0;
      rx_buffer_overflow0=1;
    };
  };
}

#ifndef _DEBUG_TERMINAL_IO_
// Get a character from the USART0 Receiver buffer
#define _ALTERNATE_GETCHAR_
#pragma used+
char getchar(void)
{
  char data;
  while (rx_counter0==0);
  data=rx_buffer0[rx_rd_index0];
  if (++rx_rd_index0 == RX_BUFFER_SIZE0) rx_rd_index0=0;
  #asm("cli")
  --rx_counter0;
  #asm("sei")
  USART_CHAR0 = data;
  return data;
}

#pragma used-
#endif

//----------------------------------------------------------
// USART1
//----------------------------------------------------------
// USART1 Receiver buffer
#define RX_BUFFER_SIZE1  16 //32
char rx_buffer1[RX_BUFFER_SIZE1];

#if RX_BUFFER_SIZE1<256
unsigned char rx_wr_index1,rx_rd_index1,rx_counter1;
#else
unsigned int rx_wr_index1,rx_rd_index1,rx_counter1;
#endif

// This flag is set on USART1 Receiver buffer overflow
bit rx_buffer_overflow1;

// USART1 Receiver interrupt service routine
interrupt [USART1_RXC] void usart1_rx_isr(void)
{
  char status,data;
  status=UCSR1A;
  data=UDR1;
  if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
  {
    rx_buffer1[rx_wr_index1]=data;
    if (++rx_wr_index1 == RX_BUFFER_SIZE1) rx_wr_index1=0;
    if (++rx_counter1 == RX_BUFFER_SIZE1)
    {
      rx_counter1=0;
      rx_buffer_overflow1=1;
    };
  };
}


// Get a character from the USART1 Receiver buffer
#pragma used+
char getchar1(void)
{
  char data;
  while (rx_counter1==0);
  data=rx_buffer1[rx_rd_index1];
  if (++rx_rd_index1 == RX_BUFFER_SIZE1) rx_rd_index1=0;
  #asm("cli")
  --rx_counter1;
  #asm("sei")
  USART_CHAR1 = data;
  return data;
}
#pragma used-


// Write a character to the USART1 Transmitter
#pragma used+
void putchar1(char c)
{
  while ((UCSR1A & DATA_REGISTER_EMPTY)==0);
  UDR1=c;
}

#pragma used-

// Standard Input/Output functions
#include <stdio.h>

//----------------------------------------------------------
// RS-485
//----------------------------------------------------------

inline int UWasChar0(void) { return rx_counter0; }

inline int UWasChar1(void) { return rx_counter1; }

inline char Ugetchar0(void) { return getchar(); }

inline char Ugetchar1(void) { return getchar1(); }

//----------------------------------------------------------

#pragma used+

void USART0_Init(unsigned char BR)
// USART0 initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART1 Receiver: On
// USART1 Transmitter: On
// USART1 Mode: Asynchronous
// USART1 Baud rate: BR
// BR: 0x2F - 9600, 0x0B - 38400, 0x07 - 57600 
{
  UCSR0A=0x00;
  UCSR0B=0x98;
  UCSR0C=0x86;
  UBRR0H=0x00;
  UBRR0L=BR;
}

void USART1_Init(unsigned char BR)
// USART1 initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART1 Receiver: On
// USART1 Transmitter: On
// USART1 Mode: Asynchronous
// USART1 Baud rate: BR
// BR: 0x0B - 38400, 0x07 - 57600 
{
  UCSR1A=0x00;
  UCSR1B=0x98;
  UCSR1C=0x86;
  UBRR1H=0x00;
  UBRR1L=BR;
}

#pragma used-
