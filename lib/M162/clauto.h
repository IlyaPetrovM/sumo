//----------------------------------------------------------
// ���������������� �������
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

//������������ ����� ����������� ������
#define MAX_STR 11

//������������ ������ �������� ��������
#define MAX_IN_SIZE 2

typedef struct
{
  byte strnum;
  byte q;                  // ������� ���������
  byte alphX;              // ����������� �������� ��������
  byte alphY;              // ����������� ��������� ��������

  byte P[MAX_STR][MAX_IN_SIZE];        // ������� ���������
  byte actp[MAX_STR][MAX_IN_SIZE][ECL_END];// ������� ������������ ��������� �������
} TClassAutomat;

int CA_accept(TClassAutomat *A, byte x);

#ifdef _WIN32_

#define CE_SVSum 1024

void CA_Load(TClassAutomat *A, FILE *f);
// ��������� ������� �������� (��������� �� ����)

void CA_read(TClassAutomat *A, char *fname);
// ������ ��������� ����� �� �����

void CA_Save(TClassAutomat *A, char *fname);
// ������ ��������� ����� � ����

void CA_Save_bin(TClassAutomat *A, char *fname);
// ������ ��������� ����� � ����

#else

void CA_Load(TClassAutomat *A);
// ��������� ������� �������� (��������� �� ����)

void CA_Save(TClassAutomat *A);
// ������ ��������� �����

#endif

#endif
