/****************************************************
 *        F12.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"
#include "doc.h"
#include "f12.h"


static short ShowMask12[]={
       1,   // Номер_счета
       2,   // Наименование_счета
       0    // end_of_list
};

file_12 * rf12;
Table *T12;
static char  * f12;

void Init12(void){
  T12= new Table("Номера_счетов_КЗР",OWNER);
  if(!T12->opened){
	printf("Error: file Номера_счетов_КЗР not opened\n");
        exit(0);
  }
  f12=new char [T12->max_rec_len];
  rf12 = (file_12*)f12;
};

void Close12(void){
  delete T12;
  delete f12;
};

static void WriteF12(void *rec, char *buf){
    int curlen=0,sz;
    buf[0]=0;
    char *buf2;
    int i=0;
    while(ShowMask12[i]){
      sz=T12->SizeForPrint(ShowMask12[i]);
      if(curlen+sz+1<MAX_STRING_WIDTH){
        buf2 = new char[sz+1];
        memset(buf2,0,sz+1);
        T12->PrintField(ShowMask12[i], rec ,buf2);
        strcat(buf,buf2);
        strcat(buf,"│");;
        curlen=curlen+sz+1;
        delete buf2;
      }
      i++;
    }
};


static void EditF12Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Update(f12);
  E->msg.K=ESC;
};

static void InsertF12Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Insert(f12);
  E->msg.K=ESC;
};

static void EditF12(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f12)) return;
  _Window *w= new _Window(1,1,73,1,E_STYLE12,stBuffered | stFrame,"EditF12");
  fv->Add(w); short wh=1;   w->HelpID=19;

  w->Add(new _Static(1,(wh++),E_STYLE12,"Номер_счета         "));
  w->Add(BuildCell(T12,1,f12,1,&wh,w->w,E_STYLE12 ));

  w->Add(new _Static(1,(wh++),E_STYLE12,"Наименование_счета  "));
  w->Add(BuildCell(T12,2,f12,1,&wh,w->w,E_STYLE12 ));

  w->Add(new _Static(1,(wh++),E_STYLE12,"Директорий \"Паруса\"  "));
  w->Add(BuildCell(T12,3,f12,1,&wh,w->w,E_STYLE12 ));

  w->Add(new _Static(1,(wh),E_STYLE12,"Псевдоним счета  "));
  w->Add(BuildCell(T12,4,f12,21,&wh,w->w,E_STYLE12 ));
  //wh++;
  w->AddCallBack(EditF12Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void InsertF12(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  memset(f12,0,sizeof(f12));
  InitRecord(T12,f12);
  _Window *w= new _Window(1,1,73,1,E_STYLE12,stBuffered | stFrame,"InsertF12");
  fv->Add(w); short wh=1;   w->HelpID=19;

  w->Add(new _Static(1,(wh++),E_STYLE12,"Номер_счета         "));
  w->Add(BuildCell(T12,1,f12,1,&wh,w->w,E_STYLE12 ));

  w->Add(new _Static(1,(wh++),E_STYLE12,"Наименование_счета  "));
  w->Add(BuildCell(T12,2,f12,1,&wh,w->w,E_STYLE12 ));
  w->Add(new _Static(1,(wh++),E_STYLE12,"Директорий \"Паруса\"  "));
  w->Add(BuildCell(T12,3,f12,1,&wh,w->w,E_STYLE12 ));
  w->Add(new _Static(1,(wh++),E_STYLE12,"Псевдоним счета  "));
  w->Add(BuildCell(T12,4,f12,1,&wh,w->w,E_STYLE12 ));
  //wh++;
  w->AddCallBack(InsertF12Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void DeleteF12(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  fv->Delete();
  E->ClearEvent((_Window*)parent);
};

static void PrintF12(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  char name[16];
  Report(T12,fv->KeyNum,name);
  _ShowPrint *sp = new _ShowPrint(name,"Печать Номера_счетов_КЗР");
  fv->Add(sp); sp->HelpID=100;
  sp->GotFocus();
  E->ClearEvent((_Window*)parent);
};

void AddF12( void (*CallBackFunc)(_Event *,void *), _Window * parent){
  if(!Dostup[12])
    return;
  _Window * tmp =parent;
  if(!tmp) tmp =GDt;
  if(tmp->Find("Номера_счетов_КЗР")){
    if(tmp ==GDt){
      MessageBox("Окно 'Номера_счетов_КЗР' уже доступно,",
                 "для переключения используйте: ",
                 "Alt + "
                );
    }else{
      MessageBox("Окно 'Номера_счетов_КЗР' уже доступно,",
                 "его нельзя активизировать еще раз"
                );
    }
    return;
  }
  _FileView * fv=new _FileView (T12,0,0,0,WriteF12,2,2,75,20,1,V_STYLE12,"Номера_счетов_КЗР");
  fv->HelpID=7;
  fv->Add(new _Static(2,0,V_STYLE12," Номера_счетов_КЗР "));
  if(Dostup[12]==D_WRITE){
    fv->AddCallBack(InsertF12, evKey,INSERT);
    fv->AddCallBack(EditF12, evKey,ENTER);
    fv->AddCallBack(DeleteF12, evKey,DELETE);
  }
  fv->AddCallBack(PrintF12, evKey, CTRL_P);

  if(CallBackFunc)
        fv->AddCallBack(CallBackFunc, evKey, ESC);
  tmp->Add(fv);
  tmp->SwitchFocus(fv);
};


