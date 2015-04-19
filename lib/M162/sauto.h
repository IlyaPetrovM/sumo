//----------------------------------------------------------
// ������� ��� ������
// (���������� ��������� = 1)
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

// ���������� ������� �������� X
#define maxNX  21

// ���������� �������� �������� Y (��������)
#define maxNY  12

typedef struct
{
  byte NX;
  byte NY;
  byte AM[maxNX][maxNY]; // ������� ��������
} TAutomat;

// ����� ��������� ��������������� �������
#define SVSum 100

//----------------------------------------------------------
void mtInit(TAutomat *A, byte cX, byte cY);
// ������������� ������� ��������
// cX, cY - ���������� ������� � �������� �������� ��������

int mtGetY(TAutomat *A, byte x);
// ����� �������� Y � ����������� �� �������� ������� x
// (�������� ����� � ������������ � �������� ������������)
// ���������� -1 � ������ �������
// ������������ �������� ���������� ��������

void mtInc(TAutomat *A, byte x, byte n, byte delta);
// ���������� �������� �������� ����� n ������ x �� �������� delta

void mtDec(TAutomat *A, byte x, byte n, byte delta);
// ���������� �������� �������� ����� n ������ x �� �������� delta

#ifdef _WIN32_
  void mtSave(TAutomat *A, FILE *f);
  // ��������� ������� �������� (��������� �� �� ����)

  void mtLoad(TAutomat *A, FILE *f);
  // ��������� ������� �������� (��������� �� ����)
#else
  
  inline char mGetch(void);

  void mtSave(TAutomat *A);
  // ��������� ������� �������� (��������� �� �� ����)

  void mtLoad(TAutomat *A);
  // ��������� ������� �������� (��������� �� ����)
#endif

#endif
