/*********************************************
���������� ���/fx ��� Mega48/88
           ������������ Timer1
           ��� M48 - 2 ������������� ������
           ��� M48-485 - 6 ������������� �������
           ��������� ������ ���������� ������ ��� (50��)
Version : 1.16
Date    : 18.08.2006
LP      : 08.10.2010/14.12.2011
Author  : ������ �.�.
Company : ����� �� / ����

Chip type : ATmega48/88
Program type        : Application
Clock frequency     : 7,372800 MHz
*********************************************/

#ifndef _SHIM48FX_H_
#define _SHIM48FX_H_

#include "m48lib.h"
#include <Delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#pragma used+

//----------------------------------------------------------

// ���������� ���-�������
#ifdef _M48_LIB_
  #define PWM_CH_NUM 2
#endif

#if defined(_M48_485_LIB_) || defined(_M48_SMP_LIB_)
  #define PWM_CH_NUM 8
#endif

// ������������ ��������� � ���
#define shimMinT  600 //900 // 900  //0.9 ms ������� ����� ��������� (������ �������)
#define shimMaxT 2600 //2200 //2200  //2.1 ms ������� ������ ��������� (�� �������)
#define shimStop 1500 //1500  //1.5 ms ����������� ���������

#define MinAng  (-90) // ����������� ���� ��������
#define MaxAng    90  // ������������ ���� ��������

// ������ ������ ��� ������� ������� ���-���������
extern unsigned char ShimReverse[];

extern unsigned char fxPWM_CH_NUM; // ���������� ���-�������

//----------------------------------------------------------  

unsigned int ANG2POS(int A);

//----------------------------------------------------------  

void InitFxPWM(void);

void ShimSet(int channel, unsigned int t);
// ��������� �������� ������� t ��� ������ channel

void ShimSetAng(int channel, int a);
// ��������� �������� ���� �������� a ������ channel

void ShimReset(void);
// ��������� ��������� ��� � ��������� ��������� (������������� ���)

void ShimInc(int channel);

void ShimDec(int channel);

//----------------------------------------------------------  

#pragma used-

#endif
