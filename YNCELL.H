/****************************************************
 *       YNCELL.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef YNCELL
#define YNCELL

class _YNCell:public _Window{
 protected:
  unsigned char *info;
  char buffer[4];
  int Handle(_Event *E);
  int MHandle(_Event *E);
  void Paint(void);
 public:
  _YNCell(short _x,short _y, short _style,unsigned char *info);
  ~_YNCell();
};

#endif
