/****************************************************
 *       CELL.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#define NO_TRACE
#include "common.h"
#undef NO_TRACE


void _Cell::Load(void){
  memcpy(buffer,info,w*h);
  {
    short dx,dy;
    dx=cur_x;
    dy=cur_y;
    Coord(&dx,&dy);
    GDt->S.Goto(dx,dy);
  }
};

int _Cell::Handle(_Event *E){
  TRACE("Cell::Handle");
  if(!(st&stLocked)){
    // обработка событий клавиатуры
    if(E->Type==evKey){
      switch(E->msg.K){
        case INSERT:
          InsMode=1-InsMode;
          break;
        case DELETE:
            {
              for(short i=pos;i<w*h-1;i++)
                buffer[i]=buffer[i+1];
              buffer[w*h-1]=' ';
            }
            Commit();
            E->ClearEvent(this);
          break;
        case SHIFT_DELETE:
            StrClear(buffer,w*h);
            pos=0;
            Commit();
            E->ClearEvent(this);
            break;
        case BACKSPACE:
            if(pos){
              pos--;
              buffer[pos]=' ';
            }
            Commit();
            E->ClearEvent(this);
          break;
        case LEFT:
            if(pos) pos--;
            E->ClearEvent(this);
          break;
        case RIGHT:
            if(pos<w*h) pos++;
            E->ClearEvent(this);
          break;
        case DOWN:
            if(pos+w<w*h){
              pos+=w;
//              if(pos>=w*h) pos=w*h;
              E->ClearEvent(this);
            }
          break;
        case UP:
            if(pos){
              pos-=w;
              if(pos<0) pos=0;
              E->ClearEvent(this);
            }
          break;

        case END:
            pos=w*h;
            E->ClearEvent(this);
          break;
        case HOME:
            pos=0;
            E->ClearEvent(this);
          break;

        default:
          if(E->msg.K>=' ' && E->msg.K<=255){
            if(pos<w*h){
              short i;
              if(InsMode){
                for(i=w*h-1;i>pos;i--)
                  buffer[i]=buffer[i-1];
              }
              buffer[pos++]=E->msg.K;
            }
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

  }
  return _Window::Handle(E);
};

void _Cell::Paint(void){
  TRACE("Cell::Paint");
    if(InsMode)
      st=st|stInsert;
    else
      st=st & ~stInsert;

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

void _Cell::Pos2XY(void){
  cur_y=pos/w;
  cur_x=pos%w;
};


void _Cell::Commit(void){
  memcpy(info,buffer,w*h);
  memcpy(buffer,info,w*h);
  _Window::Commit();
};

_Cell::_Cell(short _x,short _y,short _w,short _h, short _style,char *_info):
  _Window(_x,_y,_w,_h,_style,stCursor|stNoMove|stNoESC,"Cell"),
  info(_info),
  pos(0),
  cur_x(0),cur_y(0),InsMode(1)
  {
    TRACE("Cell::Cell");
    buffer = new char[w*h+1];
    StrClear(buffer,w*h);
    int i=strlen((char*)info);
    if(i>w*h) i=w*h;
    strncpy(buffer,(char*)info,i);
   };

_Cell::~_Cell(){
  TRACE("Cell::~Cell");
  delete buffer;
};
_Static::_Static(short _x,short _y, short _style,char *info, short locked):
  _Window(_x,_y,strlen(info),1,_style,0,"Static")
{
  TRACE("Static::Static");
  if(locked){
    st =  stLocked |  stNoMove;
  }else{
    st =  stNoMove;
  }
  buffer=new char [strlen(info)+1];
  strcpy(buffer,info);
};

_Static::~_Static(void){
  delete buffer;
};

void _Static::Paint(void){
  TRACE("Static::Paint");

  short dx=0,dy=0;
  Coord(&dx,&dy);
  {
    GDt->S.AWrite(dx,dy,w,h,buffer,
                Style2Color(style,(st&stFocused)?2:1));
  }
};

int _Static::MHandle(_Event *E){
  if(E->Type==evMouse && !(st&stLocked)){
    short dx=0,dy=0;
    Coord(&dx,&dy);
    if(dx<=E->msg.M.x && E->msg.M.x<=dx+w-1 && dy<=E->msg.M.y && E->msg.M.y<=dy+h-1){
      if(E->msg.M.buttons & 1 && st & stFocused){
        GDt->Q.AddMessage(evKey,ENTER);
        E->ClearEvent(this);
        return 0;
      }
    }
  }
  return _Window::MHandle(E);
};
int _Static::Handle(_Event *E){
  if(E->Type==evKey){
    switch(E->msg.K){
      case DOWN:
      case RIGHT:
        E->msg.K=TAB;
        break;
      case UP:
      case LEFT:
        E->msg.K =SHIFT_TAB;
        break;

      case ENTER:
      case ESC:
        return parent->Handle(E);

    }

  }
  return _Window::Handle(E);
};

_ColorCell::_ColorCell(int x,int y,int _color,char *info,short locked):
       _Static(x,y,0,info,locked),color(_color){};

_ColorCell::~_ColorCell(){};
void _ColorCell::Paint(void){
      short dx=0,dy=0;
      Coord(&dx,&dy);
      GDt->S.AWrite(dx,dy,w,h,buffer,color);
};

