/****************************************************
 *     SCREEN.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include <string.h>
#include <dos.h>


#define NO_TRACE
#include "common.h"
#undef NO_TRACE
void _Screen::Lock     (void){
  TRACE("Screen::Lock");
  if(Buf){
    if(_Mouse::present()) _Mouse::hide();
    memcpy(Buf,Video,(Col+1)*(Raw+1)*2);
    if(_Mouse::present()) _Mouse::show();
  }
};

void _Screen::Paste    (void){
  TRACE("Screen::Paste");
  if(Buf){
    if(_Mouse::present()) _Mouse::hide();
    memcpy(Video,Buf,(Col+1)*(Raw+1)*2);
    if(_Mouse::present()) _Mouse::show();
  }
};

void _Screen::Get      (short x, short y, short w, short h,char *buf){
  TRACE("Screen::Get");
  if(!Buf) return;
  int i,j;
  int xx=x;
  int yy=y;
  int ptr,ptr2=0;
  int wh2 =w *h*2;
  if(w<=0 || h<=0)
    return;

  for(j=yy; j<yy+h && j<=Col ;j++){
    ptr=j*(Raw+1)+xx<<1;
    for(i=xx; i<xx+w && i<=Raw ;i++){
      if(ptr >=0 && ptr < 3999 && ptr2 < wh2){
        buf[ptr2++]=Buf[ptr++];
        buf[ptr2++]=Buf[ptr++];
      }
    }
  }

};
void _Screen::Put      (short x, short y, short w, short h,char *buf){
  TRACE("Screen::Put");
  if(!Buf) return;
  int i,j;
  int xx=x;
  int yy=y;
  int ptr,ptr2=0,wh2 = w*h*2;
  if(w<=0 || h<=0)
    return;

  for(j=yy; j<yy+h && j<=Col ;j++){
    ptr=j*(Raw+1)+xx<<1;
    for(i=xx; i<xx+w && i<=Raw ;i++){
      if(ptr >=0 && ptr < 3999 && ptr2 <wh2 ){
        Buf[ptr++] =buf[ptr2++];
        Buf[ptr++] =buf[ptr2++];
      }
    }
  }

};


void _Screen::Fill     (short x, short y, short w, short h, char ch, char attr){
  TRACE("Screen::Fill");
  if(!Buf) return;
  int i,j;
  int xx=x;
  int yy=y;
  int ptr;
  if(w<=0 || h<=0)
    return;
  for(j=yy; j<yy+h && j<=Col ;j++){
    ptr=j*(Raw+1)+xx<<1;
    for(i=xx; i<xx+w && i<=Raw ;i++){
      if(ptr >=0 && ptr < 3999){
        Buf[ptr++]=ch;
        Buf[ptr++]=attr;
      }
    }
  }

};

void _Screen::ScrollUp (short x, short y, short w, short h, short lines,char attr){
  TRACE("Screen::ScrollUp");
  if(!Buf) return;
  int xx=x;
  int yy=y;
  if(w<=0 || h<=0)
    return;
  if(h>lines){
    /*******************************
     x,y           x,y
      1111111       4444444
      2222222  -->  5555555
      3333333 h     ....... h  |
      4444444       .......    | l
      5555555       .......    |
        w             w
     *******************************/
    Move(xx,yy+lines,w,h-lines,xx,yy);
    Fill(xx,yy+h-lines,w,lines,' ',attr);
  }else{
    Fill(xx,yy,w,h,' ',attr);
  }
};
void _Screen::ScrollDn (short x, short y, short w, short h, short lines,char attr){
  TRACE("Screen::ScrollDn");
  if(!Buf) return;
  int xx=x;
  int yy=y;
  if(w<=0 || h<=0)
    return;
  if(h>lines){
    /*******************************
     x,y           x,y
      1111111       .......    |
      2222222  -->  .......    | l
      3333333 h     ....... h  |
      4444444       1111111
      5555555       2222222
        w             w
     *******************************/
    Move(xx,yy,w,h-lines,xx,yy+lines);
    Fill(xx,yy,w,lines,' ',attr);
  }else{
    Fill(xx,yy,w,h,' ',attr);
  }
};

void _Screen::Write    (short x, short y, short w, short h, char *buf){
  TRACE("Screen::Write");
  if(!Buf) return;
  Put(x,y,w,h,buf);
};
void _Screen::AWrite   (short x, short y, short w, short h, char *buf, char attr){
  TRACE("Screen::AWrite");
  if(!Buf) return;
  int i,j;
  int xx=x;
  int yy=y;
  int ptr,ptr2=0;
  int wh2 =w*h*2;
  if(w<=0 || h<=0)
    return;

  for(j=yy; j<yy+h && j<=Col ;j++){
    ptr=j*(Raw+1)+xx<<1;
    for(i=xx; i<xx+w && i<=Raw ;i++){
      if(ptr >=0 && ptr<3999 && ptr2 < wh2){
        Buf[ptr++] =buf[ptr2++];
        Buf[ptr++] =attr;
      }
    }
  }

};
void _Screen::Write    (short x, short y, short w,  char *buf){
  TRACE("Screen::Write2");
  if(!Buf) return;
  Write(x,y,w,1,buf);
};
void _Screen::AWrite   (short x, short y, short w,  char *buf, char attr){
  TRACE("Screen::AWrite2");
  if(!Buf) return;
  AWrite(x,y,w,1,buf,attr);
};
void _Screen::AWrite   (short x, short y, char ch, char attr){
  TRACE("Screen::AWrite3");
  if(!Buf) return;
  AWrite(x,y,1,1,&ch,attr);
};
void _Screen::Write   (short x, short y, char ch){
  TRACE("Screen::Write3");
  if(!Buf) return;
  if(x<0 || x > 79) return;
  if(y<0 || y > 24) return;
  Buf[y*(Raw+1)+x<<1]=ch;
};

static char borders[4][9]={
  "╔═╗║╝═╚║",
  "┌─┐│┘─└│",
  "████████",
  "+-+|+-+|"
};

void _Screen::Bar (short x1, short y1, short w, short h, short btype, char attr){
  TRACE("Screen::Bar");
  if(!Buf) return;
  int x=x1,y=y1;
  if(x<=0) x=0;
  if(y<=0) y=0;
  int fill;
  if(btype < 0) {
    fill=0;
    btype =-btype;
  }else{
    fill=1;
  }
  int type=btype-1;
  if(btype){
    if( btype<4){
      // заливка вертикальными символами
      Fill(x,y,1,h,borders[type][3],attr);
      Fill(x+w-1,y,1,h,borders[type][3],attr);

      // угол 1
      if(x>=0 && x<=Raw && y>=0 && y<=Col)
        Write(x,   y,   borders[type][0]);

      // угол 2
      if(x+w-1>=0 && x+w-1<=Raw && y>=0 && y<=Col)
        Write(x+w-1, y,   borders[type][2]);

      // угол 3
      if(x+w-1>=0 && x+w-1<=Raw && y+h-1>=0 && y+h-1<=Col)
        Write(x+w-1, y+h-1, borders[type][4]);

      // угол 4
      if(x>=0 && x<=Raw && y+h-1>=0 && y+h-1<=Col)
        Write(x,   y+h-1, borders[type][6]);

      // верхняя черта
      Fill(x+1,y  ,w-2,1,borders[type][1],attr);

      // нижняя черта
      Fill(x+1,y+h-1,w-2,1,borders[type][5],attr);
    }else{
      Fill(x,y,w,h,type,attr);
    }
    // заливка пробелами
    if(fill)
      Fill(x+1,y+1,w-2,h-2,' ',attr);
  }else{
    Fill(x,y,w,h,' ',attr);
  }
};

_Screen::_Screen(int _Raw, int _Col) : Col(_Col),Raw(_Raw){
 TRACE("Screen::Screen");
 Buf=Clone=0;
 Buf  = new char[(Col)*(Raw)*2];
 Clone= new char[(Col)*(Raw)*2];
 Video=(char*)MK_FP(0xb800,0);
 memset(Buf,0,(Col)*(Raw)*2);
 Col--;
 Raw--;
 X=Y=Mode=0;
 asm{
  mov ah,0
  mov al,3
  int 0x10
  //mov ah,5
  //mov al,0
  //int 0x10
 }


};

_Screen::~_Screen(){
 TRACE("Screen::~Screen");
 if(!Buf) return;
 ClearScr(0x07);
 Paste();
 Goto(0,0);
 SetCursor(2);
 delete Buf;
 delete Clone;
 Clone=Buf=0;
};

void _Screen::Putc     (char ch){
  TRACE("Screen::Putc");
  if(!Buf) return;
  if(_Mouse::present()) _Mouse::hide();
  Buf[Y*(Raw+1)+X<<1]=ch;
  Video[Y*(Raw+1)+X<<1]=ch;
  if(_Mouse::present()) _Mouse::show();
  X++;
  if(X>Raw){
    Y++;X=0;
    if(Y>Col)
      Y=0;
  }
  Goto(X,Y);
};

void _Screen::Putc(unsigned char x, unsigned char y, char ch){
  TRACE("Screen::Putc");
  if(!Buf) return;
  Goto(x,y);
  Putc(ch);
};

void _Screen::Goto     (unsigned char x, unsigned char y){
  TRACE("Screen::Goto");
  if(!Buf) return;
  if(x<=Raw &&  y<=Col){
    X=x;
    Y=y;
    asm{
        mov ah,2;
        mov dh,y;
        mov dl,x;
        mov bh,0;
        int 0x10;
    }
  }else{
    asm{
        mov ah,2;
        mov dh,150;
        mov dl,51;
        mov bh,0;
        int 0x10;
    }
  }
};
void _Screen::SetCursor(unsigned char OnOff){
  TRACE("Screen::SetCursor");
  if(!Buf) return;
  Mode=OnOff;
  if(!Mode){
    /*
    asm{
      mov ah,1 ;
      mov ch,15;
      mov cl,15;
      int 0x10;
    }
    */
    asm{
       mov ah,2;
       mov dh,150;
       mov dl,51;
       mov bh,0;
       int 0x10;
    }

  }else{
   if(Mode==1)
     asm{
        mov ah,1;
        mov ch,08;
        mov cl,13;
        int 0x10;
     }
   else
     asm{
        mov ah,1;
        mov ch,12;
        mov cl,13;
        int 0x10;
     }


  }
};


