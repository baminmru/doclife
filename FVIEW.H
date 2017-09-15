/****************************************************
 *        FVIEW.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef FVIEV
#define FVIEW

#define MAX_STRING_WIDTH 256
#define MAX_LINES 25

class _FileView : public _Window
{
 public:
 Table *T;
 char *CurRec;                        // буфер текущей записи
 char LineBuffer[MAX_STRING_WIDTH+1]; // буфер экрана
 unsigned long RecPos[MAX_LINES];     // массив адресов записей
 char *Header[5];
 int KeyNum;
 char KeyFilter[256];                 // значения фильтра ключа
 int KeyFilterSize;                   // длина фильтра
 int ActivLine;                       // текщий индекс
 int MaxLine;                         // максимальное количество строк
 int TotalLine;                       // максимальное количество строк сейчас
 int FirstY;
 int PageChanged;
 int LineX;
 int LockMode;
 int Next(void);                      // следующая запись
 int Previous(void);                  // предыдущая запись
 void PageUp(void);                    // страница вверх
 void PageDn(void);                    // страница вниз
 void ToTop(void);                     // в начало
 void ToBottom(void);                  // в конец
 void (*WriteRec)(void * Rec ,char * Buf); // напечатать запись в буфер
 void ReShowLine(int Line);           // перерисовать линию
 void ReShowAll(void);                // перерисовать
 int  RecordOK(void * Rec);            // запись допускается фильтром
 void Repaint(int lock = 1);
 void SetHeader(short idx, char *HeaderLine);


 _FileView(  Table * _T
           ,int     _KeyNum
           ,void *  _KeyFilter
           ,int     _KeyFilterSize
           ,void (* _WriteRec)(void * ,char * )
           ,int _x
           ,int _y
           ,int _w
           ,int _h
           ,int _top
           ,short _style
           ,char *_name="FileView"
 );

 ~_FileView();

 void Paint(void);
 int MHandle(_Event *E);
 int Handle(_Event *E);               // цикл просмотра (возвр. код клавиши)
 unsigned long CurRecPos(void);       // получить позицию текущей записи в файле
 Table *GetTable(void) {return T;}
 void JumpToRec(void * Rec);
 void JumpToKey(void * Key);
 void JumpToPos(long newpos);
 int Insert(void *R);
 int Delete(void);
 int Update(void *R, int unlock=0);
 int Lock(void *R, int mode=200);
 void Unlock(void);
 void ChangeFilter(int _KeyNum, void *_KeyFilter, int _KeySize);
 int Find(short FieldID);
 void MoveOrGrawDone(short mode);
};

#endif
