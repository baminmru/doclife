/****************************************************
 *     RELSCELL.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef RELSCELL
#define RELSCELL

class _RelSCell :public _ShortCell{
 public:
  Table *T;
  int  CodeState;
  char *buffer2;
  short size2;
  short FieldID;
  short Key;
  void (*insertF)(_Event * E, void *parent);
  void (*deleteF)(_Event * E, void *parent);
  void Paint(void);
  _RelSCell(short _x,short _y,short _w, short _style, short _digits, short *_info, short _sign
   , short _TableID, short _Key,short FieldID, void (*_Writer)(void *rec, char *buf)=0);
  ~_RelSCell(void);
  int  CanLostFocus(void);
  int CanClose(void);
  void PaintBuf2(void);
  void LostFocus(int l=1);
  int MHandle(_Event *e);
  void (*Writer)(void *rec, char *buf);
  void SetInsertFunc(void (*_insertF)(_Event * E, void *parent)){insertF=_insertF;};
  void SetDeleteFunc(void (*_deleteF)(_Event * E, void *parent)){deleteF=deleteF;};
};


#endif
