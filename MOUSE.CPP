/****************************************************
 *      MOUSE.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include <bios.h>
#include "common.h"

#define MouseQueueMax 100

unsigned char  _Mouse::buttonCount = 0;
short _Mouse::handlerInstalled = 0;
MouseEventType *_Mouse::Q;
short _Mouse::Top,_Mouse::Bottom,_Mouse::Total;

_Mouse::_Mouse()
{
    resume();
    Q=0;
    Top = -1;
    Total = 0;
    Bottom =0;

    if(present()){
      Q = new MouseEventType[MouseQueueMax];
      registerHandler(0xffff,MouseInt);
    }
}

void _Mouse::resume()
{
    if( getvect( 0x33 ) == 0 )
        return;

    _AX = 0;
    geninterrupt( 0x33 );

    if( _AX == 0 )
        return;
    buttonCount = _BL;

    _AX = 4;
    _CX = 0;
    _DX = 0;
    geninterrupt( 0x33 );
}


void _Mouse::MoveTo(unsigned short x, unsigned short y){
    _AX = 4;
    _CX = x << 3;
    _DX = y << 3;
    geninterrupt( 0x33 );
}

_Mouse::~_Mouse()
{
    suspend();
    if(Q)
     delete Q;
}

void _Mouse::suspend()
{
    if( !present()  )
        return;
    hide();
    if( handlerInstalled )
        {
          registerHandler( 0, 0 );
          handlerInstalled = 0;
        }
    buttonCount = 0;
}

#pragma warn -asc

void _Mouse::show()
{
    asm push ax;
    asm push es;

    if( present() )
        {
        _AX = 1;
        geninterrupt( 0x33 );
        }

    asm pop es;
    asm pop ax;
}

void _Mouse::hide()
{
    asm push ax;
    asm push es;

    if( buttonCount != 0 )
        {
        _AX = 2;
        geninterrupt( 0x33 );
        }

    asm pop es;
    asm pop ax;
}

#pragma warn .asc

void _Mouse::setRange( unsigned short rx, unsigned short ry )
{
    if( buttonCount != 0 )
        {
        _DX = rx;
        _DX <<= 3;
        _CX = 0;
        _AX = 7;
        geninterrupt( 0x33 );

        _DX = ry;
        _DX <<= 3;
        _CX = 0;
        _AX = 8;
        geninterrupt( 0x33 );
        }
}

void _Mouse::getEvent( MouseEventType& me )
{
    _AX = 3;
    geninterrupt( 0x33 );
    _AL = _BL;
    me.buttons = _AL & 3;
    me.x = _CX >> 3;
    me.y = _DX >> 3;
}

void _Mouse::Shift(short x, short y){
  short x1, y1;
  _AX = 3;
  geninterrupt( 0x33 );
  x1 = _CX;
  y1 = _DX;

  if(x*8+x1 <0)
    x1=0;
  else
    x1 =x1+x*8;

  if(y*8+y1 <0)
    y1=0;
  else
    y1 =y1+y*8;
  _AX=4 ;
  _CX=x1;
  _DX=y1;
  geninterrupt( 0x33 );
}

int _Mouse::getFromQ( MouseEventType& me )
{
  if(Total){
    me=Q[Bottom++];
    Bottom=Bottom%MouseQueueMax;
    Total--;
    return 1;
  }
  return 0;
}

void _Mouse::registerHandler( unsigned mask, void (far *func)() )
{
    if( !present() )
        return;

    _AX = 12;
    _CX = mask;
    _DX = FP_OFF( func );
    _ES = FP_SEG( func );

    geninterrupt( 0x33 );
    handlerInstalled = 1;
}


void huge _Mouse::MouseInt()
{
    if(!Q)
      return;
    unsigned flag = _AX;
    MouseEventType tempMouse;
    tempMouse.buttons = _BL & 3;
    tempMouse.x       = _CX >> 3;
    tempMouse.y       = _DX >> 3;

    unsigned char shift = bioskey(0x12)&0x0f;

    if(shift ){
      if(shift&3)
        shift|=3;
      shift&=0x0e;
      shift=shift<<4;

      tempMouse.buttons=tempMouse.buttons<<3;
      tempMouse.buttons &=0x18;
      tempMouse.buttons |=shift;
    }

    if( (flag & 0x1e) != 0 )
    {
        if(Top < 0 || memcmp(&(Q[Top]),&tempMouse,sizeof(tempMouse))){
          Top++;
          Top=Top%MouseQueueMax;
          Q[Top] = tempMouse;
          if(Total<MouseQueueMax)
            Total++;
          else{
            Bottom++;
            Bottom=Bottom%MouseQueueMax;
          }
        }
    }

}

