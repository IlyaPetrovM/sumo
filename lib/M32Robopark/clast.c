/**********************************************************
КЛАСТЕРИЗАЦИЯ МЕТОДОМ k-СРЕДНИХ
Задача: кластеризовать массив измерений

Version : 1.01
Date    : 30.10.2009
LP      : 05.12.2009
**********************************************************/
//#define _PRINT_DEBUG_

#include <math.h>
#include <stdlib.h>
#include "clast.h"
        
#define MAX_OF_TYPE 65535 // максимальное значение, получаемое при измерении

#define MAX_NUM_OF_MEASURE 50                    // максимальное число измерений
#define MAX_NUM_OF_CLUSTERS   MAX_NUM_OF_MEASURE // максимальное число кластеров
        
unsigned int numOfPoints[MAX_NUM_OF_CLUSTERS]; //массив количества точек в каждом кластере
unsigned int clusters[MAX_NUM_OF_MEASURE]; //массив номеров кластеров, приписанных точкам
                                  
void Clusterize(unsigned int measures[], int NM, 
                unsigned int centers[], int NCLUST)
// Кластеризация массива измерений 
// measures - массив измерений
// NM - количество элементов в массиве measures
// centers - массив центров кластеров
// NCLAST - число кластеров
{
  int i, j, k;
  unsigned int temp;
  unsigned int minDist;
  unsigned int oldCenter; 
  unsigned int distance;
  unsigned int cluster_to_place;
  unsigned int max, min; //максимальное и минимальное значение элементов в массиве

  char ifChanged=1; //флаг, говорящий о том, изменился ли хоть один центр            

#ifdef _PRINT_DEBUG_
  //отладочная печать входного масссива
  printf("Measures: ");
  for(j=0; j<NM; j++)
    printf("%u ", measures[j]);
#endif

  // Определяем минимум и максимум
  min = max = measures[0];
  for(i=1;i<NM;i++)
  {
    if(measures[i]>max) max = measures[i];
    if(measures[i]<min) min = measures[i];
  }

  //изначально центры случайные
  for(i=0;i<NCLUST;i++)
  {                    
    centers[i] = min + rand()%(max-min); //случайное число масштабируем, чтобы оно наверняка 
    // попало в измеряемый интервал. Нужно, чтобы они были в интервале от минимального до максимального    
    numOfPoints[i]=0;
  }

#ifdef _PRINT_DEBUG_
  //отладочная печать масссива центров кластеров
  printf("Max: %u, Min: %u\r\n", max, min);
  printf("Centers: ");
  for(i=0; i<NCLUST; i++)
    printf("%u ", centers[i]);
  printf("\r\n");
#endif

  //пока центры не перестанут двигаться
  while(ifChanged)
  {
    //кластеризация точек
    for(j=0; j<NM; j++)
    {
      minDist = MAX_OF_TYPE;
      for(k=0; k<NCLUST; k++)
      {
        distance = abs(centers[k] - measures[j]);

#ifdef _PRINT_DEBUG_
        //Отладочная печать расстояния
        printf("D: %u, Cent: %u, Meas: %u\r\n", distance, centers[k], measures[j]);
        delay_ms(500);
#endif
        if(distance <= minDist)
        {
          minDist = distance;
          cluster_to_place=k; //запоминаем кластер, дистанция до которого минимальна
        }	
      }                                  
      clusters[j] = cluster_to_place;
      numOfPoints[cluster_to_place]++;  
      //точка помещена в кластер

#ifdef _PRINT_DEBUG_
      //Отладочная печать кластеров, приписанных каждой точке
      printf("\r\nClusters for each point: ");
      for(k=0; k<NM; k++)
        printf("%d ", clusters[k]);
      printf("\r\n");
#endif

    }
    ifChanged=0;

#ifdef _PRINT_DEBUG_
    //отладочная печать кластеров
    for(i=0; i<NCLUST; i++)
      printf("Center: %u, numOfPoints: %u\r\n", centers[i], numOfPoints[i]);
    printf("\r\n");
#endif

    //пересчет центров для каждого кластера
    for(i=0; i<NCLUST; i++)
    {
      oldCenter = centers[i];
      centers[i] = 0;

      for(j=0; j<NM; j++)
      {
        if(clusters[j]==i)
          centers[i]+=measures[j]/numOfPoints[i];
      }
      if(oldCenter!=centers[i])
      ifChanged=1;
      
      numOfPoints[i]=0;	
    }      
  }

  //Сортировка центров кластеров по возрастанию
  for(i=0;i<NCLUST-1;i++)
  {                                  
    for(j=i;j<NCLUST;j++)
    {
      if(centers[j]<centers[i])
      {            
        temp = centers[i];
        centers[i] = centers[j];
        centers[j] = temp;                                
      }                                
    }
  }
}
