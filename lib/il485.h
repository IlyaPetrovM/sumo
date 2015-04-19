/*
  Библиотека функций для работы с RS485
  Version 1.08
  06.04.2009
  LP 02.11.2010

  Chip type           : ATmega162/ATmega8
  Program type        : Application
  Clock frequency     : 7,372800 MHz
  Memory model        : Small
  External SRAM size  : 0
*/

#ifndef _IL_485_H
#define _IL_485_H

#ifdef _MEGA_162_
  typedef unsigned char BYTE;
  #include "uart.h"
#else
  #ifdef _MEGA_8_
    #include "usart8.h"
  #else
    #ifdef _MEGA_48_
      #include "usart48.h"
    #else
      #error "ERROR AT IL485: CHIP IS UNDEFINED: use _MEGA_162_, _MEGA_8_ or _MEGA_48_"
    #endif
  #endif
#endif

#pragma used+

extern int TICK_CNT3;

#define U_NO_WAIT_ACK  0
#define U_WAIT_ACK     1

#define U_ADDR_ALL   0   // Широковещательный адрес

#define UCMD_ACK     ' ' // Подтверждение приема

// Адрес (имя, номер) устройства
extern BYTE MY_ADDR;

#define U_CMD_TIMEOUT 75 //10 // Таймаут прихода подтверждения приема пакета

//----------------------------------------------------------

inline BYTE FORMADDR(BYTE addr, BYTE from);

void InitIL485(void);

void UResetCommand(void);

int GetCommand(BYTE *from, BYTE *cmd, char *data1, char *data2);

void WaitCommand(BYTE cmd);

int WaitCommandTime(BYTE from, BYTE cmd, char *data1, char *data2, int timeout);
// Ждать от from команду cmd в течение timeout

int WaitAnyCommandTime(BYTE from, BYTE *cmd, char *data1, char *data2, int timeout);
// Ждать от from любую команду в течение timeout

int UWriteCommand(BYTE addr, BYTE from, BYTE cmd, char data1, char data2, BYTE wait_ack);
// <Адресат>, <отправитель>, <команда>, <данные1>, <данные2>
// wait_ack - флаг режима ожидания подтвреждения приема команды

void FastWrite(BYTE addr, BYTE cmd, char data1, char data2);
// "Быстрая" отправка сообщения без подтвеждения приема
// <адресат>, <команда>, <данные1>, <данные2>
// Отправитель формируется автоматически

//----------------------------------------------------------
//
//----------------------------------------------------------

#pragma used-

#endif
