/****************************************************
 *     RELLCELL.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef RELLCELL
#define RELLCELL

class _RelLCell :public _DigCell{
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
  _RelLCell(short _x,short _y,short _w, short _style, short _digits, long *_info, short _sign
   , short _TableID, short _Key, short _FieID, void (*_Writer)(void *rec, char *buf)=0);
  ~_RelLCell(void);
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
