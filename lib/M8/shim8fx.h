/*********************************************
���������� ���/fx ��� Mega8, �������� - 6 ������������� �������
           ��������� ������ ���������� ������ ���
Version : 1.13
Date    : 18.08.2006
LP      : 09.03.2010
Author  : ������ �.�.
Company : ����� ��

Chip type : ATmega8
Program type        : Application
Clock frequency     : 7,372800 MHz
*********************************************/

#ifndef _SHIM8FX_H_
#define _SHIM8FX_H_

#include <Delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <sleep.h>
#include <math.h>

#pragma used+

//----------------------------------------------------------

// ������������ ��������� � ���
#define shimMinT  900 // 900  //0.9 ms ������� ����� ��������� (������ �������)
#define shimMaxT 2200 //2200  //2.1 ms ������� ������ ��������� (�� �������)
#define shimStop 1500 //1500  //1.5 ms ����������� ���������

#define MinAng  (-60) // ����������� ���� ��������
#define MaxAng    60  // ������������ ���� ��������

// ������ ������ ��� ������� ������� ���-���������
extern unsigned char ShimReverse[];

extern unsigned char fxPWM_CH_NUM; // ���������� ���-�������

//----------------------------------------------------------  

int ANG2POS(int A);

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
