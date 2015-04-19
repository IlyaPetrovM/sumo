/*
  ���������� ��� ATMega8. ���������� ��� (PWM)
  ���������� M8K-3,4
  Version : 1.14
  Date    : 18.08.2006/20.01.2009
  LP:       18.01.2010
  Author  : ������ �.�.
  Company : ����� (����) ���

  Chip type           : ATmega8
  Program type        : Application
  Clock frequency     : 7,372800 MHz
  Memory model        : Small
  External SRAM size  : 0
  Data Stack size     : 256
*/

#ifndef _PWM8LIB_H_
#define _PWM8LIB_H_

#include <mega8.h>
#include <stdio.h>
#include <delay.h>
#include "m8lib.h"

#pragma used+

//----------------------------------------------------------
// ��������� �������� 0 � 2

#ifndef F_XTAL
  // ������� ���������� ���������� [��]
  #define F_XTAL 7372800L
#endif

// ������� ������� 0 [��] (Clock value: 7,200 kHz)
#define T0_FREQ 500L
#define TCNT0_RESET_VAL 0xff - 7200/T0_FREQ

// ������� ������� 2 [��]
#define T2_FREQ 50000L
#define TCNT2_RESET_VAL 0x92-F_XTAL/8/T2_FREQ  //0xf0

//#define TCNT2_RESET_VAL 

extern unsigned int TICK_CNT0;
extern unsigned int TICK_CNT1;
extern unsigned int TICK_CNT2;

//----------------------------------------------------------
//
//----------------------------------------------------------
// ������� � 0.01 ��
#define PWMMinT 60
#define PWMMaxT 240
#define PWMStop ((PWMMaxT+PWMMinT)/2) 

#ifndef MinAng
  #define MinAng  (-90) // ����������� ���� ��������
#endif

#ifndef MaxAng
  #define MaxAng    90  // ������������ ���� ��������
#endif

// ������� ������� [��]
#define PWM_FREQ 50L
#define PWM_TCNT1_RESET_VAL 0x10000L-QUARTZFREQ/1024L/(2*8*PWM_FREQ)

#ifndef INIT_TIMER1
  #define INIT_TIMER1 { TCNT1H=PWM_TCNT1_RESET_VAL>>8; TCNT1L=PWM_TCNT1_RESET_VAL&0xFF; }
#endif

//----------------------------------------------------------  
void PauseTimer0(int n);
// ����� ��� ������� 0 (��. n*0.036 ���)

void PauseTimer1(int n);
// ����� ��� ������� 1

void PauseTimer2(int n);
// ����� ��� ������� 2

//----------------------------------------------------------  
// PWM. ���������� ��� (B.1 � B.2)
// ������� - 50 ��
//----------------------------------------------------------  

void InitPWM(void);
// ������������� ����������� ���� (PWM)

//----------------------------------------------------------
// ������ ���
//----------------------------------------------------------
void PWMSetPc(int A, int B);
// ���������� ������� ��� ��� OC1A � OC1B (PORTB.1 � PORTB.2)
// �������� A � B ����������� � ��������� (�� 0 �� 100)

void SetPWM(int A, int B);
// ���������� ������� ��� ��� OC1A � OC1B (PORTB.1 � PORTB.2)
// �������� A � B ����������� � ��������� (�� 0 �� 100)

void SetPWM_A(int A);
// ���������� �������� ��� ��� OC1A (PORTB.1)
// �������� A ����������� � ��������� (�� 0 �� 100)

void SetPWM_B(int B);
// ���������� �������� ��� ��� OC1B (PORTB.2)
// �������� B ����������� � ��������� (�� 0 �� 100)

//----------------------------------------------------------
// ����������� ���
//----------------------------------------------------------
void PWMSetTime(int tA, int tB);
// ���������� ������� ��� ��� OC1A � OC1B (PORTB.1 � PORTB.2)
// �������� tA � tB - ����� � �������������
             
void PWMSetTimeA(int tA);
// ���������� ������� ��� ��� OC1A (PORTB.1)
// �������� tA - ����� � �������������

void PWMSetTimeB(int tB);
// ���������� ������� ��� ��� OC1B (PORTB.2)
// �������� B - ����� � �������������

void PWMSetAng(int A, int B);
// ���������� �������� ��� �� �������� �����

void PWMSetAngA(int A);
// ���������� �������� ��� OC1A (PORTB.1) �� ��������� ���� A

void PWMSetAngB(int B);
// ���������� �������� ��� OC1B (PORTB.2) �� ��������� ���� B

#pragma used-

#endif
