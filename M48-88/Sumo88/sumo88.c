/*****************************************************
Project : �����-������� FreeSumo RP-2
Version : 1.00
Date    : 19.08.2013 
LP      : 24.04.2015
Author  : ������ ���� ����������
Company : ����������� �������������
Comments: 4 ������� Sharp-���������� (max-��������� 30cm), 1 ������ QRD1114,
          2 �����-��������� LT25GA34-370T (1:34) 185rpm, 
          �����-���������� ����������� 3700mAh 7,4V 45C 2S
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

#define FREE_SUMO /// ��� ��������
#define FEEDBACK_TESTING  /// ������� ������� ��� ������������  

/// ������������ ��������� ���������. ������� ��������� � �������� � �.�.
/* ������������ �����
            -----------------
    Motor-L-+---..          |
            |               |
    Motor-R-+---.. ___      |
     PIND.3-+..   |M88|   ..|-ADC_7
     PIND.4-+..           ..|-ADC_8
            |    ::::::     |
            |    ||||||     |
            -----++++++------
                 ||||||_ADC_1
                 |||||__ADC_2
                 ||||___ADC_3
                 |||____ADC_4
                 ||_____ADC_5
                 |______ADC_6
*/   
#ifdef FREE_SUMO
    #define ROBOT_NAME "FreeSumo RP-2"
    #define VERSION "1.00"      
    
    #define LimSharpFwd       (BYTE)  60  // 100->16cm  50-> 30cm 165->6  
    #define LimSharpClose     (BYTE)  120 // ��� �������� ����������
    #define LimSharpSide      (BYTE)  165 // 165->6cm
    #define LINE_LimLeft      (BYTE)  150 // ����� ������������ �������� ������
    #define LINE_LimRight     (BYTE)  200
    #define LINE_BACK_DELAY   (BYTE)  300 // ����� ������� �� �����
    #define LINE_TANK  (BYTE) 1000  // �������� ����� ���������� ��� ��������� �� �����
    #define TANK_START (BYTE) 1000  //�������� ����� ���������� ��� ������
    #define MAXT 2500    // ����� ����� ������� (���������� ������)

    // ������� �������� ��������
    #define CHECK_FOTO_L ReadByteADC(7)>LINE_LimLeft
    #define CHECK_FOTO_R ReadByteADC(8)>LINE_LimRight

    #define CHECK_START_SENSOR (PIND.4)==0  /// ����� �� ������� � ������ (����� �������� 5 ���)
            
    #define CHECK_SHARP_FC ((ReadByteADC(ADC_1)>LimSharpClose) || (ReadByteADC(ADC_2)>LimSharpClose)) 
    #define CHECK_SHARP_FL ReadByteADC(ADC_1)>LimSharpFwd  // 
    #define CHECK_SHARP_FR ReadByteADC(ADC_2)>LimSharpFwd  // 

    #define CHECK_SHARP_SL ReadByteADC(ADC_3)>LimSharpSide // 
    #define CHECK_SHARP_SR ReadByteADC(ADC_4)>LimSharpSide //        
#endif

//-------------------------------------------------------------
/// ��������� �������� (��� ����������� ����������� ���������) 
//-------------------------------------------------------------
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
enum {FAST, SLOW};
short unsigned int sens = NO_SENS;            // ������� ���������
short unsigned int last_sens = NO_SENS;       // ���������� ���������
BYTE SharpFL, SharpFR, SharpFC, SharpSL, SharpSR, FotoL, FotoR;

/***********************************************************
\brief ���������� �����
\param void
\return void
************************************************************/
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
  
   
  while (1)
  {         
    /** ������� � ������� ������� */    
#ifdef FEEDBACK_TESTING                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
    SharpFL = CHECK_SHARP_FL;
    SharpFR = CHECK_SHARP_FR;
    
    SharpSL = CHECK_SHARP_SL;
    SharpSR = CHECK_SHARP_SR;  
    #ifdef CHECK_SHARP_FC 
        SharpFC = CHECK_SHARP_FC;  
    #endif
   if(SharpFC)
   {     
        goFwd(); 
   }
   else
   {
        if (SharpFL && SharpFR) 
        {
            goBack(); 
        } 
       else
        {
        if (SharpFL)      {goLeft();  } 
        else if (SharpFR) {goRight(); }
        else if (SharpSL) {goFastLeft(); } 
        else if (SharpSR) {goFastRight();}
        else robotStop();  
        }
    }     
#endif
    /** ������� � ������� ��������� */
    /// ���������� �������� ���� �������� 
    re1 = ReadByteADC(ADC_E1);
    re2 = ReadByteADC(ADC_E2);

    a1 = ReadByteADC(ADC_1);
    a2 = ReadByteADC(ADC_2);
    a3 = ReadByteADC(ADC_3);
    a4 = ReadByteADC(ADC_4);
    a5 = ReadByteADC(ADC_5);
    a6 = ReadByteADC(ADC_6);
    /// ����� �������� �������� �� �����  
    printf("(%4u %4u) (%4u %4u) %4u %4u %4u %4u\r", re1, re2, a1, a2, a3, a4, a5, a6);   
  }
}

/***********************************************************
\brief ������� ���������
\param void
\return void
************************************************************/
void main(void)
{
  int T;  //< ������� ������ ��� ����� ������� ������ 
  int a;  //< ���� ���������, 0 - �����, 1 - ������, 2 - �����    
  int spd=FAST;

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
     
  /** 
   \brief �������������� (D.3  � D.4) ��� �������� ���������� ����
   Port D initialization
   Func7=Out Func6=Out Func5=Out Func4=In !!!Func3=In Func2=Out Func1=In Func0=In 
   State7=0 State6=0 State5=0 State4=P !!!!State3=0 State2=0 State1=T State0=T    
         
  ���������: 
    PORTD 0001 1000  
    Func  oooi iooo (i - in, o - out)
    
    DDRD  1110 1100
    State 000P 00TT

  PORTD=0x18;
  DDRD=0xEC; 
   */ 
  
  // ��������� ��������� �� �����. ���������� �� ����������
  PORTB.1 = 1;
  PORTB.2 = 1;
 
  robotStop();
  pip();  
  
  printf(ROBOT_NAME " " VERSION " (" __DATE__ " " __TIME__ "') file:" __FILE__);

//--------------------------------------------------------
// ������� � ���������� �����
//--------------------------------------------------------
/** ��� �������� � ���������� ����� ����������, 
    ����� ����� ���������� ������� 
    ������� ���������� �� ������� ����     
*/
#ifdef CHECK_FOTO_L 
    FotoL = CHECK_FOTO_L;
#endif    
#ifdef CHECK_FOTO_R
    FotoR = CHECK_FOTO_R;
#endif
  
  if(FotoL && FotoR)
  {
   DebugProc();
  }
//--------------------------------------------------------
// ����� �������� ����� ������
//--------------------------------------------------------
/** ��� �������� ����������� ����������, 
    ����� ������� ������ ����������� � ��� �������, 
    ���� ��� ���������� ������������
*/
  /// ���������� ��������
  sens = NO_SENS;
  while(sens == NO_SENS) 
  {
    if(CHECK_SHARP_SL) 
    {
        sens = SHARP_SL; 
        a = LEFT;  
    } 
    if(CHECK_SHARP_SR)
    { 
        sens = SHARP_SR; 
        a = RIGHT;
    }
  }   
  spd=FAST;// ���������� ��������� �������� ���������
  
  /// �������� ��������� �������
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
#ifdef CHECK_START_SENSOR
  while(CHECK_START_SENSOR);
#else
  delay_ms(5000);
#endif
  T = 0;      
//--------------------------------------------------------
// �������� ����
//--------------------------------------------------------
  while (1)
  {
/** ������� � ���� ��������� ����� ������������ �������� */
    /// ������� �� ������ ���� 
#ifdef CHECK_START_SENSOR
    if(CHECK_START_SENSOR)
    {
      robotStop();
      pip();
      while(1);
    } 
#endif
//------------------------------------------------------
// ������� �� ������� ����
//------------------------------------------------------     
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
            
        /// ������ ��������
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
                if(spd==FAST && a==LEFT) 
                    goFastLeft();
                else if(spd==FAST && a==RIGHT) 
                    goFastRight();  
                else if(spd==SLOW && a==LEFT) 
                    goLeft(); 
                else if(spd==SLOW && a==RIGHT) 
                    goRight();     
                else if(a==FWD) 
                    goFwd();
            }
    }
    T=T+1; 
    
    last_sens = sens; //< ���������� ���������� ���������
    sens = NO_SENS; //< ���������� �������� ��-���������

//------------------------------------------------------
/// ��������� ������� ��������
//------------------------------------------------------    
#ifdef CHECK_FOTO_L 
    FotoL = CHECK_FOTO_L;
#endif    
#ifdef CHECK_FOTO_R
    FotoR = CHECK_FOTO_R;
#endif
    SharpFL = CHECK_SHARP_FL;  
    SharpFR = CHECK_SHARP_FR; 
    SharpSL = CHECK_SHARP_SL;
    SharpSR = CHECK_SHARP_SR;
#ifdef CHECK_SHARP_FC 
    SharpFC = CHECK_SHARP_FC;  
#endif

//------------------------------------------------------
/// ����������� ��������� ������
//------------------------------------------------------ 
#ifdef CHECK_FOTO_L 
    if(FotoR) sens = FOTO_R;
#endif    
#ifdef CHECK_FOTO_R
    if(FotoL) sens = FOTO_L; 
#endif
    if(SharpFL && SharpFR) 
    {
        sens = SHARP_FLR;
    }
    else  
    {
        if(SharpFL) sens = SHARP_FL;
        if(SharpFR) sens = SHARP_FR;  
    }           
    if(SharpSL) sens = SHARP_SL; // red
    if(SharpSR) sens = SHARP_SR; // blue  
#ifdef CHECK_SHARP_FC   
    if(SharpFC) sens = SHARP_FC;   
#endif  
//------------------------------------------------------
/// ����� �������
//------------------------------------------------------ 
    if(T>MAXT) //!< ���� ������ �������
    {
      pip();
      T=0;
      /// �������� �������� "���������" �������
      a=rand()%3;  // ������� �� ������� �� 3 (������� �� ������ 3)  
      spd=rand()%2;
    }    
  }
}
