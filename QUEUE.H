/****************************************************
 *        QUEUE.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef _QUEUE_
#define _QUEUE_
#define SQ_MAX 10
class _SystemQueue{
  short Top;
  short Total;
  short Bottom;
  _Event SQ[10];
 public:
  _SystemQueue(void);
  ~_SystemQueue(void);
  int AddMessage(long msg);
  int AddMessage(short type, long msg);
  _Event* GetMessage(void);
};
#endif





