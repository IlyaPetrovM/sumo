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

#ifndef _SHIM8FX_H_
#define _SHIM8FX_H_

#include <Delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <sleep.h>
#include <math.h>

#pragma used+

//----------------------------------------------------------

// Длительности импульсов в мкс
#define shimMinT  900 // 900  //0.9 ms крайнее левое положение (против часовой)
#define shimMaxT 2200 //2200  //2.1 ms крайнее правое положение (по часовой)
#define shimStop 1500 //1500  //1.5 ms центральное положение

#define MinAng  (-60) // Минимальный угол поворота
#define MaxAng    60  // Максимальный угол поворота

// Массив флагов для режимов реверса ШИМ-устройств
extern unsigned char ShimReverse[];

extern unsigned char fxPWM_CH_NUM; // Количество ШИМ-каналов

//----------------------------------------------------------  

int ANG2POS(int A);

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
