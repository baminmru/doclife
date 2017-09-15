/****************************************************
 *        TRACE.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef TRACE

extern int TraceFlag;
void  Trace (char *File, int line ,char *message);
void  Debug (char *format,...);
void  Error (char *format,...);
void  SetTrace(int x);

#ifdef NO_TRACE
#define TRACE(x)
#else
#define TRACE(x)  Trace(__FILE__,__LINE__,x)
#endif

#endif

