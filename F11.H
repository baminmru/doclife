/****************************************************
 *          F11.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef F11_HEADER
#define F11_HEADER

#define E_STYLE11 ST_WINDOW
#define V_STYLE11 ST_FILEVIEW

extern file_11 * rf11;
extern Table *T11;

void Init11(void);
void Close11(void);
void AddF11( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
