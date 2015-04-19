/*********************************************
���������� ��� ��� Mega32
����������� ���
Version : 1.10
Date    : 18.08.2006
LP      : 08.06.2010
Author  : ������ �.�.
Company : �����

Chip type : ATmega32
Program type        : Application
Clock frequency     : 4,0 MHz
*********************************************/
#ifndef _SHIM32_H_
#define _SHIM32_H_

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
#define NCH_MAX 2

extern char SHIM_NCH;   //���������� ���-�������

extern char shimRegime; // ���� ������ ���

// ������ ������ ��� ������� ������� ���-���������
extern char ShimReverse[NCH_MAX];

//�������������� �������� ���������� � ������� Delay_DMS/SMS(int xms)
#define DLT_CNT 3 //2 //3

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
