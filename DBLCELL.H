/****************************************************
 *      DBLCELL.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef DBLCELL
#define DBLCELL

class _DblCell :public _Window{
  short digits;
  short sign;
  short pointed;
  char proto[15];       //%+99.9lf
 public:
  char *buffer;
  double *info;
  short cur_x,cur_y,pos;
  void Pos2XY(void);
  int Handle(_Event *E);
  void Paint(void);
  void Commit(void);
  void Load(void);
  _DblCell(short _x,short _y,short _style, short _digits, double *_info, short pts=2);
  ~_DblCell(void);
};


#endif
