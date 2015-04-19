/*********************************************
Библиотека ШИМ/fx для Mega8, максимум - 6 фиксированных каналов
           Поддержка только командного режима ШИМ
Version : 1.13
Date    : 18.08.2006
LP      : 09.03.2010
Author  : Карпов В.Э.
Company : ЛРиИИ ПМ

Chip type : ATmega8
Program type        : Application
Clock frequency     : 7,372800 MHz
*********************************************/

#include <Delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <sleep.h>
#include <math.h>

#include "m8lib.h"
#include "shim8fx.h"

//----------------------------------------------------------
//
//----------------------------------------------------------

#define PWM_CH_NUM 6 //Количество ШИМ-каналов
unsigned char fxPWM_CH_NUM = PWM_CH_NUM;

//----------------------------------------------------------
//
//----------------------------------------------------------

#define shim0 ef_1
#define shim1 ef_2
#define shim2 ef_3
#define shim3 ef_4
#define shim4 ef_5
#define shim5 ef_6

// Частота кварцевого резонатора [Гц]
#define F_XTAL 7372800L

//шаг в мкс
#define PSTEP (8L*1000000L/F_XTAL) //1.085

// Длительности импульсов, мкс
#define Tmid (shimStop/PSTEP)
#define Tmax (shimMinT/PSTEP) // Здесь пишем наоборот
#define Tmin (shimMaxT/PSTEP)

#define high(N) ((N)>>8)
#define low(N)  ((N) & 0x00FF)

unsigned int ct[PWM_CH_NUM] = {Tmid,Tmid,Tmid,Tmid,Tmid,Tmid}; // Ширина импульсов в тиках

// Массив флагов для режимов реверса ШИМ-устройств
unsigned char ShimReverse[PWM_CH_NUM] = {0,0,0,0,0,0};

//----------------------------------------------------------

unsigned char CNUM; //Номер канала

// Интервал между соседними импульсами в тиках (не период!)
// PWM_PERIOD = 15ms/PWM_CH_NUM/PSTEP
#define PWM_PERIOD (3000/PSTEP) //(2500/PSTEP)

//----------------------------------------------------------
//
//----------------------------------------------------------

interrupt [TIM1_CAPT] void timer1_capt_isr(void)
// Timer 1 input capture interrupt service routine
{
  // Очистить фронт (PClearFront)
  switch(CNUM)
  {
    case 0: shim0 = 0; break;
    case 1: shim1 = 0; break;
    case 2: shim2 = 0; break;
    case 3: shim3 = 0; break;
    case 4: shim4 = 0; break;
    case 5: shim5 = 0; break;
  }
  CNUM++;
  if(CNUM>=fxPWM_CH_NUM) CNUM=0;
}

interrupt [TIM1_COMPA] void timer1_compa_isr(void)
// Timer 1 output compare A interrupt service routine
{
  register unsigned int T;
  // Останов таймера
  TCCR1B = 0;

  // Установить период
  T = ct[CNUM];
  
  OCR1AH=high(T);
  OCR1AL=low(T);
  
  // Установить фронт (PSetFront)
  switch(CNUM)
  {
    case 0: shim0 = 1; break;
    case 1: shim1 = 1; break;
    case 2: shim2 = 1; break;
    case 3: shim3 = 1; break;
    case 4: shim4 = 1; break;
    case 5: shim5 = 1; break;
  }  

  //Запуск таймера
  TCCR1B=0b00011010;  
}

//----------------------------------------------------------
//
//----------------------------------------------------------

void InitFxPWM(void)
{
  CNUM = 0;

  ShimReset();

  //Timer/Counter 1 initialization
  TCNT1H=0x00;
  TCNT1L=0x00;

  ICR1H=high(PWM_PERIOD);
  ICR1L=low(PWM_PERIOD);
              
  OCR1AH=high(Tmid);
  OCR1AL=low(Tmid);

  OCR1BH=0x00;
  OCR1BL=0x00;

  // Timer(s)/Counter(s) Interrupt(s) initialization (TIMSK=0x01)
  TIMSK=0b00110000;

  TCCR1A=0x00;
  TCCR1B=0b00011010;
}

//----------------------------------------------------------

#define shimStep 100   //шаг изменения, мкс

int ANG2POS(int A)
{
  int p;
  p = (float)A*((float)shimMaxT - (float)shimMinT)/(MaxAng-MinAng)+shimStop;
  return p;
}
                   
void ShimSet(int channel, unsigned int t)
// Установка значения периода t (мкс) для канала channel
{
  int nt, nc;
  if(channel<0 || channel>=fxPWM_CH_NUM) return;
  // sic Извращенная логика: переворачиваем
  if(!ShimReverse[channel])
    nt = shimMaxT-t+shimMinT;
  else
    nt = t;
  if(nt<shimMinT) nt = shimMinT;
  if(nt>shimMaxT) nt = shimMaxT;

  //sic Извращенная логика: записываем в ПРЕДЫДУЩИЙ канал
  nc = channel-1;
  if(nc<0) nc=fxPWM_CH_NUM-1;
  ct[nc] = nt/PSTEP;
}

void ShimSetAng(int channel, int a)
// Установка значения угла поворота a канала channel
{
  int t;
  if(a<MinAng) a = MinAng;
  if(a>MaxAng) a = MaxAng;
  t = ANG2POS(a);
  ShimSet(channel, t);
}

void ShimReset(void)
// Установка счетчиков ШИМ в начальное положение (инициализация ШИМ)
{
  int i;
  for(i=0;i<fxPWM_CH_NUM;i++)
    ShimSet(i, shimStop);
}

void ShimInc(int channel)
{               
  int val;
  val = (unsigned char)(ct[channel]*PSTEP)+(int)shimStep;
  if(val<=shimMaxT)
    ShimSet(channel, val);
}

void ShimDec(int channel)
{               
  int val;
  val = (unsigned char)(ct[channel]*PSTEP)-(unsigned char)shimStep;
  if(val>=shimMinT)
    ShimSet(channel, val);
}

//----------------------------------------------------------            
