/****************************************************
 *     DATECELL.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef DATECELL
#define DATECELL

class _DateCell:public _Window{
 protected:
  _Date *info;
  char *buffer;
  short cur_x,cur_y,pos;
  void Pos2XY(void);
  int Handle(_Event *E);
  void Paint(void);
 public:
  void Commit(void);
  void Load(void);
  _DateCell(short _x,short _y, short _style,_Date *info);
  ~_DateCell();
};

#endif
