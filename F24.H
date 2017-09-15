/****************************************************
 *          F24.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef F24_HEADER
#define F24_HEADER

#define E_STYLE24 ST_WINDOW
#define V_STYLE24 ST_FILEVIEW

extern file_24 * rf24;
extern Table *T24;

void Init24(void);
void Close24(void);
void AddF24( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
