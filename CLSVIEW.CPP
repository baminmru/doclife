/****************************************************
 *    CLSVIEW.CPP                                   *
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

static void _WriteRec(void * Rec ,char * Buf){
  clsctrs *r=(clsctrs *) Rec;
  strcpy(Buf,r->name);
};
Table _ClsView::T("X$Classificator");

_ClsView::_ClsView(
  long *_result
  ,short ID
  ,int _X
  ,int _Y
  ,int _W
  ,int _H
  ,int _FirstY
  //,short _style
  ,char *name
  ):
  _Window(_X,_Y,_W,_H,ST_CLSVIEW,stBuffered | stNoMove ,name){
      result=_result;
      KeyFilterSize= sizeof(short);
      KeyFilter[0] =ID;
      WriteRec     = _WriteRec;
      FirstY       = _FirstY  ;
      memset(RecPos,0,sizeof(RecPos));
      ActivLine=-1;
      MaxLine=_H-2;
      TotalLine=0;
      min_h=5;
};


_ClsView::~_ClsView(){
};

void _ClsView::Paint(void){
  _Window::Paint();
  short dx=0,dy=0;
  Coord(&dx,&dy);
  GDt->S.AWrite(dx+w-1,dy+1,'',Style2Color(style,1));
  GDt->S.AWrite(dx+w-1,dy+h-2,'',Style2Color(style,1));

  if(!TotalLine){
    ToTop();
  }
  ReShowAll();
};

void _ClsView::ReShowAll(void){
	for(int i=0; i<MaxLine; i++)
		ReShowLine(i);
};

void _ClsView::ReShowLine(int Line){
  short color=1;
	memset(LineBuffer,0x20,sizeof(LineBuffer));
	if(ActivLine<TotalLine){

    if(!T.GDir(&CurRec,&RecPos[Line],0)){
      WriteRec(&CurRec,LineBuffer);
			if(Line!=ActivLine)
        color=1;
			else
        color=2;
		}else{
      color=1;
      strcpy(LineBuffer," ... ");
		}
	}
	LineBuffer[strlen(LineBuffer)]=' ';
  LineBuffer[w-2]=0;
  short dx,dy;
  dx=1;
  dy=Line+FirstY;
  Coord(&dx,&dy);
  //color+=(2-2*(st&stFocused));
  GDt->S.AWrite(dx,dy,w-2,LineBuffer,Style2Color(style,color));

};

int _ClsView::MHandle(_Event *E){
  if(E->Type==evMouse && st &stFocused){
    short dx=0,dy=0;
    Coord(&dx,&dy);
    if(E->msg.M.buttons & 1 && !FindFocused()){
      if(E->msg.M.x==dx+w-1){
        if(E->msg.M.y==dy+1){
          GDt->Q.AddMessage(evKey,UP);
          E->ClearEvent(this);
          return 0;
        }
        if(E->msg.M.y==dy+h-2){
          GDt->Q.AddMessage(evKey,DOWN);
          E->ClearEvent(this);
          return 0;
        }
      }

      if(dx<E->msg.M.x && E->msg.M.x<dx+w-1 && dy<E->msg.M.y && E->msg.M.y<dy+h-1){
        if(E->msg.M.y-dy-FirstY == ActivLine){
          GDt->Q.AddMessage(evKey,ENTER);
          E->ClearEvent(this);
          return 0;
        }else{
          if(E->msg.M.y-dy-FirstY < TotalLine){
            ActivLine=E->msg.M.y-dy-FirstY;
            Show(1);
            E->ClearEvent(this);
            return 0;
          }
        }
      }

    }

  }
  return _Window::MHandle(E);
};

int _ClsView::Handle(_Event *E){
  int OldActivLine;
  int ch;
  short done=0;
  PageChanged=0;
  if(E->Type==evKey){
  switch(E->msg.K){
    case DOWN:
      OldActivLine=ActivLine;
      Next();
      if(PageChanged){
        Show(1);
        done=1;
        break;
      }
      if(ActivLine!=OldActivLine){
        GDt->S.Lock();
        ReShowLine(OldActivLine);
        ReShowLine(ActivLine);
        GDt->S.Paste();

      }
      done=1;
      break;

    case UP:
      OldActivLine=ActivLine;
      Previous();
      if(PageChanged){
        Show(1);
        done=1;
        break;
      }
      if(ActivLine!=OldActivLine){
        GDt->S.Lock();
        ReShowLine(OldActivLine);
        ReShowLine(ActivLine);
        GDt->S.Paste();
      }
      done=1;
      break;

    case PGUP:
      PageUp();
      Show(1);
      done=1;
      break;

    case PGDN:
      PageDn();
      Show(1);
      done=1;
      break;

    case HOME:
      ToTop();
      Show(1);
      done=1;
      break;

    case END:
      ToBottom();
      Show(1);
      done=1;
      break;

    case ESC:
      *result=0;
      break;

    case ENTER:
      *result=RecPos[ActivLine];
      break;

    default:
      break;

    }
  }
  if(done) E->ClearEvent(this);

  // отправили на стандартную обработку
  return _Window::Handle(E);
};



void _ClsView::Repaint(void){
    unsigned long pos;
    memset(RecPos,0,sizeof(RecPos));
    ActivLine=0;
    TotalLine=0;
    T.GPos(&pos);
    RecPos[ActivLine]=pos;
    TotalLine=0;
    for(int i=0;i<MaxLine;i++)
      if(!Next()){
         ToBottom();
         break;
      }
    for(i=0;i<MaxLine;i++)
      if(RecPos[i]==pos){
        ActivLine=i;
        break;
      }
    PageChanged=1;
    Show(1);
};


int _ClsView::Next(void){

    if(ActivLine<TotalLine-1)
      // движение внутри страници
      ActivLine++;
    else{
      T.GDir(&CurRec,&RecPos[ActivLine],0);
      if(!T.GNext(&CurRec)){
        if(RecordOK(&CurRec)){

          // появилась новая запись
          if(TotalLine<MaxLine){
            T.GPos(&RecPos[++ActivLine]);
            TotalLine++;
          }else{

            // прокрутка страници
            T.GPos(&RecPos[MaxLine]);
            for(int i=0; i<MaxLine; i++){
              RecPos[i]=RecPos[i+1];
            }
            PageChanged=1;

          }
        }else
          return 0;
      } else
        return 0;

    }
    return 1;
};

int _ClsView::Previous(void){

    if(ActivLine>0)
      // движение внутри страници
      ActivLine--;
    else{
      T.GDir(&CurRec,&RecPos[ActivLine],0);
      if(!T.GPrev(&CurRec)){
        if(RecordOK(&CurRec)){

          // прокрутка страници
          for(int i=MaxLine; i>0; i--){
            RecPos[i]=RecPos[i-1];
            if(TotalLine<MaxLine)
              TotalLine++;
          }
          T.GPos(&RecPos[0]);

          PageChanged=1;


        } else
          return 0;
      }else
        return 0;
    }
    return 1;
};


void _ClsView::PageUp(void){
  for(int i=0;i<MaxLine-1;i++)
    Previous();
};

void _ClsView::PageDn(void){
  for(int i=0;i<MaxLine-1;i++)
    Next();
};

void _ClsView::ToTop(void){
  char _Key[256];
  memset(RecPos,0,sizeof(RecPos));
  TotalLine=0;
  ActivLine=0;
  PageChanged=1;
  memset(_Key,0,256);
  memcpy(_Key,KeyFilter,KeyFilterSize);
  if(!T.GGE(&CurRec,_Key,0)){
    if(RecordOK(&CurRec)){
      memset(RecPos,0,sizeof(RecPos));
      ActivLine=0;
      T.GPos(&RecPos[ActivLine]);
      TotalLine=1;
      for(int i=0;i<MaxLine-1;i++)
        Next();
      ActivLine=0;
    }
  };
};

void _ClsView::ToBottom(void){
  char _Key[256];
  memset(RecPos,0,sizeof(RecPos));
  TotalLine=0;
  ActivLine=0;

  memset(_Key,0xFF,256);
  memcpy(_Key,KeyFilter,KeyFilterSize);
  memset(RecPos,0,sizeof(RecPos));

  if(!T.GLE(&CurRec,_Key,0)){
    for(int i=0;i<MaxLine;){
      if(T.GPrev(&CurRec))
        break;
      if(RecordOK(&CurRec)){
        T.GPos(&RecPos[ActivLine]);
        i++;
      }
    }

    if(RecPos[0])
      TotalLine=1;
    else
      TotalLine=0;
    ActivLine=0;
    for(;i>=0;i--)
      Next();
    ActivLine=TotalLine-1;
    PageChanged=1;
  };
};

int _ClsView::RecordOK(void *Rec){
  char _Key[256];
  if( !KeyFilterSize)
    return 1;

  else{
    T.BuildIndex(Rec,_Key,0);
    if(memcmp(_Key,KeyFilter,KeyFilterSize))
      return 0;
    else
      return 1;
  }
};
