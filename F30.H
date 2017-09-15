/****************************************************
 *          F30.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef F30_HEADER
#define F30_HEADER

#define E_STYLE30 ST_WINDOW		
#define V_STYLE30 ST_FILEVIEW

extern file_30 * rf30;
extern Table *T30;

void Init30(void);
void Close30(void);
void AddF30( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif
 
 
