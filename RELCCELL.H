/****************************************************
 *     RELCCELL.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef RELCCELL
#define RELCCELL

class _RelCCell :public _Cell{
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
  void LostFocus(int l=1);
  _RelCCell(short _x,short _y,short _w, short _style, short _w1, char *_info,
   short _TableID, short _Key, short _FieldID, void(*_Writer)(void *rec, char *buf)=0);

  ~_RelCCell(void);
  int  CanLostFocus(void);
  int CanClose(void);
  void PaintBuf2(void);
  int MHandle(_Event *e);
  void (*Writer)(void *rec, char *buf);
  void SetInsertFunc(void (*_insertF)(_Event * E, void *parent)){insertF=_insertF;};
  void SetDeleteFunc(void (*_deleteF)(_Event * E, void *parent)){deleteF=deleteF;};

};


#endif
