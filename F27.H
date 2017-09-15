/****************************************************
 *          F27.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef F27_HEADER
#define F27_HEADER

#define E_STYLE27 ST_WINDOW
#define V_STYLE27 ST_FILEVIEW

extern file_27 * rf27;
extern Table *T27;

void Init27(void);
void Close27(void);
void AddF27( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
