/*****************************************************
Project : Робопарк. Робот. Библиотека
Version : 1.06
Date    : 30.10.2009
LP      : 21.11.2009
Author  : Карпов В.Э.
Company : ЛРиИИ
Comments: 

Chip type           : ATmega32L
Program type        : Application
Clock frequency     : 4,000000 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 512
*****************************************************/

#include <mega32.h>

#include <stdio.h>
#include <stdlib.h>
#include <delay.h>
#include <sleep.h>

#include "rplib.h"

//----------------------------------------------------------
// Системные функции
//----------------------------------------------------------

unsigned long TICK_CNT = 0;

#define ADC_VREF_TYPE 0x60

unsigned char read_adc(unsigned char adc_input)
// Read the 8 most significant bits
// of the AD conversion result
{
  ADMUX=adc_input|ADC_VREF_TYPE;
  // Start the AD conversion
  ADCSRA|=0x40;
  // Wait for the AD conversion to complete
  while ((ADCSRA & 0x10)==0);
  ADCSRA|=0x10;
  return ADCH;
}

float ReadADC(BYTE adc_input)
{
  BYTE n;
  float r;
  n = read_adc(adc_input);
  r = n/255.0*U_ADC_MAX;
  return r;
}

BYTE ReadByteADC(BYTE adc_input)
{
  return read_adc(adc_input);
}

float BYTE2U(BYTE val)
{
  float r;
  r = val/255.0*U_ADC_MAX;
  return r;
}

float ReadADCEtalon(void)
// Измеряем эталонное напряжение (Vbg = 1.23 V)
{
  float u;
  ADMUX = 0b01111110;

  ADCSRA|=0x40;
  // Wait for the AD conversion to complete
  while ((ADCSRA & 0x10)==0);
  ADCSRA|=0x10;
  u = BYTE2U(ADCH);
  return u;
}

// Опорные точки
#define adc1  1.17
#define avcc1 5.31

#define adc2  1.51
#define avcc2 4.15

float ReadAVCC(void)
// Измеряем напряжение питания (по значению Vbg)
{
  float adc, avcc;
  float k, b;
  k = (avcc1-avcc2)/(adc1-adc2);
  b = avcc1-k*adc1;
  adc = ReadADCEtalon();
  avcc = k*adc+b;
  return avcc;
}
 
//----------------------------------------------------------  
// Инициализация контроллера
// PWM. Аппаратный ШИМ (D.4 и D.5)
// Частота - 50 Гц
//----------------------------------------------------------  

void CRPInit(void)
// Инициализация контроллера
{
  // Input/Output Ports initialization

  // Input/Output Ports initialization
  // Port A initialization
  // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
  // State7=T State6=T State5=T State4=T State3=T State2=T State1=P State0=T 
  PORTA=0x02;
  DDRA=0x00;

  // Port B initialization
  // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
  // State7=T State6=T State5=T State4=T State3=P State2=P State1=P State0=P 
  //PORTB=0x0F;
  //DDRB=0x00;

  // Port B initialization
  // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
  // State7=P State6=P State5=P State4=P State3=P State2=P State1=P State0=P 
  PORTB=0xFF;
  DDRB=0x00;  

  // Port C initialization
  // Func7=In Func6=In Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out 
  // State7=P State6=P State5=0 State4=0 State3=0 State2=0 State1=0 State0=0 
  PORTC=0xC0;
  DDRC=0x3F;

  // Port D initialization
  // Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=In Func0=In 
  // State7=0 State6=0 State5=0 State4=0 State3=0 State2=0 State1=T State0=T 
  PORTD=0x00;
  DDRD=0xFC;

  //--------------------------------------------------------
  // Инициализация таймера 0
  //--------------------------------------------------------
  // Timer/Counter 0 initialization
  // Clock source: System Clock
  // Clock value: 3,906 kHz
  // Mode: Normal top=FFh
  // OC0 output: Disconnected
  TCCR0=0x05;
  TCNT0=0x00;
  OCR0=0x00;

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
  //#define  t_all 9216 //20000/8*7.3728/2 = 9216
  #define  t_all 5000 //20000/8*4.0/2 = 5000

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

  //--------------------------------------------------------
  // Инициализация таймера 2
  //--------------------------------------------------------
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
  // INT2: Off
  MCUCR=0x00;
  MCUCSR=0x00;

  // Timer(s)/Counter(s) Interrupt(s) initialization
  TIMSK=0x01;

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
  UBRRL=0x19;

  // Analog Comparator initialization
  // Analog Comparator: Off
  // Analog Comparator Input Capture by Timer/Counter 1: Off
  ACSR=0x80;
  SFIOR=0x00;

  // ADC initialization
  // ADC Clock frequency: 125,000 kHz
  // ADC Voltage Reference: AVCC pin
  // Only the 8 most significant bits of
  // the AD conversion result are used
  ADMUX=ADC_VREF_TYPE;
  ADCSRA=0x85;

  // Global enable interrupts
  #asm("sei")
}

void USART_9600init(void)
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: On
// USART Mode: Asynchronous
// USART Baud rate: 9600
{
UCSRA=0x00;
UCSRB=0x98;
UCSRC=0x86;
UBRRH=0x00;
UBRRL=0x19;
}

void USART_57600init(void)
// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: On
// USART Mode: Asynchronous
// USART Baud rate: 57600
{
  UCSRA=0x00;
  UCSRB=0x18;
  UCSRC=0x86;
  UBRRH=0x00;
  UBRRL=0x03;
}

//----------------------------------------------------------
// PWM functions
//----------------------------------------------------------
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

//----------------------------------------------------------

void Beep(BYTE n)
{    
  BYTE i;
  for(i=0;i<n;i++)
  {
    OUT_SOUND = 1;
    delay_ms(200);
    OUT_SOUND = 0;
    delay_ms(200);
  }
}
               
void PLight(BYTE color, BYTE SwitchOn)
{
  switch(color)
  {
    case L_RED:   OUT_LED_RED = SwitchOn; break;
    case L_BLUE:  OUT_LED_BLUE = SwitchOn; break;
    case L_GREEN: OUT_LED_GREEN = SwitchOn; break;
    case L_LINE:  OUT_LED_P = SwitchOn; break;
    case L_USR:   OUT_LED_USR = SwitchOn; break;
  }
}

void Flash(BYTE color, int mtime)
{
  PLight(color, 1);
  delay_ms(mtime);
  PLight(color, 0);
}

//----------------------------------------------------------
// Двигательные функции
//----------------------------------------------------------

void RobotStop(void)
{
  OUT_M1_1 = 0;
  OUT_M1_2 = 0;
  OUT_M2_1 = 0;
  OUT_M2_2 = 0;
}

void RobotGoFwd(void)
{
  OUT_M1_1 = 0;
  OUT_M1_2 = 1;
  OUT_M2_1 = 0;
  OUT_M2_2 = 1;
}

void RobotGoBack(void)
{
  OUT_M1_1 = 1;
  OUT_M1_2 = 0;
  OUT_M2_1 = 1;
  OUT_M2_2 = 0;
}

void RobotGoLeft(void)
{
  OUT_M1_1 = 1;
  OUT_M1_2 = 0;
  OUT_M2_1 = 0;
  OUT_M2_2 = 1;
}

void RobotGoRight(void)
{
  OUT_M1_1 = 0;
  OUT_M1_2 = 1;
  OUT_M2_1 = 1;
  OUT_M2_2 = 0;
}

void RobotGoSlowLeft(void)
{
  OUT_M1_1 = 0;
  OUT_M1_2 = 0;
  OUT_M2_1 = 0;
  OUT_M2_2 = 1;
}

void RobotGoSlowRight(void)
{
  OUT_M1_1 = 0;
  OUT_M1_2 = 1;
  OUT_M2_1 = 0;
  OUT_M2_2 = 0;
}

void RobotLeftMotorFwd(void)
{
  OUT_M1_1 = 0;
  OUT_M1_2 = 1;
}

void RobotLeftMotorBack(void)
{
  OUT_M1_1 = 0;
  OUT_M1_2 = 1;
}

void RobotRightMotorFwd(void)
{
  OUT_M2_1 = 0;
  OUT_M2_2 = 1;
}

void RobotRightMotorBack(void)
{
  OUT_M2_1 = 0;
  OUT_M2_2 = 1;
}

void RobotTurnRandom(void)
// Случайный разворот
{
  if(rand()%2) RobotGoLeft(); else RobotGoRight();
}

void RobotGoRandomDir(void)
// Движение в случайном направлении
{
  switch(rand()%3)
  {
    case 0: RobotGoFwd(); break;
    case 1: RobotGoLeft(); break;
    case 2: RobotGoRight(); break;
  }
}

BYTE BumperReaction(BYTE BLeft, BYTE BRight)
// Реакция на препятствие
{         
  if(!BLeft && !BRight) return 0;
  
  RobotStop(); 

  Flash(L_RED, 200);
  Beep(1);  

  RobotGoBack(); 
  delay_ms(750); 
  
  if(BLeft && BRight) RobotTurnRandom();
  if(BLeft) RobotGoRight();
  if(BRight) RobotGoLeft();

  delay_ms(750); 
  RobotStop(); 
  return 1;
}

void SetSpeed(BYTE speed)
{
  #define kLeft  0.90
  #define kRight 1.00
  PWMSetPc(kRight*speed,kLeft*speed);
}

void halt(void)
// Останов
{
  sleep_enable();
  powerdown();
}

