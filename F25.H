/****************************************************
 *          F25.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef F25_HEADER
#define F25_HEADER

#define E_STYLE25 ST_WINDOW
#define V_STYLE25 ST_FILEVIEW

extern file_25 * rf25;
extern Table *T25;

void Init25(void);
void Close25(void);
void AddF25( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
