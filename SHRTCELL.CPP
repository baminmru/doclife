/****************************************************
 *   SHRTCELL.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include <stdio.h>
#include <string.h>

#define NO_TRACE
#include "common.h"
#undef NO_TRACE

int _ShortCell::Handle(_Event *E){
  TRACE("DigCell::Handle");
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
              for(short i=pos;i<digits;i++)
                buffer[i]=buffer[i+1];
              buffer[digits]=' ';
            }
            E->ClearEvent(this);
          break;

        case SHIFT_DELETE:
            if(sign){
              buffer[0]='+';
            }
            StrClear(buffer+sign,digits);
            buffer[sign]='0';
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
              E->ClearEvent(this);
            }
          break;

        case MINUS:
            if(sign){
              buffer[0]='-';
              E->ClearEvent(this);
            }
          break;

        default:
          if(E->msg.K>='0' && E->msg.K<='9'){
            if(pos<digits+sign)
              buffer[pos++]=E->msg.K;
            E->ClearEvent(this);
          }
      }
      if(E->Type==0){
        *info=0;
        if(sign)
          sscanf(buffer+sign,"%d",info);
        else
          sscanf(buffer,"%u", info);
        if(sign && buffer[0]=='-') {
          (*info)*=-1;
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
void _ShortCell::Commit(void){
  *info=0;
  if(sign)
    sscanf(buffer+sign,"%d",info);
  else
    sscanf(buffer,"%u", info);
  if(sign && buffer[0]=='-') {
    (*info)*=-1;
  }
  _Window::Commit();
};


void _ShortCell::Pos2XY(void){
  cur_y=pos/w;
  cur_x=pos%w;
};

void _ShortCell::Load(void){
  int l=*info;
  StrClear(buffer,digits+sign);
  if(sign)
    sprintf(buffer,"%+d",l);
  else
    sprintf(buffer,"%u",l);
  {
    short dx,dy;
    dx=cur_x;
    dy=cur_y;
    Coord(&dx,&dy);
    GDt->S.Goto(dx,dy);
  }
};

void _ShortCell::Paint(void){
  TRACE("DigCell::Paint");

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

_ShortCell::_ShortCell(short _x,short _y, short _style,short _digits, short *_info, short _sign):
  _Window(_x,_y,_digits+_sign,1,_style,
    stCursor|stInsert|stNoMove|stNoESC,"DigCell")
{
  buffer = new char[_digits+_sign+1];
  StrClear(buffer,_digits+_sign);
  info=_info;
  digits=_digits;
  sign=_sign;
  cur_x=cur_y=0;
  pos=sign;
  char tmp[11];
  if(sign)
    sprintf(tmp,"%+d",*info);
  else
    sprintf(tmp,"%u",*info);
  strncpy(buffer,tmp,_digits+sign);
};

_ShortCell::~_ShortCell(void){
  delete buffer;
};


