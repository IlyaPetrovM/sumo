/*********************************************
���������� ��� ��� Mega8
Version : 1.11
Date    : 18.08.2006
LP      : 13.01.2008
Author  : ������ �.�.
Company : ���� ���

Chip type : ATmega8
Program type        : Application
Clock frequency     : 7,372800 MHz
*********************************************/

#ifndef _SHIM8_H_
#define _SHIM8_H_

#include <Delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <sleep.h>
#include <math.h>

#pragma used+

//----------------------------------------------------------
//
//----------------------------------------------------------

// ���� ���� ������ ����������� ��������
#define shimMinT  35    //152 1.52 ms ����������� ���������
#define shimMaxT 230    // 90  0.9 ms ������� ����� ��������� (������ �������)
#define shimStop ((shimMaxT+shimMinT)/2) //210 2.1 ms ������� ������ ��������� (�� �������)

#ifndef MinAng
  #define MinAng  (-90) // ����������� ���� ��������
#endif

#ifndef MaxAng
  #define MaxAng    90  // ������������ ���� ��������
#endif

//������������ ���������� ���-�������
#define NCH_MAX 7

extern char SHIM_NCH;   //���������� ���-�������

extern char shimRegime; // ���� ������ ���

// ������ ������ ��� ������� ������� ���-���������
extern char ShimReverse[NCH_MAX];

//�������������� �������� ���������� � ������� Delay_DMS/SMS(int xms)
#define DLT_CNT 2 //3

// ������� ���������� ���������� [��]
#define F_XTAL 7372800L
// ������� ������� [��]
#define SHIM_FREQ 50L
#define SHIM_TCNT_RESET_VAL 0x10000L-F_XTAL/1024L/SHIM_FREQ
#define INIT_TIMER3 { TCNT3H=SHIM_TCNT_RESET_VAL>>8; TCNT3L=SHIM_TCNT_RESET_VAL&0xFF; }
#define INIT_TIMER1 { TCNT1H=SHIM_TCNT_RESET_VAL>>8; TCNT1L=SHIM_TCNT_RESET_VAL&0xFF; }

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

void Delay_SMS(int sms);
//�������� � 0.01 ms

//----------------------------------------------------------  

#pragma used-

#endif
