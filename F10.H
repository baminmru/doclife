/****************************************************
 *          F10.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef F10_HEADER
#define F10_HEADER

#define E_STYLE10 ST_WINDOW
#define V_STYLE10 ST_FILEVIEW

extern file_10 * rf10;
extern Table *T10;

void Init10(void);
void Close10(void);
void AddF10( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
