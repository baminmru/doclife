/****************************************************
 *          F12.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef F12_HEADER
#define F12_HEADER

#define E_STYLE12 ST_WINDOW
#define V_STYLE12 ST_FILEVIEW

extern file_12 * rf12;
extern Table *T12;

void Init12(void);
void Close12(void);
void AddF12( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
