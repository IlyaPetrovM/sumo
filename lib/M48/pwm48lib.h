/*
  Библиотека для ATMega48. Аппаратный ШИМ (PWM)
  Контроллер M48K-1
  Version : 1.14
  Date    : 18.08.2006/20.01.2009
  LP:       18.01.2010
  Author  : Карпов В.Э.
  Company : ЛРиИИ (ТНТЛ) ГПМ

  Chip type           : ATmega48
  Program type        : Application
  Clock frequency     : 7,372800 MHz
  Memory model        : Small
  External SRAM size  : 0
  Data Stack size     : 256
*/

#ifndef _PWM48LIB_H_
#define _PWM48LIB_H_

#include <stdio.h>
#include <delay.h>

#pragma used+

#ifndef BYTE_TYPE
  #define BYTE_TYPE
  typedef unsigned char BYTE;
#endif

//----------------------------------------------------------
// Параметры таймеров 0 и 2

#ifndef F_XTAL
  // Частота кварцевого резонатора [Гц]
  #define F_XTAL 7372800L
#endif

// Частота таймера 0 [Гц] (Clock value: 7,200 kHz)
#define T0_FREQ 500L
#define TCNT0_RESET_VAL 0xff - 7200/T0_FREQ

// Частота таймера 2 [Гц]
#define T2_FREQ 50000L
#define TCNT2_RESET_VAL 0x92-F_XTAL/8/T2_FREQ  //0xf0

extern unsigned int TICK_CNT0;
extern unsigned int TICK_CNT1;
extern unsigned int TICK_CNT2;

//----------------------------------------------------------
//
//----------------------------------------------------------
// Времена в 0.01 мс
#define PWMMinT 60
#define PWMMaxT 240
#define PWMStop ((PWMMaxT+PWMMinT)/2) 

#ifndef MinAng
  #define MinAng  (-90) // Минимальный угол поворота
#endif

#ifndef MaxAng
  #define MaxAng    90  // Максимальный угол поворота
#endif

// Частота таймера [Гц] PWM
extern unsigned long PWM_FREQ;

#define PWM_TCNT1_RESET_VAL 0x10000L-QUARTZFREQ/1024L/(2*8*PWM_FREQ)

#ifndef INIT_TIMER1
  #define INIT_TIMER1 { TCNT1H=PWM_TCNT1_RESET_VAL>>8; TCNT1L=PWM_TCNT1_RESET_VAL&0xFF; }
#endif

//----------------------------------------------------------  
void PauseTimer0(int n);
// Пауза для таймера 0 (ок. n*0.036 сек)

void PauseTimer1(int n);
// Пауза для таймера 1

void PauseTimer2(int n);
// Пауза для таймера 2

//----------------------------------------------------------  
// PWM. Аппаратный ШИМ (B.1 и B.2)
// Частота - 50 Гц
//----------------------------------------------------------  

void InitPWM(unsigned int FPWM, int mult8k);
// Инициализация аппаратного ШИМа (PWM)
// FPWM - частота PWM
// mult8k = 1 - использовать множитель частоты 8, = 0 - не использовать. 
// Это для установки режима делителя на 8   

//----------------------------------------------------------
// Полный ШИМ
//----------------------------------------------------------
void PWMSetPc(int A, int B);
// Установить значене ШИМ для OC1A и OC1B (PORTB.1 и PORTB.2)
// Значения A и B указываются в процентах (от 0 до 100)

void SetPWM(BYTE A, BYTE B);
// Установить значене ШИМ для OC1A и OC1B (PORTB.1 и PORTB.2)
// Значения A и B указываются в процентах (от 0 до 100)

void SetPWM_A(BYTE A);
// Установить значение ШИМ для OC1A (PORTB.1)
// Значение A указывается в процентах (от 0 до 100)

void SetPWM_B(BYTE B);
// Установить значение ШИМ для OC1B (PORTB.2)
// Значение B указывается в процентах (от 0 до 100)

//----------------------------------------------------------
// Управляющий ШИМ
//----------------------------------------------------------
void PWMSetTime(int tA, int tB);
// Установить значене ШИМ для OC1A и OC1B (PORTB.1 и PORTB.2)
// Значения tA и tB - время в микросекундах
             
void PWMSetTimeA(int tA);
// Установить значене ШИМ для OC1A (PORTB.1)
// Значения tA - время в микросекундах

void PWMSetTimeB(int tB);
// Установить значене ШИМ для OC1B (PORTB.2)
// Значения B - время в микросекундах

void PWMSetAng(int A, int B);
// Установить значение ШИМ по заданным углам

void PWMSetAngA(int A);
// Установить значение ШИМ OC1A (PORTB.1) по заданному углу A

void PWMSetAngB(int B);
// Установить значение ШИМ OC1B (PORTB.2) по заданному углу B

#pragma used-

#endif
