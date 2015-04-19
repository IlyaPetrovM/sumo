//
// AdapLib
// Date: 21.09.2004
// Sept 05, 2005
// 22.03.2006
// ATMega162 SCM-2

#ifndef _ADAPLIB_H_
#define _ADAPLIB_H_

#include <mega162.h>
#include "es13x19.h"

extern byte TICK_CNT3;

// Вектор входных сигналов
extern byte InputVector[SIGNUM];
#define signal(N) InputVector[N]

// Вектор эффекторов ESW

extern byte ESW[CMD_END];

#define nl "\r\n"

//----------------------------------------------------------
// i_no_reaction:      Блокированы все прерывания
// i_hight_level_only: Раегируем только на сигналы высшего приоритета
// i_default:          Реагируем на все сигналы
//----------------------------------------------------------

extern byte InterruptLevel;

#define i_no_reaction      0
#define i_hight_level_only 1
#define i_default          2

//----------------------------------------------------------
// Эффекторы (управление двигателями)
//----------------------------------------------------------

#ifdef _NEW_MOTOR_CTL_
//----------------------------------------------------------
// Управление для "новой" схемы
//----------------------------------------------------------
// Стоп
#define MOTOR_STOP  { ef_B0 = 0; ef_B1 = 0; ef_B2 = 0; ef_B3 = 0; }

// Назад
#define MOTOR_BACK  { ef_B0 = 0; ef_B1 = 1; ef_B2 = 0; ef_B3 = 1; }

//Вперед
#define MOTOR_FWD   { ef_B0 = 1; ef_B1 = 0; ef_B2 = 1; ef_B3 = 0; }

//Налево
#define MOTOR_LEFT  { ef_B0 = 0; ef_B1 = 1; ef_B2 = 1; ef_B3 = 0; }

//Налево через "вперед"
#define MOTOR_RIGHTF { ef_B0 = 1; ef_B1 = 0; ef_B2 = 0; ef_B3 = 0; }

//Направо
#define MOTOR_RIGHT  { ef_B0 = 1; ef_B1 = 0; ef_B2 = 0; ef_B3 = 1; }

//Направо через "вперед"
#define  MOTOR_LEFTF { ef_B0 = 0; ef_B1 = 0; ef_B2 = 1; ef_B3 = 0; }

#else

//----------------------------------------------------------
// Управление для "старой" схемы
//----------------------------------------------------------
// Стоп
#define MOTOR_STOP  { ef_B0 = 1; ef_B1 = 0; ef_B2 = 1; ef_B3 = 0; }

// Назад
#define MOTOR_BACK  { ef_B0 = 0; ef_B1 = 1; ef_B2 = 0; ef_B3 = 1; }

//Вперед
#define MOTOR_FWD   { ef_B0 = 0; ef_B1 = 0; ef_B2 = 0; ef_B3 = 0; }

//Налево
#define MOTOR_LEFT  { ef_B0 = 0; ef_B1 = 1; ef_B2 = 0; ef_B3 = 0; }

//Налево через "вперед"
#define MOTOR_LEFTF { ef_B0 = 1; ef_B1 = 0; ef_B2 = 0; ef_B3 = 0; }

//Налево через "назад"
#define MOTOR_LEFTB { ef_B0 = 0; ef_B1 = 1; ef_B2 = 1; ef_B3 = 0; }

//Направо
#define MOTOR_RIGHT { ef_B0 = 0; ef_B1 = 0; ef_B2 = 0; ef_B3 = 1; }

//Направо через "вперед"
#define MOTOR_RIGHTF { ef_B0 = 0; ef_B1 = 0; ef_B2 = 1; ef_B3 = 0; }

//Направо через "назад"
#define MOTOR_RIGHTB { ef_B0 = 1; ef_B1 = 0; ef_B2 = 0; ef_B3 = 1; }

#endif

//----------------------------------------------------------
//Эффекторы (команды)
//----------------------------------------------------------
#define cmdUSR0            ef_E1
#define cmdUSR1            ef_E2
                               
#define cmdWaitSignal      ef_D2

#define cmdIndic           ef_D3

//----------------------------------------------------------
// Сенсоры
//----------------------------------------------------------
#define sen_PUSK0          sn_C0
#define sen_PUSK1          sn_C6
#define sen_PUSK2          sn_C7

#define sen_POOSR          sn_C1
#define sen_NAKAZ          sn_C2
#define sen_FOTO_LEFT      sn_C3
#define sen_FOTO_RIGHT     sn_C4

#define sen_MK_CENTER      sn_A5

//sic Сигнал инверсный
#define sen_HALT           !sn_B4

//----------------------------------------------------------
// Звуки
//----------------------------------------------------------
extern byte SoundOn;

//Длительность звукового сигнала
#define DTIME_SAY      3000

#define SND_NOSOUND      0

#define SND_VPERED       1
#define SND_NAZAD        2
#define SND_NAPRAVO      3
#define SND_NALEVO       4
#define SND_ACCUM        5
#define SND_SEARCH_LINE  6
#define SND_SEARCH_GOAL  7
#define SND_NAKAZ        8
#define SND_POOSHR       9
#define SND_USR0        10
#define SND_GOLOS       SND_USR0
#define SND_HELLO       11
#define SND_USR1        12
#define SND_BABAH       SND_USR1
#define SND_WAITSIGNAL  13
#define SND_ATTACK      14
#define SND_CONTROL     15

//----------------------------------------------------------
//
//----------------------------------------------------------

#define DELAY_DREBEZG 100

#define TIME_ACTION  20

// Время движения вперед, назад, поворот
#define TIME_GOFWD  TIME_ACTION
#define TIME_GOBCK  TIME_ACTION
#define TIME_TURN   TIME_ACTION/2

#define TIME_ATTACK TIME_ACTION/4
#define TIME_ALERT  TIME_ACTION*2
                      
//Время ожидания реакции
#define TIME_WAITREACT  TIME_GOFWD

// Режимы работы
#define REG_MAIN         1
#define REG_LINE_SEARCH  2
#define REG_FOTO_SEARCH  3
#define REG_EVOL         4
#define REG_MANUAL       5
#define REG_CTL          6
#define REG_JSM          REG_CTL

//----------------------------------------------------------

// Сигналы высшего приоритета
#define GETSSW0  (unsigned int)sn_D4 +       \
                ((unsigned int)sn_D5<<1) +   \
                ((unsigned int)sn_D6<<2) +   \
                ((unsigned int)sn_D7<<3) +   \
                ((unsigned int)sn_C0<<4)

// "Обычные" сигналы
#define GETSSW1  (unsigned int)sn_C1 +      \
                ((unsigned int)sn_C2<<1) +  \
                ((unsigned int)sn_C3<<2) +  \
                ((unsigned int)sn_C4<<3) +  \
                ((unsigned int)sn_C5<<4) +  \
                ((unsigned int)sn_C6<<5) +  \
                ((unsigned int)sn_C7<<7)

void Say(unsigned char n, int tdelay);

int FIWait(int time);
// Возвращает 1, если истекло время, 0, если сработали датчики и UART_CHAR,
// если был принят сигнал

void ClearESWVector(void);
// Очистка выходного вектора

void ClearInputVector(void);
// Очистка входного вектора

//interrupt [TIM3_OVF] void timer3_overflow(void);
// TIMER1 overflow interrupt service routine
// occurs every 0.xxx seconds

void Stop(void);
int goForward(int time);
int goBack(int time);
int TurnLeft(int time);
int TurnLeftSmall(int time);
int TurnRight(int time);
int TurnRightSmall(int time);

//----------------------------------------------------------
// Безусловные рефлексы
//----------------------------------------------------------

void BackTurnRight(int time);
// Отъехать и повернуть направо (реакция на левый передний датчик)
void BackTurnLeft(int time);
// Отъехать и повернуть налево (реакция на правый передний датчик)
void Alert(int time);
// Тревога

int GetRegNum(void);
// Получение номера режима работы

#endif
