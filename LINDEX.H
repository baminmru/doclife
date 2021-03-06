/****************************************************
 *       LINDEX.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef LINDEX
#define LINDEX

class LIndex
{
  int h;         // file handle
  long counter;  // record counter
  int closed;    // list state
  char *name;    // list file name
 public:
  LIndex(char *name);
  ~LIndex();
  long GetPosition(long number);  // 0 if error, position else
  int AddPosition(long pos);      // 0 if error
  void CloseList(void);
  int NewList(void);              // 0 if error
  long Total(void){return counter;};
};

#endif
