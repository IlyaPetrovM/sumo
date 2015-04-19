/*********************************************
Библиотека ШИМ/fx для Mega48/88
           Используется Timer1
           для M48 - 2 фиксированных канала
           для M48-485 - 6 фиксированных каналов
           Поддержка только командного режима ШИМ (50Гц)
Version : 1.16
Date    : 18.08.2006
LP      : 08.10.2010/14.12.2011
Author  : Карпов В.Э.
Company : ЛРиИИ ПМ / МИЭМ

Chip type : ATmega48/88
Program type        : Application
Clock frequency     : 7,372800 MHz
*********************************************/

#ifndef _SHIM48FX_H_
#define _SHIM48FX_H_

#include "m48lib.h"
#include <Delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#pragma used+

//----------------------------------------------------------

// Количество ШИМ-каналов
#ifdef _M48_LIB_
  #define PWM_CH_NUM 2
#endif

#if defined(_M48_485_LIB_) || defined(_M48_SMP_LIB_)
  #define PWM_CH_NUM 8
#endif

// Длительности импульсов в мкс
#define shimMinT  600 //900 // 900  //0.9 ms крайнее левое положение (против часовой)
#define shimMaxT 2600 //2200 //2200  //2.1 ms крайнее правое положение (по часовой)
#define shimStop 1500 //1500  //1.5 ms центральное положение

#define MinAng  (-90) // Минимальный угол поворота
#define MaxAng    90  // Максимальный угол поворота

// Массив флагов для режимов реверса ШИМ-устройств
extern unsigned char ShimReverse[];

extern unsigned char fxPWM_CH_NUM; // Количество ШИМ-каналов

//----------------------------------------------------------  

unsigned int ANG2POS(int A);

//----------------------------------------------------------  

void InitFxPWM(void);

void ShimSet(int channel, unsigned int t);
// Установка значения периода t для канала channel

void ShimSetAng(int channel, int a);
// Установка значения угла поворота a канала channel

void ShimReset(void);
// Установка счетчиков ШИМ в начальное положение (инициализация ШИМ)

void ShimInc(int channel);

void ShimDec(int channel);

//----------------------------------------------------------  

#pragma used-

#endif
