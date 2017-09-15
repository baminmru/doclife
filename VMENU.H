/****************************************************
 *        VMENU.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef VMENU
#define VMENU

class _VMenu:public _Window {
  short *result;
  public:
  // конструктор с количеством строк
  _VMenu(short _x, short _y, short _cnt, char **_items, short *result);

  // конструктор с подсчетом количества строк
  _VMenu(short _x, short _y, char **_items, short *result);
  ~_VMenu(void);
  int Handle(_Event* E);

};
#endif

