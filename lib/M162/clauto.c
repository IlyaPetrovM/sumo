//----------------------------------------------------------
// ���������������� �������
// LP December 21, 2005
// ATMega162 SCM-2
//----------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>

#include "sign.h"
#include "sauto.h"
#include "clauto.h"

#ifdef _WIN32_
  #include "plib.h"
#endif  

int CA_GetY(TClassAutomat *A, byte x)
// ����� �������� Y � ����������� �� �������� ������� x
// (�������� ����� � ������������ � �������� ������������)
// ���������� -1 � ������ �������
// ������������ �������� ���������� ��������
{
  int i, s, p;
  p = rand() % SVSum;
  s = 0;
  for(i=0;i<A->alphY;i++)
  {
    s+=A->actp[A->q][x][i];
    if(s>=p) return i;
  }
  return A->alphY-1;
}

int CA_accept(TClassAutomat *A, byte x)
{
  int y;
  y=CA_GetY(A,x);
  if(y<0) return 0;
  // ������� � ������ ���������
  A->q=A->P[A->q][x];
  return y;
}

#ifdef _WIN32_
void CA_Load(TClassAutomat *A, FILE *f)
// ��������� ������� �������� (��������� �� ����)
{
  byte nq, ni, no;
  A->strnum = (byte)fgetc(f);
  A->alphX = (byte)fgetc(f);
  A->alphY = (byte)fgetc(f);

  // ������� ���������
  for(nq=0;nq<A->strnum;nq++)
    for(ni=0;ni<A->alphX;ni++)
      A->P[nq][ni] = (byte)fgetc(f);
  // ������� ������������ ��������� �������
  for(nq=0;nq<A->strnum;nq++)
    for(ni=0;ni<A->alphX;ni++)
      for(no=0;no<A->alphY;no++)
        A->actp[nq][ni][no] = (byte)fgetc(f);
  A->q = 0;
}

void CA_read(TClassAutomat *A, char *fname)
// ������ ��������� ����� �� �����
{
  int alive, TPtbreed, tbreed;
  long age;
  float quality, straf;
  int CopyFeno, q, q0;
  long N0, N1;

  FILE *f;
  if((f=fopen(fname,"rt"))==NULL)
  {
    warning("Open file '%s' error", fname);
    return;
  }
  int numauto; // ���������� ����������� ���������
               // (������, ���� ����������� �������)
  fscanf(f,"%d",&numauto);

  fscanf(f,"%d %d %d %d", &alive, &A->strnum, &TPtbreed, &tbreed);
  fscanf(f,"%ld",&age);
  fscanf(f,"%f %f %d", &quality, &straf, &CopyFeno);

  fscanf(f,"%d %d",&q0,&q);
  fscanf(f,"%d %d",&A->alphX,&A->alphY);
  fscanf(f,"%ld %ld",&N0,&N1);

  // ������� ���������
  for(int nq=0;nq<A->strnum;nq++)
    for(int ni=0;ni<A->alphX;ni++)
      fscanf(f,"%d",&A->P[nq][ni]);
  // ������� ������������ ��������� �������
  for(int nq=0;nq<A->strnum;nq++)
    for(int ni=0;ni<A->alphX;ni++)
    {
      for(int no=0;no<A->alphY;no++)
      {
        int a;
        fscanf(f,"%d",&a);
        a = 1.0*a*SVSum/CE_SVSum;
        A->actp[nq][ni][no] = (byte)a;
      }
    }
  A->q = 0;
  fclose(f);
}

void CA_Save(TClassAutomat *A, char *fname)
// ������ ��������� ����� � ����
{
  FILE *f;
  if((f=fopen(fname,"wt"))==NULL)
  {
    warning("Create file '%s' error", fname);
    return;
  }
  fprintf(f,"%d\n", A->strnum);
  fprintf(f,"%d %d\n",A->alphX,A->alphY);

  // ������� ���������
  for(int nq=0;nq<A->strnum;nq++)
  {
    for(int ni=0;ni<A->alphX;ni++)
      fprintf(f,"%d ",A->P[nq][ni]);
    fprintf(f,"\n");
  }
  // ������� ������������ ��������� �������
  for(int nq=0;nq<A->strnum;nq++)
  {
    for(int ni=0;ni<A->alphX;ni++)
    {
      for(int no=0;no<A->alphY;no++)
        fprintf(f,"%d ",A->actp[nq][ni][no]);
      fprintf(f,"\n");
    }
    fprintf(f,"\n");
  }
  fclose(f);
}

void CA_Save_bin(TClassAutomat *A, char *fname)
// ������ ��������� ����� � ����
{
  FILE *f;
  if((f=fopen(fname,"wb"))==NULL)
  {
    warning("Create file '%s' error", fname);
    return;
  }
  fprintf(f,"%c", A->strnum);
  fprintf(f,"%c%c",A->alphX,A->alphY);

  // ������� ���������
  for(int nq=0;nq<A->strnum;nq++)
    for(int ni=0;ni<A->alphX;ni++)
      fprintf(f,"%c",A->P[nq][ni]);
  // ������� ������������ ��������� �������
  for(int nq=0;nq<A->strnum;nq++)
    for(int ni=0;ni<A->alphX;ni++)
      for(int no=0;no<A->alphY;no++)
        fprintf(f,"%c",A->actp[nq][ni][no]);
  fclose(f);
}


#else

void CA_Load(TClassAutomat *A)
// ��������� ������� �������� (��������� �� ����)
{
  byte nq, ni, no;

  A->strnum = mGetch();
  A->alphX = mGetch();
  A->alphY = mGetch();

  // ������� ���������
  for(nq=0;nq<A->strnum;nq++)
    for(ni=0;ni<A->alphX;ni++)
      A->P[nq][ni] = mGetch();
  // ������� ������������ ��������� �������
  for(nq=0;nq<A->strnum;nq++)
    for(ni=0;ni<A->alphX;ni++)
      for(no=0;no<A->alphY;no++)
        A->actp[nq][ni][no] = mGetch();
  A->q = 0;
}

void CA_Save(TClassAutomat *A)
// ������ ��������� �����
{         
  byte nq, ni, no;
  printf("%d\n", A->strnum);
  printf("%d %d\n",A->alphX,A->alphY);

  // ������� ���������
  for(nq=0;nq<A->strnum;nq++)
  {
    for(ni=0;ni<A->alphX;ni++)
      printf("%d ",A->P[nq][ni]);
    printf("\n");
  }
  // ������� ������������ ��������� �������
  for(nq=0;nq<A->strnum;nq++)
  {
    for(ni=0;ni<A->alphX;ni++)
    {
      for(no=0;no<A->alphY;no++)
        printf("%d ",A->actp[nq][ni][no]);
      printf("\n");
    }
    printf("\n");
  }
}

#endif
