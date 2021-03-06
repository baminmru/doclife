/****************************************************
 *     YNCELL.CPP                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#define NO_TRACE
#include "common.h"
#undef NO_TRACE

int _YNCell::MHandle(_Event *E){
  if(E->Type==evMouse){
    short dx=0,dy=0;
    Coord(&dx,&dy);
    if(dx<=E->msg.M.x && E->msg.M.x<=dx+w-1 && dy<=E->msg.M.y && E->msg.M.y<=dy+h-1){
      if(E->msg.M.buttons & 1 && st & stFocused){
        GDt->Q.AddMessage(evKey,' ');
        E->ClearEvent(this);
        return 0;
      }
    }
  }
  return _Window::MHandle(E);
};
int _YNCell::Handle(_Event *E){
  TRACE("_YNCell::Handle");
  if(!(st&stLocked)){
    // ��ࠡ�⪠ ᮡ�⨩ ����������
    if(E->Type==evKey){
      switch(E->msg.K){
        case ' ':
        *info=1-*info;
        if(*info)
          strcpy(buffer,"�� ");
        else
          strcpy(buffer,"���");
        Commit();
      }
      Show(1);
    }

  }
  return _Window::Handle(E);
};

void _YNCell::Paint(void){
  TRACE("_YNCell::Paint");

    short dx=0,dy=0;
    Coord(&dx,&dy);
    {
      GDt->S.AWrite(dx,dy,w,h,buffer,
                  Style2Color(style,(st&stFocused)?2:1));
    }
};

_YNCell::_YNCell(short _x,short _y, short _style,unsigned char *_info):
  _Window(_x,_y,3,1,_style,stNoMove|stNoESC,"_YNCell"),
  info(_info)
{
    TRACE("_YNCell::_YNCell");
    if(*info)
      strcpy(buffer,"�� ");
    else
      strcpy(buffer,"���");
};

_YNCell::~_YNCell(){
  TRACE("_YNCell::~_YNCell");
}
