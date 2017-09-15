/****************************************************
 *   PASSWORD.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#define NO_TRACE
#include "common.h"
#undef NO_TRACE

#include "password.h"

void _Password::Paint(void){
    if(InsMode)
      st=st|stInsert;
    else
      st=st & ~stInsert;

    short dx=0,dy=0;
    Coord(&dx,&dy);

    for(int i=w; i>0;i--){
      if(buffer[i-1]>' ')
        break;
    }
    GDt->S.Fill(dx,dy,w  ,1,' ',Style2Color(style,(st&stFocused)?2:1));
    GDt->S.Fill(dx,dy,i  ,1,'*',Style2Color(style,(st&stFocused)?2:1));
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
};

