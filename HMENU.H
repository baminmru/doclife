/****************************************************
 *        HMENU.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef HMENU
#define HMENU

class _HMenu:public _Window {
  short *result;
  public:

  // ��������� � �����⮬ ������⢠ ��ப
  _HMenu(short _x, short _y,  char **_items, short *result);
  ~_HMenu(void);
  int Handle(_Event* E);

};
#endif

