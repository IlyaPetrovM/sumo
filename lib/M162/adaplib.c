//
// AdapLib
// Date: 21.09.2004
// Sept 05, 2005
// 22.03.2006
// ATMega162 SCM-2

#include <stdlib.h>
#include <stdio.h>
#include <delay.h>
#include "es13x19.h"
#include "uart.h"
#include "sign.h"
#include "adaplib.h"

byte TICK_CNT3 = 0;

byte InterruptLevel=i_default;

// Вектор входных сигналов
byte InputVector[SIGNUM];

// Вектор эффекторов ESW
byte ESW[CMD_END];

void ClearESWVector(void)
// Очистка выходного вектора
{
  int i;
  for(i=0;i<CMD_END;i++)
    ESW[i]=0;
}

void ClearInputVector(void)
// Очистка входного вектора
{
  int i;
  for(i=0;i<SIGNUM;i++)
    signal(i)=0;
}
    
//----------------------------------------------------------
// Звуки
//----------------------------------------------------------
byte SoundOn = 1;

void Say(unsigned char n, int tdelay)
{
  byte s0, s1, s2, s3;

  if(!SoundOn) return;

  s3 = (n & 0x08);
  s2 = (n & 0x04);
  s1 = (n & 0x02);
  s0 = (n & 0x01);

  ef_E0 = s3;
  ef_A7 = s2;
  ef_A6 = s1;
  ef_A4 = s0;

  delay_ms(tdelay);
}

int FIWait(int time)
// Возвращает 1, если истекло время, 0, если сработали датчики и USART_CHAR0,
// если был принят сигнал
{ 
  unsigned int oldSSW0, oldSSW1, SSW0, SSW1;
  char UCH, oldUCH;
                     
  USART_CHAR0 = 0;
  // Устраняем дребезг контактов
  delay_ms(DELAY_DREBEZG);
  
  oldSSW0 = GETSSW0;  
  oldSSW1 = GETSSW1;
  oldUCH = USART_CHAR0;

  TICK_CNT3 = 0;

  //Очищаем вектор сигналов
  ClearInputVector();

  while(TICK_CNT3<time)
  {
    SSW0 = GETSSW0;
    SSW1 = GETSSW1;
    UCH = USART_CHAR0;

    // Формируем вектор сигналов
    // Физические сигналы
    signal(SIG_KLF)   |= sn_D4 || UCH=='0';
    signal(SIG_KRT)   |= sn_D5 || UCH=='1';
    signal(SIG_KBC)   |= sn_D6 || UCH=='2';
    signal(SIG_ACC)   |= sn_D7 || UCH=='3';

    //signal(SIG_HLT)   |= sn_C0 || UCH=='4'; // sic Внимание! Этот датчик исключен из списка
    signal(SIG_POOSR) |= sn_C1 || UCH=='5';
    signal(SIG_NAKAZ) |= sn_C2 || UCH=='6';
    signal(SIG_FOTO1) |= sn_C3 || UCH=='7';
    signal(SIG_FOTO2) |= sn_C4 || UCH=='8';
    signal(SIG_FOTO3) |= sn_C5 || UCH=='9';
    signal(SIG_PUSK1) |= sn_C6 || UCH=='a';
    signal(SIG_PUSK2) |= sn_C7 || UCH=='b';

    // Виртуальные сигналы
    signal(12) |= UCH=='c';
    signal(13) |= UCH=='d';
    signal(14) |= UCH=='e';
    signal(SIG_LOAD) |= UCH=='f';
    signal(SIG_SAVE) |= UCH=='g';

    signal(SIG_A0) |= sn_A0 || UCH=='h';
    signal(SIG_A1) |= sn_A1 || UCH=='i';
    signal(SIG_A2) |= sn_A2 || UCH=='j';
    signal(SIG_A3) |= sn_A3 || UCH=='k';

     // Сигнал высшего приоритета
    if((SSW0!=oldSSW0 || (UCH!=oldUCH && UCH<='4')) && InterruptLevel>i_no_reaction) return 0;
    if((SSW1!=oldSSW1 || (UCH!=oldUCH && UCH>='5')) && InterruptLevel>i_hight_level_only) return 0;
  }
  return 1;
}

interrupt [TIM3_OVF] void timer3_ovf_isr(void)
// TIMER3 overflow interrupt service routine
// occurs every 0.xxx seconds
{
  // preset again TIMER3
  TCNT3H=TCNT3_RESET_VAL>>8;
  TCNT3L=TCNT3_RESET_VAL&0xFF;
  TICK_CNT3++;
  //Индикация
  if(TICK_CNT3%10) cmdIndic=0; else cmdIndic=1;
}

//----------------------------------------------------------
// Действия
//----------------------------------------------------------
void Stop(void)
{
  /* Звуки
  ef_A4 = 0;
  ef_A6 = 0;
  ef_A7 = 0;
  
  ef_E0 = 0;
  */

  ef_E1 = 0;
  ef_E2 = 0;

  ef_B0 = 0;
  ef_B1 = 0;
  ef_B2 = 0;

  ef_D2 = 0;
  ef_D3 = 0;
    
  Say(SND_NOSOUND,0);
  MOTOR_STOP;
  ClearESWVector();
}

int goForward(int time)
{   
  int n;
  printf(":Fw\r\n");
  Stop();
  Say(SND_VPERED,0);
  MOTOR_FWD;
  n = FIWait(time);
  Stop();
  ESW[CMD_GOFWD] = 1;
  return n;
}

int goBack(int time)
{       
  int n;
  printf(":Bk\r\n");
  Stop();
  Say(SND_NAZAD,0);
  MOTOR_BACK;
  n = FIWait(time);
  Stop();
  ESW[CMD_GOBACK] = 1;
  return n;
}

int TurnLeft(int time)
{
  int n;
  printf(":Lf\r\n");
  Stop();
  Say(SND_NALEVO,0);
  MOTOR_LEFT;
  n = FIWait(time);
  Stop();
  ESW[CMD_TURNLEFT] = 1;
  return n;
}

int TurnLeftSmall(int time)
{
  int n;
  printf(":Lf\r\n");
  Stop();
  Say(SND_NALEVO,0);
  MOTOR_LEFTF;
  n = FIWait(time);
  Stop();
  ESW[CMD_TURNLEFT] = 1;
  return n;
}

int TurnRight(int time)
{       
  int n;
  printf(":Rt\r\n");
  Stop();
  Say(SND_NAPRAVO,0);
  MOTOR_RIGHT;
  n = FIWait(time);
  Stop();
  ESW[CMD_TURNRIGHT] = 1;
  return n;
}

int TurnRightSmall(int time)
{       
  int n;
  printf(":Rt\r\n");
  Stop();
  Say(SND_NAPRAVO,0);
  MOTOR_RIGHTF;
  n = FIWait(time);
  Stop();
  ESW[CMD_TURNRIGHT] = 1;
  return n;
}

//----------------------------------------------------------
// Безусловные рефлексы
//----------------------------------------------------------

void BackTurnRight(int time)
// Отъехать и повернуть направо (реакция на левый передний датчик)
{
  goBack(time);
  TurnRight(time);
}

void BackTurnLeft(int time)
// Отъехать и повернуть налево (реакция на правый передний датчик)
{
  goBack(time);
  TurnLeft(time);
}

void Alert(int time)
// Тревога
{
  int oldsound;
  oldsound = SoundOn;
  Stop();
  SoundOn = 1;
  Say(SND_NAKAZ,DTIME_SAY/2);
  Say(SND_ACCUM,DTIME_SAY);
  FIWait(time);
  Stop();
  SoundOn = oldsound;
}

int GetRegNum(void)
// Получение номера режима работы
{
  int n;
  n = (int)(!sen_PUSK2) + (int)((!sen_PUSK1)<<1) + (int)((!sen_PUSK0)<<2);
  return n;
}
