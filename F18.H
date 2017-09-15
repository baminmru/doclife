/****************************************************
 *          F18.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef F18_HEADER
#define F18_HEADER

#define E_STYLE18 ST_WINDOW
#define V_STYLE18 ST_FILEVIEW

extern file_18 * rf18;
extern Table *T18;

void Init18(void);
void Close18(void);
void AddF18( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
