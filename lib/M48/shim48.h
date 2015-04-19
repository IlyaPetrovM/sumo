/*********************************************
���������� ��� ��� Mega48
Version : 1.10
Date    : 18.08.2006
LP      : 08.06.2010
Author  : ������ �.�.
Company : ���� ���

Chip type : ATmega48
Program type        : Application
Clock frequency     : 7.3728 MHz
*********************************************/

#ifndef _SHIM48_H_
#define _SHIM48_H_

#ifndef _M48_LIB_
  #ifndef _M48_485_LIB_
      #error "ERROR AT SHIM48: define _M48_LIB_ or _M48_485_LIB_"
  #endif
#endif

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
#define shimMinT  60    // 90 (0.9 ms) ������� ����� ��������� (������ �������)
#define shimMaxT 260    // 210 (2.1 ms) ������� ������ ��������� (�� �������) 90  
#define shimStop ((shimMaxT+shimMinT)/2) // 152 (1.52 ms) ����������� ��������� 

#ifndef MinAng
  #define MinAng  (-90) // ����������� ���� ��������
#endif

#ifndef MaxAng
  #define MaxAng    90  // ������������ ���� ��������
#endif

// ������������ ���������� ���-�������
#ifdef _M48_LIB_
 #define NCH_MAX 2
#endif

#ifdef _M48_485_LIB_
  #define NCH_MAX 6
#endif


extern char SHIM_NCH;   //���������� ���-�������

extern char shimRegime; // ���� ������ ���

// ������ ������ ��� ������� ������� ���-���������
extern char ShimReverse[NCH_MAX];

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
