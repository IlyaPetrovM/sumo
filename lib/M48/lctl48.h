/*****************************************************
Project : Робот, движущийся по полосе по различным законам управления
Version : 1.04
Date    : 26.04.2010
Author  :
Company :
Comments: 
LP      : 07.05.2010
Chip type           : ATmega48
Clock frequency     : 7,372800 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 128
*****************************************************/

#ifndef _LCTL48_H_
#define _LCTL48_H_

#include <math.h>
#include "m48lib.h"
#include "pwm48lib.h"

#pragma used+

//----------------------------------------------------------
//#define USE_QRD1114    // Использовать датчики, подключенные ко внешним входам АЦП

#ifdef USE_QRD1114
  // Используем входы АЦП 5 и 6
  #define ADC_FL  ADC_5
  #define ADC_FR  ADC_6
#else
  // Используем эмиттерные повторители
  #define ADC_FL  ADC_E1
  #define ADC_FR  ADC_E2
#endif

#define Pusk (sen_2)

//----------------------------------------------------------
// Минимальная и максимальная скорости (диапазон скоростей)
#define Vmin  10
extern BYTE Vmax;

//----------------------------------------------------------

void WaitPusk(void);

// Результаты калибровки
extern int LimLeft, LimRight;
extern int FLW, FLB, FRW, FRB;

int U2Speed(int u, int FotoMin, int FotoMax);
// Напряжение (сигнал от датчика) в скорость

void Calibrate(void);
void SimpleCalibrate(void);

void TestMotors(void);

//----------------------------------------------------------
// Законы управления
//----------------------------------------------------------
         
extern int MinVal, MaxVal;

void RgPropor(int a, int *VL, int *VR);
// Пропорциональное управление
// a - отклонение регулируемой величины,
// VL, VR - скорость левого и правого колеса

void RgCos(int a, int *VL, int *VR);
// Косинусное управление
// a - отклонение регулируемой величины (от 0 до 90),

#pragma used-

#endif