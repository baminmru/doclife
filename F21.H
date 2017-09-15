/****************************************************
 *          F21.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef F21_HEADER
#define F21_HEADER

#define E_STYLE21 ST_WINDOW
#define V_STYLE21 ST_FILEVIEW

extern file_21 * rf21;
extern Table *T21;

void Init21(void);
void Close21(void);
void AddF21( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
