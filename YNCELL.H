/****************************************************
 *       YNCELL.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef YNCELL
#define YNCELL

class _YNCell:public _Window{
 protected:
  unsigned char *info;
  char buffer[4];
  int Handle(_Event *E);
  int MHandle(_Event *E);
  void Paint(void);
 public:
  _YNCell(short _x,short _y, short _style,unsigned char *info);
  ~_YNCell();
};

#endif
