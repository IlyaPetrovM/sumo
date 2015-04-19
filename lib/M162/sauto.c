//----------------------------------------------------------
// Автомат без памяти
// (количество состояний = 1)
// LP December 21, 2005
// ATMega162 SCM-2
//----------------------------------------------------------
#include "sauto.h"
#include "sign.h"

#ifndef _WIN32_
#include "uart.h"
#endif

#pragma used+

//----------------------------------------------------------

void mtInit(TAutomat *A, byte cX, byte cY)
// Инициализация матрицы автомата
// cX, cY - количество входных и выходных сигналов автомата
{
  byte i, j;
  byte n;
  A->NX = (byte)cX;
  A->NY = (byte)cY;

  n = (byte)(SVSum/A->NY);
  for(i=0;i<A->NX;i++)
    for(j=0;j<A->NY;j++)
      A->AM[i][j] = n;
}

int mtGetY(TAutomat *A, byte x)
// Выбор действия Y в зависимости от входного сигнала x
// (получить номер в соответствии с вектором вероятностей)
// Возвращает -1 в случае неудачи
// Предпочтение отдается ПОСЛЕДНЕМУ элементу
{
  int i, s, p;
  if(x>=A->NX) return -1;
  p = rand() % SVSum;
  s = 0;
  for(i=0;i<A->NY;i++)
  {
    s+=A->AM[x][i];
    if(s>=p) return i;
  }
  return A->NY-1;
}

void mtInc(TAutomat *A, byte x, byte n, byte delta)
// Увеличение значения элемента номер n строки x на величину delta
{
  int i,m,d,dost;
  if(A->AM[x][n]+delta>=SVSum)
  {
    for(i=0;i<A->NY;i++)
      A->AM[x][i]=0;
    A->AM[x][n]=SVSum;
    return;
  }
  A->AM[x][n]+=delta;
  m=0;
  d=delta/(A->NY-1);
  dost=delta-d*(A->NY-1);
  for(i=0;i<A->NY;i++)
    if (i!=n)
    {
      if(A->AM[x][i]-d>=0) A->AM[x][i]-=(byte)d;
      else
      {
        m+=d-A->AM[x][i];
        A->AM[x][i]=0;
      }
    }
  //распределим dost
  m+=dost;
  while(m>0)
    for(i=0;i<A->NY;i++)
      if(A->AM[x][i]-1>=0)
      {
        A->AM[x][i]--;
        m--;
        if(m<=0) return;
      }
}

void mtDec(TAutomat *A, byte x, byte n, byte delta)
// Уменьшение значения элемента номер n строки x на величину delta
{
  int i,m,d,dost;

  m = delta;
  if(A->AM[x][n]-delta>A->AM[x][n] || A->AM[x][n]-delta<0)
  {
    m=A->AM[x][n];
    A->AM[x][n]=0;
  }
  else
    A->AM[x][n]-=delta;
  d=m/(A->NY-1);
  dost=m-d*(A->NY-1);
  for(i=0;i<A->NY;i++)
    if(i!=n) A->AM[x][i]+=(byte)d;
  //распределим dost
  while(dost>0)
    for(i=0;i<A->NY;i++)
      if(A->AM[x][i]+1<=SVSum)
      {
        A->AM[x][i]++;
        dost--;
        if(dost<=0) return;
      }
}

//----------------------------------------------------------

#ifdef _WIN32_

void mtSave(TAutomat *A, FILE *f)
// Сохранить матрицу действий (переслать ее на УЭВМ)
{
  int i,j;
  fprintf(f,"%d %d\n",A->NX,A->NY);
  for(i=0;i<A->NX;i++)
  {
    for(j=0;j<A->NY;j++)
      fprintf(f,"%d ",A->AM[i][j]);
    fprintf(f,"\n");
  }
}

void mtLoad(TAutomat *A, FILE *f)
// Загрузить матрицу действий (загрузить из УЭВМ)
{
  int i,j;
  A->NX = (byte)((int)fgetc(f)-CMD_OFFS);
  A->NY = (byte)((int)fgetc(f)-CMD_OFFS);
  for(i=0;i<A->NX;i++)
    for(j=0;j<A->NY;j++)
      A->AM[i][j] = (byte)((byte)fgetc(f)-CMD_OFFS);
}

#else

inline char mGetch(void) { return getchar(); };

void mtSave(TAutomat *A)
// Сохранить матрицу действий (переслать ее на УЭВМ)
{
  int i,j;
  printf("%d %d\n",A->NX,A->NY);
  for(i=0;i<A->NX;i++)
  {
    for(j=0;j<A->NY;j++)
      printf("%d ",A->AM[i][j]);
    printf("\n");
  }
}

void mtLoad(TAutomat *A)
// Загрузить матрицу действий (загрузить из УЭВМ)
{
  int i,j;
  A->NX = mGetch();
  A->NY = mGetch();
  for(i=0;i<A->NX;i++)
    for(j=0;j<A->NY;j++)
      A->AM[i][j] = mGetch();
}

#endif

#pragma used-
