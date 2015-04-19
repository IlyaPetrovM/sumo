/*********************************************
���������� ��� (����������� ������������ ���)
Version : 1.0.8
Date    : 18.08.2006
LP      : 03.01.2008
Author  : ������ �.�.
Company : ���� ���

Chip type : ATmega162
Program type        : Application
Clock frequency     : 7,372800 MHz
*********************************************/

#ifndef _SHIMLIB_H_
#define _SHIMLIB_H_

#pragma used+

#include <mega162.h>
#include <Delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <sleep.h>
#include <math.h>

//----------------------------------------------------------
//
//----------------------------------------------------------

#define shimMinT  70  //  90 0.9 ms ������� ����� ��������� (������ �������)
#define shimMaxT 230  // 210 2.1 ms ������� ������ ��������� (�� �������)
#define shimStop ((shimMaxT+shimMinT)/2)  //152 1.52 ms ����������� ���������

#ifndef MinAng
  #define MinAng  (-90) // ����������� ���� ��������
#endif

#ifndef MaxAng
  #define MaxAng    90  // ������������ ���� ��������
#endif

//������������ ���������� �������
#define NCH_MAX 6

extern int SHIM_NCH;       //���������� ���-�������

extern int shimRegime; // ���� ������ ���
extern int shimEcho;   // ���� ������ "���"

// ������ ������ ��� ������� ������� ���-���������
extern int ShimReverse[NCH_MAX];

extern int cN[NCH_MAX];       //������������ ���������� � 0.01 ms
extern int pred_cN[NCH_MAX];  //���������� �������� ������������ ���������� 
                              //(��� �������� �������� ������������ ��������)

#ifndef F_XTAL
// ������� ���������� ���������� [��]
#define F_XTAL 7372800L
#endif

// ������� ������� [��]
#define SHIM_FREQ 50L
#define SHIM_TCNT_RESET_VAL 0x10000L-F_XTAL/1024L/SHIM_FREQ
#define INIT_TIMER3 { TCNT3H=SHIM_TCNT_RESET_VAL>>8; TCNT3L=SHIM_TCNT_RESET_VAL&0xFF; }

//----------------------------------------------------------  

int ANG2POS(int A);

//----------------------------------------------------------  

void ShimSet(int channel, int t);
// ��������� �������� ������� t ��� ������ channel

void ShimSetAng(int channel, int a);
// ��������� �������� ���� �������� a ������ channel

void ShimReset(void);
// ��������� ��������� ��� � ��������� ��������� (������������� ���)

void ShimInc(int channel);

void ShimDec(int channel);

void ShimOut(int channel, int stat);
//������ �������

void ShimOutputImpuls(void);

//----------------------------------------------------------  

#pragma used-

#endif
