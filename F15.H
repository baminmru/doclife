/****************************************************
 *          F15.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef F15_HEADER
#define F15_HEADER

#define E_STYLE15 ST_WINDOW
#define V_STYLE15 ST_FILEVIEW

extern file_15 * rf15;
extern Table *T15;

void Init15(void);
void Close15(void);
void AddF15( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
