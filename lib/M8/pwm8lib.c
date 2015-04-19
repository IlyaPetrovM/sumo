/*
  Библиотека для ATMega8. Аппаратный ШИМ (PWM)
  Контроллер M8K-3,4
  Version : 1.14
  Date    : 18.08.2006/20.01.2009
  LP:       18.01.2010
  Author  : Карпов В.Э.
  Company : ЛРиИИ (ТНТЛ) ГПМ

  Chip type           : ATmega8
  Program type        : Application
  Clock frequency     : 7,372800 MHz
  Memory model        : Small
  External SRAM size  : 0
  Data Stack size     : 256
*/

#include <mega8.h>
#include <stdio.h>
#include <delay.h>
#include "pwm8lib.h"

//----------------------------------------------------------
#pragma used+

unsigned int TICK_CNT0 = 0;
unsigned int TICK_CNT1 = 0;
unsigned int TICK_CNT2 = 0;

//----------------------------------------------------------  

void PauseTimer0(int n)
{
  TICK_CNT0=0;
  while(TICK_CNT0<n);
}

//----------------------------------------------------------

void PauseTimer1(int n)
{
  TICK_CNT1=0;
  while(TICK_CNT1<n);
}

//----------------------------------------------------------  

void PauseTimer2(int n)
{
  TICK_CNT2=0;
  while(TICK_CNT2<n);
}
//----------------------------------------------------------  
// PWM. Аппаратный ШИМ (B.1 и B.2)
// Частота - 50 Гц
//----------------------------------------------------------  

#define high(N) N>>8
#define low(N) (N & 0xFF)

#define T1QUARTZFREQ QUARTZFREQ

#define t_a (long)(50000000L*7.3728/8 /2)
#define PWM_TOP (T1QUARTZFREQ / (2*(PWM_FREQ+1))) // Значение регистра задания частоты

void InitPWM(void)
// Инициализация аппаратного ШИМа (PWM)
{
  // Переопределяем порты B.1 и B.2 на вывод
  // Input/Output Ports initialization
  // Port B initialization
  // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=Out Func1=Out Func0=Out 
  // State7=T State6=T State5=T State4=T State3=T State2=0 State1=0 State0=0 
  PORTB=0x00;
  DDRB=0x07;

  //--------------------------------------------------------
  // Инициализация таймера 1
  //--------------------------------------------------------
  /*
  Режим -  PWM, Phase and Frequency Correct with ICR1
  он работает БЕЗ ПРЕРЫВАНИЙ, т.е. полностью аппаратно
  счетчик считает от 0 до ICR1, затем вниз до 0, и так циклически
  т.е. период = удвоенное значение ICR1
  выход OC1A =1 при совпадении счетчика с OCR1A при счете вверх
  выход OC1A =0 при совпадении счетчика с OCR1A при счете вниз
  длительность импульса = удвоенное значение OCR1A
  для выхода OC1B аналогично
  */  

  TCNT1H = 0;   // инициализация счетчика
  TCNT1L = 0;

  // период генерации для 20 мс    t_all = 20000/8*7.3728/2 = 9216
  #define  t_all 9216 //20000/8*7.3728/2 = 9216

  ICR1H=high(t_all);
  ICR1L=low(t_all);

  // генерация импульса для выхода  OC1A c длительностью 2*t_a
  // t_a = длительность_мкс*7.3728/8 /2

  OCR1AH = high(t_a);
  OCR1AL = low(t_a);
  // генерация импульса для выхода  OC1B c длительностью 2*t_b
  // t_b = длительность_мкс*7.3728/8 /2
  #define t_b  t_a
  OCR1BH = high(t_b);
  OCR1BL = low(t_b);

  //         COM1A1=1
  //         | COM1B1=1
  //         | |
  TCCR1A = 0b10100000;
  // PWM, Phase and Frequency Correct with ICR1
  // clkI/O/8
  //            WGM13=1
  //            |  CS11 = 1
  //            |  |
  TCCR1B = 0b00010010;       //  устанавливать последним !!!

  //*****************************************************

  // Чтоб не оставаться совсем без таймера, инициализируем Timer0,2
  // Timer/Counter 0 initialization
  // Clock source: System Clock
  // Clock value: 7,200 kHz
  TCCR0=0x05;
  TCNT0=0x00;

  //--------------------------------------------------------
  // Timer/Counter 2 initialization
  // Clock source: System Clock
  // Clock value: 7372,800 kHz
  // Mode: Normal top=FFh
  // OC2 output: Disconnected
  ASSR=0x00;
  TCCR2=0x01;
  TCNT2=0x00;
  OCR2=0x00;
  //--------------------------------------------------------
 
  MCUCR=0x00; MCUCSR=0x00;

  // Timer(s)/Counter(s) Interrupt(s) initialization (TIMSK=0x01)
  TIMSK=0x41;
  
  // preset again TIMER 0, 2
  TCNT0=TCNT0_RESET_VAL;
  TCNT2=TCNT2_RESET_VAL;
}

void PWMSetPc(int A, int B)
// Установить значене ШИМ для OC1A и OC1B (PORTB.1 и PORTB.2)
// Значения A и B указываются в процентах (от 0 до 100)
{                         
  unsigned int LV, RV;
  
  LV = (unsigned int)(t_a*A/1000L/100/2.5);
  RV = (unsigned int)(t_a*B/1000L/100/2.5);
  
  OCR1AH = LV >> 8;
  OCR1AL = LV & 0xFF;
  
  OCR1BH = RV >> 8;
  OCR1BL = RV & 0xFF;
}

void PWMSetTimeA(int tA)
// Установить значене ШИМ для OC1A (PORTB.1)
// Значения tA - время в микросекундах
{                         
  unsigned int V;
  V = tA*10.0*7.3728/8/2;
  OCR1AH = V >> 8;
  OCR1AL = V & 0xFF;
}

void PWMSetTimeB(int tB)
// Установить значене ШИМ для OC1B (PORTB.2)
// Значения tB - время в микросекундах
{                         
  unsigned int V;
  V = tB*10.0*7.3728/8/2;
  OCR1BH = V >> 8;
  OCR1BL = V & 0xFF;
}

void PWMSetTime(int tA, int tB)
// Установить значене ШИМ для OC1A и OC1B (PORTB.1 и PORTB.2)
// Значения tA и tB - время в микросекундах
{                         
  PWMSetTimeA(tA);
  PWMSetTimeB(tB);
}

int PWMAng2Pos(int A)
{
  int p;
  p = (A-MinAng)*(PWMMaxT - PWMMinT)/(MaxAng-MinAng)+PWMMinT;
  return p;
}

void PWMSetAngA(int A)
// Установить значение ШИМ OC1A (PORTB.1) по заданному углу A
{
  int p;
  p = PWMAng2Pos(A);
  PWMSetTimeA(p);
}

void PWMSetAngB(int B)
// Установить значение ШИМ OC1B (PORTB.2) по заданному углу B
{
  int p;
  p = PWMAng2Pos(B);
  PWMSetTimeB(p);
}

void PWMSetAng(int A, int B)
// Установить значение ШИМ по заданным углам
{
  PWMSetAngA(A);
  PWMSetAngB(B);
}

void SetPWM_A(int A)
// Установить значение ШИМ для OC1A (PORTB.1)
// Значение A указывается в процентах (от 0 до 100)
{                         
  unsigned int LV;
  LV = (unsigned int)(PWM_TOP/1024)*A;
  OCR1AH = LV >> 8;
  OCR1AL = LV & 0xFF;
}

void SetPWM_B(int B)
// Установить значение ШИМ для OC1B (PORTB.2)
// Значение B указывается в процентах (от 0 до 100)
{                         
  unsigned int RV;
  RV = (unsigned int)(PWM_TOP/1024)*B;
  OCR1BH = RV >> 8;
  OCR1BL = RV & 0xFF;
}

void SetPWM(int A, int B)
// Установить значение ШИМ для OC1A и OC1B (PORTB.1 и PORTB.2)
// Значения A и B указываются в процентах (от 0 до 100)
{                         
  SetPWM_A(A);
  SetPWM_B(B);
}

#pragma used-
