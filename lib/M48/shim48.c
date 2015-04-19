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

#include <Delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <sleep.h>
#include <math.h>

#include "m48lib.h"
#include "shim48.h"

#ifndef _M48_LIB_
  #ifndef _M48_485_LIB_
      #error "ERROR AT SHIM48LIB: define _M48_LIB_ or _M48_485_LIB_"
  #endif
#endif

//----------------------------------------------------------
//
//----------------------------------------------------------

//���������� ���-�������
char SHIM_NCH = NCH_MAX;

int cN[NCH_MAX];  //������������ ���������� � 0.01 ms

char shimRegime = 1; // ���� ������ ���

// ������ ������ ��� ������� ������� ���-���������
#ifdef _M48_LIB_
char ShimReverse[NCH_MAX] = {0,0};
#endif
#ifdef _M48_485_LIB_
char ShimReverse[NCH_MAX] = {0,0,0,0,0,0};
#endif

#define shimStep   1   //0.01 ms ��� ���������

#ifdef _M48_LIB_
  #define shim0  ef_1
  #define shim1  ef_2
#endif

//�������������� �������� ���������� � ������� Delay_DMS/SMS(int xms)
#define DLT_CNT 3 //2 //3

int ANG2POS(int A)
{
  int p;
  p = A*(shimMaxT - shimMinT)/(MaxAng-MinAng)+shimStop;
  return p;
}
                   
void ShimSet(int channel, int t)
// ��������� �������� ������� t ��� ������ channel
{
  cN[channel] = t;
}

void ShimSetAng(int channel, int a)
// ��������� �������� ���� �������� a ������ channel
{
  int t;
  t = ANG2POS(a);
  cN[channel] = t;
}

void ShimReset(void)
// ��������� ��������� ��� � ��������� ��������� (������������� ���)
{
  int i;
  for(i=0;i<SHIM_NCH;i++)
    ShimSet(i, shimStop);
}

void ShimInc(int channel)
{               
  int val;
  val = cN[channel]+shimStep;
  if(val<=shimMaxT)
    ShimSet(channel, val);
}

void ShimDec(int channel)
{               
  int val;
  val = cN[channel]-shimStep;
  if(val>=shimMinT)
    ShimSet(channel, val);
}

void ShimOut(int channel, int stat)
//������ �������
{
  switch(channel)
  {
    case 0: shim0 = stat; break;
    case 1: shim1 = stat; break;
  }
}

void ShimOutputImpuls(void)
{
  int i;
  for(i=0;i<SHIM_NCH;i++)
  {
    //������ �������
    ShimOut(i, 1);
    if(ShimReverse[i]) // �������� ��� ���-i
      Delay_SMS(shimMaxT-cN[i]+shimMinT);
    else
      Delay_SMS(cN[i]);
    ShimOut(i, 0);    
  }
}

//----------------------------------------------------------            

void Delay_SMS(int sms)
//�������� � 0.01 ms
{                          
  int i;
  for(i=0;i<sms;i++)
    delay_us(10-DLT_CNT); // 0.01ms
}

//----------------------------------------------------------  
