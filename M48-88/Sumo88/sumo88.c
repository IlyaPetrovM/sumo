/*****************************************************
Project : �����-������� uS RPM-1
Version : 3.00
Date    : 19.08.2013 
LP      : 30.09.2014
Author  : ������ ���� ����������, ������� ��������� �������������
Company : ����������� �������������
Comments: 4 ������� Sharp-��������, 1 ������ QRD1114,
          2 �����-��������� Gekko Turbo (1:50), 
          �����-���������� ����������� 2 �����
Chip type           : ATmega88
Clock frequency     : 7,372800 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 128
*****************************************************/
/*****************************************************
Features:
3.0
    + ��� PWM
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
//#include "pwm48lib.h"

//--------------------------
const short maxPower = 100;
const short minPower = 30;
const int pwmT       = 100; // ������ �������� ���������� ��������  (� ������)
//--------------------------

#define LimLeft  (BYTE)100 // ����� ������������ �������� ������
#define LimRight (BYTE)100
#define LINE_T   (BYTE)150// ����� ������� �� �����
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
    SHARP_FR
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

BYTE SharpFL, SharpFR, SharpSL, SharpSR, FotoL, FotoR;

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
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
    SharpFL = (sen_1==0);
    SharpFR = (sen_2==0);
    SharpSL = (sen_3==0);
    SharpSR = (sen_4==0);
    
    if (SharpFL && SharpFR) 
        {robotStop();} 
    else
    {
        if (SharpFL) {goLeft();  }
        if (SharpFR) {goRight(); }                  
    }
    if (SharpSL) {goFastLeft(); }
    if (SharpSR) {goFastRight();}     
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

//int tmr = pwmT;   
//setPWM(int pwr1, int pwr2)
//{ 
//  if( tmr ==0)
// {
//   PWMSetPc(pwr1,pwr2);
//   tmr = pwmT ;
// }
//}
   

//----------------------------------------------------------

void main(void)
{
  int T;  // ������� ������ ��� ����� ������� ������
  int a;  // ���� ���������, 0 - �����, 1 - ������, 2 - �����
   
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

  
  // ���������� ���������
  PORTB.1 = 1;
  PORTB.2 = 1;      
       
//  InitPWM(50, 8);
//  PWMSetPc(100,100);
 
  robotStop();

  
  pip();
  printf("\nSumo 88 uS RPM-1.1 Version 3.00 24.01.2014\n");

//--------------------------------------------------------
// ������� � ���������� �����
// ��� �������� � ���������� ����� ����������, ���� ����� ���������� ������� 
// ������� ���������� �� ������� ����
//--------------------------------------------------------
//  FotoL = ReadByteADC(6)>LimLeft;
  FotoR = ReadByteADC(ADC_E2)>LimRight;
  
  if(FotoR)
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
  
  // ������ ����������� ��������� �� �����, ��� ��������� ������
  ef_1 = 1;
  delay_ms(10);
  ef_1 = 0; 
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
          // ��������� �����
        case(FOTO_L):{  
//        PWMSetPc(100,100);
        
          robotStop(); delay_ms(10); 
          goBack();
          delay_ms(LINE_T/2); 
          // �������� ���������
          LeftMotorBack();
          delay_ms(LINE_T/2);
          goFastRight();
          T = 0;
          break;
        }
    //  ---*-
    //  --O--
    //  -----
        case(FOTO_R):
        {
          // ��������� �����
//          PWMSetPc(100,100);
          
          robotStop(); delay_ms(10); 
          goBack();
          delay_ms(LINE_T/2); 
          RightMotorBack();
          delay_ms(LINE_T/2);
          // �������� ���������
          goFastLeft(); 
          T = 0;
          break;                       
        }  
    }

// if (last_sens != sens )  
//  {
//    robotStop();
//    delay_ms(5);           
     // �������� ��� ������������ ������                                                   
//  }   
//------------------------------------------------------
// ����� ��������
// ����������� ����������
//------------------------------------------------------              
    switch (sens)
    {   
       
//  -+-+-
//  --O--
//  ----- 
        case SHARP_FLR:
            { 
//            setPWM(maxPower,maxPower);
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
//              setPWM(minPower,      0);
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
              //setPWM(0,     minPower);
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
             //setPWM(maxPower,maxPower);  
             goLeft();  
//             goFastLeft();
              T = 0;
              a = LEFT;
              break;
            }
//  -----
//  --O+-
//  -----     
        case SHARP_SR:
            {     
              //setPWM(maxPower,maxPower);  
              goRight();  
//              goFastRight();
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
//                if(T>AGRESSIVE_T)
//                {  // ��������� ����� ������, ����� � ���������� ����������� ��� ����� 1000 ������
                    if(a==LEFT) goFastLeft();
                    if(a==RIGHT) goFastRight();
                    if(a==FWD) goFwd();
//                } 
//                else
//                { // ����������� �����: ����� �� ��� ������, ��� ����� ���������� � ��������� �����
//                    if(a==LEFT) goFastLeft();
//                    if(a==RIGHT) goFastRight();
//                }
                   
            }
    }
    T=T+1;
//    tmr--;
    last_sens = sens; // ���������� ���������� ���������
    sens = NO_SENS; // ���������� �������� ��-���������
    
    // ��������� ������� ��������
//    if(ReadByteADC(6)>LimLeft) sens = FOTO_L;
    if(ReadByteADC(7)>LimRight)sens = FOTO_R;
    SharpFL = (sen_3==0);
    SharpFR = (sen_4==0);

    if(SharpFL && SharpFR) 
    {
        sens = SHARP_FLR;
    }
    else  
    {
        if(SharpFL) sens = SHARP_FL;
        if(SharpFR) sens = SHARP_FR;  
    }
    if(sen_1==0) sens = SHARP_SR;        
    if(sen_2==0) sens = SHARP_SL;
    
    
        
    if(T>MAXT) // ���� ������ �������
    {
      pip();
      T=0;
      // �������� �������� "���������" �������
      a=rand()%3;  // ������� �� ������� �� 3 (������� �� ������ 3)
    }    
  }
}
