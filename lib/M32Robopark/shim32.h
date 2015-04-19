/*********************************************
Библиотека ШИМ для Mega32
Программный ШИМ
Version : 1.10
Date    : 18.08.2006
LP      : 08.06.2010
Author  : Карпов В.Э.
Company : ЛРиИИ

Chip type : ATmega32
Program type        : Application
Clock frequency     : 4,0 MHz
*********************************************/
#ifndef _SHIM32_H_
#define _SHIM32_H_

#include <Delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <sleep.h>
#include <math.h>

#pragma used+

//----------------------------------------------------------
//
//----------------------------------------------------------

// либо надо точнее настраивать задержки
#define shimMinT  35    //152 1.52 ms центральное положение
#define shimMaxT 230    // 90  0.9 ms крайнее левое положение (против часовой)
#define shimStop ((shimMaxT+shimMinT)/2) //210 2.1 ms крайнее правое положение (по часовой)

#ifndef MinAng
  #define MinAng  (-90) // Минимальный угол поворота
#endif

#ifndef MaxAng
  #define MaxAng    90  // Максимальный угол поворота
#endif

//Максимальное количество ШИМ-каналов
#define NCH_MAX 2

extern char SHIM_NCH;   //Количество ШИМ-каналов

extern char shimRegime; // Флаг режима ШИМ

// Массив флагов для режимов реверса ШИМ-устройств
extern char ShimReverse[NCH_MAX];

//Дополнительная задержка вычислений в функции Delay_DMS/SMS(int xms)
#define DLT_CNT 3 //2 //3

//----------------------------------------------------------  

int ANG2POS(int A);

//----------------------------------------------------------  

void ShimSet(int channel, int t);
// Установка значения периода t для канала channel

void ShimSetAng(int channel, int a);
// Установка значения угла поворота a канала channel

void ShimReset(void);
// Установка счетчиков ШИМ в начальное положение (инициализация ШИМ)

void ShimInc(int channel);

void ShimDec(int channel);

void ShimOut(int channel, int stat);
//выдача сигнала

void ShimOutputImpuls(void);

//----------------------------------------------------------            

void Delay_SMS(int sms);
//Задержка в 0.01 ms

//----------------------------------------------------------  

#pragma used-

#endif
