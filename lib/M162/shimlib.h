/*********************************************
Библиотека ШИМ (программный нестабильный ШИМ)
Version : 1.0.8
Date    : 18.08.2006
LP      : 03.01.2008
Author  : Карпов В.Э.
Company : ТНТЛ ГПМ

Chip type : ATmega162
Program type        : Application
Clock frequency     : 7,372800 MHz
*********************************************/

#ifndef _SHIMLIB_H_
#define _SHIMLIB_H_

#pragma used+

#include <mega162.h>
#include <Delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <sleep.h>
#include <math.h>

//----------------------------------------------------------
//
//----------------------------------------------------------

#define shimMinT  70  //  90 0.9 ms крайнее левое положение (против часовой)
#define shimMaxT 230  // 210 2.1 ms крайнее правое положение (по часовой)
#define shimStop ((shimMaxT+shimMinT)/2)  //152 1.52 ms центральное положение

#ifndef MinAng
  #define MinAng  (-90) // Минимальный угол поворота
#endif

#ifndef MaxAng
  #define MaxAng    90  // Максимальный угол поворота
#endif

//Максимальное количество каналов
#define NCH_MAX 6

extern int SHIM_NCH;       //Количество ШИМ-каналов

extern int shimRegime; // Флаг режима ШИМ
extern int shimEcho;   // Флаг режима "Эхо"

// Массив флагов для режимов реверса ШИМ-устройств
extern int ShimReverse[NCH_MAX];

extern int cN[NCH_MAX];       //Длительность интервалов в 0.01 ms
extern int pred_cN[NCH_MAX];  //Предыдущие значения длительность интервалов 
                              //(для процедур плавного регулрования скорости)

#ifndef F_XTAL
// Частота кварцевого резонатора [Гц]
#define F_XTAL 7372800L
#endif

// Частота таймера [Гц]
#define SHIM_FREQ 50L
#define SHIM_TCNT_RESET_VAL 0x10000L-F_XTAL/1024L/SHIM_FREQ
#define INIT_TIMER3 { TCNT3H=SHIM_TCNT_RESET_VAL>>8; TCNT3L=SHIM_TCNT_RESET_VAL&0xFF; }

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

#pragma used-

#endif
