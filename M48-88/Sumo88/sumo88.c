/*****************************************************
Project : �����-������� uS RP-1.1 
Version : 0.01 
Date    : 26.04.2010
LP      : 03.10.2014
Author  : ������ ������� ����������, ������ ���� ����������
Company : ����������� �������������
Comments: 4 ������� Sharp-��������, 1 ������ QRD1114,
          2 �����-��������� Gekko Turbo (1:50), 
          �����-������� ����������� (2 �����), ����� ��� ���������� ������
Chip type           : ATmega88V
Clock frequency     : 7,372800 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 128
*****************************************************/

#include <mega88.h>
#include <math.h>
#include <stdlib.h>
#include "m48lib.h"

#define LimLeft  (BYTE)250 // ����� ������������ �������� ������
#define LimRight (BYTE)250
#define LimSharp  40// ����� ������������ �������� ����
#define MAXT 1500    // ����� ����� ������� (���������� ������)

BYTE SharpFL, SharpFR, SharpSL, SharpSR, SharpCL, SharpCR, FotoL, FotoR;
enum {
    LEFT,
    RIGHT,
    FWD
};
void DebugProc(void)
{
  BYTE re1, re2;
  BYTE a1, a2, a3, a4, a5, a6;
  
  printf("\r\nTEST REGIME\r\n");

  // ���������� ������������ 
  // (�������� ������������ ����������� ����������: ������, �����, �����, ������)
  #define Time 500
  goFwd(); delay_ms(Time);
  goBack(); delay_ms(Time);
  goLeft(); delay_ms(Time);
  goRight(); delay_ms(Time); 
  robotStop(); 
  pip();
  
  goFastLeft(); delay_ms(Time);
  goFastRight(); delay_ms(Time); 
  robotStop(); 
  pip();

  while (1)
  {   
    re1 = ReadByteADC(ADC_E1);
    re2 = ReadByteADC(ADC_E2);  // QRD

    a1 = ReadByteADC(ADC_1);  // SharpSR
    a2 = ReadByteADC(ADC_2);  // SharpSL
    a3 = ReadByteADC(ADC_3);  // SharpFL
    a4 = ReadByteADC(ADC_4);  // SharpFR
    a5 = ReadByteADC(ADC_5);  
    a6 = ReadByteADC(ADC_6);  
    
    delay_ms(20);
    
    printf("(%4u %4u) (%4u %4u) %4u %4u %4u %4u\r", re1, re2, a1, a2, a3, a4, a5, a6);
  }
}

//----------------------------------------------------------

void main(void)
{
  int n;
  int T;  // ������� ������ ��� ����� ������� ������
  int a;  // ���� ���������, 0 - �����, 1 - ������, 2 - �����
  
  // ������������� �����������
  M48Init();
  
  // Port C initialization
  // Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
  // State6=T State5=P State4=P State3=P State2=P State1=T State0=T 
  PORTC=0x3C;
  DDRC=0x00;
  
  // ��������� ��������� �� �����. ���������� �� ����������
  PORTB.1 = 1;
  PORTB.2 = 1;
 
  robotStop();

  pip();
  printf("\r\nSumo 88 uS RP-1.1 Version 0.01\r\n\r\n");

  //--------------------------------------------------------
  // ������� � ���������� �����
  // ��� �������� � ���������� ����� ����������, ���� ����� ���������� ������� 
  // ������� ���������� �� ������� ����
  //--------------------------------------------------------
  FotoR = ReadByteADC(ADC_E2)>LimRight;
  if(FotoR)
    DebugProc();
 
  robotStop();
    

  // ������ ����������� ��������� �� �����, ��� ��������� ������
//  while(FotoR!=1) 
//  {
//  SharpSR = (sen_1==0);
//  FotoR = ReadByteADC(ADC_E2)>LimRight;
//  if(SharpSR)
//  { 
//        ef_1 = 1;
//  }
//  else
//  { ef_1 = 0; }
//  }

//--------------------------------------------------------
// ����� �������� ����� ������
//--------------------------------------------------------
  a = -1;
  while(a == -1) 
  {
    SharpSR = (sen_1==0);
    SharpSL = (sen_2==0);
    if(SharpSL) 
    {
        a = LEFT;  
    } 
    if(SharpSR)
    { 
        a = RIGHT;
    }

  }
  // �������� ��������� �������
  if(SharpSL) 
  {

    pip(); delay_ms(200); 
    goLeft(); 
  }
  if(SharpSR) 
  {
    pip(); delay_ms(150); 
    robotStop(); delay_ms(100); 
    pip(); delay_ms(150); 
    goRight(); 
  }
  delay_ms(10);
  robotStop(); 
    
//--------------------------------------------------------
// �������� ������� START
//--------------------------------------------------------
  while(SENSOR_START==0);
  //delay_ms(5000);   
  T = 0;
  // ������ ����������� ��������� �� �����, ��� ��������� ������
  ef_1 = 1;
  delay_ms(100);
  ef_1 = 0;
  
  T = 0;
  a = 0;
        
  //--------------------------------------------------------
  // �������� ����
  //--------------------------------------------------------
  while (1)
  {
    // ��������� ������� ��������
    
//    //������ �����������
//    SharpFL = ReadByteADC(ADC_1)>LimSharp;
//    SharpFR = ReadByteADC(ADC_2)>LimSharp;
//    SharpSL = (sen_3==0);
//    SharpSR = (sen_4==0);
//    SharpCL = (sen_5==0);
//    SharpCR = (sen_6==0);
    
    //������� ���������� ��-�������!
    SharpSR = (sen_1==0);
    SharpSL = (sen_2==0);
    
    SharpFL = (sen_3==0);
    SharpFR = (sen_4==0);
    
    FotoR = ReadByteADC(ADC_E2)>LimRight;
    
    //------------------------------------------------------
    // ����������� ��������
    // ������� � ���� ��������� ����� ������������ ��������
    //------------------------------------------------------
    // ������� �� ������� ����
    
    if(FotoL)
    {
      // ��������� �����
      goBack();
      delay_ms(100);
      // �������� ���������
      goFastRight();
      T = 0;
      continue;
    }
    if(FotoR)
    {
      // ��������� �����
      goBack();
      delay_ms(100);
      // �������� ���������
      goFastLeft();
      T = 0;
      continue;
    }
    //------------------------------------------------------
    // ����� ��������
    // ����������� ���������� ��������� ���������
    //------------------------------------------------------
    if(SharpFL&&SharpFR)
    {
      goFwd();
      T = 0;
      continue;
    }
    if(SharpSL)
    {
      goFastLeft();
      T = 0;
      a = 0;
      continue;
    }                 
    if(SharpSR)
    {
      goFastRight();
      T = 0;
      a = 1;
      continue;
    }
    //------------------------------------------------------
    // ������ �� ����������
    // ����� ����������
    //------------------------------------------------------
    if(a==0) goLeft();
    if(a==1) goRight();
    if(a==2) goFwd();
    
    T=T+1;
    if(T>MAXT) // ���� ������ �������
    {
      //pip();
      T=0;
      // �������� �������� "���������" �������
      a=rand()%3;  // ������� �� ������� �� 3 (������� �� ������ 3)
    }    
  }
}
