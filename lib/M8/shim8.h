/*********************************************
Библиотека ШИМ для Mega8
Version : 1.11
Date    : 18.08.2006
LP      : 13.01.2008
Author  : Карпов В.Э.
Company : ТНТЛ ГПМ

Chip type : ATmega8
Program type        : Application
Clock frequency     : 7,372800 MHz
*********************************************/

#ifndef _SHIM8_H_
#define _SHIM8_H_

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
#define NCH_MAX 7

extern char SHIM_NCH;   //Количество ШИМ-каналов

extern char shimRegime; // Флаг режима ШИМ

// Массив флагов для режимов реверса ШИМ-устройств
extern char ShimReverse[NCH_MAX];

//Дополнительная задержка вычислений в функции Delay_DMS/SMS(int xms)
#define DLT_CNT 2 //3

// Частота кварцевого резонатора [Гц]
#define F_XTAL 7372800L
// Частота таймера [Гц]
#define SHIM_FREQ 50L
#define SHIM_TCNT_RESET_VAL 0x10000L-F_XTAL/1024L/SHIM_FREQ
#define INIT_TIMER3 { TCNT3H=SHIM_TCNT_RESET_VAL>>8; TCNT3L=SHIM_TCNT_RESET_VAL&0xFF; }
#define INIT_TIMER1 { TCNT1H=SHIM_TCNT_RESET_VAL>>8; TCNT1L=SHIM_TCNT_RESET_VAL&0xFF; }

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
