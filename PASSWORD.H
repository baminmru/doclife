/****************************************************
 *     PASSWORD.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef PASSWORD
#define PASSWORD

class _Password:public _Cell{
 public:
  void Paint(void);
  _Password(short _x,short _y,short _w, short _style,char *info):_Cell(_x, _y, _w, 1,  _style,info){};
  ~_Password(){};
};

#endif
