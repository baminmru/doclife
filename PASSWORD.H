/****************************************************
 *     PASSWORD.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef PASSWORD
#define PASSWORD

class _Password:public _Cell{
 public:
  void Paint(void);
  _Password(short _x,short _y,short _w, short _style,char *info):_Cell(_x, _y, _w, 1,  _style,info){};
  ~_Password(){};
};

#endif
