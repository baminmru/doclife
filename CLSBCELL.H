/****************************************************
 *     CLSBCELL.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef CLSBCELL
#define CLSBCELL

class _ClsBCell:public _Window{
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
  unsigned char *info;
  long pos;
  _ClsBCell(short _x,short _y, short _w,short _style,unsigned char *_info,short _ID);

  ~_ClsBCell();
  void LostFocus(int l=1);
  void Next(void);
  void Current(void);
  void Pos2Info(void);
};

#endif
