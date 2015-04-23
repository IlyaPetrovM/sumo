/*****************************************************
Project : Робот-сумоист FreeSumo RP-2
Version : 1.00
Date    : 19.08.2013 
LP      : 24.04.2015
Author  : Петров Илья Михайлович
Company : Лаборатория робототехники
Comments: 4 датчика Sharp-аналоговые (max-дальность 30cm), 1 датчик QRD1114,
          2 мотор-редуктора LT25GA34-370T (1:34) 185rpm, 
          литий-полимерный аккумулятор 3700mAh 7,4V 45C 2S
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

#define FREE_SUMO /// Тип сумоиста
#define FEEDBACK_TESTING  /// Крутить колёсами при тестировании  

/// Конфигурация отдельных сумоистов. Способы обращения к сенсорам и т.п.
/* Расположение пинов
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
    #define LimSharpClose     (BYTE)  120 // Для близкого расстояния
    #define LimSharpSide      (BYTE)  165 // 165->6cm
    #define LINE_LimLeft      (BYTE)  150 // Порог срабатывания датчиков полосы
    #define LINE_LimRight     (BYTE)  200
    #define LINE_BACK_DELAY   (BYTE)  300 // Время отъезда от линии
    #define LINE_TANK  (BYTE) 1000  // активный поиск противника при развороте от линии
    #define TANK_START (BYTE) 1000  //Активный поиск противника при старте
    #define MAXT 2500    // Время смены тактики (количество тактов)

    // Макросы проверки датчиков
    #define CHECK_FOTO_L ReadByteADC(7)>LINE_LimLeft
    #define CHECK_FOTO_R ReadByteADC(8)>LINE_LimRight

    #define CHECK_START_SENSOR (PIND.4)==0  /// Старт по сигналу с пульта (иначе задержка 5 сек)
            
    #define CHECK_SHARP_FC ((ReadByteADC(ADC_1)>LimSharpClose) || (ReadByteADC(ADC_2)>LimSharpClose)) 
    #define CHECK_SHARP_FL ReadByteADC(ADC_1)>LimSharpFwd  // 
    #define CHECK_SHARP_FR ReadByteADC(ADC_2)>LimSharpFwd  // 

    #define CHECK_SHARP_SL ReadByteADC(ADC_3)>LimSharpSide // 
    #define CHECK_SHARP_SR ReadByteADC(ADC_4)>LimSharpSide //        
#endif

//-------------------------------------------------------------
/// Константы сенсоров (для запоминания предыдущего состояния) 
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
short unsigned int sens = NO_SENS;            // текущее состояние
short unsigned int last_sens = NO_SENS;       // предыдущее состояние
BYTE SharpFL, SharpFR, SharpFC, SharpSL, SharpSR, FotoL, FotoR;

/***********************************************************
\brief Отладочный режим
\param void
\return void
************************************************************/
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
  
   
  while (1)
  {         
    /** Отладка с помощью моторов */    
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
    /** Отладка с помощью терминала */
    /// Считывание значений всех датчиков 
    re1 = ReadByteADC(ADC_E1);
    re2 = ReadByteADC(ADC_E2);

    a1 = ReadByteADC(ADC_1);
    a2 = ReadByteADC(ADC_2);
    a3 = ReadByteADC(ADC_3);
    a4 = ReadByteADC(ADC_4);
    a5 = ReadByteADC(ADC_5);
    a6 = ReadByteADC(ADC_6);
    /// Вывод значений датчиков на экран  
    printf("(%4u %4u) (%4u %4u) %4u %4u %4u %4u\r", re1, re2, a1, a2, a3, a4, a5, a6);   
  }
}

/***********************************************************
\brief Главная программа
\param void
\return void
************************************************************/
void main(void)
{
  int T;  //< Счетчик тактов для смены тактики поиска 
  int a;  //< Куда крутиться, 0 - влево, 1 - вправо, 2 - вперёд    
  int spd=FAST;

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
     
  /** 
   \brief Переопределяем (D.3  и D.4) как цифровой подтянутый вход
   Port D initialization
   Func7=Out Func6=Out Func5=Out Func4=In !!!Func3=In Func2=Out Func1=In Func0=In 
   State7=0 State6=0 State5=0 State4=P !!!!State3=0 State2=0 State1=T State0=T    
         
  Подробнее: 
    PORTD 0001 1000  
    Func  oooi iooo (i - in, o - out)
    
    DDRD  1110 1100
    State 000P 00TT

  PORTD=0x18;
  DDRD=0xEC; 
   */ 
  
  // Скоростью управлять не будем. Выставляем по максиммуму
  PORTB.1 = 1;
  PORTB.2 = 1;
 
  robotStop();
  pip();  
  
  printf(ROBOT_NAME " " VERSION " (" __DATE__ " " __TIME__ "') file:" __FILE__);

//--------------------------------------------------------
// Переход в отладочный режим
//--------------------------------------------------------
/** Для перехода в отладочный режим необходимо, 
    чтобы перед включением датчики 
    границы находились на светлом фоне     
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
// Выбор поворота после старта
//--------------------------------------------------------
/** Для указания направления необходимо, 
    чтобы сумоист увидел препятствие с той стороны, 
    куда ему необходимо развернуться
*/
  /// Считывание сенсоров
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
  spd=FAST;// Выставляем начальную скорость разворота
  
  /// Проверка выбранной стороны
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
#ifdef CHECK_START_SENSOR
  while(CHECK_START_SENSOR);
#else
  delay_ms(5000);
#endif
  T = 0;      
//--------------------------------------------------------
// Основной цикл
//--------------------------------------------------------
  while (1)
  {
/** Сначала и идет обработка самых приоритетных сигналов */
    /// Реакция на сигнал СТОП 
#ifdef CHECK_START_SENSOR
    if(CHECK_START_SENSOR)
    {
      robotStop();
      pip();
      while(1);
    } 
#endif
//------------------------------------------------------
// Реакция на границу поля
//------------------------------------------------------     
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
            
        /// резкие повороты
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
    
    last_sens = sens; //< Запоминаем предыдущее состояние
    sens = NO_SENS; //< Выставляем значение по-умолчанию

//------------------------------------------------------
/// Считываем сигналы датчиков
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
/// Определение состояния робота
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
/// Смена тактики
//------------------------------------------------------ 
    if(T>MAXT) //!< Пора менять тактику
    {
      pip();
      T=0;
      /// Выбираем действие "случайным" образом
      a=rand()%3;  // Остаток от деления на 3 (деление по модулю 3)  
      spd=rand()%2;
    }    
  }
}
