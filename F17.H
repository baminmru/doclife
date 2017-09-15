/****************************************************
 *          F17.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef F17_HEADER
#define F17_HEADER

#define E_STYLE17 ST_WINDOW
#define V_STYLE17 ST_FILEVIEW

extern file_17 * rf17;
extern Table *T17;

void Init17(void);
void Close17(void);
void AddF17( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
