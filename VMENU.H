/****************************************************
 *        VMENU.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef VMENU
#define VMENU

class _VMenu:public _Window {
  short *result;
  public:
  // ��������� � ������⢮� ��ப
  _VMenu(short _x, short _y, short _cnt, char **_items, short *result);

  // ��������� � �����⮬ ������⢠ ��ப
  _VMenu(short _x, short _y, char **_items, short *result);
  ~_VMenu(void);
  int Handle(_Event* E);

};
#endif

