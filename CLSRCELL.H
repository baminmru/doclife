/****************************************************
 *     CLSRCELL.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef CLSRCELL
#define CLSRCELL

class _ClsrCell:public _Window{
 protected:

  char *buffer;
  Table *T;

  int MHandle(_Event *E);
  int Handle(_Event *E);
  void Paint(void);
 public:
  short ID;
  static clsctrs_0 k;
  static clsctrs r;
  short *info;
  long pos;
  _ClsrCell(short _x,short _y, short _w, short _style,short *info, short _ID );
  ~_ClsrCell();
  void LostFocus(int l=1);
  void Next(void);
  void Current(void);
  void Pos2Info(void);
};

#endif
