/*****************************************************
Project : �����, ���������� �� ������ �� ��������� ������� ����������
Version : 1.04
Date    : 26.04.2010
Author  :
Company :
Comments: 
LP      : 07.05.2010
Chip type           : ATmega48
Clock frequency     : 7,372800 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 128
*****************************************************/

#ifndef _LCTL48_H_
#define _LCTL48_H_

#include <math.h>
#include "m48lib.h"
#include "pwm48lib.h"

#pragma used+

//----------------------------------------------------------
//#define USE_QRD1114    // ������������ �������, ������������ �� ������� ������ ���

#ifdef USE_QRD1114
  // ���������� ����� ��� 5 � 6
  #define ADC_FL  ADC_5
  #define ADC_FR  ADC_6
#else
  // ���������� ���������� �����������
  #define ADC_FL  ADC_E1
  #define ADC_FR  ADC_E2
#endif

#define Pusk (sen_2)

//----------------------------------------------------------
// ����������� � ������������ �������� (�������� ���������)
#define Vmin  10
extern BYTE Vmax;

//----------------------------------------------------------

void WaitPusk(void);

// ���������� ����������
extern int LimLeft, LimRight;
extern int FLW, FLB, FRW, FRB;

int U2Speed(int u, int FotoMin, int FotoMax);
// ���������� (������ �� �������) � ��������

void Calibrate(void);
void SimpleCalibrate(void);

void TestMotors(void);

//----------------------------------------------------------
// ������ ����������
//----------------------------------------------------------
         
extern int MinVal, MaxVal;

void RgPropor(int a, int *VL, int *VR);
// ���������������� ����������
// a - ���������� ������������ ��������,
// VL, VR - �������� ������ � ������� ������

void RgCos(int a, int *VL, int *VR);
// ���������� ����������
// a - ���������� ������������ �������� (�� 0 �� 90),

#pragma used-

#endif