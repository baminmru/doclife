/****************************************************
 *       LINDEX.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef LINDEX
#define LINDEX

class LIndex
{
  int h;         // file handle
  long counter;  // record counter
  int closed;    // list state
  char *name;    // list file name
 public:
  LIndex(char *name);
  ~LIndex();
  long GetPosition(long number);  // 0 if error, position else
  int AddPosition(long pos);      // 0 if error
  void CloseList(void);
  int NewList(void);              // 0 if error
  long Total(void){return counter;};
};

#endif
