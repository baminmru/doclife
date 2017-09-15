/****************************************************
 *          F28.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef F28_HEADER
#define F28_HEADER

#define E_STYLE28 ST_WINDOW		
#define V_STYLE28 ST_FILEVIEW

extern file_28 * rf28;
extern Table *T28;

void Init28(void);
void Close28(void);
void AddF28( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif
 
 
