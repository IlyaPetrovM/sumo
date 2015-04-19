/*
  Библиотека для ATMega8
  Version : 1.07
  Date    : 18.08.2006
          : 07.10.2008
  LP      : 09.03.2010
  Author  : Карпов В.Э.
  Company : ЛРиИИ ПМ

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
#include <sleep.h>
#include "m8lib.h"

//----------------------------------------------------------

int TICK_CNT = 0;
float ADCUMAX = 2.5;

//----------------------------------------------------------

unsigned char ADC_VREF_TYPE = 0xE0;

// ADC interrupt service routine

#define ADEN 7
#define ADSC 6 
#define ADIF 4
#define ADPS1 1   

inline float Byte2U(unsigned char B)
// Преобразование байта в напряжение
{
  return B*ADCUMAX/256.0;
}
#pragma warn-
unsigned char ReadByteADC(unsigned char channel)
// Read the 8 most significant bits
// of the AD conversion result
// Чтение значения напряжения на заданном входе
// Результат - байт
{
  unsigned char r_hi,r_low;  

  ADCSRA=(1<<ADIF);             // сброс флага прерывания, выключение ADC
  ADMUX=channel|ADC_VREF_TYPE;  // установка канала, источника опорного напряжения
  ADCSRA=(1<<ADEN)|(1<<ADPS1);  // включить, fclk/4, без прерываний
  ADCSRA|=(1<<ADSC);            // запуск измерения
  while ((ADCSRA & (1<<ADIF))==0);  // ждем завершения измерения
  ADCSRA|=(1<<ADIF);            // сброс флага прерывания
  r_low = ADCL;                 // чтение младшего байта
  r_hi = ADCH;                  // чтение старшего байта (именно в таком порядке!)
  return r_hi;
}
#pragma warn+

float ReadADC(unsigned char channel)
// Read the 8 most significant bits
// of the AD conversion result
// Чтение значения напряжения на заданном входе
// Результат - действительное число - напряжение
{
  float r;
  unsigned char B;
  B = ReadByteADC(channel);
  r = Byte2U(B);
  return r;
}

//----------------------------------------------------------

float InitADC(unsigned char rtype)
// Инициализация АЦП. rtype: ADCE5, ADCI25
// Возвращает верхнее значение измеряемого напряжения
{
  if(rtype==ADCI25)
  {
    // ADC Clock frequency: 115,200 kHz
    // ADC Voltage Reference: Int., cap. on AREF
    // Only the 8 most significant bits of
    // the AD conversion result are used
    ADC_VREF_TYPE = 0xE0;
    
    ADMUX=ADC_VREF_TYPE;
    ADCSRA=0x86;
    ADCUMAX = 2.5;
  }
  else
  {
    // ADC Clock frequency: 125,000 kHz
    // ADC Voltage Reference: AVCC pin
    // ADC High Speed Mode: Off
    // ADC Auto Trigger Source: Free Running
    // Only the 8 most significant bits of
    // the AD conversion result are used  
    ADC_VREF_TYPE = 0x60; //7 | (0x60 & 0xff); //0x40
    
    ADMUX = ADC_VREF_TYPE;
    ADCSRA = 0x86;//0b11100111; //0x86
    SFIOR &= 0x0F;
    ADCUMAX = 5.0;
  }
  return ADCUMAX;
}

unsigned char SET_PORT_C_PSTAT(char signum)
// Переопределение ног порта C, чтоб использовать АЦП C.0, C.1, C.2, C.3
// Func(signum)=In, State(signum)=P
// signum - номер ноги (0..3)
// Функция может быть использована для работы с АЦП
{
  PORTC = PORTC | (1<<signum);
  return PORTC;
}

//----------------------------------------------------------

void InitCM8(void)
{
  // Input/Output Ports initialization
  
  // Port B initialization
  // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=Out
  // State7=T State6=T State5=T State4=T State3=T State2=P State1=P State0=0
  PORTB=0x06;
  DDRB=0x01;
  
  // Port C initialization
  // Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
  // State6=T State5=T State4=T State3=P State2=P State1=P State0=P
  PORTC=0x0F;
  DDRC=0x00;
  
  // Port D initialization
  // Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=In Func0=In
  // State7=0 State6=0 State5=0 State4=0 State3=0 State2=0 State1=T State0=T
  PORTD=0x00;
  DDRD=0xFC;

  // Timer/Counter 0 initialization
  // Clock source: System Clock
  // Clock value: Timer 0 Stopped
  TCCR0=0x00;
  TCNT0=0x00;

  // Timer/Counter 1 initialization
  // Clock source: System Clock
  // Clock value: 7,200 kHz
  // Mode: Normal top=FFFFh
  // OC1A output: Discon.
  // OC1B output: Discon.
  // Noise Canceler: Off
  // Input Capture on Falling Edge
  TCCR1A=0x00;
  TCCR1B=0x05;
  TCNT1H=0x00;
  TCNT1L=0x00;
  ICR1H=0x00;
  ICR1L=0x00;
  OCR1AH=0x00;
  OCR1AL=0x00;
  OCR1BH=0x00;
  OCR1BL=0x00;

  // Timer/Counter 2 initialization
  // Clock source: System Clock
  // Clock value: Timer 2 Stopped
  // Mode: Normal top=FFh
  // OC2 output: Disconnected
  ASSR=0x00;
  TCCR2=0x00;
  TCNT2=0x00;
  OCR2=0x00;

  // External Interrupt(s) initialization
  // INT0: Off
  // INT1: Off
  MCUCR=0x00;

  // Timer(s)/Counter(s) Interrupt(s) initialization
  TIMSK=0x04;

  // Analog Comparator initialization
  // Analog Comparator: Off
  // Analog Comparator Input Capture by Timer/Counter 1: Off
  // Analog Comparator Output: Off
  ACSR=0x80;
  SFIOR=0x00;

  // USART initialization
  // Communication Parameters: 8 Data, 1 Stop, No Parity
  // USART Receiver: On
  // USART Transmitter: On
  // USART Mode: Asynchronous
  // USART Baud rate: 9600
  UCSRA=0x00;
  UCSRB=0x18;
  UCSRC=0x86;
  UBRRH=0x00;
  UBRRL=0x2F;

  // preset again TIMER1
  TCNT1H=TCNT_RESET_VAL>>8;
  TCNT1L=TCNT_RESET_VAL&0xFF;

  // Global enable interrupts
  #asm("sei")
}

//----------------------------------------------------------

void halt(void)
// Останов
{
  sleep_enable();
  powerdown();
}

void Reset(void)
// Сброс #asm("jmp __RESET")
{
  #asm("rjmp __RESET")
}

void Pause(int n)
{
  TICK_CNT = 0;
  while(TICK_CNT<n);
}
