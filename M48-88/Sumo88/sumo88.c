/*****************************************************
Project : Робот-сумоист RPM-1 "Floppy" мини-сумо
Version : 1.00 
Date    : 26.04.2010
LP      : 19.08.2013
Author  : Петров Илья Михайлович, Малышев Александр Александрович
Company : Лаборатория робототехники
Comments: 4 датчика Sharp-цифровые, 2 датчика QRD1114,
          2 мотор-редуктора Gekko Turbo (1:50), 
          литий-полимерный аккумулятор
Chip type           : ATmega88
Clock frequency     : 7,372800 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 128
*****************************************************/

#include <mega88.h>
#include <math.h>
#include <stdlib.h>
#include "m48lib.h"

#define LimLeft  (BYTE)150 // Порог срабатывания датчиков полосы
#define LimRight (BYTE)150
#define LINE_T   (BYTE)400// Время отъезда от линии
#define CHECK_LINE (BYTE) 1 // >0 - проверка реакции на край поля

#define MAXT 2500    // Время смены тактики (количество тактов)

#define SENSOR_START PIND.4 // Стартовый сигнал

BYTE SharpFL, SharpFR, SharpSL, SharpSR, FotoL, FotoR;
void turning()
{      short t;
    for (t=0; (sen_1==0)&&(sen_2==0)&&(sen_3==0)&&(sen_4==0)&&(t<=2300); t++) 
    { 
        goFastRight();
    }
    
}
void DebugProc(void)
{
  //BYTE re1, re2;
  //BYTE a1, a2, a3, a4, a5, a6;
  
  printf("\r\nTEST REGIME\r\n");

  // Отладочные телодвижения 
  
  #define Time 150
  delay_ms(3000);
  // (проверка правильности подключения двигателей: вперед, назад, влево, вправо)
  goFwd(); delay_ms(Time);       robotStop(); delay_ms(Time+200);
  goBack(); delay_ms(Time);      robotStop(); delay_ms(Time+200);
  goFastLeft(); delay_ms(Time);  robotStop(); delay_ms(Time+200);
  goFastRight(); delay_ms(Time); robotStop(); 
  pip();
  // Проверка правильности подключения световых датчиков
  //if(FotoL) goFastLeft(); delay_ms(Time);
  //if(FotoR) goFastRight(); delay_ms(Time); 
  //robotStop(); 
  pip();
           
  while (1)
  {     

    delay_ms(10);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
    SharpFL = (sen_1==0);
    SharpFR = (sen_2==0);
    SharpSL = (sen_3==0);
    SharpSR = (sen_4==0);
    
    if (SharpFL || SharpFR) {robotStop();}
    if (SharpFL) {goLeft(); }
    if (SharpFR) {goRight(); }
    if (SharpSL&&SharpSR) {robotStop();} // Защита от помех
    if (SharpSL) {goFastLeft(); }
    if (SharpSR) {goFastRight();}       
  }

}
    //FotoL = ReadByteADC(6)>LimLeft;
    //FotoR = ReadByteADC(7)>LimRight;
   //  if (FotoL) {goBack(); delay_ms(500); goLeft(); delay_ms(500); robotStop();}
  // if (FotoR) {goBack(); delay_ms(500); goRight(); delay_ms(500); robotStop();}
    
      /*
    re1 = ReadByteADC(ADC_E1);
    re2 = ReadByteADC(ADC_E2);

    a1 = ReadByteADC(ADC_1);
    a2 = ReadByteADC(ADC_2);
    a3 = ReadByteADC(ADC_3);
    a4 = ReadByteADC(ADC_4);
    a5 = ReadByteADC(ADC_5);
    a6 = ReadByteADC(ADC_6);
    
    printf("(%4u %4u) (%4u %4u) %4u %4u %4u %4u\r", re1, re2, a1, a2, a3, a4, a5, a6);   
   */ 
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
  printf("\r\nSumo 88 RPM-1 Floppy micro Version 1.0\r\n\r\n");

  //--------------------------------------------------------
  // Переход в отладочный режим
  // Для перехода в отладочный режим необходимо, чтоб перед включением датчики 
  // границы находились на светлом фоне
  //--------------------------------------------------------
  FotoL = ReadByteADC(6)>LimLeft;
  FotoR = ReadByteADC(7)>LimRight;
  
  if(FotoL || FotoR)
  {
   DebugProc();
   }
 
  robotStop();
   
  //--------------------------------------------------------
  // Ожидание сигнала START
  //--------------------------------------------------------
  //while(SENSOR_START==0);
  
  delay_ms(5000);

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
    SharpFL = (sen_1==0);
    SharpFR = (sen_2==0);
    SharpSL = (sen_3==0);
    SharpSR = (sen_4==0);
    
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
      robotStop();
      goBack();
      delay_ms(LINE_T);
      // Начинаем крутиться
      goFastRight();
      goFastRight(); goFastRight();
      goFastRight();
      T = 0;
      //while(CHECK_LINE);
      continue;
    }
    if(FotoR)
    {
      // Отъезжаем назад
      robotStop();
      goBack();
      delay_ms(LINE_T);
      // Начинаем крутиться
       robotStop();
      goFastLeft();
      goFastLeft();  goFastLeft();
      goFastLeft();
      T = 0;
      //while(CHECK_LINE);
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
    // Плавные повороты
    if(SharpFL)
    {  
      goLeft();
      T = 0;
      a = 0;
      continue;
    }                 
    if(SharpFR)
    {            
      goRight();
      T = 0;
      a = 1;
      continue;
    }  
    // резкие повороты
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
    // Никого не обнаружили
    // Поиск противника
    //------------------------------------------------------
    if(a==0) goFastLeft();
    if(a==1) goFastRight();
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
