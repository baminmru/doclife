/****************************************************
 *     TIMECELL.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef TIMECELL
#define TIMECELL

class _TimeCell:public _Window{
 protected:
  _Time *info;
  char *buffer;
  short cur_x,cur_y,pos;
  void Pos2XY(void);
  int Handle(_Event *E);
 public:
  void Paint(void);
  void Commit(void);
  void Load(void);
  _TimeCell(short _x,short _y, short _style,_Time *info);
  ~_TimeCell();
};

#endif
