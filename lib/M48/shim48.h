/*********************************************
Библиотека ШИМ для Mega48
Version : 1.10
Date    : 18.08.2006
LP      : 08.06.2010
Author  : Карпов В.Э.
Company : ТНТЛ ГПМ

Chip type : ATmega48
Program type        : Application
Clock frequency     : 7.3728 MHz
*********************************************/

#ifndef _SHIM48_H_
#define _SHIM48_H_

#ifndef _M48_LIB_
  #ifndef _M48_485_LIB_
      #error "ERROR AT SHIM48: define _M48_LIB_ or _M48_485_LIB_"
  #endif
#endif

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
#define shimMinT  60    // 90 (0.9 ms) крайнее левое положение (против часовой)
#define shimMaxT 260    // 210 (2.1 ms) крайнее правое положение (по часовой) 90  
#define shimStop ((shimMaxT+shimMinT)/2) // 152 (1.52 ms) центральное положение 

#ifndef MinAng
  #define MinAng  (-90) // Минимальный угол поворота
#endif

#ifndef MaxAng
  #define MaxAng    90  // Максимальный угол поворота
#endif

// Максимальное количество ШИМ-каналов
#ifdef _M48_LIB_
 #define NCH_MAX 2
#endif

#ifdef _M48_485_LIB_
  #define NCH_MAX 6
#endif


extern char SHIM_NCH;   //Количество ШИМ-каналов

extern char shimRegime; // Флаг режима ШИМ

// Массив флагов для режимов реверса ШИМ-устройств
extern char ShimReverse[NCH_MAX];

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
