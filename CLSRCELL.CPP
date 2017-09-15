/****************************************************
 *   CLSRCELL.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#define NO_TRACE
#include "common.h"
#undef NO_TRACE


static void ShowChoice(_Event *E, void * parent){
  TRACE("ShowChoice");
  ((_Window*) parent)->LostFocus(0);
  _ClsrCell *w= (_ClsrCell*) (((_Window*)parent)->parent);
  if(w->pos)
    w->Pos2Info();
};


static void ViewCls(_Event *E, void * parent){
  TRACE("ViewCls");
  _ClsrCell *w= (_ClsrCell*) parent;
  _Window *w1;
  w->Add(w1=new _ClsView(&w->pos,w->ID,0,1,30,5,1,"ViewCls"));
  w1->AddCallBack(ShowChoice,evKey,ESC);
  w1->AddCallBack(ShowChoice,evKey,ENTER);
  w1->st&=~stBuffered;
  w->SwitchFocus(w1);
  E->ClearEvent((_Window*)parent);
};


clsctrs    _ClsrCell::r;
clsctrs_0  _ClsrCell::k;

int _ClsrCell::MHandle(_Event *E){
  if(E->Type==evMouse){
    short dx=0,dy=0;
    Coord(&dx,&dy);
    if(dx<=E->msg.M.x && E->msg.M.x<=dx+w-1 && dy<=E->msg.M.y && E->msg.M.y<=dy+h-1){
      if(E->msg.M.buttons & 1 && st & stFocused){
        GDt->Q.AddMessage(evKey,ALT_DOWN);
        E->ClearEvent(this);
        return 0;
      }
    }
  }
  return _Window::MHandle(E);
};

int _ClsrCell::Handle(_Event *E){
  TRACE("_ClsrCell::Handle");
  if(!(st&stLocked)){

    // обработка событий клавиатуры
    if(E->Type==evKey){
      switch(E->msg.K){
        case ' ':
          Next();
          Commit();
          Show(1);
          break;
      }
    }

  }
  return _Window::Handle(E);
};

void _ClsrCell::Paint(void){
  TRACE("_ClsrCell::Paint");

    short dx=0,dy=0;
    Coord(&dx,&dy);
    {
      GDt->S.AWrite(dx,dy,w,h,buffer,
                  Style2Color(style,(st&stFocused)?2:1));
    }
};

_ClsrCell::_ClsrCell(short _x,short _y, short _w,short _style,short *_info,short _ID):
  _Window(_x,_y,_w,1,_style,stNoMove|stNoESC,"_ClsrCell"),
  info(_info),
  ID(_ID),
  pos(0)
{
    TRACE("_ClsrCell::_ClsrCell");
    buffer = new char [w+1];
    T= T->St.FindTable("X$Classificator");
    //Debug("T=%p\n",T);
    if(!T) return;
    k.id=ID;
    k.item=*info;
    if(T->GGE(&r,&k,0)){
      memset(buffer,0,w);
      *info=0;
    }else{
      if(r.id==ID){
        memset(buffer,0,w);
        strncpy(buffer,r.name,w);
        *info=k.item;
      }else{
        memset(buffer,0,w);
        *info=0;
      }
    }
    AddCallBack(ViewCls,evKey,ALT_DOWN);
};

_ClsrCell::~_ClsrCell(){
  TRACE("_ClsrCell::~_ClsrCell");
  delete buffer;
}

void _ClsrCell::Next(void){
   TRACE("_ClsrCell::Next");

    if(!T) return;
    k.id=ID;
    k.item=*info;
    if(T->GEQ(&r,&k,0)){
First:
      k.id=ID;
      k.item=0;
      if(T->GGE(&r,&k,0)){
        memset(buffer,0,w);
        *info=0;
      }else{
       if(k.id==ID){
         *info=k.item;
         memset(buffer,0,w);
         strncpy(buffer,r.name,w);
       }else{
         memset(buffer,0,w);
         *info=0;
       }
      }
    }else{
      if(!T->GNext(&r,&k)){
       if(k.id==ID){
         *info=k.item;
         memset(buffer,0,w);
         strncpy(buffer,r.name,w);
       }else
        goto First;
      }else
        goto First;
    }
};

void _ClsrCell::Current(void){
   TRACE("_ClsrCell::Current");

    if(!T) return;
    k.id=ID;
    k.item=*info;
    if(T->GEQ(&r,&k,0)){
      k.id=ID;
      k.item=0;
      if(T->GGE(&r,&k,0)){
        memset(buffer,0,w);
        *info=0;
      }else{
       if(k.id==ID){
         *info=k.item;
         memset(buffer,0,w);
         strncpy(buffer,r.name,w);
       }else{
         memset(buffer,0,w);
         *info=0;
       }
      }
    }else{
      memset(buffer,0,w);
      strncpy(buffer,r.name,w);
    }
}

void _ClsrCell::Pos2Info(void){
  memset(buffer,0,w);
  *info=0;
  if(!T->GDir(&r,&pos,0)){
    *info=r.item;
  }
  Current();
  Commit();
};
void _ClsrCell::LostFocus(int l){
  if(Find("ViewCls"))
    Del(Find("ViewCls"));
  _Window::LostFocus(l);
};
