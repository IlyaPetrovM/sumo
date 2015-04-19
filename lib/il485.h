/*
  ���������� ������� ��� ������ � RS485
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

#define U_ADDR_ALL   0   // ����������������� �����

#define UCMD_ACK     ' ' // ������������� ������

// ����� (���, �����) ����������
extern BYTE MY_ADDR;

#define U_CMD_TIMEOUT 75 //10 // ������� ������� ������������� ������ ������

//----------------------------------------------------------

inline BYTE FORMADDR(BYTE addr, BYTE from);

void InitIL485(void);

void UResetCommand(void);

int GetCommand(BYTE *from, BYTE *cmd, char *data1, char *data2);

void WaitCommand(BYTE cmd);

int WaitCommandTime(BYTE from, BYTE cmd, char *data1, char *data2, int timeout);
// ����� �� from ������� cmd � ������� timeout

int WaitAnyCommandTime(BYTE from, BYTE *cmd, char *data1, char *data2, int timeout);
// ����� �� from ����� ������� � ������� timeout

int UWriteCommand(BYTE addr, BYTE from, BYTE cmd, char data1, char data2, BYTE wait_ack);
// <�������>, <�����������>, <�������>, <������1>, <������2>
// wait_ack - ���� ������ �������� ������������� ������ �������

void FastWrite(BYTE addr, BYTE cmd, char data1, char data2);
// "�������" �������� ��������� ��� ������������ ������
// <�������>, <�������>, <������1>, <������2>
// ����������� ����������� �������������

//----------------------------------------------------------
//
//----------------------------------------------------------

#pragma used-

#endif
