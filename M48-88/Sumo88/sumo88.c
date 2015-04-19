/*****************************************************
Project : Робот-сумоист mS RPM-2 "Gekko"
Version : 1.00
Date    : 19.08.2013 
LP      : 26.02.2014
Author  : Петров Илья Михайлович, Малышев Александр Александрович
Company : Лаборатория робототехники
Comments: 5 датчиов Sharp-цифровые, 2 датчика QRD1114,
          2 мотор-редуктора Gekko Turbo (1:50), 
          литий-полимерный аккумулятор
Chip type           : ATmega88
Clock frequency     : 7,372800 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 128
*****************************************************/
/*****************************************************
Features:
2.02
    + Центральный датчик SharpFC  
    + Танковый разворот для поиска противника  включается только при старте и отъезде от линии 
2.01
    + Остановка с задержкой вынесена перед switch и добавлена проверка времени, в течение которого робот ещё ищет цель
    + Индикация поворота в после старта по моторам и по светодиоду
    + Случайное выставление времени агрессии после неудачного нападения
2.00
    + Хранение предыдущего состояния робота
    + Введена задержка перед запуском проверки двигателей (чтобы успеть поставить робота на поле)
*****************************************************/
#include <mega88.h>
#include <math.h>
#include <stdlib.h>
#include "m48lib.h"

#define LINE_LimLeft  (BYTE)100 // Порог срабатывания датчиков полосы
#define LINE_LimRight (BYTE)100
#define LINE_BACK_DELAY   (BYTE)  300// Время отъезда от линии
#define LINE_TANK (BYTE) 1000      // активный поиск противника при развороте от линии

#define TANK_START (BYTE) 1000  //Активный поиск противника при старте

#define MAXT 2500    // Время смены тактики (количество тактов)

//-------------------------------------------------------------
//--Константы сенсоров (для запоминания предыдущего состояния) 
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
short unsigned int sens = NO_SENS;            // текущее состояние
short unsigned int last_sens = NO_SENS;       // предыдущее состояние
//-------------------------------------------------------------


#define SENSOR_START PIND.4 // Стартовый сигнал

BYTE SharpFL, SharpFR, SharpFC, SharpSL, SharpSR, FotoL, FotoR;

void DebugProc(void)
{
  BYTE re1, re2;
  BYTE a1, a2, a3, a4, a5, a6;
  
  printf("\r\nTEST REGIME\r\n");

  // Отладочные телодвижения 
  delay_ms(3000);
  #define Time 150
  // (проверка правильности подключения двигателей: вперед, назад, влево, вправо)
  goFwd(); delay_ms(Time);        robotStop(); delay_ms(Time+200);
  goBack(); delay_ms(Time);       robotStop(); delay_ms(Time+200);
  goFastRight(); delay_ms(Time);  robotStop(); delay_ms(Time+200);
  goFastLeft(); delay_ms(Time);   robotStop(); 
  pip();
  // Проверка правильности подключения световых датчиков
  //if(FotoL) goFastLeft(); delay_ms(Time);
  //if(FotoR) goFastRight(); delay_ms(Time); 
  //robotStop(); 
  pip();
           
  while (1)
  {     
    // Отладка с помощью моторов                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
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
    
    // Вывод значений датчиков на экран   
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
  int T;  // Счетчик тактов для смены тактики поиска 
  int a;  // Куда крутиться, 0 - влево, 1 - вправо, 2 - вперёд 

  unsigned short tTankLimit=TANK_START; // Необходимое количество тактов танкового разворота
  unsigned short tTank=0;         // Счётчик тактов танкового разворота для поиска противника  
  
  // Инициализация контроллера
  M48Init();
  
  // Port C initialization
  // Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
  // State6=T State5=P State4=P State3=P State2=P State1=T State0=T 
  PORTC=0x3C;
  DDRC=0x00;

  // Переопределяем ef_2 (D.4) как цифровой подтянутый вход
  // Port D initialization
  // Func7=Out Func6=Out Func5=Out Func4=In Func3=Out Func2=Out Func1=In Func0=In 
  // State7=0 State6=0 State5=0 State4=P State3=0 State2=0 State1=T State0=T 
  PORTD=0x10;
  DDRD=0xEC;

  
  // Скроостью управлять не будем. Выставляем по максиммуму
  PORTB.1 = 1;
  PORTB.2 = 1;
 
  robotStop();

  
  pip();
  printf("\r\nSumo 88 mS RPM-1 'Floppy' Version 2.01 07.12.2013\r\n\r\n");

//--------------------------------------------------------
// Переход в отладочный режим
// Для перехода в отладочный режим необходимо, чтоб перед включением датчики 
// границы находились на светлом фоне
//--------------------------------------------------------
  FotoL = ReadByteADC(6)>LINE_LimLeft;
  FotoR = ReadByteADC(7)>LINE_LimRight;
  
  if(FotoL && FotoR)
  {
   DebugProc();
  }
 
  robotStop();


//--------------------------------------------------------
// Выбор поворота после старта
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
  // Проверка выбранной стороны
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
// Ожидание сигнала START
//--------------------------------------------------------
    while(SENSOR_START==0);
  
  //delay_ms(5000);
  
  T = 0;      
//--------------------------------------------------------
// Основной цикл
//--------------------------------------------------------
  while (1)
  {

//------------------------------------------------------
// Безусловные рефлексы
// Сначала и идет обработка самых приоритетных сигналов
//------------------------------------------------------

    // Реакция на сигнал СТОП
    if(SENSOR_START==0)
    {
      robotStop();
      pip();
      while(1);
    }
    
    // Реакция на границу поля
     switch (sens)
    { 
          
    //  -*---
    //  --O--
    //  -----      
        case(FOTO_L):{
          
          robotStop(); 
          delay_ms(3);
           
          goBack(); // Отъезжаем назад
          delay_ms(LINE_BACK_DELAY);
           
          goFastRight(); // Начинаем крутиться
           
          // Выставляем более активный режим для разворота
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
          
          goBack();  // Отъезжаем назад
          delay_ms(LINE_BACK_DELAY); 
        
          goFastLeft(); // Начинаем крутиться
          
          // Выставляем более активный режим для разворота
          a=LEFT;
          tTank=0;
          tTankLimit=LINE_TANK;
          
          T = 0;
          break;                       
        }  
    }

//------------------------------------------------------
// Общие действия
// Обнаружение противника
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
        // Плавные повороты 
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
            
        // резкие повороты
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
// Никого не обнаружили
// Поиск противника
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
    
    last_sens = sens; // Запоминаем предыдущее состояние
    sens = NO_SENS; // Выставляем значение по-умолчанию
    
    // Считываем сигналы датчиков
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
       
    if(T>MAXT) // Пора менять тактику
    {
      pip();
      T=0;
      // Выбираем действие "случайным" образом
      a=rand()%3;  // Остаток от деления на 3 (деление по модулю 3)
    }    
  }
}
