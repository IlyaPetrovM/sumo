//----------------------------------------------------------
// Классифицирующий автомат
// LP December 21, 2005
// ATMega162 SCM-2
//----------------------------------------------------------

#ifndef _clauto_h
#define _clauto_h

#include <stdlib.h>
#include <stdio.h>
#include "sign.h"
#include "sauto.h"

#ifndef _WIN32_
#include "uart.h"
#endif

//максимальное число структурных единиц
#define MAX_STR 11

//максимальный размер входного алфавита
#define MAX_IN_SIZE 2

typedef struct
{
  byte strnum;
  byte q;                  // Текущее состояние
  byte alphX;              // Размерность входного алфавита
  byte alphY;              // Размерность выходного алфавита

  byte P[MAX_STR][MAX_IN_SIZE];        // Правила переходов
  byte actp[MAX_STR][MAX_IN_SIZE][ECL_END];// Матрица вероятностей выходного сигнала
} TClassAutomat;

int CA_accept(TClassAutomat *A, byte x);

#ifdef _WIN32_

#define CE_SVSum 1024

void CA_Load(TClassAutomat *A, FILE *f);
// Загрузить матрицу действий (загрузить из УЭВМ)

void CA_read(TClassAutomat *A, char *fname);
// Чтение структуры особи из файла

void CA_Save(TClassAutomat *A, char *fname);
// Запись структуры особи в файл

void CA_Save_bin(TClassAutomat *A, char *fname);
// Запись структуры особи в файл

#else

void CA_Load(TClassAutomat *A);
// Загрузить матрицу действий (загрузить из УЭВМ)

void CA_Save(TClassAutomat *A);
// Запись структуры особи

#endif

#endif
