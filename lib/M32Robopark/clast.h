/**********************************************************
КЛАСТЕРИЗАЦИЯ МЕТОДОМ k-СРЕДНИХ
Задача: кластеризовать массив измерений

Version : 1.01
Date    : 30.10.2009
LP      : 05.12.2009
**********************************************************/
#ifndef _CLAST_H_
#define _CLAST_H_

#pragma used+

void Clusterize(unsigned int measures[], int NM, 
                unsigned int centers[], int NCLUST);
// Кластеризация массива измерений 
// measures - массив измерений
// NM - количество элементов в массиве measures
// centers - массив центров кластеров
// NCLAST - число кластеров

#pragma used-

#endif
