/****************************************************
 *          F13.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef F13_HEADER
#define F13_HEADER

#define E_STYLE13 ST_WINDOW
#define V_STYLE13 ST_FILEVIEW

extern file_13 * rf13;
extern Table *T13;

void Init13(void);
void Close13(void);
void AddF13( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
