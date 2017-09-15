/****************************************************
 *     BYTECELL.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef BYTECELL
#define BYTECELL

class _ByteCell :public _Window{
  short digits;
  short sign;
 public:
  char *buffer;
  char *info;
  short cur_x,cur_y,pos;
  void Pos2XY(void);
  int Handle(_Event *E);
  void Paint(void);
  void Commit(void);
  void Load(void);
  _ByteCell(short _x,short _y,short _style, short _digits, char *info, short _sign=0);
  ~_ByteCell(void);
};


#endif
