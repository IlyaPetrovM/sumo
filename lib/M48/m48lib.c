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

#include <mega48.h>
#include <stdio.h>
#include <delay.h>
#include "m48lib.h"

//----------------------------------------------------------
// Общая часть
//----------------------------------------------------------

#define ADC_VREF_TYPE 0x60

// Read the 8 most significant bits
// of the AD conversion result
BYTE read_adc(BYTE adc_input)
{
  ADMUX=adc_input|ADC_VREF_TYPE;
  // Start the AD conversion
  ADCSRA|=0x40;
  // Wait for the AD conversion to complete
  while ((ADCSRA & 0x10)==0);
  ADCSRA|=0x10;
  return ADCH;
}

inline BYTE ReadByteADC(BYTE adc_input)
{
  return read_adc(adc_input);
}

float ReadADC(BYTE adc_input)
{ 
  float r;
  BYTE n;
  n=read_adc(adc_input);
  r=n*5.0/255.0;
  return r;
}

//----------------------------------------------------------
// Эти функции отвечают за измерение напряжения питания контроллера
//----------------------------------------------------------

#define U_ADC_MAX 4.8
inline float BYTE2U(BYTE val) { return val/255.0*U_ADC_MAX; }

BYTE ReadADCByteEtalon(void)
// Измеряем эталонное напряжение (Vbg = 1.23 V)
{
  ADMUX = 0b01111110;
  ADCSRA|=0x40;
  // Wait for the AD conversion to complete
  while ((ADCSRA & 0x10)==0);
  ADCSRA|=0x10;
  return ADCH;
}

// Опорные точки
#define adc1  68.0 // Значение АЦП (байт)
#define avcc1 4.98 // Показания вольтметра, V

#define adc2  103.0 // Значение АЦП (байт)
#define avcc2 3.56  // Показания вольтметра, V

float ReadAVCC(void)
// Измеряем напряжение питания (по значению Vbg)
{
  float avcc;
  BYTE adc;
  #define cK (avcc1-avcc2)/(adc1-adc2)
  #define cB (avcc1-cK*adc1)
  adc = ReadADCByteEtalon();
  avcc = cK*adc+cB;
  return avcc;
}

//----------------------------------------------------------

interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
// Place your code here
}

//----------------------------------------------------------

inline void pip(void)
{
  SOUND = 1;
  delay_ms(200);
  SOUND = 0;
}

//----------------------------------------------------------
// Библиотека контроллера M48 (RS232)
//----------------------------------------------------------
#if defined(_M48_LIB_) || defined(_M48_SMP_LIB_)

void M48Init(void)
{
  // Crystal Oscillator division factor: 1
  #pragma optsize-
  CLKPR=0x80;
  CLKPR=0x00;
  #ifdef _OPTIMIZE_SIZE_
  #pragma optsize+
  #endif

  // Input/Output Ports initialization
  // Port B initialization
  // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=Out Func1=Out Func0=Out 
  // State7=T State6=T State5=T State4=T State3=T State2=0 State1=0 State0=0 
  PORTB=0x00;
  DDRB=0x07;

  // Port C initialization
  // Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
  // State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
  PORTC=0x00;
  DDRC=0x00;

  // Port D initialization
  // Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=In Func0=In 
  // State7=0 State6=0 State5=0 State4=0 State3=0 State2=0 State1=T State0=T 
  PORTD=0x00;
  DDRD=0xFC;

  // Timer/Counter 0 initialization
  // Clock source: System Clock
  // Clock value: Timer 0 Stopped
  // Mode: Normal top=FFh
  // OC0A output: Disconnected
  // OC0B output: Disconnected
  TCCR0A=0x00;
  TCCR0B=0x00;
  TCNT0=0x00;
  OCR0A=0x00;
  OCR0B=0x00;

  // Timer/Counter 1 initialization
  // Clock source: System Clock
  // Clock value: 7,200 kHz
  // Mode: Normal top=FFFFh
  // OC1A output: Discon.
  // OC1B output: Discon.
  // Noise Canceler: Off
  // Input Capture on Falling Edge
  // Timer 1 Overflow Interrupt: On
  // Input Capture Interrupt: Off
  // Compare A Match Interrupt: Off
  // Compare B Match Interrupt: Off
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
  // OC2A output: Disconnected
  // OC2B output: Disconnected
  ASSR=0x00;
  TCCR2A=0x00;
  TCCR2B=0x00;
  TCNT2=0x00;
  OCR2A=0x00;
  OCR2B=0x00;

  // External Interrupt(s) initialization
  // INT0: Off
  // INT1: Off
  // Interrupt on any change on pins PCINT0-7: Off
  // Interrupt on any change on pins PCINT8-14: Off
  // Interrupt on any change on pins PCINT16-23: Off
  EICRA=0x00;
  EIMSK=0x00;
  PCICR=0x00;

  // Timer/Counter 0 Interrupt(s) initialization
  TIMSK0=0x00;
  // Timer/Counter 1 Interrupt(s) initialization
  TIMSK1=0x01;
  // Timer/Counter 2 Interrupt(s) initialization
  TIMSK2=0x00;

  // USART initialization
  // Communication Parameters: 8 Data, 1 Stop, No Parity
  // USART Receiver: On
  // USART Transmitter: On
  // USART0 Mode: Asynchronous
  // USART Baud rate: 9600
  UCSR0A=0x00;
  UCSR0B=0x18;
  UCSR0C=0x06;
  UBRR0H=0x00;
  UBRR0L=0x2F;

  // Analog Comparator initialization
  // Analog Comparator: Off
  // Analog Comparator Input Capture by Timer/Counter 1: Off
  ACSR=0x80;
  ADCSRB=0x00;

  // ADC initialization
  // ADC Clock frequency: 115,200 kHz
  // ADC Voltage Reference: AVCC pin
  // ADC Auto Trigger Source: None
  // Only the 8 most significant bits of
  // the AD conversion result are used
  // Digital input buffers on ADC0: On, ADC1: On, ADC2: On, ADC3: On
  // ADC4: On, ADC5: On
  DIDR0=0x00;
  ADMUX=ADC_VREF_TYPE;
  ADCSRA=0x86;

}
#endif

#if defined(_M48_LIB_)

//----------------------------------------------------------

inline void goFwd(void)
{
  ML1 = 0; ML2 = 1;
  MR1 = 0; MR2 = 1;
}

inline void goBack(void)
{
  ML1 = 1; ML2 = 0;
  MR1 = 1; MR2 = 0;
}

inline void goLeft(void)
{
  ML1 = 0; ML2 = 0;
  MR1 = 0; MR2 = 1;
}

inline void goRight(void)
{
  ML1 = 0; ML2 = 1;
  MR1 = 0; MR2 = 0;
}
 
inline void goFastRight(void)
{
  ML1 = 0; ML2 = 1;
  MR1 = 1; MR2 = 0;
}
 
inline void goFastLeft(void)
{
  ML1 = 1; ML2 = 0;
  MR1 = 0; MR2 = 1;
}

inline void robotStop(void)
{
  ML1 = 0; ML2 = 0;
  MR1 = 0; MR2 = 0;
}

inline void LeftMotorFwd(void) { ML1 = 0; ML2 = 1; }

inline void LeftMotorBack(void) { ML1 = 1; ML2 = 0; }

inline void LeftMotorStop(void) { ML1 = 0; ML2 = 0; }

inline void RightMotorFwd(void) { MR1 = 0; MR2 = 1; }

inline void RightMotorBack(void) { MR1 = 1; MR2 = 0; }

inline void RightMotorStop(void) { MR1 = 0; MR2 = 0; }

//----------------------------------------------------------

#endif

//----------------------------------------------------------
// Библиотека контроллера M48-484 (RS485)
//----------------------------------------------------------
#ifdef _M48_485_LIB_

void M48_485Init(void)
{
  // Crystal Oscillator division factor: 1
  #pragma optsize-
  CLKPR=0x80;
  CLKPR=0x00;
  #ifdef _OPTIMIZE_SIZE_
  #pragma optsize+
  #endif

  // Input/Output Ports initialization
  // Port B initialization
  // Func7=In Func6=In Func5=In Func4=In Func3=Out Func2=Out Func1=Out Func0=Out 
  // State7=T State6=T State5=T State4=T State3=0 State2=0 State1=0 State0=0 
  PORTB=0x00;
  DDRB=0x0F;

  // Port C initialization
  // Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
  // State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
  PORTC=0x00;
  DDRC=0x00;

  // Port D initialization
  // Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=In Func0=In 
  // State7=0 State6=0 State5=0 State4=0 State3=0 State2=0 State1=T State0=T 
  PORTD=0x00;
  DDRD=0xFC;

  // Timer/Counter 0 initialization
  // Clock source: System Clock
  // Clock value: Timer 0 Stopped
  // Mode: Normal top=FFh
  // OC0A output: Disconnected
  // OC0B output: Disconnected
  TCCR0A=0x00;
  TCCR0B=0x00;
  TCNT0=0x00;
  OCR0A=0x00;
  OCR0B=0x00;

  // Timer/Counter 1 initialization
  // Clock source: System Clock
  // Clock value: 7,200 kHz
  // Mode: Normal top=FFFFh
  // OC1A output: Discon.
  // OC1B output: Discon.
  // Noise Canceler: Off
  // Input Capture on Falling Edge
  // Timer 1 Overflow Interrupt: On
  // Input Capture Interrupt: Off
  // Compare A Match Interrupt: Off
  // Compare B Match Interrupt: Off
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
  // OC2A output: Disconnected
  // OC2B output: Disconnected
  ASSR=0x00;
  TCCR2A=0x00;
  TCCR2B=0x00;
  TCNT2=0x00;
  OCR2A=0x00;
  OCR2B=0x00;

  // External Interrupt(s) initialization
  // INT0: Off
  // INT1: Off
  // Interrupt on any change on pins PCINT0-7: Off
  // Interrupt on any change on pins PCINT8-14: Off
  // Interrupt on any change on pins PCINT16-23: Off
  EICRA=0x00;
  EIMSK=0x00;
  PCICR=0x00;

  // Timer/Counter 0 Interrupt(s) initialization
  TIMSK0=0x00;
  // Timer/Counter 1 Interrupt(s) initialization
  TIMSK1=0x01;
  // Timer/Counter 2 Interrupt(s) initialization
  TIMSK2=0x00;

  // USART initialization
  // Communication Parameters: 8 Data, 1 Stop, No Parity
  // USART Receiver: On
  // USART Transmitter: On
  // USART0 Mode: Asynchronous
  // USART Baud Rate: 57600
  UCSR0A=0x00;
  UCSR0B=0x98;
  UCSR0C=0x06;
  UBRR0H=0x00;
  UBRR0L=0x07;

  // Analog Comparator initialization
  // Analog Comparator: Off
  // Analog Comparator Input Capture by Timer/Counter 1: Off
  ACSR=0x80;
  ADCSRB=0x00;

  // ADC initialization
  // ADC Clock frequency: 115,200 kHz
  // ADC Voltage Reference: AVCC pin
  // ADC Auto Trigger Source: None
  // Only the 8 most significant bits of
  // the AD conversion result are used
  // Digital input buffers on ADC0: On, ADC1: On, ADC2: On, ADC3: On
  // ADC4: On, ADC5: On
  DIDR0=0x00;
  ADMUX=ADC_VREF_TYPE;
  ADCSRA=0x86;

  // Global enable interrupts
  #asm("sei")
}

//----------------------------------------------------------

#endif
