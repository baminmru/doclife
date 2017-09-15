/****************************************************
 *   LDBLCELL.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#define NO_TRACE
#include "common.h"
#undef NO_TRACE

int _LDblCell::Handle(_Event *E){
  TRACE("LDblCell::Handle");
  if(!(st&stLocked)){

    // обработка событий клавиатуры
    if(E->Type==evKey){
      switch(E->msg.K){
        case BACKSPACE:
            if(pos>sign) pos--;
            else
              break;
        case DELETE:
            if(pos>=sign){
              if(buffer[pos]=='.') pointed=0;

              for(short i=pos;i<digits;i++)
                buffer[i]=buffer[i+1];
              buffer[digits]=' ';
              Commit();
            }
            E->ClearEvent(this);
          break;

        case SHIFT_DELETE:
            if(sign){
              buffer[0]='+';
            }
            StrClear(buffer+sign,digits);
            buffer[sign]='0';
            pointed=0;
            pos=sign;
            Commit();
            E->ClearEvent(this);
            break;

        case LEFT:
            if(pos>sign) pos--;
            E->ClearEvent(this);
          break;
        case RIGHT:
            if(pos<digits+sign) pos++;
            E->ClearEvent(this);
          break;
        case END:
            pos=digits+sign;
            E->ClearEvent(this);
          break;
        case HOME:
            pos=sign;
            E->ClearEvent(this);
          break;
        case PLUS:
            if(sign){
              buffer[0]='+';
              Commit();
              E->ClearEvent(this);
            }
          break;

        case MINUS:
            if(sign){
              buffer[0]='-';
              Commit();
              E->ClearEvent(this);
            }
          break;

        default:
          if(E->msg.K>='0' && E->msg.K<='9' || E->msg.K=='.'){
            if(pos<digits+sign){
              if(E->msg.K=='.'){
               if(!pointed)
                  buffer[pos++]=E->msg.K;
                pointed=1;
              }else{
                if(buffer[pos]=='.') pointed=0;
                buffer[pos++]=E->msg.K;
              }
              Commit();
            }
            E->ClearEvent(this);
          }
      }
      short ppos=pos;
      if(pos==digits+sign) pos--;
      Pos2XY();
      pos=ppos;
      short dx,dy;
      dx=cur_x;
      dy=cur_y;
      Coord(&dx,&dy);
      GDt->S.Goto(dx,dy);
      Show(1);
    }

  }
  return _Window::Handle(E);
};
void _LDblCell::Commit(void){
  *info=0.0;
  sscanf(buffer+sign,"%Lf",info);
  if(sign && buffer[0]=='-') {
    (*info)*=-1;
  }
  _Window::Commit();
};

void _LDblCell::Pos2XY(void){
  cur_y=pos/w;
  cur_x=pos%w;
};

void _LDblCell::Load(void){
  StrClear(buffer,w);
  char tmp[31];
  StrClear(tmp,30);
  sprintf(tmp,proto,*info);
  StrTrim(tmp);
  if(*info>=0.0){
    buffer[0]='+';
    strncpy(buffer+sign,tmp,w-1);
  }else{
    buffer[0]='-';
    strncpy(buffer+sign,tmp+1,w-1);
  }
  {
    short dx,dy;
    dx=cur_x;
    dy=cur_y;
    Coord(&dx,&dy);
    GDt->S.Goto(dx,dy);
  }
};

void _LDblCell::Paint(void){
  TRACE("LDblCell::Paint");

    short dx=0,dy=0;
    Coord(&dx,&dy);
    {
      GDt->S.AWrite(dx,dy,w,h,buffer,
                  Style2Color(style,(st&stFocused)?2:1));
      if(st&stFocused){
        short ppos=pos;
        if(pos==w*h) pos--;
        Pos2XY();
        pos=ppos;
        dx=cur_x;
        dy=cur_y;
        Coord(&dx,&dy);
        GDt->S.Goto(dx,dy);
      }
    }
};

_LDblCell::_LDblCell(short _x,short _y,short _style, short _digits, long double *_info, short pts):
  _Window(_x,_y,_digits+1,1,_style,
       stCursor|stInsert|stNoMove|stNoESC,"LDblCell")
{
  buffer = new char[_digits+2];
  sprintf(proto,"%%%d.%dLf",_digits-1,pts);
  StrClear(buffer,w);
  pointed=0;
  info=_info;
  digits=_digits;
  sign=1;
  cur_x=cur_y=0;
  pos=sign;
  char tmp[31];
  StrClear(tmp,30);
  sprintf(tmp,proto,*info);
  StrTrim(tmp);
  if(*info>=0.0){
    buffer[0]='+';
    strncpy(buffer+sign,tmp,w-1);
  }else{
    buffer[0]='-';
    strncpy(buffer+sign,tmp+1,w-1);
  }

  if(strchr(buffer,'.')) pointed=1;
};

_LDblCell::~_LDblCell(void){
  delete buffer;
};


