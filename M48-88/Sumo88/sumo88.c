/*****************************************************
Project : Робот-сумоист uS RP-1.1 
Version : 0.01 
Date    : 26.04.2010
LP      : 03.10.2014
Author  : Карпов Валерий Валерьевич, Петров Илья Михайлович
Company : Лаборатория Робототехники
Comments: 4 датчика Sharp-цифровые, 1 датчик QRD1114,
          2 мотор-редуктора Gekko Turbo (1:50), 
          литий-ионовый аккумулятор (2 банки), мотор для открывания отвала
Chip type           : ATmega88V
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
#define LimSharp  40// Порог срабатывания датчиков Шарп
#define MAXT 1500    // Время смены тактики (количество тактов)

BYTE SharpFL, SharpFR, SharpSL, SharpSR, SharpCL, SharpCR, FotoL, FotoR;
enum {
    LEFT,
    RIGHT,
    FWD
};
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
    re2 = ReadByteADC(ADC_E2);  // QRD

    a1 = ReadByteADC(ADC_1);  // SharpSR
    a2 = ReadByteADC(ADC_2);  // SharpSL
    a3 = ReadByteADC(ADC_3);  // SharpFL
    a4 = ReadByteADC(ADC_4);  // SharpFR
    a5 = ReadByteADC(ADC_5);  
    a6 = ReadByteADC(ADC_6);  
    
    delay_ms(20);
    
    printf("(%4u %4u) (%4u %4u) %4u %4u %4u %4u\r", re1, re2, a1, a2, a3, a4, a5, a6);
  }
}

//----------------------------------------------------------

void main(void)
{
  int n;
  int T;  // Счетчик тактов для смены тактики поиска
  int a;  // Куда крутиться, 0 - влево, 1 - вправо, 2 - вперёд
  
  // Инициализация контроллера
  M48Init();
  
  // Port C initialization
  // Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
  // State6=T State5=P State4=P State3=P State2=P State1=T State0=T 
  PORTC=0x3C;
  DDRC=0x00;
  
  // Скроостью управлять не будем. Выставляем по максиммуму
  PORTB.1 = 1;
  PORTB.2 = 1;
 
  robotStop();

  pip();
  printf("\r\nSumo 88 uS RP-1.1 Version 0.01\r\n\r\n");

  //--------------------------------------------------------
  // Переход в отладочный режим
  // Для перехода в отладочный режим необходимо, чтоб перед включением датчики 
  // границы находились на светлом фоне
  //--------------------------------------------------------
  FotoR = ReadByteADC(ADC_E2)>LimRight;
  if(FotoR)
    DebugProc();
 
  robotStop();
    

  // Выдача управляющах импульсов на мотор, для опускания отвала
//  while(FotoR!=1) 
//  {
//  SharpSR = (sen_1==0);
//  FotoR = ReadByteADC(ADC_E2)>LimRight;
//  if(SharpSR)
//  { 
//        ef_1 = 1;
//  }
//  else
//  { ef_1 = 0; }
//  }

//--------------------------------------------------------
// Выбор поворота после старта
//--------------------------------------------------------
  a = -1;
  while(a == -1) 
  {
    SharpSR = (sen_1==0);
    SharpSL = (sen_2==0);
    if(SharpSL) 
    {
        a = LEFT;  
    } 
    if(SharpSR)
    { 
        a = RIGHT;
    }

  }
  // Проверка выбранной стороны
  if(SharpSL) 
  {

    pip(); delay_ms(200); 
    goLeft(); 
  }
  if(SharpSR) 
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
  delay_ms(100);
  ef_1 = 0;
  
  T = 0;
  a = 0;
        
  //--------------------------------------------------------
  // Основной цикл
  //--------------------------------------------------------
  while (1)
  {
    // Считываем сигналы датчиков
    
//    //Старое подключение
//    SharpFL = ReadByteADC(ADC_1)>LimSharp;
//    SharpFR = ReadByteADC(ADC_2)>LimSharp;
//    SharpSL = (sen_3==0);
//    SharpSR = (sen_4==0);
//    SharpCL = (sen_5==0);
//    SharpCR = (sen_6==0);
    
    //Датчики подключены по-особому!
    SharpSR = (sen_1==0);
    SharpSL = (sen_2==0);
    
    SharpFL = (sen_3==0);
    SharpFR = (sen_4==0);
    
    FotoR = ReadByteADC(ADC_E2)>LimRight;
    
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
      goFastRight();
      T = 0;
      continue;
    }
    if(FotoR)
    {
      // Отъезжаем назад
      goBack();
      delay_ms(100);
      // Начинаем крутиться
      goFastLeft();
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
