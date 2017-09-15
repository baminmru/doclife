/****************************************************
 *       DIAL.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <time.h>
#include <conio.h>
#include <dos.h>
#include <stdarg.h>
#include "common.h"
#include "dial.h"
#include "mail.h"
void ScrollWrite(_Window *Win, char *f,...);


#define CPS 18


_Window *dlrWin=0;

static short ProcSetted=0;
short Connected=0;
static int   InHangUp=0;

static MODEM_INIT ModemInit;

static COMMAND Cmd[10]={
  {{0},0},{{0},0},{{0},0},{{0},0},{{0},0},
  {{0},0},{{0},0},{{0},0},{{0},0},{{0},0}
};

static void _far _saveregs _loadds ReceiveProc(char c);
static void DoAction(short Action);
static short DialLoop(void);
static short AnswerLoop(void);
static short AnswerOffLoop(void);
static short HangUpLoop(void);

static char DialNumber[64];
unsigned short RegAX, RegBX,RegCX,RegDX;

void CallDriverRegs(unsigned short Func) {
  _asm {
    push bx
    mov bx,RegBX
    mov cx,RegCX
    mov dx,RegDX
    mov ax,Func
    int 61h
    mov RegBX,bx
    mov RegCX,cx
    mov RegDX,dx
    mov RegAX,ax
    pop bx
  }
}
short DriverInstalled(void) {
  long _far *lp=(long _far *)(0x61*4);
  if (*lp) {
    CallDriverRegs(0x0A00);
    return RegAX==18969;
  }
  return FALSE;
}


void dlrWrite(char *f){
  if(dlrWin){
    ScrollWrite(dlrWin,f);
  }
}


int  ReadInit(void) {
  short h,i; COMMAND *cm;
  // читаем из файла настройку модема
  h=open("dial.id",O_RDONLY|O_BINARY);
  if (h==-1) return 1;
  read(h,&ModemInit,sizeof(ModemInit));
  close(h);
  // инициализируем структуру распознавания команд модема
  strcpy(Cmd[0].Message,"OK"); Cmd[0].Action=MM_OK;
  strcpy(Cmd[1].Message,ModemInit.ConnectString); Cmd[1].Action=MM_CONNECT;
  StrTrim(Cmd[1].Message);
  for (i=0; i<5; i++) {
    strcpy(Cmd[i+2].Message,ModemInit.NotConnect[i]);
    StrTrim(Cmd[i+2].Message);
    Cmd[i+2].Action=MM_NO_CONNECTION;
  }

  for (cm=Cmd,i=0; i<7; cm++,i++){
    cm->ptr=cm->Message;
    //dlrWrite(cm->Message);
  }
  // находим адрес буфера обмена
  //CallDriverRegs(0x0A04);
  //ExchangeBuffer=(char _far *)(((((unsigned long)RegBX)<<8)<<8)+RegDX);
  //ExchangeBuffer=(char _far *)MK_FP(RegBX,RegDX);
  return 0;
}
// послать строку в порт напрямую
void SendStringDirect(char *String) {
  memcpy((Mailer->ND->ExBuf),String,RegDX=strlen(String));
  CallDriverRegs(0x0A0C);
}

void SendString(char *String) {
  char buf[64];
  strcpy(buf,String); strcat(buf,"\015\012");
  SendStringDirect(buf);
}

// установить мою процедуру получения символов из порта
static void SetMyReceiveProc(void) {
  RegBX=FP_SEG( ReceiveProc);
  RegDX=FP_OFF( ReceiveProc);
  CallDriverRegs(0x0A0A);
  ProcSetted=1;
}

// восстановить процедуру получения символов драйвера
static void SetDriverReceiveProc(void) {
  CallDriverRegs(0x0A0B); ProcSetted=0;
}

short dialer(char arg, char *phone, _Window * parent) {

  if (!DriverInstalled()) return 1;
  if (!Mailer || !Mailer->OK) return 1;

  dlrWin = new _Window(40,12,50,10,ST_OPTION,stFrame,"Монитор модема");
  dlrWin->Add(new _Static(18,0,ST_OPTION, " Монитор модема "));
  dlrWin->Center();
  parent->Add(dlrWin);
  parent->SwitchFocus(dlrWin);

  int ret=0;
  if(ReadInit()) return 1;
  if (arg=='i') {
    strcpy(DialNumber,phone);
    SetMyReceiveProc();
    ret=DialLoop();
    SetDriverReceiveProc();
  }
  else if (arg=='a') {
    SetMyReceiveProc();
    ret=AnswerLoop();
    SetDriverReceiveProc();
  }
  else if (arg=='o') {
    SetMyReceiveProc();
    ret=AnswerOffLoop();
    SetDriverReceiveProc();
  }
  else if (arg=='h') {
    SetMyReceiveProc();
    ret=HangUpLoop();
    SetDriverReceiveProc();
  }
  parent->Del(dlrWin);
  return ret;
}

static volatile short SomethingHappened;
static volatile short CmdNumber;
static short ModemMessage=MM_UNKNOWN;

static void _far _saveregs _loadds ReceiveProc(char c) {
  static COMMAND *cm; static short i;
  //ScrollWrite(dlrWin,"%c",c);

  for (cm=Cmd,i=0; i<7; cm++,i++)
  if (cm->Message[0]) {
 aaa:
    if (c==cm->ptr[0]) {
      (cm->ptr)++;
      if (cm->ptr[0]==0) {
        cm->ptr=cm->Message;
        CmdNumber=i;
        DoAction(cm->Action);
        return;
      }
    }
    else if (cm->ptr!=cm->Message) {
      cm->ptr=cm->Message; goto aaa;
    }
  }
}


static void DoAction(short Action) {
  SomethingHappened=TRUE;
  switch (Action) {
    case MM_NO_CONNECTION:
    case MM_CONNECT:
    case MM_OK:
      ModemMessage=Action;
      break;
    default:
      ModemMessage=MM_UNKNOWN;
      break;
  }
}

static void DialExit(void) {
  if (ProcSetted) {
    if(!InHangUp) HangUpLoop();
  }
}

static long prev=0;
static int PauseWhileNothingHappened(short Time,short MakeEsc=1) {

  clock_t t=clock();
  prev=t;
  SomethingHappened=FALSE;
  ScrollWrite(dlrWin,"Пауза %d сек ...",Time);
  while (clock()<t+((long)Time)*CPS) {
    if(prev+CPS<clock()){
      prev=clock();
      ScrollWrite(dlrWin,"Пауза %d сек ...",(t+((long)Time)*CPS-clock())/CPS);
    }
    if (SomethingHappened){
      dlrWrite(Cmd[CmdNumber].Message);
      break;
    }
    if (kbhit()) {
      while (kbhit())
      if (getch()==27){
       if(MakeEsc){
         dlrWrite("Нажали ESC");
         DialExit(); return 1;
       }
      }
    }
  }
  return 0;
}

static void Delay(short Time) {
  clock_t t=clock();
  while (clock()<=t+Time*CPS);
}




static short DialLoop() {
  if(Connected) HangUpLoop();

  char MyDialNumber[64];
  // склеиваем префикс и номер вызова
  strcpy(MyDialNumber,ModemInit.DialPrefix); strcat(MyDialNumber,DialNumber);
  // инициализируем модем
  dlrWrite("Инициализация модема");
  SendString(ModemInit.InitString);
  if( PauseWhileNothingHappened(5)) return 1;
  if (SomethingHappened) {
    if (ModemMessage!=MM_OK) {
      dlrWrite("Ошибка ...");
      return 1;
    }
  }
  else {
    dlrWrite("Нет ответа ...");
    return 1;
  }
  Delay(1);

  // цикл вызова
  for (;;) {
    dlrWrite("Набираем номер");
    SendString(MyDialNumber);
    if(PauseWhileNothingHappened(60)) return 1;
    if (SomethingHappened) {
      switch (ModemMessage) {
        case MM_CONNECT:
          dlrWrite("Есть соединение!");

          Connected=1;
          return 0;
        case MM_NO_CONNECTION:
          dlrWrite("Соединения не произощло ...");
          break;
        default:
          ;

      }
    }
    else dlrWrite("Тайаут ...");
    HangUpLoop();
    Delay(1);
  }
}


static short AnswerLoop() {
  HangUpLoop();
  dlrWrite("Инициализация модема");
  SendString(ModemInit.InitString);
  if(PauseWhileNothingHappened(10)) return 1;
  if (SomethingHappened) {
    if (ModemMessage!=MM_OK)  {
      dlrWrite("Ошибка инициализации ...");
      return 1;
    }
  }
  else {
     dlrWrite("Нет ответа ...");
     return 1;
  }
  Delay(1);
  for (;;) {
    dlrWrite("Устанавливаем режим ожидания");
    SendString(ModemInit.AutoAnswer);
    if(PauseWhileNothingHappened(5)) return 1;
    if (SomethingHappened) {
      if (ModemMessage!=MM_OK) {
        dlrWrite("Ошибка ...");
        return 1;
      }
    }
    else {
      dlrWrite("Нет ответа ...");
      return 1;
    }
    dlrWrite("Ожидаем звонка");
    for (;;) {
      SomethingHappened=FALSE;
      while (!SomethingHappened) {
        if (kbhit()) {
          while (kbhit())
            if (getch()==27) { AnswerOffLoop(); return 1; }
        }
      }
      dlrWrite("К нам позвонили ?!");
      switch (ModemMessage) {
        case MM_CONNECT:
          dlrWrite("Есть соединение");
          Connected =1;
          return 0;
        case MM_NO_CONNECTION:
          dlrWrite("Соединение не установлено ...");
          HangUpLoop();
          break;
        default:
          HangUpLoop();
          break;
      }
      break;
    }
  }
}

static short AnswerOffLoop() {
  if(Connected) HangUpLoop();
  for (;;) {
    dlrWrite("Закрываем режим ожидания звонка");
    Delay(1);
    SendString(ModemInit.AutoAnswerOff);
    PauseWhileNothingHappened(15,0);
    if (SomethingHappened) {
      switch (ModemMessage) {
        case MM_OK:
          dlrWrite("Закрыт");
          return 0;
        default:
          dlrWrite("Ошибка ...");

      }
    }
    else dlrWrite("Таймаут ...");
  }
}


static short HangUpLoop() {
  InHangUp=1;
  for (;;) {
    if(Connected){
      dlrWrite("Закрываем режим передачи");
      Delay(2);
      SendStringDirect("+++");
      Delay(2);
    }
    dlrWrite("Кладем трубку");

    SendString(ModemInit.HangUpString);
    PauseWhileNothingHappened(15,0);
    if (SomethingHappened) {
      switch (ModemMessage) {
        case MM_OK:
          dlrWrite("Положили");
          Connected=0;
          InHangUp=0;
          return 0;
        default:
          dlrWrite("Ошибка ...");
      }
    }
    else dlrWrite("Таймаут ...");
  }
}
