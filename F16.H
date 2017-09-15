/****************************************************
 *          F16.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef F16_HEADER
#define F16_HEADER

#define E_STYLE16 ST_WINDOW
#define V_STYLE16 ST_FILEVIEW

extern file_16 * rf16;
extern Table *T16;

void Init16(void);
void Close16(void);
void AddF16( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
