/****************************************************
 *      FLTCELL.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef FLTCELL
#define FLTCELL

class _FltCell :public _Window{
  short digits;
  short sign;
  short pointed;
  char proto[15];       //%+99.9lf
 public:
  void Commit(void);
  void Load(void);
  char *buffer;
  float *info;
  short cur_x,cur_y,pos;
  void Pos2XY(void);
  int Handle(_Event *E);
  void Paint(void);
  _FltCell(short _x,short _y,short _style, short _digits, float *_info, short pts=2);
  ~_FltCell(void);
};


#endif
