/*********************************************
Библиотека ШИМ для Mega8
Version : 1.10
Date    : 18.08.2006
LP      : 13.01.2008
Author  : Карпов В.Э.
Company : ТНТЛ ГПМ

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
#include "shim8.h"

//----------------------------------------------------------
//
//----------------------------------------------------------

//Количество ШИМ-каналов
char SHIM_NCH = 2;

int cN[NCH_MAX];  //Длительность интервалов в 0.01 ms

char shimRegime = 1; // Флаг режима ШИМ

// Массив флагов для режимов реверса ШИМ-устройств
char ShimReverse[NCH_MAX] = {0,0,0,0,0,0,0};

#define shimStep   1   //0.01 ms шаг изменения

#define shim0 ef_1
#define shim1 ef_2
#define shim2 ef_3
#define shim3 ef_4
#define shim4 ef_5
#define shim5 ef_6
#define shim6 ef_7

int ANG2POS(int A)
{
  int p;
  p = A*(shimMaxT - shimMinT)/(MaxAng-MinAng)+shimStop;
  return p;
}
                   
void ShimSet(int channel, int t)
// Установка значения периода t для канала channel
{
  cN[channel] = t;
}

void ShimSetAng(int channel, int a)
// Установка значения угла поворота a канала channel
{
  int t;
  t = ANG2POS(a);
  cN[channel] = t;
}

void ShimReset(void)
// Установка счетчиков ШИМ в начальное положение (инициализация ШИМ)
{
  int i;
  for(i=0;i<SHIM_NCH;i++)
    ShimSet(i, shimStop);
}

void ShimInc(int channel)
{               
  int val;
  val = cN[channel]+shimStep;
  if(val<=shimMaxT)
    ShimSet(channel, val);
}

void ShimDec(int channel)
{               
  int val;
  val = cN[channel]-shimStep;
  if(val>=shimMinT)
    ShimSet(channel, val);
}

void ShimOut(int channel, int stat)
//выдача сигнала
{
  switch(channel)
  {
    case 0: shim0 = stat; break;
    case 1: shim1 = stat; break;
    case 2: shim2 = stat; break;
    case 3: shim3 = stat; break;
    case 4: shim4 = stat; break;
    case 5: shim5 = stat; break;
    case 6: shim6 = stat; break;
  }
}

void ShimOutputImpuls(void)
{
  int i;
  for(i=0;i<SHIM_NCH;i++)
  {
    //выдача сигнала
    ShimOut(i, 1);
    if(ShimReverse[i]) //sic Наоборот для ШИМ-i!
      Delay_SMS(shimMaxT-cN[i]+shimMinT);
    else
      Delay_SMS(cN[i]);
    ShimOut(i, 0);    
  }
}

//----------------------------------------------------------            

void Delay_SMS(int sms)
//Задержка в 0.01 ms
{                          
  int i;
  for(i=0;i<sms+20;i++) //sic
    delay_us(10-DLT_CNT); // 0.01ms
}

//----------------------------------------------------------  
