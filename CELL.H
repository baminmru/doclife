/****************************************************
 *         CELL.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef CELL
#define CELL

class _Cell:public _Window{
 protected:
  short InsMode;
  short cur_x,cur_y,pos;
  void Pos2XY(void);
  int Handle(_Event *E);
 public:
  char *buffer;
  void *info;
  void Paint(void);
  void Commit(void);
  void Load(void);
  _Cell(short _x,short _y,short _w,short _h, short _style,char *info);
  ~_Cell();
};

class _Static:public _Window{
  public:
  char *buffer;
  _Static(short _x,short _y,short _style,char *info, short locked=1);
  void Paint(void);
  int Handle(_Event *E);
  int MHandle(_Event *E);
  ~_Static(void);
};

class _ColorCell:public _Static{
  public:
    short color;
    _ColorCell(int x,int y,int _color,char *info,short locked);
    ~_ColorCell();
    void Paint(void);
};

#endif
