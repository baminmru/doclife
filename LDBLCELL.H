/****************************************************
 *     LDBLCELL.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef LDBLCELL
#define LDBLCELL

class _LDblCell :public _Window{
  short digits;
  short sign;
  short pointed;
  char proto[15];
 public:
  char *buffer;
  long double *info;
  short cur_x,cur_y,pos;
  void Pos2XY(void);
  int Handle(_Event *E);
  //void LostFocus(int l=1);
  //void GotFocus(int l=1);
  void Paint(void);
  void Commit();
  void Load();
  _LDblCell(short _x,short _y,short _style, short _digits, long double *_info, short pts=2);
  ~_LDblCell(void);
};


#endif
