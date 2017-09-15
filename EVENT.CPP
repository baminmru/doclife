/****************************************************
 *      EVENT.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#define NO_TRACE
#include "common.h"
#undef NO_TRACE
//#include "option.h"
#include "logo.h"


void (*UserFunc)(void);
void (*ModemFunc)(void);

_Event::Init(void){
  memset(this,0,sizeof(_Event));
  return 0;
};

_Event::_Event(short _Type, short size){
  TRACE("_Event::_Event (main)");
  Init();
  Type=_Type;
  allocated=0;
  if(size){
    msg.info = new char [size];
    allocated=1;
  }
};

_Event::_Event(unsigned long _L){
  TRACE("_Event::_Event (system)");
  Init();
  Type=evSystem;
  msg.L=_L;
  allocated=0;
};

_Event::_Event(short _Type, unsigned short x, unsigned short y, unsigned char button){
  TRACE("_Event::_Event (mouse)");
  Init();
  Type=_Type;
  msg.M.x=x;
  msg.M.y=y;
  msg.M.buttons=button;
  allocated=0;

};

_Event::~_Event(void){
  TRACE("_Event::~_Event");
  if(allocated) delete msg.info;
};

void _Event::ClearEvent(_Window* owner){
  TRACE("_Event::ClearEvent");
  if(allocated){
    allocated=0;
    delete msg.info;
  }
  msg.info=owner;
  Type=0;
};

short PauseCnt=0;
long SaverCnt=0;

_Event *KeyHandler(void){
  TRACE("KeyHandler");
  _Event *res=new _Event(evKey,0);
  res->msg.K=GetKey(1);
  if(res->msg.K) { SaverCnt=0; PauseCnt=0;  return res;}
  delete res;
  return 0;

};

static short left=0,right=0,send=0,wait=15;
static short shift_pressed=0;
static unsigned long prev_event=0;
static MouseEventType e;


_Event *MouseHandler(void){

  if(_Mouse::present())
    if(_Mouse::getFromQ(e)){
      // пропускаем только собщения о нажатии и отпускании

      send =0;

      // если флаг нажатия левой кнопки установлен
      if(e.buttons & 1){
         // если она уже была нажата - начался драг
         if(left){
            e.buttons=4;
            send = 1;
         }
         else {
          prev_event=clock();
          wait = 15;
          left = 1;
          send = 1;
         }

      }else{
        if(left){
          left =0;
          send =1;
        }
      }
      if (send){
        e.buttons =e.buttons & 5;
        PauseCnt=0;
        SaverCnt=0;
        return new _Event(evMouse,e.x,e.y,e.buttons);
      }

      e.buttons = e.buttons & 0xfe;

      // если флаг нажатия правой кнопки установлен
      if(e.buttons & 2){
         // если она уже была нажата - сбросить флаг нажатия
         if(right)
           e.buttons=0;
         else {
          right =1;
          send  =1;
         }

      }else{
        if(right){
          right =0;
          send  =1;
        }
      }

      if (send){
        PauseCnt=0;
        SaverCnt=0;
        return new _Event(evMouse,e.x,e.y,e.buttons);
      }

      // обрабатываем нажатия Alt Ctrl Shift
      if(e.buttons & 0xE0){

        if((e.buttons&Alt_1)==Alt_1){
          e.buttons=Alt_1;
          send=1;
        }
        if((e.buttons&Alt_2)==Alt_2){
          e.buttons=Alt_2;
          send=1;
        }

        if((e.buttons&Ctrl_1)==Ctrl_1){
          e.buttons=Ctrl_1;
          send=1;
        }

        if((e.buttons&Ctrl_2)==Ctrl_2){
          e.buttons=Ctrl_2;
          send=1;
        }

        if((e.buttons&Shift_1)==Shift_1){
          e.buttons=Shift_1;
          send=1;
        }

        if((e.buttons&Shift_2)==Shift_2){
          e.buttons=Shift_2;
          send=1;
        }

          if(send){
            if(!shift_pressed){
              shift_pressed=1;
              //Debug("btn = %x",e.buttons);
              PauseCnt=0;
              SaverCnt=0;
              return new _Event(evMouse,e.x,e.y,e.buttons);
            }
            return 0;
          }else{
            shift_pressed=0;
          }

        }else
          return 0;

    }else{

     // обработка автоповтора
     if(left && prev_event+wait<clock()){
        wait=1;
        prev_event=clock();
        PauseCnt=0;
        SaverCnt=0;
        return new _Event(evMouse,e.x,e.y,e.buttons);
     }

    }
  return 0;
};



static unsigned long prev_clock=0;
_Event *TimerHandler(void){
  TRACE("TimerHandler");

  _Event *res=new _Event(evTime,0);

  res->msg.L=clock();
  if(res->msg.L>prev_clock){
    prev_clock=res->msg.L;
    return res;
  }
  delete res;
  return 0;
};


// функции для поддержки background процессов
// события не возвращаются ...
_Event *ScanHandler(void){

  if(1){
    PauseCnt++;
    if(PauseCnt>=5000){
      PauseCnt=0;
      if(UserFunc)
       (*UserFunc)();
    }
  }

  if(1){
    if(!SaverCnt){
      SaverOFF();
      SaverCnt=clock();
    }

    if(SaverCnt+1092<clock()){
      SaverON();
      SaverCnt=clock();
    }

  }
  return 0;

};

_Event *ModemHandler(void){
  static unsigned long m_clock=0;
  unsigned long now;
  now=clock();
  if(now > m_clock+1){
    m_clock=now;
    if(ModemFunc)
      (*ModemFunc)();
  }
  return 0;
};




