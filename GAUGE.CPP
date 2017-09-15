/****************************************************
 *      GAUGE.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"

void _Gauge::SetRange(long NewRange){
  Range = NewRange;
  Show(1);
};

extern void (*ModemFunc)(void);
static long prev=0;


void _Gauge::Update(long _info){
  if(info!=_info){
    info=_info;
    Show(1);
  }
  if(prev!=clock()){
    prev=clock();
    if(ModemFunc)
      ModemFunc();
  }
};

void _Gauge::Halt(void){
  GDt->Q.AddMessage(sysGaugeStop);
};

#define RANGE 10
static char RangeStr[RANGE]={
   29,    //'-'      ,
   9,     //'\\'     ,
   '+',   //'|'      ,
   '*',   //'/'      ,
   15,    //'-'      ,
   2,     //'\\'     ,
   '░',   //'|'      ,
   '▒',   //'/'      ,
   '▓',   //'▌'      ,
   '█'    //'█'
};

void _Gauge::Paint(void){
  _Window::Paint();
  if(info>Range)
    Range = info;
  short ax=1,ay=1;
  Coord(&ax,&ay);
  GDt->S.Fill(ax,ay,w-2,1,'',Style2Color(style,1));
  long r,c;
  if(!Range)
   Range =1;
  c = RANGE * (w-2) * info / Range;

  GDt->S.Fill(ax,ay,c/RANGE,1,RangeStr[RANGE-1],Style2Color(style,1));
  if((Range >= (w-2)*RANGE) && (r=c%RANGE))
    GDt->S.AWrite(ax+c/RANGE,ay,RangeStr[r-1],Style2Color(style,1));
};

_Gauge::_Gauge(short _x, short _y, short _w, short _style,  long _Range , char *_name):
  _Window( _x,_y,_w+2,3,_style, stBuffered | stNoMove | stNoESC, _name)
{
  Range = _Range;
  info = 0;
};
_Gauge::~_Gauge(){};

int _Gauge::Handle(_Event *E){
  if(E->Type == evSystem ){
    switch( E->msg.L){
      case sysGaugeUpdate:
         Show(1);
         break;
      case sysGaugeStop:
         E->Type = evKey;
         E->msg.K = ESC;
         return _Window::Handle(E);
    }

  }
  return 0;
};
