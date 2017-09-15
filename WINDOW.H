/****************************************************
 *       WINDOW.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef WINDOW
#define WINDOW

/*
  окна имеют фиксированный размер
  они также не имеют буфера для сохранения предыдущего состояния
*/

#define stBuffered  1
#define stHide      2
#define stCursor    4
#define stLocked    8
#define stNoMove    16
#define stNoESC     32
#define stInsert    64
#define stFrame     128
#define stFocused   256

class _Desktop;
class _Window;


struct CallBack
{
  void (*func)(_Event *,void *);      // функция обработки
  int type;                   // тип события
  long code;                  // код события
  CallBack *next;
  CallBack(void (*_func)(_Event*,void *), short _type, long _code):
    func(_func), code(_code), type(_type) ,next(0) {};
  ~CallBack();
};


class _WinList{
  public:
  short index;
  short total;
  _Window **array;
  _Window *FindWin  ( char *name);
  int Find( _Window *it);
  int Del ( _Window *it);
  void Add( _Window *it);
  _WinList(void);
  ~_WinList(void);
};

class _Window{
  public:
  static _Window *Zord[512];
  static short MaxZ;
  short Result;
  char *name;
  short min_w,min_h;
  short x,y,w,h; // координаты
  short style;       // стиль окраски

  _Window *parent;   //родительское окно
  _WinList *child;    // дети
  unsigned short st; //состояние окна
  char NotOnScreen;
  char *WBuffer;
  char *ExecBuf;
  CallBack *F;
  int HelpID;


  virtual int CanClose(void);      // возвращает 1 если можно закрыть окно
  virtual int CanSwitch(void);     // возвращает 1 если можно переключить процесс
  virtual int  CanLostFocus(void); // возвращает 1 если можно передать фокус
  virtual void LostFocus(int l=1);    // потеря фокуса
  virtual void GotFocus(int l=1);     // получение фокуса
  virtual void Commit(void);       // преобразовать содержимое буфера в выходное
                                   // значение
  virtual void Load(void);         // действие обратное Commit
  virtual void MoveOrGrawDone(short mode);

  void SwitchFocus(short Down=1, int l=1);  // переключить фокус на следующего/предыдущего сына
  void SwitchFocus(_Window *Focus, int l=1);// переключить фокус на конкретное окно
  _Window *FindFocused(void);      // найти окно которое в фокусе

  virtual int Handle(_Event * E);  // обработка событий
  virtual int MHandle(_Event *E);


  void Show(short a=1);            // показать окно
  virtual void Paint(void);        // перерисовать себя

  void Hide(short a=1);            // убрать окно с экрана
  void UnHide(short a=1);          // восстановить окно на экране
  void Move(short dx, short dy);   // подвинуть окно
  virtual void Graw(short dx, short dy);   // подвинуть окно

  void Add(_Window* NewWindow);
  void Del(_Window *OldWindow);
  _Window*  Find( char * name);      // найти окно по имени

  void Coord(short *x, short *y);  // преобразовать координаты
                                   // из относительных  абсолютные

  _Window(short _x,short _y,short _w,short _h, short _style, short _Status=0,char *_name="Window");
  virtual ~_Window(void);
  void dbg(short start=1);
  void AddCallBack(void (*func)(_Event*, void *), short type, long code);
  void DelCallBack(short type, long code);
  CallBack* FindCallBack(short type, long code);
  int Exec(void);
  void FreeAllBuffers(void);

  void Center(void);
  void CenterScr(void);
  void BottomRight(void);
  void CheckSize();
  void (*Changed)(void*);
  void (*Befor)(void*);
  void (*After)(void*);
  short ZFind  (_Window*);
  void  ZInsert(_Window *);
  void  ZDelete (_Window*);
  int SaveSize(void);
  int LoadSize(void);
};
void ScrollWrite(_Window *Win, char *f,...);
extern _Window *ShowBase;

#endif
