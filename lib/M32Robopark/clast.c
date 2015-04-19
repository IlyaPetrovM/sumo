/**********************************************************
������������� ������� k-�������
������: �������������� ������ ���������

Version : 1.01
Date    : 30.10.2009
LP      : 05.12.2009
**********************************************************/
//#define _PRINT_DEBUG_

#include <math.h>
#include <stdlib.h>
#include "clast.h"
        
#define MAX_OF_TYPE 65535 // ������������ ��������, ���������� ��� ���������

#define MAX_NUM_OF_MEASURE 50                    // ������������ ����� ���������
#define MAX_NUM_OF_CLUSTERS   MAX_NUM_OF_MEASURE // ������������ ����� ���������
        
unsigned int numOfPoints[MAX_NUM_OF_CLUSTERS]; //������ ���������� ����� � ������ ��������
unsigned int clusters[MAX_NUM_OF_MEASURE]; //������ ������� ���������, ����������� ������
                                  
void Clusterize(unsigned int measures[], int NM, 
                unsigned int centers[], int NCLUST)
// ������������� ������� ��������� 
// measures - ������ ���������
// NM - ���������� ��������� � ������� measures
// centers - ������ ������� ���������
// NCLAST - ����� ���������
{
  int i, j, k;
  unsigned int temp;
  unsigned int minDist;
  unsigned int oldCenter; 
  unsigned int distance;
  unsigned int cluster_to_place;
  unsigned int max, min; //������������ � ����������� �������� ��������� � �������

  char ifChanged=1; //����, ��������� � ���, ��������� �� ���� ���� �����            

#ifdef _PRINT_DEBUG_
  //���������� ������ �������� ��������
  printf("Measures: ");
  for(j=0; j<NM; j++)
    printf("%u ", measures[j]);
#endif

  // ���������� ������� � ��������
  min = max = measures[0];
  for(i=1;i<NM;i++)
  {
    if(measures[i]>max) max = measures[i];
    if(measures[i]<min) min = measures[i];
  }

  //���������� ������ ���������
  for(i=0;i<NCLUST;i++)
  {                    
    centers[i] = min + rand()%(max-min); //��������� ����� ������������, ����� ��� ��������� 
    // ������ � ���������� ��������. �����, ����� ��� ���� � ��������� �� ������������ �� �������������    
    numOfPoints[i]=0;
  }

#ifdef _PRINT_DEBUG_
  //���������� ������ �������� ������� ���������
  printf("Max: %u, Min: %u\r\n", max, min);
  printf("Centers: ");
  for(i=0; i<NCLUST; i++)
    printf("%u ", centers[i]);
  printf("\r\n");
#endif

  //���� ������ �� ���������� ���������
  while(ifChanged)
  {
    //������������� �����
    for(j=0; j<NM; j++)
    {
      minDist = MAX_OF_TYPE;
      for(k=0; k<NCLUST; k++)
      {
        distance = abs(centers[k] - measures[j]);

#ifdef _PRINT_DEBUG_
        //���������� ������ ����������
        printf("D: %u, Cent: %u, Meas: %u\r\n", distance, centers[k], measures[j]);
        delay_ms(500);
#endif
        if(distance <= minDist)
        {
          minDist = distance;
          cluster_to_place=k; //���������� �������, ��������� �� �������� ����������
        }	
      }                                  
      clusters[j] = cluster_to_place;
      numOfPoints[cluster_to_place]++;  
      //����� �������� � �������

#ifdef _PRINT_DEBUG_
      //���������� ������ ���������, ����������� ������ �����
      printf("\r\nClusters for each point: ");
      for(k=0; k<NM; k++)
        printf("%d ", clusters[k]);
      printf("\r\n");
#endif

    }
    ifChanged=0;

#ifdef _PRINT_DEBUG_
    //���������� ������ ���������
    for(i=0; i<NCLUST; i++)
      printf("Center: %u, numOfPoints: %u\r\n", centers[i], numOfPoints[i]);
    printf("\r\n");
#endif

    //�������� ������� ��� ������� ��������
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

  //���������� ������� ��������� �� �����������
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
