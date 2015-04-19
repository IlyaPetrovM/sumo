/*********************************************
Библиотека ШИМ (программный нестабильный ШИМ)
Version : 1.0.9
Date    : 18.08.2006
LP      : 03.01.2008
Author  : Карпов В.Э.
Company : ТНТЛ ГПМ

Chip type : ATmega162
Program type        : Application
Clock frequency     : 7,372800 MHz
*********************************************/

#include <mega162.h>
#include <Delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <sleep.h>
#include <math.h>

#include "es13x19.h"
#include "shimlib.h"

//----------------------------------------------------------
//
//----------------------------------------------------------

//Количество ШИМ-каналов
int SHIM_NCH = NCH_MAX;

int cN[NCH_MAX];  //Длительность интервалов в 0.01 ms
int pred_cN[NCH_MAX];  //Предыдущие значения длительность интервалов 
                       //(для процедур плавного регулрования скорости)

int shimRegime = 1; // Флаг режима ШИМ
int shimEcho   = 1; // Флаг режима "Эхо"

// Массив флагов для режимов реверса ШИМ-устройств
int ShimReverse[NCH_MAX] = {0,0,0,0,0,0};

#define shimStep   1   //0.01 ms шаг изменения

#define shim0 ef_A4
#define shim1 ef_A6
#define shim2 ef_A7
#define shim3 ef_E0
#define shim4 ef_E1
#define shim5 ef_E2

int ANG2POS(int A)
{
  int p;
  p = A*(shimMaxT - shimMinT)/(MaxAng-MinAng)+shimStop;
  return p;
}
                   
void ShimSet(int channel, int t)
// Установка значения периода t для канала channel
{
  pred_cN[channel] = cN[channel];
  cN[channel] = t;
}

void ShimSetAng(int channel, int a)
// Установка значения угла поворота a канала channel
{
  int t;
  t = ANG2POS(a);
  ShimSet(channel,t);
}

void ShimReset(void)
// Установка счетчиков ШИМ в начальное положение (инициализация ШИМ)
{
  int i;
  for(i=0;i<SHIM_NCH;i++)
  {
    ShimSet(i, shimStop);
    pred_cN[i] = cN[i]; 
  }
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
