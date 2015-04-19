/*
  Работа с USART
  Реализация функций для работы с RS485
  Version 1.06
  09.12.2008
  LP 10.06.2009
  Chip type : ATmega8
  Program type        : Application
  Clock frequency     : 7,372800 MHz
*/

#include <mega8.h>
#include "usart8.h"

//==========================================================
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

// USART Receiver buffer
#define RX_BUFFER_SIZE 32
char rx_buffer[RX_BUFFER_SIZE];

#if RX_BUFFER_SIZE<256
unsigned char rx_wr_index,rx_rd_index,rx_counter;
#else
unsigned int rx_wr_index,rx_rd_index,rx_counter;
#endif

// This flag is set on USART Receiver buffer overflow
bit rx_buffer_overflow;

int UWasChar1(void)
{
  return rx_counter;
}

// USART Receiver interrupt service routine
interrupt [USART_RXC] void usart_rx_isr(void)
{
  char status,data;
  status=UCSRA;
  data=UDR;
  if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
  {
    rx_buffer[rx_wr_index]=data;
    if (++rx_wr_index == RX_BUFFER_SIZE) rx_wr_index=0;
    if (++rx_counter == RX_BUFFER_SIZE)
    {
      rx_counter=0;
      rx_buffer_overflow=1;
    };
  };
}

#ifndef _DEBUG_TERMINAL_IO_
// Get a character from the USART Receiver buffer
#define _ALTERNATE_GETCHAR_
#pragma used+
char getchar(void)
{
  char data;
  while (rx_counter==0);
  data=rx_buffer[rx_rd_index];
  if (++rx_rd_index == RX_BUFFER_SIZE) rx_rd_index=0;
  #asm("cli")
  --rx_counter;
  #asm("sei")
  return data;
}

char Ugetchar1(void)
{
  return getchar();
}


#pragma used-
#endif

//==========================================================

void putchar1(char c)
{
  putchar(c);
}

void USART_Init(BYTE BR)
// BR: 0x0B - 38400, 0x07 - 57600 
{
  // USART initialization
  // Communication Parameters: 8 Data, 1 Stop, No Parity
  // USART Receiver: On
  // USART Transmitter: On
  // USART Mode: Asynchronous
  // USART Baud rate: BR
  UCSRA=0x00;
  UCSRB=0x98;
  UCSRC=0x86;
  UBRRH=0x00;
  UBRRL=BR;
}
