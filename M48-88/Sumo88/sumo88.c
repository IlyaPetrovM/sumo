/*****************************************************
Project : Робот-сумоист RK-27 микро-сумо
          Модифицированная программа
Version : 1.05 
Date    : 26.04.2010
LP      : 09.03.2013
Author  : Карпов Валерий Валерьевич
Company : Лаборатория робототехники
Comments: 2 датчика Sharp, 3 датчика Sharp-цифровые, 2 датчика QRD1114,
          4 мотор-редуктора Gekko Turbo (1:150), 
          литий-ионовый аккумулятор, сервопривод микро  
Chip type           : ATmega88PA
Clock frequency     : 7,372800 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 128
*****************************************************/

#include <mega88.h>
#include <math.h>
#include <stdlib.h>
#include "m48lib.h"

#define LimLeft  (BYTE)250 // Порог срабатывания датчиков полосы
#define LimRight (BYTE)250

#define MAXT 1500    // Время смены тактики (количество тактов)

#define SENSOR_START PIND.4 // Стартовый сигнал

BYTE SharpFL, SharpFR, SharpSL, SharpSR, FotoL, FotoR;

void DebugProc(void)
{
  BYTE re1, re2;
  BYTE a1, a2, a3, a4, a5, a6;
  
  printf("\r\nTEST REGIME\r\n");

  // Отладочные телодвижения 
  // (проверка правильности подключения двигателей: вперед, назад, влево, вправо)
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
    re2 = ReadByteADC(ADC_E2);

    a1 = ReadByteADC(ADC_1);
    a2 = ReadByteADC(ADC_2);
    a3 = ReadByteADC(ADC_3);
    a4 = ReadByteADC(ADC_4);
    a5 = ReadByteADC(ADC_5);
    a6 = ReadByteADC(ADC_6);
    
    delay_ms(20);
    
    printf("(%4u %4u) (%4u %4u) %4u %4u %4u %4u\r", re1, re2, a1, a2, a3, a4, a5, a6);
  }
}

//----------------------------------------------------------

#define MAX_CNT_OTVAL 10 // Число управляющих импульсов на сервопривод для опускания отвала

void main(void)
{
  int T;  // Счетчик тактов для смены тактики поиска
  int a;  // Куда крутиться, 0 - влево, 1 - вправо, 2 - вперёд
  BYTE cntOtval = 0; // Счетчик числа импульсов для опускания отвала
  
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
  printf("\r\nSumo 88 RK-25/26MS Version 1.05\r\n\r\n");

  //--------------------------------------------------------
  // Переход в отладочный режим
  // Для перехода в отладочный режим необходимо, чтоб перед включением датчики 
  // границы находились на светлом фоне
  //--------------------------------------------------------
  FotoL = ReadByteADC(6)>LimLeft;
  FotoR = ReadByteADC(7)>LimRight;
  if(FotoL && FotoR)
    DebugProc();
 
  robotStop();
   
  //--------------------------------------------------------
  // Ожидание сигнала START
  //--------------------------------------------------------
  while(SENSOR_START==0);

/***    
  // Выдача управляющах импульсов на сервомашинку, для опускания отвала
  // Изображаем управляющий ШИМ
  // Это займет около 10*(18+0.9) = 189 мс.
  for(n=0;n<MAX_CNT_OTVAL;n++)
  {
    ef_1 = 0;
    delay_us(18000); // Период импульсов. Вообще должно быть так: 20000 - <ширина импульса> = 20000-900 = 19100

    ef_1 = 1;
    delay_us(900); // Ширина импульса 900 мкс (0.9 мс) - крайнее положение
  }
***/
  ef_1 = 0;
  T = 0;
  a = 0;
        
  //--------------------------------------------------------
  // Основной цикл
  //--------------------------------------------------------
  while (1)
  {
    //------------------------------------------------------
    // Опускаем отвал на ходу
    //------------------------------------------------------
  
    if(cntOtval<MAX_CNT_OTVAL)
    {
      cntOtval++;
      //Выдаем импульс (с задержками)
      delay_us(18000); // Период импульсов. Вообще должно быть так: 20000 - <ширина импульса> = 20000-900 = 19100
      ef_1 = 1;
      delay_us(900); // Ширина импульса 900 мкс (0.9 мс) - крайнее положение
      ef_1 = 0;      
    }   
    // Реакция на сигнал СТОП
    if(SENSOR_START==0)
    {
      robotStop();
      pip();
      while(1);
    }
  
    // Считываем сигналы датчиков    
    SharpSL = (sen_3==0);
    SharpSR = (sen_4==0);
    SharpFL = (sen_5==0);
    SharpFR = (sen_6==0);
    
    FotoL = ReadByteADC(6)>LimLeft;
    FotoR = ReadByteADC(7)>LimRight;
    
    //------------------------------------------------------
    // Безусловные рефлексы
    // Сначала и идет обработка самых приоритетных сигналов
    //------------------------------------------------------
    // Реакция на границу поля
    
    if(FotoL)
    {
      // Отъезжаем назад
      goBack();
      delay_ms(100);
      // Начинаем крутиться
      goRight();
      T = 0;
      continue;
    }
    if(FotoR)
    {
      // Отъезжаем назад
      goBack();
      delay_ms(100);
      // Начинаем крутиться
      goLeft();
      T = 0;
      continue;
    }
    //------------------------------------------------------
    // Общие действия
    // Обнаружение противника передними датчиками
    //------------------------------------------------------
    if(SharpFL&&SharpFR)
    {
      goFwd();
      T = 0;
      continue;
    }
    if(SharpFL||SharpSL)
    {
      goLeft();
      T = 0;
      a = 0;
      continue;
    }                 
    if(SharpFR||SharpSR)
    {
      goRight();
      T = 0;
      a = 1;
      continue;
    }
    //------------------------------------------------------
    // Никого не обнаружили
    // Поиск противника
    //------------------------------------------------------
    if(a==0) goLeft();
    if(a==1) goRight();
    if(a==2) goFwd();
    
    T=T+1;
    if(T>MAXT) // Пора менять тактику
    {
      //pip();
      T=0;
      // Выбираем действие "случайным" образом
      a=rand()%3;  // Остаток от деления на 3 (деление по модулю 3)
    }    
  }
}
