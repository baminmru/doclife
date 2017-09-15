/****************************************************
 *          F22.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef F22_HEADER
#define F22_HEADER

#define E_STYLE22 ST_WINDOW
#define V_STYLE22 ST_FILEVIEW

extern file_22 * rf22;
extern Table *T22;

void Init22(void);
void Close22(void);
void AddF22( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
