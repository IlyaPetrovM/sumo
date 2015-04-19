/*****************************************************
Project : Робопарк. Робот. Библиотека
Version : 1.07
Date    : 30.10.2009
LP      : 05.12.2009
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
#ifndef _RPLIB_H_
#define _RPLIB_H_

#pragma used+

typedef unsigned char UCHAR;
typedef unsigned char BYTE;
typedef unsigned int  WORD;

extern unsigned long TICK_CNT;

//----------------------------------------------------------
// Определения сигналов
//----------------------------------------------------------
// Входы
#define IN_CODE1_1      PINB.1
#define IN_CODE1_2      PINB.0
#define EYE_RIGHT_FOOD  (!IN_CODE1_1)
#define EYE_RIGHT_BAIT  (!IN_CODE1_2)

#define IN_CODE2_1      PINB.3
#define IN_CODE2_2      PINB.2
#define EYE_LEFT_FOOD   (!IN_CODE2_1)
#define EYE_LEFT_BAIT   (!IN_CODE2_2)

#define IN_CODE3_1      PINC.7
#define IN_CODE3_2      PINC.6
#define EYE_UP_BAIT     (!IN_CODE3_1)
#define EYE_UP_KILL     (!IN_CODE3_2)

#define IN_ADC0         PINA.0
#define IN_REGIME1      PINA.1
#define IN_REGIME2      PINB.4
#define IN_ADC2         PINA.2
#define IN_ADC3         PINA.3
#define IN_ADC4         PINA.4
#define IN_ADC5         PINA.5

// Выходы
#define OUT_SOUND       PORTD.2
#define OUT_M2_2        PORTD.3
#define OUT_PWM0        PORTD.4
#define OUT_PWM1        PORTD.5
#define OUT_M2_1        PORTD.6
#define OUT_M1_1        PORTD.7
#define OUT_M1_2        PORTC.0
#define OUT_LED_RED     PORTC.1
#define OUT_LED_GREEN   PORTC.2
#define OUT_LED_BLUE    PORTC.3
#define OUT_LED_P       PORTC.4
#define OUT_LED_LINE    OUT_LED_P
#define OUT_LED_USR     PORTC.5

//----------------------------------------------------------
// Системные функции
//----------------------------------------------------------

#define high(N) N>>8
#define low(N) (N & 0xFF)

#define U_ADC_MAX 4.8

unsigned char read_adc(unsigned char adc_input);
// Read the 8 most significant bits
// of the AD conversion result

float ReadADC(BYTE adc_input);

BYTE ReadByteADC(BYTE adc_input);

float BYTE2U(BYTE val);
 
//----------------------------------------------------------  
// PWM. Аппаратный ШИМ (D.4 и D.5)
// Частота - 50 Гц
//----------------------------------------------------------  

#define QUARTZFREQ 4000000

// Частота таймера [Гц]
#define PWM_FREQ 50L

#define T1QUARTZFREQ QUARTZFREQ

#define t_a (long)(50000000L*4.0/8 /2)
#define PWM_TOP (T1QUARTZFREQ / (2*(PWM_FREQ+1))) // Значение регистра задания частоты

void CRPInit(void);
// Инициализация контроллера

void USART_9600init(void);
// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: On
// USART Mode: Asynchronous
// USART Baud rate: 9600

void USART_57600init(void);
// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: On
// USART Mode: Asynchronous
// USART Baud rate: 57600

void PWMSetPc(int A, int B);
// Установить значене ШИМ для OC1A и OC1B (PORTB.1 и PORTB.2)
// Значения A и B указываются в процентах (от 0 до 100)

//----------------------------------------------------------
// Диоды подсветки
enum {L_RED, L_BLUE, L_GREEN, L_LINE, L_USR};

// Входы эмиттерных повторителей (АЦП)
#define ADC_FOTO_UP    0 // Верхний датчик освещенности
#define ADC_FOTO_RT    2 // Правый фотодатчик полосы
#define ADC_FOTO_LF    3 // Левый фотодатчик полосы
#define ADC_BUMPER_LF  4 // Бампер левый
#define ADC_BUMPER_RT  5 // Бампер правый

void Beep(BYTE n);

void PLight(BYTE color, BYTE SwitchOn);

void Flash(BYTE color, int mtime);

//----------------------------------------------------------
// Двигательные функции
//----------------------------------------------------------

void RobotStop(void);
void RobotGoFwd(void);
void RobotGoBack(void);
void RobotGoLeft(void);
void RobotGoRight(void);
void RobotGoSlowLeft(void);
void RobotGoSlowRight(void);

void RobotLeftMotorFwd(void);
void RobotLeftMotorBack(void);
void RobotRightMotorFwd(void);
void RobotRightMotorBack(void);

// Случайный разворот
void RobotTurnRandom(void);

// Движение в случайном направлении
void RobotGoRandomDir(void);

// Реакция на препятствие
BYTE BumperReaction(BYTE BLeft, BYTE BRight);

void SetSpeed(BYTE speed);

// Измеряем эталонное напряжение (Vbg = 1.23 V)
float ReadADCEtalon(void);

// Измеряем напряжение питания (по значению Vbg)
float ReadAVCC(void);

// Останов
void halt(void);

#pragma used-

#endif
