/****************************************************
 *      DIGCELL.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef DIGCELL
#define DIGCELL

class _DigCell :public _Window{
  short digits;
  short sign;
 public:
  char *buffer;
  long *info;
  short cur_x,cur_y,pos;
  void Pos2XY(void);
  int Handle(_Event *E);
  void Paint(void);
  void Commit(void);
  void Load(void);
  _DigCell(short _x,short _y,short _style, short _digits, long *info, short _sign=0);
  ~_DigCell(void);
};


#endif
