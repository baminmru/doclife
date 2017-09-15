/****************************************************
 *     MESWIN.CPP                                   *
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

_MesWin::_MesWin(short _x, short _y, short _style, short *_result,
        char *l0,  char *l1,  char *l2,  char *l3,  char *l4,
        char *l5,  char *l6,  char *l7,  char *l8,  char *l9,
        char *l10, char *l11, char *l12, char *l13, char *l14,
        char *l15, char *l16, char *l17, char *l18, char *l19
 ):
  _Window(_x,_y,0,0,_style,stBuffered | stFrame,"Messae Window" )
{
  TRACE("MesWin::MesWin");
  short i;
  l[0]=l0;
  l[1]=l1;
  l[2]=l2;
  l[3]=l3;
  l[4]=l4;
  l[5]=l5;
  l[6]=l6;
  l[7]=l7;
  l[8]=l8;
  l[9]=l9;
  l[10]=l10;
  l[11]=l11;
  l[12]=l12;
  l[13]=l13;
  l[14]=l14;
  l[15]=l15;
  l[16]=l16;
  l[17]=l17;
  l[18]=l18;
  l[19]=l19;
  for(i=0;i<20;i++){
    if(l[i]){
      Add(new _Static(1,i+1,_style,l[i]));
    }
  }
  result=_result;
};

_MesWin::~_MesWin(void){
  TRACE("MesWin::~MesWin");
};

int _MesWin::Handle(_Event *E){
  TRACE("MesWin::Handle");
  short flag=0;
  if(E->Type==evKey){
    switch(E->msg.K){
      case ALT_S:
        E->ClearEvent(this);
        return 0;
      case ESC:
        *result=0;
        flag=1;
        break;
      case ENTER:
        {
          *result=1;
          flag=1;

          break;
        }
    }
  }
  return _Window::Handle(E);
};


short MessageBox(
        char *l0, char *l1,   char *l2,   char *l3,    char *l4,
        char *l5, char *l6,   char *l7,   char *l8,    char *l9,
        char *l10, char *l11, char *l12, char *l13, char *l14,
        char *l15, char *l16, char *l17, char *l18, char *l19

 ){
  short result;
   _MesWin * MW = new _MesWin(0,0,ST_MESWIN,&result,l0 ,l1 ,l2 ,l3 ,l4 ,l5 ,l6 ,l7 ,l8 ,l9 ,
                                                    l10,l11,l12,l13,l14,l15,l16,l17,l18,l19);
   MW->CenterScr();
   MW->Exec();
   delete MW;
   return result;
 }




