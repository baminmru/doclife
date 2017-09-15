/****************************************************
 *      DESKTOP.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef DESKTOP
#define DESKTOP

#define MAX_INPUT_CHANEL 16
#define MX 80
#define MY 25



class _Desktop :public _Window{
 public:
  _Event * (*proc[MAX_INPUT_CHANEL])(void);     //массив процедур источников информации
  short ProcessEvent(_Event *E);                // обработать сообщения
  short NextProcess;
  _Mouse M;
  _SystemQueue Q;
  _Screen S;                                    // класс управления экраном
  void Run(void);                               //основной цикл системы
  void Paint(void);
  int Handle(_Event *E);
  int MHandle(_Event *E);
  void AddProc(_Event * (*pp)(void), short idx);// добавить процедуру источник событий
  _Desktop(char * _name);
  ~_Desktop(void);
  StoreFocus(_Window **);
  RestoreFocus(_Window *);
};

extern _Desktop *GDt;

#endif
