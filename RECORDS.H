/****************************************************
 *      RECORDS.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
//------------------------ _Region ------------------------------------
struct _Region
{
  char Name[31];
  int lnum;
  int code;
};

struct _Region_0
{
  char Name[31];
};

struct _Region_1
{
  int lnum;
};

struct _Region_2
{
  int code;
};

//----------------------- _Data ----------------------------------------
struct _Data
{
  int R;
  int G;
  int P;
  _Date date;
  long double money[40];
};

struct _Data_0
{
  int R;
  int G;
  int P;
  _Date date;
};


struct _Data_1
{
  int R;
  int P;
  _Date date;
};

struct _Data_2
{
  int G;
  int P;
  _Date date;
};


struct _Data_3
{
  int G;
  _Date date;
};

struct _Data_4
{
  int P;
  _Date date;
};

struct _Data_5
{
  _Date date;
};

