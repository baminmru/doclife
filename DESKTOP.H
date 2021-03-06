/****************************************************
 *      DESKTOP.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef DESKTOP
#define DESKTOP

#define MAX_INPUT_CHANEL 16
#define MX 80
#define MY 25



class _Desktop :public _Window{
 public:
  _Event * (*proc[MAX_INPUT_CHANEL])(void);     //���ᨢ ��楤�� ���筨��� ���ଠ樨
  short ProcessEvent(_Event *E);                // ��ࠡ���� ᮮ�饭��
  short NextProcess;
  _Mouse M;
  _SystemQueue Q;
  _Screen S;                                    // ����� �ࠢ����� ��࠭��
  void Run(void);                               //�᭮���� 横� ��⥬�
  void Paint(void);
  int Handle(_Event *E);
  int MHandle(_Event *E);
  void AddProc(_Event * (*pp)(void), short idx);// �������� ��楤��� ���筨� ᮡ�⨩
  _Desktop(char * _name);
  ~_Desktop(void);
  StoreFocus(_Window **);
  RestoreFocus(_Window *);
};

extern _Desktop *GDt;

#endif
