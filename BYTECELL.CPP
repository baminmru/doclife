/****************************************************
 *   BYTECELL.CPP                                   *
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

void _ByteCell::Commit(void){
  *info=0;
  int ttt;
  sscanf(buffer+sign,"%d",&ttt);
  if(sign)
    *info=ttt;
  else
    *(unsigned char*)info = *((unsigned int *)(&ttt));
  if(sign && buffer[0]=='-') {
    (*info)*=-1;
  }
  _Window::Commit();
}

int _ByteCell::Handle(_Event *E){
  TRACE("DigCell::Handle");
  if(!(st&stLocked)){

    // обработка событий клавиатуры
    if(E->Type==evKey){
      switch(E->msg.K){
        case BACKSPACE:
            if(pos>sign){
              pos--;
              Commit();
            }else
              break;
        case DELETE:
            if(pos>=sign){
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
          if(E->msg.K>='0' && E->msg.K<='9'){
            if(pos<digits+sign)
              buffer[pos++]=E->msg.K;
            Commit();
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
    //
  }
  return _Window::Handle(E);
};

void _ByteCell::Pos2XY(void){
  cur_y=pos/w;
  cur_x=pos%w;
};

void _ByteCell::Load(void){
  StrClear(buffer,digits+sign);
  int l;
  if(sign){
   l=*info;
   sprintf(buffer,"%+d",l);
  }else{
   l=*(unsigned char*)info;
   sprintf(buffer,"%u",l);
  }
  {
    short dx,dy;
    dx=cur_x;
    dy=cur_y;
    Coord(&dx,&dy);
    GDt->S.Goto(dx,dy);
  }
};

void _ByteCell::Paint(void){
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

_ByteCell::_ByteCell(short _x,short _y, short _style,short _digits, char *_info, short _sign):
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
  int ttt;
  if(sign){
    ttt=*info;
    sprintf(buffer,"%+d",ttt);
  }else{
    ttt=*(unsigned char*)info;
    sprintf(buffer,"%u",ttt);
  }
};

_ByteCell::~_ByteCell(void){
  delete buffer;
};


