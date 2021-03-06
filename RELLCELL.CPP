/****************************************************
 *   RELLCELL.CPP                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#include "COMMON.H"

static void ReturnChoice(_Event *E, void * parent){
  long pos;
  _FileView *fv =(_FileView*) parent;
  pos=fv->CurRecPos();
  fv->LostFocus(0);
  _RelLCell *w= (_RelLCell*) (((_Window*)parent)->parent);
  char *c =new char [w->T->max_rec_len];
  w->T->GDir(c,&pos,w->Key);
  w->T->BuildIndex(c,w->info,w->Key);
  w->Load();
  delete c;
  w->Commit();

};

static Table *T=0;
static short FieldID=0;
static void Writer(void *rec, char *buf){
  if(T)
    memcpy(buf,(char *)rec+T->FieldOffset(FieldID),T->FieldSize(FieldID));
};
static void ViewCls(_Event *E, void * parent){
  _RelLCell *w= (_RelLCell*) parent;
  _FileView *w1;
  T=w->T;
  FieldID=w->FieldID;
  if(w->Writer)
    w->Add(w1=new _FileView(w->T,w->Key,0,0,
                            w->Writer
                            ,0,w->h,w->size2,8,1,ST_RELVIEW,"RelLView"));
  else
    w->Add(w1=new _FileView(w->T,w->Key,0,0,
                            Writer
                            ,0,w->h,w->size2,8,1,ST_RELVIEW,"RelLView"));
  w1->st =w1->st & ~stFrame;
  w1->st =w1->st | stNoMove;
  w1->AddCallBack(ReturnChoice,evKey,ENTER);
  w1->JumpToKey(w->info);
  if(w->deleteF)
        w1->AddCallBack(w->deleteF,evKey,DELETE);
  if(w->insertF)
        w1->AddCallBack(w->insertF,evKey,INSERT);
  w->SwitchFocus(w1);
  E->ClearEvent((_Window*)parent);
};


void _RelLCell::Paint(void){
    PaintBuf2();
  short dx=w,dy=0;
  Coord(&dx,&dy);
  GDt->S.AWrite(dx,dy,25,Style2Color(style,(st&stFocused)?2:1));
  GDt->S.AWrite(dx+1,dy,' ',Style2Color(style,(st&stFocused)?2:1));
  GDt->S.AWrite(dx+2,dy,size2,1,buffer2,
           Style2Color(style,(st&stFocused)?2:1));
  _DigCell::Paint();
};


_RelLCell::_RelLCell
(short _x,short _y,short _w, short _style, short _digits,  long *_info,
 short _sign, short _TableID, short _Key, short _FieldID, void (*_Writer)(void *rec, char *buf)):
  _DigCell(_x,_y,_style,_digits,_info,_sign),
  Key(_Key), FieldID(_FieldID)
{
  T=T->St.FindTable(_TableID);
  size2 = _w - _digits - _sign;
  buffer2= new char [size2+1];
  CodeState=1;
  min_w=_w+2;
  Writer=_Writer;
  insertF=deleteF=0;
};

_RelLCell::~_RelLCell(void){
  delete buffer2;
};

int _RelLCell:: CanLostFocus(void){
  return CodeState;
};
int _RelLCell:: CanClose(void){
  return CodeState;
};

void _RelLCell::PaintBuf2(void){
  StrClear(buffer2,size2);
  //Debug("PaintBuf2->");
  if(!T){
    CodeState = 1;
  }else{
    char *rec = new char [T->max_rec_len];
    int off=T->FieldOffset(FieldID);
    int ss=T->FieldSize(FieldID);
    if(T->GEQ(rec,info,Key)){
      CodeState = 0;
      if(st&stFocused){
        sound(440);
        delay(25);
        nosound();
      }
    }else{
      if(ss>size2)
        ss=size2;
      strncpy(buffer2,rec+off,ss);
      CodeState = 1;
    }
    delete rec;
  }
  //Debug("info=%d state =%d\n",*info,CodeState);
};
int _RelLCell::MHandle(_Event *E){
  if(E->Type==evMouse){
    short dx=0,dy=0;
    Coord(&dx,&dy);
    if(E->msg.M.x==dx+w && dy==E->msg.M.y){
      if(E->msg.M.buttons & 1 && st & stFocused){
        GDt->Q.AddMessage(evKey,ALT_DOWN);
        E->ClearEvent(this);
        return 0;
      }
    }
  }
  return _Window::MHandle(E);
};
void _RelLCell::LostFocus(int l){
  if(Find("RelLView")){
    Del(Find("RelLView"));
  }
  _Window::LostFocus(l);
};
