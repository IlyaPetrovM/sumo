/*
  Библиотека для ATMega8
  Version : 1.07
  Date    : 18.08.2006
          : 07.10.2008
  LP      : 01.02.2010
  Author  : Карпов В.Э.
  Company : ЛРиИИ ПМ

  Chip type           : ATmega8
  Program type        : Application
  Clock frequency     : 7,372800 MHz
  Memory model        : Small
  External SRAM size  : 0
  Data Stack size     : 256
*/

#ifndef _M8LIB_H_
#define _M8LIB_H_

#include <mega8.h>
#include <stdio.h>
#include <delay.h>

#pragma used+

//----------------------------------------------------------

#define  ef_1 PORTD.2
#define  ef_2 PORTD.3
#define  ef_3 PORTD.4
#define  ef_4 PORTD.5
#define  ef_5 PORTD.6
#define  ef_6 PORTD.7
#define  ef_7 PORTB.0

//#define  ef_indicator ef_7
//#define  cmdIndic     ef_7

#define  sen_1  PINB.1
#define  sen_2  PINB.2
#define  sen_3  PINC.0
#define  sen_4  PINC.1
#define  sen_5  PINC.2
#define  sen_6  PINC.3
              
// Номера АЦП
#define ADC1 4
#define ADC2 5

//----------------------------------------------------------
 
// Частота кварцевого резонатора [Гц]
#define QUARTZFREQ 7372800
#define TICK_FACTOR 20
#define TCNT_RESET_VAL 0x10000-(QUARTZFREQ/1024/TICK_FACTOR)

extern int TICK_CNT;

extern float ADCUMAX;

#define ADCE5   0 //измеряемое напряжение до 5 В (ADC Voltage Reference: AVCC pin)
#define ADCI25  1 //измеряемое напряжение до 2.5 В

//----------------------------------------------------------

float ReadADC(unsigned char channel);
// Read the 8 most significant bits
// of the AD conversion result
// Чтение значения напряжения на заданном входе
// Результат - действительное число - напряжение

float Byte2U(unsigned char B);
// Преобразование байта в напряжение

unsigned char ReadByteADC(unsigned char channel);
// Read the 8 most significant bits
// of the AD conversion result
// Чтение значения напряжения на заданном входе
// Результат - байт

float InitADC(unsigned char rtype);
// Инициализация АЦП. rtype: ADCE5, ADCI25
// Возвращает верхнее значение измеряемого напряжения

unsigned char SET_PORT_C_PSTAT(char signum);
// Переопределение ног порта C, чтоб использовать АЦП C.0, C.1, C.2, C.3
// Func(signum)=In, State(signum)=P
// signum - номер ноги (0..3)
// Функция может быть использована для работы с АЦП

void InitCM8(void);

void halt(void);
// Останов

void Reset(void);
// Сброс

void Pause(int n);
// Пауза на n циклов таймера

#pragma used-

#endif
