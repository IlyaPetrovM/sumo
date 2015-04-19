/*****************************************************
Project : �����-������� mS RPM-2 "Gekko"
Version : 1.00
Date    : 19.08.2013 
LP      : 26.02.2014
Author  : ������ ���� ����������, ������� ��������� �������������
Company : ����������� �������������
Comments: 5 ������� Sharp-��������, 2 ������� QRD1114,
          2 �����-��������� Gekko Turbo (1:50), 
          �����-���������� �����������
Chip type           : ATmega88
Clock frequency     : 7,372800 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 128
*****************************************************/
/*****************************************************
Features:
2.02
    + ����������� ������ SharpFC  
    + �������� �������� ��� ������ ����������  ���������� ������ ��� ������ � ������� �� ����� 
2.01
    + ��������� � ��������� �������� ����� switch � ��������� �������� �������, � ������� �������� ����� ��� ���� ����
    + ��������� �������� � ����� ������ �� ������� � �� ����������
    + ��������� ����������� ������� �������� ����� ���������� ���������
2.00
    + �������� ����������� ��������� ������
    + ������� �������� ����� �������� �������� ���������� (����� ������ ��������� ������ �� ����)
*****************************************************/
#include <mega88.h>
#include <math.h>
#include <stdlib.h>
#include "m48lib.h"

#define LINE_LimLeft  (BYTE)100 // ����� ������������ �������� ������
#define LINE_LimRight (BYTE)100
#define LINE_BACK_DELAY   (BYTE)  300// ����� ������� �� �����
#define LINE_TANK (BYTE) 1000      // �������� ����� ���������� ��� ��������� �� �����

#define TANK_START (BYTE) 1000  //�������� ����� ���������� ��� ������

#define MAXT 2500    // ����� ����� ������� (���������� ������)

//-------------------------------------------------------------
//--��������� �������� (��� ����������� ����������� ���������) 
enum {
    NO_SENS,
    FOTO_L,   
    FOTO_R,
    SHARP_SL,
    SHARP_SR, 
    SHARP_FLR,
    SHARP_FL,
    SHARP_FR,
    SHARP_FC
};
enum {
    LEFT,
    RIGHT,
    FWD
};
short unsigned int sens = NO_SENS;            // ������� ���������
short unsigned int last_sens = NO_SENS;       // ���������� ���������
//-------------------------------------------------------------


#define SENSOR_START PIND.4 // ��������� ������

BYTE SharpFL, SharpFR, SharpFC, SharpSL, SharpSR, FotoL, FotoR;

void DebugProc(void)
{
  BYTE re1, re2;
  BYTE a1, a2, a3, a4, a5, a6;
  
  printf("\r\nTEST REGIME\r\n");

  // ���������� ������������ 
  delay_ms(3000);
  #define Time 150
  // (�������� ������������ ����������� ����������: ������, �����, �����, ������)
  goFwd(); delay_ms(Time);        robotStop(); delay_ms(Time+200);
  goBack(); delay_ms(Time);       robotStop(); delay_ms(Time+200);
  goFastRight(); delay_ms(Time);  robotStop(); delay_ms(Time+200);
  goFastLeft(); delay_ms(Time);   robotStop(); 
  pip();
  // �������� ������������ ����������� �������� ��������
  //if(FotoL) goFastLeft(); delay_ms(Time);
  //if(FotoR) goFastRight(); delay_ms(Time); 
  //robotStop(); 
  pip();
           
  while (1)
  {     
    // ������� � ������� �������                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
    SharpFL = (sen_1==0);
    SharpFR = (sen_2==0);
    SharpSL = (sen_3==0);
    SharpSR = (sen_4==0);
    SharpFC = (sen_5==0); // Sharp Front Center
    
    if(SharpFC)
        goFwd();
    else
    {    
        if (SharpFL && SharpFR) 
        {
            robotStop();
        } 
        else
        {
            if (SharpFL) {goLeft();  }
            if (SharpFR) {goRight(); }                  
        }
        if (SharpSL) {goFastLeft(); }
        if (SharpSR) {goFastRight();} 
    } 
    
    // ����� �������� �������� �� �����   
    re1 = ReadByteADC(ADC_E1);
    re2 = ReadByteADC(ADC_E2);

    a1 = ReadByteADC(ADC_1);
    a2 = ReadByteADC(ADC_2);
    a3 = ReadByteADC(ADC_3);
    a4 = ReadByteADC(ADC_4);
    a5 = ReadByteADC(ADC_5);
    a6 = ReadByteADC(ADC_6);
    
    printf("(%4u %4u) (%4u %4u) %4u %4u %4u %4u\r", re1, re2, a1, a2, a3, a4, a5, a6);   
  }
     

}

    //FotoL = ReadByteADC(6)>LINE_LimLeft;
    //FotoR = ReadByteADC(7)>LINE_LimRight;
   //  if (FotoL) {goBack(); delay_ms(500); goLeft(); delay_ms(500); robotStop();}
  // if (FotoR) {goBack(); delay_ms(500); goRight(); delay_ms(500); robotStop();}
    
 
   

//----------------------------------------------------------

void main(void)
{
  int T;  // ������� ������ ��� ����� ������� ������ 
  int a;  // ���� ���������, 0 - �����, 1 - ������, 2 - ����� 

  unsigned short tTankLimit=TANK_START; // ����������� ���������� ������ ��������� ���������
  unsigned short tTank=0;         // ������� ������ ��������� ��������� ��� ������ ����������  
  
  // ������������� �����������
  M48Init();
  
  // Port C initialization
  // Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
  // State6=T State5=P State4=P State3=P State2=P State1=T State0=T 
  PORTC=0x3C;
  DDRC=0x00;

  // �������������� ef_2 (D.4) ��� �������� ���������� ����
  // Port D initialization
  // Func7=Out Func6=Out Func5=Out Func4=In Func3=Out Func2=Out Func1=In Func0=In 
  // State7=0 State6=0 State5=0 State4=P State3=0 State2=0 State1=T State0=T 
  PORTD=0x10;
  DDRD=0xEC;

  
  // ��������� ��������� �� �����. ���������� �� ����������
  PORTB.1 = 1;
  PORTB.2 = 1;
 
  robotStop();

  
  pip();
  printf("\r\nSumo 88 mS RPM-1 'Floppy' Version 2.01 07.12.2013\r\n\r\n");

//--------------------------------------------------------
// ������� � ���������� �����
// ��� �������� � ���������� ����� ����������, ���� ����� ���������� ������� 
// ������� ���������� �� ������� ����
//--------------------------------------------------------
  FotoL = ReadByteADC(6)>LINE_LimLeft;
  FotoR = ReadByteADC(7)>LINE_LimRight;
  
  if(FotoL && FotoR)
  {
   DebugProc();
  }
 
  robotStop();


//--------------------------------------------------------
// ����� �������� ����� ������
//--------------------------------------------------------
  sens = NO_SENS;
  while(sens == NO_SENS) 
  {
    if(sen_3==0) 
    {
        sens = SHARP_SL; 
        a = LEFT;  
    } 
    if(sen_4==0)
    { 
        sens = SHARP_SR; 
        a = RIGHT;
    }

  }
  // �������� ��������� �������
  if(sens == SHARP_SL) 
  {
    pip(); delay_ms(200); 
    goLeft(); 
  }
  if(sens == SHARP_SR) 
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
//--------------------------------------------------------
// �������� ����
//--------------------------------------------------------
  while (1)
  {

//------------------------------------------------------
// ����������� ��������
// ������� � ���� ��������� ����� ������������ ��������
//------------------------------------------------------

    // ������� �� ������ ����
    if(SENSOR_START==0)
    {
      robotStop();
      pip();
      while(1);
    }
    
    // ������� �� ������� ����
     switch (sens)
    { 
          
    //  -*---
    //  --O--
    //  -----      
        case(FOTO_L):{
          
          robotStop(); 
          delay_ms(3);
           
          goBack(); // ��������� �����
          delay_ms(LINE_BACK_DELAY);
           
          goFastRight(); // �������� ���������
           
          // ���������� ����� �������� ����� ��� ���������
          a=RIGHT;
          tTank=0;
          tTankLimit=LINE_TANK;

          T = 0;
          break;
        }
    //  ---*-
    //  --O--
    //  -----
        case(FOTO_R):
        {
          robotStop(); 
          delay_ms(3); 
          
          goBack();  // ��������� �����
          delay_ms(LINE_BACK_DELAY); 
        
          goFastLeft(); // �������� ���������
          
          // ���������� ����� �������� ����� ��� ���������
          a=LEFT;
          tTank=0;
          tTankLimit=LINE_TANK;
          
          T = 0;
          break;                       
        }  
    }

//------------------------------------------------------
// ����� ��������
// ����������� ����������
//------------------------------------------------------              
    switch (sens)
    {   
//  --+--
//  --O--
//  ----- 
        case SHARP_FC:
            {  
              goFwd();
              T = 0;
              break;
            }       
//  -+-+-
//  --O--
//  ----- 
        case SHARP_FLR:
            {  
              goFwd();
              T = 0;
              break;
            }
        // ������� �������� 
//  -+---
//  --O--
//  -----    
        case SHARP_FL:
            { 
              //goFwd();
              goLeft();
              T = 0;
              a = LEFT;
              break;
            } 
        
//  ---+-
//  --O--
//  -----        
        case SHARP_FR:
            { 
              //goFwd();          
              goRight();
              T = 0;            
              a = RIGHT;
              break;
            }  
            
        // ������ ��������
//  -----
//  -+O--
//  ----- 
        case SHARP_SL:                                 
            {     
            //goLeft();                    
             goFastLeft();
              T = 0;
              a = LEFT;
              break;
            }
//  -----
//  --O+-
//  -----     
        case SHARP_SR:
            {     
            //goRight();        
              goFastRight();
              T = 0;
              a = RIGHT;
              break;
            }
         default: 
            { 
//------------------------------------------------------
// ������ �� ����������
// ����� ����������
//------------------------------------------------------
                if(a==LEFT) 
                {
                    if(tTank>tTankLimit)
                    {    
                        goLeft();
                    }
                    else
                    {   
                        goFastLeft();
                        tTank++;
                    }  
                }
                if(a==RIGHT) 
                {       
                    if(tTank>tTankLimit)
                        goRight();
                    else
                    {
                        goFastRight();
                        tTank++; 
                    }
                }
                if(a==FWD) goFwd();
                   
            }
    }
    T=T+1; 
    
    last_sens = sens; // ���������� ���������� ���������
    sens = NO_SENS; // ���������� �������� ��-���������
    
    // ��������� ������� ��������
    if(ReadByteADC(6)>LINE_LimLeft) sens = FOTO_L;
    if(ReadByteADC(7)>LINE_LimRight)sens = FOTO_R;
    SharpFL = (sen_1==0);
    SharpFR = (sen_2==0);

    if(SharpFL && SharpFR) 
    {
        sens = SHARP_FLR;
    }
    else  
    {
        if(SharpFL) sens = SHARP_FL;
        if(SharpFR) sens = SHARP_FR;  
    }
            
    if(sen_3==0) sens = SHARP_SL;
    if(sen_4==0) sens = SHARP_SR;
    if(sen_5==0) sens = SHARP_FC;
       
    if(T>MAXT) // ���� ������ �������
    {
      pip();
      T=0;
      // �������� �������� "���������" �������
      a=rand()%3;  // ������� �� ������� �� 3 (������� �� ������ 3)
    }    
  }
}
