/****************************************************
 *          F29.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef F29_HEADER
#define F29_HEADER

#define E_STYLE29 ST_WINDOW
#define V_STYLE29 ST_FILEVIEW

extern file_29 * rf29;
extern Table *T29;

void Init29(void);
void Close29(void);
void AddF29( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
