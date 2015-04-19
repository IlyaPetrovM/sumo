/*****************************************************
Project : 
Version : 
Date    : 26.04.2010
Author  :
Company :
Comments: 

Chip type           : ATmega88PA
Clock frequency     : 7,372800 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 128
*****************************************************/

#include <mega88p.h>
#include "m48lib.h"

//----------------------------------------------------------

void main(void)
{
  BYTE re1, re2;
  BYTE a1, a2, a3, a4, a5, a6;
  
  M48Init();
        
  printf("\r\nM88PA demo\r\n");

  pip();

#define T 1000
PORTB.1 = 1;
PORTB.2 = 1;

  robotStop();

  goFwd();
  delay_ms(T);

  goBack();
  delay_ms(T);

  goLeft();
  delay_ms(T);

  goRight();
  delay_ms(T);
 
  robotStop();
 
  pip();
  
  while (1)
  { //char c;
    //c = getchar();
    //printf("[%c] %3d\r\n",c, (int)c);
    
    re1 = ReadByteADC(ADC_E1);
    re2 = ReadByteADC(ADC_E2);

    a1 = ReadByteADC(ADC_1);
    a2 = ReadByteADC(ADC_2);
    a3 = ReadByteADC(ADC_3);
    a4 = ReadByteADC(ADC_4);
    a5 = ReadByteADC(ADC_5);
    a6 = ReadByteADC(ADC_6);
    
    delay_ms(20);
    
    printf("%4u %4u   %4u %4u %4u %4u %4u %4u\r", re1, re2, a1, a2, a3, a4, a5, a6);
  }
}
