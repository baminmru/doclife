/****************************************************
 *      CLSVIEW.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef CLSVIEV
#define CLSVIEW


struct clsctrs {
  short   id       ;
  short   item     ;
  char    name[31] ;
};

struct clsctrs_0 {
  short   id       ;
  short   item     ;
};


class _ClsView : public _Window
{
 long * result;
 static Table T;                      //("X$Classificator");
 clsctrs CurRec;                      // буфер текущей записи
 char LineBuffer[81];                 // буфер экрана
 unsigned long RecPos[25];            // массив адресов записей (2 страници)
 short KeyFilter[2];
 int KeyFilterSize;                   // длина фильтра
 int ActivLine;                       // текщий индекс
 int MaxLine;                         // максимальное количество строк
 int TotalLine;                       // максимальное количество строк сейчас
 int FirstY;
 int PageChanged;


 int Next(void);                      // следующая запись
 int Previous(void);                  // предыдущая запись
 void PageUp(void);                    // страница вверх
 void PageDn(void);                    // страница вниз
 void ToTop(void);                     // в начало
 void ToBottom(void);                  // в конец
 void (*WriteRec)(void * Rec ,char * Buf); // напечатать запись в буфер
 void ReShowLine(int Line);           // перерисовать линию
 void ReShowAll(void);                // перерисовать
 void Repaint(void);
 int RecordOK(void *Rec);
 void Paint(void);

 public:

 _ClsView( long * _result
           ,short ID
           ,int _x
           ,int _y
           ,int _w
           ,int _h
           ,int _top
           ,char *_name="ClsView"
 );

 ~_ClsView();

 int MHandle(_Event *E);               // цикл просмотра (возвр. код клавиши)
 int Handle(_Event *E);               // цикл просмотра (возвр. код клавиши)
};

#endif
