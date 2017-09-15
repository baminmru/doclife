/****************************************************
 *        GAUGE.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef GAUGE
#define GAUGE

class _Gauge : public _Window {
  public:
   long info;
   long Range;
   void SetRange(long NewRange);
   void Paint(void);
   _Gauge(short _x, short _y, short _w, short _style, long _Range , char *name="Gauge");
   ~_Gauge();
  int Handle(_Event *E);
  void Update( long info);
  void Halt( void );
};
#endif
