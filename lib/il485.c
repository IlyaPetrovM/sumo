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

#include <delay.h>
#include "il485.h"

#pragma used+

int TICK_CNT3 = 0;

// Адрес (имя, номер) устройства
BYTE MY_ADDR = 0;

BYTE U_ADDR = 0;  // Адресная часть
BYTE U_CMD = 0;   // Команда
char U_DATA1 = 0; // Данные1
char U_DATA2 = 0; // Данные2

// Флаги готовности
char U_addr_ready = 0;
char U_cmd_ready = 0;
char U_data1_ready = 0;

inline BYTE FORMADDR(BYTE addr, BYTE from) { return (BYTE)((addr<<4) | from); }
inline BYTE A2ADDR(BYTE addr) { return (BYTE)(addr>>4); }
inline BYTE A2FROM(BYTE addr) { return (BYTE)(addr & 0x0F); }

void UResetCommand(void)
{
  U_addr_ready = 0;
  U_cmd_ready = 0;
  U_data1_ready = 0;
  U_ADDR = 0;
  U_CMD = 0;
  U_DATA1 = 0;
  U_DATA2 = 0;
}

unsigned int TAL = 0;

#define CMD_TIMEOUT 20

int UReadCommand(void)
{
  char c; 
  /***
  if(U_addr_ready)
  {
    if(TICK_CNT3-TAL>CMD_TIMEOUT)
    {
      UResetCommand();
      return 0;
    }
  }
  ***/
  if(!UWasChar1()) return 0;
  c = Ugetchar1();
//sic
//printf(":[%x]\r\n",c);

  if(!U_addr_ready)
  {
    U_addr_ready = 1;
    U_cmd_ready = 0;
    U_data1_ready = 0;
    U_ADDR = c;
    TAL = TICK_CNT3;
    return 0;
  }
  if(!U_cmd_ready)
  {
    U_cmd_ready = 1;
    U_CMD = c;
    return 0;
  }

  if(!U_data1_ready)
  {
    U_data1_ready = 1;
    U_DATA1 = c;
    return 0;
  }
  
  U_addr_ready = 0;
  U_cmd_ready = 0;
  U_data1_ready = 0;

  U_DATA2 = c;
  return 1;
}

int GetCommand(BYTE *from, BYTE *cmd, char *data1, char *data2)
{
  BYTE adr;
  U485CTL = U485READ;
  if(!UReadCommand()) return 0;
  *from = A2FROM(U_ADDR);
  adr = A2ADDR(U_ADDR);
  if((adr!=MY_ADDR) && (adr!=U_ADDR_ALL)) return 0;
  *cmd = U_CMD;
  *data1 = U_DATA1;
  *data2 = U_DATA2;
  return 1;
}

void WaitCommand(BYTE cmd)
{
  char d1, d2;
  BYTE from, n_cmd;
  while(1)
  {
    if(GetCommand(&from, &n_cmd, &d1, &d2))
      if(n_cmd==cmd) 
        return;
  }
}

int WaitCommandTime(BYTE from, BYTE cmd, char *data1, char *data2, int timeout)
// Ждать от from команду cmd в течение timeout
{
  BYTE n_cmd;
  BYTE n_from;
  TICK_CNT3 = 0;
  while(TICK_CNT3<timeout)
  {
    if(GetCommand(&n_from, &n_cmd, data1, data2))
    {
//printf(":(%x/%x %x/%x %x %x)\r\n",n_from, from, n_cmd, cmd, *data1, *data2);
      if(n_cmd==cmd && n_from==from) 
        return 1;
    }
  }
  return 0;
}

int WaitAnyCommandTime(BYTE from, BYTE *cmd, char *data1, char *data2, int timeout)
// Ждать от from любую команду в течение timeout
{
  BYTE n_from;
  TICK_CNT3 = 0;
  while(TICK_CNT3<timeout)
  {
    if(GetCommand(&n_from, cmd, data1, data2))
    {
      if(n_from==from) 
        return 1;
    }
  }
  return 0;
}

inline void rs485putchar(char c)
{
  putchar1(c);
  delay_us(1000);
}

int UWriteCommand(BYTE addr, BYTE from, BYTE cmd, char data1, char data2, BYTE wait_ack)
// <адресат>, <отправитель>, <команда>, <данные1>, <данные2>
// wait_ack - флаг режима ожидания подтвреждения приема команды
{  
  BYTE ADDR, ACK_ADDR, ACK_FROM;
  ADDR = FORMADDR(addr, from);

  U485CTL = U485WRITE;
  delay_us(1000);

  rs485putchar(ADDR); 
  rs485putchar(cmd);
  rs485putchar(data1);
  rs485putchar(data2);
  
//printf("(%x)[%d %d %d %d %d]\r\n",(int)ADDR, (int)A2ADDR(ADDR), (int)A2FROM(ADDR), (int)cmd, (int)data1, (int)data2);  
//printf("[%x %x %x %x]\r\n",ADDR, cmd, data1, data2);  
  delay_us(500);
  U485CTL = U485READ;

  if(!wait_ack) return 1;
  
  //Ждем подтверждения приема
  UResetCommand();
  TICK_CNT3 = 0;
  while(TICK_CNT3<U_CMD_TIMEOUT)
  {
    if(UReadCommand())
    {
      ACK_ADDR = A2ADDR(U_ADDR);
      ACK_FROM = A2FROM(U_ADDR); 
      return ((ACK_ADDR==from) && 
              ((ACK_FROM==addr) || (addr==U_ADDR_ALL)) && 
              (U_CMD==UCMD_ACK));
    }
  }
  // Не дождались
  return 0;
}

void InitIL485(void)
{
  U485CTL = U485READ;
  UResetCommand();
}

void FastWrite(BYTE addr, BYTE cmd, char data1, char data2)
// "Быстрая" отправка сообщения без подтвеждения приема
// <адресат>, <команда>, <данные1>, <данные2>
// Отправитель формируется автоматически
{  
  BYTE ADDR;
  ADDR = FORMADDR(addr, MY_ADDR);

  U485CTL = U485WRITE;

  putchar1(ADDR); 
  putchar1(cmd);
  putchar1(data1);
  putchar1(data2);
  
  U485CTL = U485READ;
}

#pragma used-
