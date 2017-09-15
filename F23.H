/****************************************************
 *          F23.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef F23_HEADER
#define F23_HEADER

#define E_STYLE23 ST_WINDOW
#define V_STYLE23 ST_FILEVIEW

extern file_23 * rf23;
extern Table *T23;

void Init23(void);
void Close23(void);
void AddF23( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
