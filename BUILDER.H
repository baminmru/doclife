/****************************************************
 *      BUILDER.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
_Window * BuildCell(Table * T, int field, void * rec, short x,short* y ,short w, int style);
void BuildForm(_Window * w, Table *t, void * rec, short style, short *f_list);
void InitRecord(Table *t , void *rec);
