/*****************************************************
Project : Робот-сумоист uS RPM-1
Version : 3.00
Date    : 19.08.2013 
LP      : 30.09.2014
Author  : Петров Илья Михайлович, Малышев Александр Александрович
Company : Лаборатория робототехники
Comments: 4 датчика Sharp-цифровые, 1 датчик QRD1114,
          2 мотор-редуктора Gekko Turbo (1:50), 
          литий-полимерный аккумулятор 2 банки
Chip type           : ATmega88
Clock frequency     : 7,372800 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 128
*****************************************************/
/*****************************************************
Features:
3.0
    + Без PWM
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
//#include "pwm48lib.h"

//--------------------------
const short maxPower = 100;
const short minPower = 30;
const int pwmT       = 100; // Период действия назначения мощности  (в циклах)
//--------------------------

#define LimLeft  (BYTE)100 // Порог срабатывания датчиков полосы
#define LimRight (BYTE)100
#define LINE_T   (BYTE)150// Время отъезда от линии
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
    SHARP_FR
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

BYTE SharpFL, SharpFR, SharpSL, SharpSR, FotoL, FotoR;

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
  int T;  // Счетчик тактов для смены тактики поиска
  int a;  // Куда крутиться, 0 - влево, 1 - вправо, 2 - вперёд
   
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

  
  // Управление скоростью
  PORTB.1 = 1;
  PORTB.2 = 1;      
       
//  InitPWM(50, 8);
//  PWMSetPc(100,100);
 
  robotStop();

  
  pip();
  printf("\nSumo 88 uS RPM-1.1 Version 3.00 24.01.2014\n");

//--------------------------------------------------------
// Переход в отладочный режим
// Для перехода в отладочный режим необходимо, чтоб перед включением датчики 
// границы находились на светлом фоне
//--------------------------------------------------------
//  FotoL = ReadByteADC(6)>LimLeft;
  FotoR = ReadByteADC(ADC_E2)>LimRight;
  
  if(FotoR)
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
  
  // Выдача управляющах импульсов на мотор, для опускания отвала
  ef_1 = 1;
  delay_ms(10);
  ef_1 = 0; 
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
          // Отъезжаем назад
        case(FOTO_L):{  
//        PWMSetPc(100,100);
        
          robotStop(); delay_ms(10); 
          goBack();
          delay_ms(LINE_T/2); 
          // Начинаем крутиться
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
          // Отъезжаем назад
//          PWMSetPc(100,100);
          
          robotStop(); delay_ms(10); 
          goBack();
          delay_ms(LINE_T/2); 
          RightMotorBack();
          delay_ms(LINE_T/2);
          // Начинаем крутиться
          goFastLeft(); 
          T = 0;
          break;                       
        }  
    }

// if (last_sens != sens )  
//  {
//    robotStop();
//    delay_ms(5);           
     // Задержка для выравнивания робота                                                   
//  }   
//------------------------------------------------------
// Общие действия
// Обнаружение противника
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
        // Плавные повороты 
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
            
        // резкие повороты
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
// Никого не обнаружили
// Поиск противника
//------------------------------------------------------
//                if(T>AGRESSIVE_T)
//                {  // спокойный режим поиска, робот с последнего обнаружения уже искал 1000 циклов
                    if(a==LEFT) goFastLeft();
                    if(a==RIGHT) goFastRight();
                    if(a==FWD) goFwd();
//                } 
//                else
//                { // агрессивный режим: робот всё ещё уверен, что найдёт противника и двигается резко
//                    if(a==LEFT) goFastLeft();
//                    if(a==RIGHT) goFastRight();
//                }
                   
            }
    }
    T=T+1;
//    tmr--;
    last_sens = sens; // Запоминаем предыдущее состояние
    sens = NO_SENS; // Выставляем значение по-умолчанию
    
    // Считываем сигналы датчиков
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
    
    
        
    if(T>MAXT) // Пора менять тактику
    {
      pip();
      T=0;
      // Выбираем действие "случайным" образом
      a=rand()%3;  // Остаток от деления на 3 (деление по модулю 3)
    }    
  }
}
