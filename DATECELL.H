/****************************************************
 *     DATECELL.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef DATECELL
#define DATECELL

class _DateCell:public _Window{
 protected:
  _Date *info;
  char *buffer;
  short cur_x,cur_y,pos;
  void Pos2XY(void);
  int Handle(_Event *E);
  void Paint(void);
 public:
  void Commit(void);
  void Load(void);
  _DateCell(short _x,short _y, short _style,_Date *info);
  ~_DateCell();
};

#endif
