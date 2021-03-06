/****************************************************
 *       DIAL.CPP                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
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
  // �⠥� �� 䠩�� ����ன�� ������
  h=open("dial.id",O_RDONLY|O_BINARY);
  if (h==-1) return 1;
  read(h,&ModemInit,sizeof(ModemInit));
  close(h);
  // ���樠�����㥬 �������� �ᯮ�������� ������ ������
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
  // ��室�� ���� ���� ������
  //CallDriverRegs(0x0A04);
  //ExchangeBuffer=(char _far *)(((((unsigned long)RegBX)<<8)<<8)+RegDX);
  //ExchangeBuffer=(char _far *)MK_FP(RegBX,RegDX);
  return 0;
}
// ��᫠�� ��ப� � ���� �������
void SendStringDirect(char *String) {
  memcpy((Mailer->ND->ExBuf),String,RegDX=strlen(String));
  CallDriverRegs(0x0A0C);
}

void SendString(char *String) {
  char buf[64];
  strcpy(buf,String); strcat(buf,"\015\012");
  SendStringDirect(buf);
}

// ��⠭����� ��� ��楤��� ����祭�� ᨬ����� �� ����
static void SetMyReceiveProc(void) {
  RegBX=FP_SEG( ReceiveProc);
  RegDX=FP_OFF( ReceiveProc);
  CallDriverRegs(0x0A0A);
  ProcSetted=1;
}

// ����⠭����� ��楤��� ����祭�� ᨬ����� �ࠩ���
static void SetDriverReceiveProc(void) {
  CallDriverRegs(0x0A0B); ProcSetted=0;
}

short dialer(char arg, char *phone, _Window * parent) {

  if (!DriverInstalled()) return 1;
  if (!Mailer || !Mailer->OK) return 1;

  dlrWin = new _Window(40,12,50,10,ST_OPTION,stFrame,"������ ������");
  dlrWin->Add(new _Static(18,0,ST_OPTION, " ������ ������ "));
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
  ScrollWrite(dlrWin,"��㧠 %d ᥪ ...",Time);
  while (clock()<t+((long)Time)*CPS) {
    if(prev+CPS<clock()){
      prev=clock();
      ScrollWrite(dlrWin,"��㧠 %d ᥪ ...",(t+((long)Time)*CPS-clock())/CPS);
    }
    if (SomethingHappened){
      dlrWrite(Cmd[CmdNumber].Message);
      break;
    }
    if (kbhit()) {
      while (kbhit())
      if (getch()==27){
       if(MakeEsc){
         dlrWrite("������ ESC");
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
  // ᪫������ ��䨪� � ����� �맮��
  strcpy(MyDialNumber,ModemInit.DialPrefix); strcat(MyDialNumber,DialNumber);
  // ���樠�����㥬 �����
  dlrWrite("���樠������ ������");
  SendString(ModemInit.InitString);
  if( PauseWhileNothingHappened(5)) return 1;
  if (SomethingHappened) {
    if (ModemMessage!=MM_OK) {
      dlrWrite("�訡�� ...");
      return 1;
    }
  }
  else {
    dlrWrite("��� �⢥� ...");
    return 1;
  }
  Delay(1);

  // 横� �맮��
  for (;;) {
    dlrWrite("����ࠥ� �����");
    SendString(MyDialNumber);
    if(PauseWhileNothingHappened(60)) return 1;
    if (SomethingHappened) {
      switch (ModemMessage) {
        case MM_CONNECT:
          dlrWrite("���� ᮥ�������!");

          Connected=1;
          return 0;
        case MM_NO_CONNECTION:
          dlrWrite("���������� �� �ந��髮 ...");
          break;
        default:
          ;

      }
    }
    else dlrWrite("������ ...");
    HangUpLoop();
    Delay(1);
  }
}


static short AnswerLoop() {
  HangUpLoop();
  dlrWrite("���樠������ ������");
  SendString(ModemInit.InitString);
  if(PauseWhileNothingHappened(10)) return 1;
  if (SomethingHappened) {
    if (ModemMessage!=MM_OK)  {
      dlrWrite("�訡�� ���樠����樨 ...");
      return 1;
    }
  }
  else {
     dlrWrite("��� �⢥� ...");
     return 1;
  }
  Delay(1);
  for (;;) {
    dlrWrite("��⠭�������� ०�� ��������");
    SendString(ModemInit.AutoAnswer);
    if(PauseWhileNothingHappened(5)) return 1;
    if (SomethingHappened) {
      if (ModemMessage!=MM_OK) {
        dlrWrite("�訡�� ...");
        return 1;
      }
    }
    else {
      dlrWrite("��� �⢥� ...");
      return 1;
    }
    dlrWrite("������� ������");
    for (;;) {
      SomethingHappened=FALSE;
      while (!SomethingHappened) {
        if (kbhit()) {
          while (kbhit())
            if (getch()==27) { AnswerOffLoop(); return 1; }
        }
      }
      dlrWrite("� ��� ��������� ?!");
      switch (ModemMessage) {
        case MM_CONNECT:
          dlrWrite("���� ᮥ�������");
          Connected =1;
          return 0;
        case MM_NO_CONNECTION:
          dlrWrite("���������� �� ��⠭������ ...");
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
    dlrWrite("����뢠�� ०�� �������� ������");
    Delay(1);
    SendString(ModemInit.AutoAnswerOff);
    PauseWhileNothingHappened(15,0);
    if (SomethingHappened) {
      switch (ModemMessage) {
        case MM_OK:
          dlrWrite("������");
          return 0;
        default:
          dlrWrite("�訡�� ...");

      }
    }
    else dlrWrite("������� ...");
  }
}


static short HangUpLoop() {
  InHangUp=1;
  for (;;) {
    if(Connected){
      dlrWrite("����뢠�� ०�� ��।��");
      Delay(2);
      SendStringDirect("+++");
      Delay(2);
    }
    dlrWrite("������ ��㡪�");

    SendString(ModemInit.HangUpString);
    PauseWhileNothingHappened(15,0);
    if (SomethingHappened) {
      switch (ModemMessage) {
        case MM_OK:
          dlrWrite("��������");
          Connected=0;
          InHangUp=0;
          return 0;
        default:
          dlrWrite("�訡�� ...");
      }
    }
    else dlrWrite("������� ...");
  }
}
