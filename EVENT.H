/****************************************************
 *        EVENT.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef EVENT
#define EVENT

#define evSystem 1
#define evKey    2
#define evMouse  4
#define evCom    8
#define evNet    16
#define evTime   32
//...

// системные сообщения
#define sysStop          0
#define sysChildClosed   1
#define sysGraw          2
#define sysGrawBuf       3
#define sysMove          4
#define sysMoveBuf       5
#define sysGaugeUpdate   6
#define sysGaugeStop     7
#define sysHelp          8

class _Window;

class _Event{
 public:
  short Type;
  char allocated;
  union{
    unsigned K;               // клавиатура
    MouseEventType M;         // мыша
    unsigned long L;          // код
    void *info;               // кусок памяти
  }msg;


  _Event(short _Type, short size);
  _Event(unsigned long L=sysStop); //System messges
  _Event(short _Type, unsigned short X, unsigned short Y, unsigned char BUTTONS);
  ~_Event(void);
  Init(void);
  void ClearEvent(_Window* Owner);
};

_Event * MouseHandler(void); // обработчик мыши
_Event * KeyHandler(void);   // обработчик клавиатуры
_Event * TimerHandler(void); // обработчик таймера

// маски для обработки нажатия мыши и спец клавиш
// левая нажата
#define Alt_1    0x88
#define Ctrl_1   0x48
#define Shift_1  0x28

// правая нажата
#define Alt_2    0x90
#define Ctrl_2   0x50
#define Shift_2  0x30

// клавиша отпущена
#define Alt_     0x80
#define Ctrl_    0x40
#define Shift_   0x20

#endif
