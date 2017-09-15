/****************************************************
 *        QUEUE.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef _QUEUE_
#define _QUEUE_
#define SQ_MAX 10
class _SystemQueue{
  short Top;
  short Total;
  short Bottom;
  _Event SQ[10];
 public:
  _SystemQueue(void);
  ~_SystemQueue(void);
  int AddMessage(long msg);
  int AddMessage(short type, long msg);
  _Event* GetMessage(void);
};
#endif





