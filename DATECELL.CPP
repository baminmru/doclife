/****************************************************
 *   DATECELL.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/

#define NO_TRACE
#include "common.h"
#undef NO_TRACE


void _DateCell::Commit(void){
  info->Set(0,0,0);
  info->C2D(buffer);
  if(pos>=6)
    info->D2C(buffer);
  _Window::Commit();
};

void _DateCell::Load(void){
  info->D2C(buffer);
  {
    short dx,dy;
    dx=cur_x;
    dy=cur_y;
    Coord(&dx,&dy);
    GDt->S.Goto(dx,dy);
  }
};

int _DateCell::Handle(_Event *E){
  TRACE("DateCell::Handle");
  if(!(st&stLocked)){
    // обработка событий клавиатуры
    if(E->Type==evKey){
      switch(E->msg.K){
        /*
        case DELETE:
            {
              for(short i=pos;i<w*h;i++)
                buffer[i]=buffer[i+1];
              buffer[w*h]=' ';
            }
            E->ClearEvent(this);
          break;
        */
        case SHIFT_DELETE:
            info->Set(0,0,0);
            info->D2C(buffer);
            pos=0;
            Commit();
            E->ClearEvent(this);
            break;
        /*
        case BACKSPACE:
            if(pos){
              pos--;
              buffer[pos]=' ';
            }
            E->ClearEvent(this);
          break;
        */
        case LEFT:
            if(pos) pos--;
            if(pos==2 ||pos==5)
              pos--;
            E->ClearEvent(this);
          break;
        case RIGHT:
            if(pos<w*h-1) pos++;
            if(pos==2 ||pos==5)
              pos++;
            E->ClearEvent(this);
          break;
        /*
        case DOWN:
            pos+=w;
            if(pos>w*h) pos=w*h;
          break;
        case UP:
            pos-=w;
            if(pos<0) pos=0;
          break;
        */
        case END:
            pos=w*h;
            E->ClearEvent(this);
          break;
        case HOME:
            pos=0;
            E->ClearEvent(this);
          break;

        default:
          if(E->msg.K>='0' && E->msg.K<='9'){
            if(pos<w*h)
            buffer[pos++]=E->msg.K;
            if(pos==2 ||pos==5)
            buffer[pos++]='.';
            Commit();
            E->ClearEvent(this);
          }
      }
      short ppos=pos;
      if(pos==w*h) pos--;
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

void _DateCell::Paint(void){
  TRACE("DateCell::Paint");

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

void _DateCell::Pos2XY(void){
  cur_y=pos/w;
  cur_x=pos%w;
};

_DateCell::_DateCell(short _x,short _y, short _style,_Date *_info):
  _Window(_x,_y,10,1,_style,
    stCursor|stInsert|stNoMove|stNoESC,"DateCell"),
  info(_info),
  pos(0),
  cur_x(0),cur_y(0)
  {
    TRACE("DateCell::DateCell");
    buffer = new char[w*h+1];
    StrClear(buffer,w*h);
    info->D2C(buffer);
   };

_DateCell::~_DateCell(){
  TRACE("DateCell::~DateCell");
  delete buffer;
}
