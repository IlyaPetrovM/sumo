/*****************************************************
Project : Библиотека контроллеров M48L293 (RS232), M48SMP, M48-484 (RS485) 
Version : 2.03
Date    : 26.04.2010
Author  : 
Company : 
Comments:
LP      : 10.07.2012

Chip type           : ATmega48/88
Clock frequency     : 7,372800 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 128
*****************************************************/

#ifndef _M48LIB_H_
#define _M48LIB_H_

#include <stdio.h>
#include <delay.h>

#ifndef _M48_LIB_
  #ifndef _M48_485_LIB_
    #ifndef _M48_SMP_LIB_
      #error "ERROR AT M48LIB: define _M48_LIB_ or _M48_SMP_LIB_ or _M48_485_LIB_"
    #endif
  #endif
#endif

#pragma used+

//----------------------------------------------------------
// Общая часть
//----------------------------------------------------------
/* Расположение пинов (вид со стороны пинов. Мега на обратной стороне)
            -----------------
    Motor-R-+--..             |
            |  12             |
    Motor-L-+--..    ___      | 
            |  43    ___      |
     PIND.4-+--.    |M88|  .--+--ADC_8
     PIND.3-+--.           .--+--ADC_7
            |      ......     |
            |      ||||||     |
            -------++++++------
                   ||||||_ADC_1
                   |||||__ADC_2
                   ||||___ADC_3
                   |||____ADC_4
                   ||_____ADC_5
                   |______ADC_6
*/ 
#ifndef BYTE_TYPE
  #define BYTE_TYPE
  typedef unsigned char BYTE;
#endif

#define ADC_1 0
#define ADC_2 1
#define ADC_3 2
#define ADC_4 3
#define ADC_5 4
#define ADC_6 5

#define ADC_E1 6
#define ADC_E2 7

#define sen_1 PINC.0
#define sen_2 PINC.1
#define sen_3 PINC.2
#define sen_4 PINC.3
#define sen_5 PINC.4
#define sen_6 PINC.5

inline void pip(void);

BYTE read_adc(BYTE  adc_input);
inline BYTE ReadByteADC(BYTE  adc_input);
float ReadADC(BYTE  adc_input);

inline float BYTE2U(BYTE val);

BYTE ReadADCByteEtalon(void);
// Измеряем эталонное напряжение (Vbg = 1.23 V)

float ReadAVCC(void);
// Измеряем напряжение питания (по значению Vbg)

//**********************************************************
// Контроллер M48L293 
//**********************************************************
#ifdef _M48_LIB_

#define ef_1  PORTD.3
#define ef_2  PORTD.4

#define M1_1 PORTD.7
#define M1_2 PORTD.6
#define M2_1 PORTB.0
#define M2_2 PORTD.5

#define ML1  M1_1
#define ML2  M1_2
#define MR1  M2_1
#define MR2  M2_2

#define SOUND PORTD.2

//----------------------------------------------------------

void M48Init(void);

//----------------------------------------------------------

inline void goFwd(void);

inline void goBack(void);

inline void goLeft(void);

inline void goRight(void);
 
inline void goFastRight(void);
 
inline void goFastLeft(void);

inline void robotStop(void);

inline void LeftMotorFwd(void);
inline void LeftMotorBack(void);
inline void LeftMotorStop(void);

inline void RightMotorFwd(void);
inline void RightMotorBack(void);
inline void RightMotorStop(void);

#endif

//**********************************************************
// Контроллер M48-485 
//**********************************************************
#ifdef _M48_485_LIB_

#define ef_1  PORTB.2
#define ef_2  PORTB.1

#define ef_3  PORTB.0
#define ef_4  PORTD.7
#define ef_5  PORTD.6
#define ef_6  PORTD.5

#define out1  PORTD.3
#define out2  PORTD.4

#define SOUND PORTB.3

//----------------------------------------------------------

void M48_485Init(void);

#endif

//**********************************************************
// Контроллер M48smp 
//**********************************************************
#ifdef _M48_SMP_LIB_

#define ef_1  PORTB.2
#define ef_2  PORTB.1

#define ef_3  PORTB.0
#define ef_4  PORTD.7
#define ef_5  PORTD.6
#define ef_6  PORTD.5

#define out1  PORTD.3
#define out2  PORTD.4

#define SOUND PORTD.2

//----------------------------------------------------------

void M48Init(void);

#endif

#pragma used-

#endif
