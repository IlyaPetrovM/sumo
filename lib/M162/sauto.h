//----------------------------------------------------------
// Автомат без памяти
// (количество состояний = 1)
// LP December 21, 2005
// ATMega162 SCM-2
//----------------------------------------------------------
#ifndef sauto_h
#define sauto_h

#include <stdlib.h>
#include <stdio.h>

#ifndef byte
  #ifndef _WIN32_
    #define byte unsigned char
  #else
    typedef unsigned char byte;
  #endif
#endif

// Количество входных сигналов X
#define maxNX  21

// Количество выходных сигналов Y (действий)
#define maxNY  12

typedef struct
{
  byte NX;
  byte NY;
  byte AM[maxNX][maxNY]; // Матрица действий
} TAutomat;

// Сумма элементов стохастического вектора
#define SVSum 100

//----------------------------------------------------------
void mtInit(TAutomat *A, byte cX, byte cY);
// Инициализация матрицы автомата
// cX, cY - количество входных и выходных сигналов автомата

int mtGetY(TAutomat *A, byte x);
// Выбор действия Y в зависимости от входного сигнала x
// (получить номер в соответствии с вектором вероятностей)
// Возвращает -1 в случае неудачи
// Предпочтение отдается ПОСЛЕДНЕМУ элементу

void mtInc(TAutomat *A, byte x, byte n, byte delta);
// Увеличение значения элемента номер n строки x на величину delta

void mtDec(TAutomat *A, byte x, byte n, byte delta);
// Уменьшение значения элемента номер n строки x на величину delta

#ifdef _WIN32_
  void mtSave(TAutomat *A, FILE *f);
  // Сохранить матрицу действий (переслать ее на УЭВМ)

  void mtLoad(TAutomat *A, FILE *f);
  // Загрузить матрицу действий (загрузить из УЭВМ)
#else
  
  inline char mGetch(void);

  void mtSave(TAutomat *A);
  // Сохранить матрицу действий (переслать ее на УЭВМ)

  void mtLoad(TAutomat *A);
  // Загрузить матрицу действий (загрузить из УЭВМ)
#endif

#endif
