/*****************************************************
Project : �����, ���������� �� ������ �� ��������� ������� ����������
Version : 1.04
Date    : 26.04.2010
Author  :
Company :
Comments: 
LP      : 07.05.2010
Chip type           : ATmega48
Clock frequency     : 7,372800 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 128
*****************************************************/

#include <math.h>
#include "m48lib.h"
#include "pwm48lib.h"
#include "lctl48.h"

//----------------------------------------------------------

BYTE Vmax = 100;

void WaitPusk(void)
{
  pip();
  while(Pusk) delay_ms(15);  
  while(!Pusk) delay_ms(15);
  while(Pusk) delay_ms(15);  
  pip();
}

// ���������� ����������
int LimLeft, LimRight;
int FLW, FLB, FRW, FRB;

int U2Speed(int u, int FotoMin, int FotoMax)
// ���������� (������ �� �������) � ��������
{
  int v;
  v = (u-FotoMin)*(Vmax-Vmin)/(FotoMax-FotoMin);
  if(v<Vmin) v = Vmin;
  if(v>Vmax) v = Vmax;
  return v;
}

void Calibrate(void)
{
  #define NR 10
  int r;
  BYTE i;

  WaitPusk();
  //��������� �� ����� ����
  FLW = FRW = 0;
  for(i=0;i<NR;i++) // ��������� NR ���
  {
    r = read_adc(ADC_FL);
    FLW = FLW + r;  // ����� ���� ��������
    r = read_adc(ADC_FR);
    FRW = FRW + r;
    delay_ms(20);  
  }
  //������� ��������
  FLW = FLW/NR;
  FRW = FRW/NR;
  WaitPusk();
  FLB = FRB = 0;
  for(i=0;i<NR;i++)
  {
    r = read_adc(ADC_FL);
    FLB = FLB + r;
    r = read_adc(ADC_FR);
    FRB = FRB + r;
    delay_ms(20);  
  }
  FLB = FLB/NR;
  FRB = FRB/NR;
  // ���������� �������
  LimLeft = 3*(FLW-FLB)/4+FLB;
  LimRight = 3*(FRW-FRB)/4+FRB;

  WaitPusk();
} 

void SimpleCalibrate(void)
{
  WaitPusk();
  //��������� �� ����� ����
  FLW = FRW = 0;
  FLW = read_adc(ADC_FL);
  FRW = read_adc(ADC_FR);

  delay_ms(100);  
  
  WaitPusk();
  FLB = read_adc(ADC_FL);
  FRB = read_adc(ADC_FR);

  delay_ms(100);  

  // ���������� �������
  LimLeft = 3*(FLW-FLB)/4+FLB;
  LimRight = 3*(FRW-FRB)/4+FRB;

  WaitPusk();
} 

//----------------------------------------------------------

//----------------------------------------------------------
// ������ ����������
//----------------------------------------------------------
         
int MinVal, MaxVal;

void RgPropor(int a, int *VL, int *VR)
// ���������������� ����������
// a - ���������� ������������ ��������,
// VL, VR - �������� ������ � ������� ������
{ 
  int d;

  *VL = *VR = Vmax;
  
  if(a>0)
    d = (MaxVal-a)*(Vmax-Vmin)/MaxVal+Vmin;
  else // a<0
    d = (MinVal-a)*(Vmax-Vmin)/MinVal+Vmin;        

  if(d<Vmin) d = Vmin;
  if(d>Vmax) d = Vmax;

  if(a>0)
    *VL = d;
  else
    *VR = d;  
}

void RgCos(int a, int *VL, int *VR)
// ���������� ����������
// a - ���������� ������������ �������� (�� 0 �� 90),
{
  float COSA;
  int d;

  COSA = cos(a*PI/180.0);
  *VL = *VR = Vmax;

  d = (Vmax-Vmin)*COSA+Vmin;

  if(d<Vmin) d = Vmin;
  if(d>Vmax) d = Vmax;
  
  if(a>0)
    *VL = d;
  else
    *VR = d;  
}

void TestMotors(void)
{
#define T 1000
  
  SetPWM((Vmax+Vmin)/2, (Vmax+Vmin)/2);

  robotStop();

  goFwd();
  delay_ms(T);

  goBack();
  delay_ms(T);

  goLeft();
  delay_ms(T);

  goRight();
  delay_ms(T);
}
