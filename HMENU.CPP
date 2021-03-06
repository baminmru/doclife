/****************************************************
 *      HMENU.CPP                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#include <string.h>
#define NO_TRACE
#include "common.h"
#undef NO_TRACE

// ��������� � �����⮬ ������⢠ ��ப
_HMenu::_HMenu(short _x, short _y,  char **_items,short * _result):
  _Window(_x,_y,0,3,ST_MENU,stBuffered)
{
  TRACE("HMenu::HMenu");
  short l=1,i;
  result=_result;
  for(i=0;_items[i];i++){
    _Static * tmp;
    Add(tmp=new _Static(l,1,ST_MENU,_items[i],0));
    if(i+1==(*result)){
      tmp->st|=stFocused;
      child->index=i;
    }
    l+=strlen(_items[i]);
    l++;
  }
  w=l;
};

_HMenu::~_HMenu(void){
  TRACE("HMenu::~HMenu");
};

int _HMenu::Handle(_Event *E){
  TRACE("HMenu::Handle");
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


