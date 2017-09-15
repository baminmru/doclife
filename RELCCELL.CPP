/****************************************************
 *   RELCCELL.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "COMMON.H"


void _RelCCell::Paint(void){
  PaintBuf2();
  short dx=w,dy=0;
  Coord(&dx,&dy);
  GDt->S.AWrite(dx,dy,(char)25,Style2Color(style,(st&stFocused)?2:1));
  GDt->S.AWrite(dx+1,dy,' ',Style2Color(style,(st&stFocused)?2:1));
  GDt->S.AWrite(dx+2,dy,size2,1,buffer2,Style2Color(style,(st&stFocused)?2:1));
  _Cell::Paint();
};


static void ReturnChoice(_Event *E, void * parent){
  long pos;
  _FileView *fv =(_FileView*) parent;
  pos=fv->CurRecPos();
  fv->LostFocus(0);
  _RelCCell *w= (_RelCCell*) (((_Window*)parent)->parent);
  char *c =new char [w->T->max_rec_len];
  w->T->GDir(c,&pos,w->Key);
  w->T->BuildIndex(c,w->info,w->Key);
  w->Load();
  w->Commit();
  delete c;

};

static Table *T=0;
static short FieldID=0;
static void Writer(void *rec, char *buf){
  if(T)
    memcpy(buf,(char *)rec+T->FieldOffset(FieldID),T->FieldSize(FieldID));
};

static void ViewCls(_Event *E, void * parent){
  _RelCCell *w= (_RelCCell*) parent;
  _FileView *w1;
  T=w->T;
  FieldID=w->FieldID;
  if(w->Writer)
    w->Add(w1=new _FileView(w->T,w->Key,0,0,
                            w->Writer
                            ,0,w->h,w->size2,10,1,ST_RELVIEW,"RelCView"));
  else
    w->Add(w1=new _FileView(w->T,w->Key,0,0,
                            Writer
                            ,0,w->h,w->size2,10,1,ST_RELVIEW,"RelCView"));
  w1->st =w1->st & ~stFrame;
  w1->st =w1->st | stNoMove;
  w1->AddCallBack(ReturnChoice,evKey,ENTER);
  w1->JumpToKey(w->info);
  w->SwitchFocus(w1);
  if(w->deleteF)
        w1->AddCallBack(w->deleteF,evKey,DELETE);
  if(w->insertF)
        w1->AddCallBack(w->insertF,evKey,INSERT);
  E->ClearEvent((_Window*)parent);
};



_RelCCell::_RelCCell
(short _x,short _y,short _w, short _style, short _w1,  char *_info,
 short _TableID, short _Key, short fid, void (*_Writer)(void *rec, char *buf)):
  _Cell(_x,_y,_w1,1,_style,_info),
  Key(_Key), FieldID(fid),Writer(_Writer)
{
  T=T->St.FindTable(_TableID);
  size2 = _w - _w1;
  min_w=_w+2;
  buffer2= new char [size2+1];
  CodeState=1;
  insertF=deleteF=0;
  AddCallBack(ViewCls,evKey,ALT_DOWN);
};


_RelCCell::~_RelCCell(void){
  delete buffer2;
};

int _RelCCell:: CanLostFocus(void){
  return CodeState;
};

int _RelCCell:: CanClose(void){
  return CodeState;
};


void _RelCCell::PaintBuf2(void){
  StrClear(buffer2,size2);
  if(!T){
    CodeState = 1;
  }else{
    char *rec = new char [T->max_rec_len];
    int off=T->FieldOffset(FieldID);
    int ss=T->FieldSize(FieldID);
    if(T->GEQ(rec,buffer,Key)){
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
};

int _RelCCell::MHandle(_Event *E){
  if(E->Type==evMouse){
    short dx=0,dy=0;
    Coord(&dx,&dy);
    if(E->msg.M.x==dx+w && dy==E->msg.M.y ){
      if(E->msg.M.buttons & 1 && st & stFocused){
        GDt->Q.AddMessage(evKey,ALT_DOWN);
        E->ClearEvent(this);
        return 0;
      }
    }
  }
  return _Window::MHandle(E);
};

void _RelCCell::LostFocus(int l){
  if(Find("RelCView")){
    Del(Find("RelCView"));
  }
  _Window::LostFocus(l);
};
