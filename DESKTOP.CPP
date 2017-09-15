/****************************************************
 *    DESKTOP.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#define NO_TRACE
#include "common.h"
#undef NO_TRACE

extern short ShutDown;
_Event *ScanHandler(void);
_Event *ModemHandler(void);


_Desktop *GDt=0;
_Window *ShowBase=0;

_Desktop::_Desktop(char *name):
  S(MX,MY)
  ,_Window(0,0,MX,MY,ST_DESKTOP,stFocused,name)
{
  HelpID=1;
  TRACE("_Desktop::Desktop");
  NextProcess=0;

  for(int i=0; i<MAX_INPUT_CHANEL; i++)
    proc[i]=0;

  AddProc(KeyHandler,0);
  AddProc(MouseHandler,1);
  // AddProc(TimerHandler,2);
  AddProc(ScanHandler,3);
  AddProc(ModemHandler,4);

  GDt = this;
  ShowBase =(_Window*) this;
  if(M.present())
    M.show();
}
_Desktop::~_Desktop(void){
  TRACE("_Desktop::~Desktop");
  if(M.present())
    M.hide();
};


void _Desktop::AddProc(_Event * (*pp)(void), short idx){
  TRACE("_Desktop::AddProc");
  proc[idx] =pp;
};



void _Desktop::Run(void){
  TRACE("Desktop::Run");
  short Quit=0;
  Show();
  _Event *E=0;
  while(!Quit){

   // опрос системных сообщений
   while((E=Q.GetMessage())) {
     Quit = ProcessEvent(E);
     if(Quit){
      E=0;
      break;
     }
   }
   if(!Quit)
     for(int i=0; i<MAX_INPUT_CHANEL ;i++){
       if(proc[i]!=0 &&
          (E=(proc[i])())
         ) {
         Quit=ProcessEvent(E);
         delete E;

         if(Quit)
          break;
       }
     }

  }
  S.SetCursor(2);
};

short _Desktop::ProcessEvent(_Event *E){
  TRACE("Desktop::ProcessEvent");
  short result=0;

  // передать сообщение активному процессу
  _Window *tmp;
  tmp=FindFocused();
  if(tmp)
    result=tmp->Handle(E);
  else
    result=Handle(E);

  // если никто не признался сбрасываем сообщение
  if(E->Type) E->ClearEvent(this);

  return result;
};
void _Desktop::Paint(void){
  _Window::Paint();
  S.AWrite(0,0,'■',Style2Color(style,1));
  S.AWrite(3,0,27,Style2Color(style,1));
  S.AWrite(5,0,18,Style2Color(style,1));
  S.AWrite(7,0,26,Style2Color(style,1));
  S.Fill(1,1,78,23,'░',Style2Color(style,1));
  /*S.AWrite(10,0,'F',Style2Color(style,1));
  S.AWrite(11,0,'1',Style2Color(style,1));

  for(int i=2; i<=10;i++){
    S.AWrite(5+5*i,0,'F',Style2Color(style,1));
    S.AWrite(5+5*i+1,0,'0'+i%10,Style2Color(style,1));
  }
  */
};
/*
static short F_arr[]={
  F2,F3,F4,F5,F6,F7,F8,F9,F10
};
*/
int _Desktop::MHandle(_Event * E){
  if(E->Type==evMouse){
    if(E->msg.M.y==0 && E->msg.M.buttons & 1){
      switch(E->msg.M.x){
        case 0:
          Q.AddMessage(evKey,ALT_X);
          E->ClearEvent(this);
          return 0;
        case 3:
          Q.AddMessage(evKey,ALT_LEFT);
          E->ClearEvent(this);
          return 0;
        case 5:
          Q.AddMessage(evKey,ALT_UP);
          E->ClearEvent(this);
          return 0;
        case 7:
          Q.AddMessage(evKey,ALT_RIGHT);
          E->ClearEvent(this);
          return 0;
        /*
        case 10:
        case 11:
          Q.AddMessage(sysHelp);
          E->ClearEvent(this);
          return 0;
       */
      }
      /*
      for(int i=2; i<=12;i++){
        if(E->msg.M.x==5+5*i || E->msg.M.x==5+5*i+1){
          Q.AddMessage(evKey,F_arr[i-2]);
          E->ClearEvent(this);
          return 0;
        }
      }
     */
    }
  }
  return _Window::MHandle(E);
};
int _Desktop::Handle(_Event * E){
  TRACE("Desktop::Handle");

  switch(E->Type){
    case evSystem:
      switch(E->msg.L){
        case 0:
          E->ClearEvent(this);
          return 1;
        default:
          break;
      };
      break;
    case evTime:
      {
      }
      break;
    case evKey:
      switch(E->msg.K){
        case ALT_X:
          ShutDown=1;
          E->ClearEvent(this);
          return 1;

        case ALT_UP:
           {
            char **names=new char* [child->total+1];
            for(int ii=0;ii < child->total; ii++)
              names[ii]=child->array[ii]->name;
            names[ii]=0;
            NextProcess=child->index+1;
            _Window *ttt=new _VMenu(30,1,names,&NextProcess);
            delete names;
            ttt->Exec();
            delete ttt;
            // показать меню активных процессов
            if(NextProcess){
              SwitchFocus(child->array[NextProcess-1]);
            }
            E->ClearEvent(this);
          }
          break;

        case ALT_RIGHT:
          if( child->total){
            if(CanSwitch())
              SwitchFocus();
            E->ClearEvent(this);
            Show();
          }
          break;

        case ALT_LEFT:
          if( child->total){
            if(CanSwitch())
              SwitchFocus((short)0);
            E->ClearEvent(this);
            Show();
          }
          break;

        default:
          break;
      }
      break;
  }

  return _Window::Handle(E);
};

int _Desktop::StoreFocus(_Window ** OldFocus){
  *OldFocus=FindFocused();
  return 0;
};

int _Desktop::RestoreFocus(_Window * OldFocus){
  _Window * tmp=OldFocus;
  while(tmp){
    tmp->GotFocus();
    tmp=tmp->parent;
  }
  return 0;
};

