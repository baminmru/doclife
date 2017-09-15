/****************************************************
 *          F19.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef F19_HEADER
#define F19_HEADER

#define E_STYLE19 ST_WINDOW
#define V_STYLE19 ST_FILEVIEW

extern file_19 * rf19;
extern Table *T19;

void Init19(void);
void Close19(void);
void AddF19( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
