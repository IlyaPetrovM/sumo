/*********************************************
es13x19 (SCM2) Library 

Version 1.3

July 14, 2005/April 25, 2007
LP 28.11.2010

Chip type           : ATmega162
Program type        : Application
Clock frequency     : 7,372800 MHz
*********************************************/

#ifndef es13x19_h
#define es13x19_h

#pragma used+

#define CLRBUTTON(B)  { while(B) delay_ms(100); }

typedef unsigned char byte;

// Этот символ определяет частоту кварца (QF7M - 7,372800 либо QF4M - 4,000000)
// Его надо засунуть в Project->Configure->C Compiler->Globally #define: QF7M

// quartz crystal frquency [Hz] (3686400)
#ifdef QF4M
  #define QUARTZFREQ 4000000
#endif

#ifdef QF7M
  #define QUARTZFREQ 7372800
#endif

#ifndef QUARTZFREQ
#error QUARTZFREQ is not defined. Use QF7M or QF4M
#endif

//--------------------------------------------------------
// Определение сенсоров
//--------------------------------------------------------

#define sn_A0 PINA.0
#define sn_A1 PINA.1
#define sn_A2 PINA.2
#define sn_A3 PINA.3
#define sn_A5 PINA.5

#define sn_B4 PINB.4

#define sn_D4 PIND.4
#define sn_D5 PIND.5
#define sn_D6 PIND.6
#define sn_D7 PIND.7

#define sn_C0 PINC.0
#define sn_C1 PINC.1
#define sn_C2 PINC.2
#define sn_C3 PINC.3
#define sn_C4 PINC.4
#define sn_C5 PINC.5
#define sn_C6 PINC.6
#define sn_C7 PINC.7

//--------------------------------------------------------
// Определение эффекторов
//--------------------------------------------------------
#define ef_A4 PORTA.4
#define ef_A6 PORTA.6
#define ef_A7 PORTA.7

#define ef_E0 PORTE.0
#define ef_E1 PORTE.1
#define ef_E2 PORTE.2
  
#define ef_B0 PORTB.0
#define ef_B1 PORTB.1
#define ef_B2 PORTB.2
#define ef_B3 PORTB.3

#define ef_D2 PORTD.2
#define ef_D3 PORTD.3

//----------------------------------------------------------
// Альтернативные имена входов и выходов
#define ef_1 ef_A4
#define ef_2 ef_A6
#define ef_3 ef_A7

#define ef_4 ef_E0
#define ef_5 ef_E1
#define ef_6 ef_E2
#define ef_7 ef_D2
#define ef_8 ef_D3

#define sen_1   sn_C0
#define sen_2   sn_C0
#define sen_3   sn_C1
#define sen_4   sn_C1
#define sen_5   sn_C2
#define sen_6   sn_C2
#define sen_7   sn_D7
#define sen_8   sn_D7
#define sen_9   sn_C3
#define sen_10  sn_C3
#define sen_11  sn_D6
#define sen_12  sn_D6
#define sen_13  sn_C4
#define sen_14  sn_C4
#define sen_15  sn_C5
#define sen_16  sn_C5
#define sen_17  sn_D5
#define sen_18  sn_D5
#define sen_19  sn_C6
#define sen_20  sn_C6
#define sen_21  sn_D4
#define sen_22  sn_D4
#define sen_23  sn_C7
#define sen_24  sn_C7

//--------------------------------------------------------

// Фактор частоты [Hz]
// Для увеличения частоты необходимо его увеличить
// (значение 1 соответствует примерно 1 секунде при QUARTZFREQ=4MHz)
#define TICK_FACTOR 20

#define TCNT3_RESET_VAL 0x10000-(QUARTZFREQ/1024/TICK_FACTOR)

extern int TICK_CNT0;

//Дополнительная задержка вычислений в функции Delay_DMS/SMS(int xms)
#define DLT_CNT 2

// Функция инициализации портов, таймеров, прерываний и проч.
void es13x19init();

float getQF(void);

// Останов
void halt(void);
// Сброс
void Reset(void);

// Задержка в 0.1 ms
void Delay_DMS(int dms);

// Задержка в 0.01 ms
void Delay_SMS(int sms);

// Настройка прерываний (A0 - PCINT0)
void  INIT_INTERRUPT_A0(void);


#pragma used-

#endif
