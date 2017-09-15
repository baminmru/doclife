/****************************************************
 *      VMENU.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include <string.h>
#define NO_TRACE
#include "common.h"
#undef NO_TRACE

// конструктор с количеством строк
_VMenu::_VMenu(short _x, short _y, short _cnt, char **_items, short *_result):
  _Window(_x,_y,0,_cnt+2,ST_VMENU,stBuffered,"CntVMenu")
{
  TRACE("VMenu::VMenu (cnt)");
  result=_result;
  short l=0,i;
  for(i=0;i<_cnt;i++){
    if(l<strlen(_items[i]))
      l=strlen(_items[i]);
  }
  char *cc = new char [l+1];
  for(i=0;i<_cnt;i++){
    StrClear(cc,l);
    strncpy(cc,_items[i],strlen(_items[i]));
    _Static *tmp;
    Add(tmp=new _Static(1,i+1,ST_VMENU,cc,0));
    if(i+1==(*_result)){
      tmp->st|=stFocused;
      child->index=i;
    }
  }
  w=l+2;
  delete cc;
};

// конструктор с подсчетом количества строк
_VMenu::_VMenu(short _x, short _y, char **_items,short * _result):
  _Window(_x,_y,0,0,ST_VMENU,stBuffered,"VMenu")
{
  TRACE("VMenu::VMenu");
  short cnt,l=0,i;
  result=_result;
  for(i=0;_items[i];i++){
    if(l<strlen(_items[i]))
      l=strlen(_items[i]);
  }
  cnt=i;
  char *cc = new char [l+1];
  for(i=0;i<cnt;i++){
    StrClear(cc,l);
    strncpy(cc,_items[i],strlen(_items[i]));
    _Static *tmp;
    Add(tmp=new _Static(1,i+1,ST_VMENU,cc,0));
    if(i+1==(*_result)){
       tmp->st|=stFocused;
       child->index=i;
    }
  }
  w=l+2;
  h=cnt+2;
  delete cc;
};

_VMenu::~_VMenu(void){
  TRACE("VMenu::~VMenu");
};

int _VMenu::Handle(_Event *E){
  TRACE("VMenu::Handle");
  short flag=0;
  if(E->Type==evKey){
    switch(E->msg.K){
      case ESC:
        *result=0;
        flag=1;
        break;
      case ENTER:
        {
          *result=child->index+1;
          flag=1;
          break;
        }
    }
  }
  /*
  if(flag && parent==D)
   st=st&(~stFocused);
  */
  return _Window::Handle(E);
};


