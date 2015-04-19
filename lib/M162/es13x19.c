/*********************************************
es13x19 (SCM2) Library 

Version 1.3

July 14, 2005/April 25, 2007
LP 28.11.2010

Chip type           : ATmega162
Program type        : Application
Clock frequency     : 7,372800 MHz
*********************************************/

#define __POWERDOWN_SUPPORTED__

#include <mega162.h>
#include <sleep.h>
#include <delay.h>
#include "es13x19.h"

#pragma used+

int TICK_CNT0 = 0;

//----------------------------------------------------------            

void es13x19init()
{ 
  // Crystal Oscillator division factor: 1
  CLKPR=0x80;
  CLKPR=0x00;

  // Input/Output Ports initialization
  // Port A initialization
  // Func7=Out Func6=Out Func5=In Func4=Out Func3=In Func2=In Func1=In Func0=In
  // State7=0 State6=0 State5=P State4=0 State3=P State2=P State1=P State0=P
  PORTA=0x2F;
  DDRA=0xD0;
  
  // Port B initialization
  // Func7=In Func6=In Func5=In Func4=In Func3=Out Func2=Out Func1=Out Func0=Out
  // State7=P State6=P State5=P State4=P State3=1 State2=1 State1=1 State0=1
  PORTB=0xFF;
  DDRB=0x0F;
  
  // Port C initialization
  // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
  //All pullup
  PORTC=0xFF;
  DDRC=0x00;

  // Port D initialization
  // Func7=In Func6=In Func5=In Func4=In Func3=Out Func2=Out Func1=In Func0=In
  // State7=T State6=T State5=T State4=T State3=0 State2=0 State1=T State0=T
  PORTD=0x00;
  DDRD=0x0C;

  // Port E initialization
  // Func2=Out Func1=Out Func0=Out
  // State2=0 State1=0 State0=0
  PORTE=0x00;
  DDRE=0x07;                   

  //----------------------------------------------------------
  // Timer/Counter 0 initialization
  // Clock source: System Clock
  // Clock value: Timer 0 Stopped
  // Mode: Normal top=FFh
  // OC0 output: Disconnected
  TCCR0=0x00;
  TCNT0=0x00;
  OCR0=0x00;
  //----------------------------------------------------------
  // Timer/Counter 1 initialization
  // Clock source: System Clock
  // Clock value: Timer 1 Stopped
  // Mode: Normal top=FFFFh
  // OC1A output: Discon.
  // OC1B output: Discon.
  // Noise Canceler: Off
  // Input Capture on Falling Edge
  TCCR1A=0x00;
  TCCR1B=0x00;
  TCNT1H=0x00;
  TCNT1L=0x00;
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

  // Timer/Counter 3 initialization
  // Clock value: 7.200 kHz
  // Mode: Normal top=FFFFh
  // Noise Canceler: Off
  // Input Capture on Falling Edge
  // OC3A output: Discon.
  // OC3B output: Discon.
  TCCR3A=0x00;
  TCCR3B=0x05;
  // preset again TIMER3
  TCNT3H=TCNT3_RESET_VAL>>8;
  TCNT3L=TCNT3_RESET_VAL&0xFF;
  ICR3H=0x00;
  ICR3L=0x00;
  OCR3AH=0x00;
  OCR3AL=0x00;
  OCR3BH=0x00;
  OCR3BL=0x00; 
  
  //----------------------------------------------------------
  // External Interrupt(s) initialization
  // INT0: Off
  // INT1: Off
  // INT2: Off
  //----------------------------------------------------------
  MCUCR=0x00;
  EMCUCR=0x00;

  //----------------------------------------------------------
  // Timer(s)/Counter(s) Interrupt(s) initialization
  //----------------------------------------------------------
  TIMSK=0x00;
  ETIMSK=0x04;
  
  // Инициализация USART0 в 'нормальном режиме'
  // USART0 initialization
  // Communication Parameters: 8 Data, 1 Stop, No Parity
  // USART0 Receiver: On
  // USART0 Transmitter: On
  // USART0 Mode: Asynchronous
  // USART0 Baud rate: 9600
  UCSR0A=0x00;
  UCSR0B=0x18;
  UCSR0C=0x86;
  UBRR0H=0x00;
  UBRR0L=0x2F;

  //----------------------------------------------------------
  // Analog Comparator initialization
  // Analog Comparator: Off
  // Analog Comparator Input Capture by Timer/Counter 1: Off
  // Analog Comparator Output: Off
  //----------------------------------------------------------
  ACSR=0x80;

  //----------------------------------------------------------
  // global enable interrupts
  //----------------------------------------------------------
#asm
    sei
#endasm
}

//----------------------------------------------------------            

float getQF(void)
// Возвращает частоту кварца
{
#ifdef QF7M
  return 7.372800;
#endif

#ifdef QF4M
  return 4.000000;
#endif

  return -1;
}

//----------------------------------------------------------            

void halt(void)
// Останов
{
  sleep_enable();
  powerdown();
}

void Reset(void)
// Сброс
{
  #asm("jmp __RESET")
}

//----------------------------------------------------------            

void Delay_DMS(int dms)
//Задержка в 0.1 ms
{                          
  int i;
  for(i=0;i<dms;i++)
    delay_us(100-DLT_CNT); // ms/10
}

void Delay_SMS(int sms)
//Задержка в 0.01 ms
{                          
  int i;
  for(i=0;i<sms;i++)
    delay_us(10-DLT_CNT); // 0.01ms
}

//----------------------------------------------------------            

void  INIT_INTERRUPT_A0(void)
// Настройка прерываний (A0 - PCINT0)
{
  // External Interrupt(s) initialization
  // INT0: Off
  // INT1: Off
  // INT2: Off
  // Interrupt on any change on pins PCINT0-7: On
  // Interrupt on any change on pins PCINT8-15: Off
  GICR|=0x08;
  PCMSK0=0x01;
  MCUCR=0x00;
  EMCUCR=0x00;
  GIFR=0x08;
}

//----------------------------------------------------------            

#pragma used-
