/****************************************************
 *        F14.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"
#include "doc.h"
#include "f14.h"


static short ShowMask14[]={
       1,   // Номер_района
       2,   // Наименование_района
       0    // end_of_list
};

file_14 * rf14;
Table *T14;
static char  * f14;

void Init14(void){
  T14= new Table("Справочник_районов",OWNER);
  if(!T14->opened){
	printf("Error: file Справочник_районов not opened\n");
        exit(0);
  }
  f14=new char [T14->max_rec_len];
  rf14 = (file_14*)f14;
};

void Close14(void){
  delete T14;
  delete f14;
};

static void WriteF14(void *rec, char *buf){
    int curlen=0,sz;
    buf[0]=0;
    char *buf2;
    int i=0;
    while(ShowMask14[i]){
      sz=T14->SizeForPrint(ShowMask14[i]);
      if(curlen+sz+1<MAX_STRING_WIDTH){
        buf2 = new char[sz+1];
        memset(buf2,0,sz+1);
        T14->PrintField(ShowMask14[i], rec ,buf2);
        strcat(buf,buf2);
        strcat(buf,"│");;
        curlen=curlen+sz+1;
        delete buf2;
      }
      i++;
    }
};


static void EditF14Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Update(f14);
  E->msg.K=ESC;
};

static void InsertF14Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Insert(f14);
  E->msg.K=ESC;
};

static void EditF14(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f14)) return;
  _Window *w= new _Window(1,1,73,1,E_STYLE14,stBuffered | stFrame,"EditF14");
  fv->Add(w); short wh=1;   w->HelpID=19;

  //w->Add(new _Static(1,(wh++),E_STYLE14,"Номер_района        "));
  //w->Add(BuildCell(T14,1,f14,1,&wh,w->w,E_STYLE14 ));

  w->Add(new _Static(1,(wh++),E_STYLE14,"Наименование_района "));
  w->Add(BuildCell(T14,2,f14,1,&wh,w->w,E_STYLE14 ));
  //wh++;
  w->AddCallBack(EditF14Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void InsertF14(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  memset(f14,0,sizeof(f14));
  InitRecord(T14,f14);
  _Window *w= new _Window(1,1,73,1,E_STYLE14,stBuffered | stFrame,"InsertF14");
  fv->Add(w); short wh=1;   w->HelpID=19;

  //w->Add(new _Static(1,(wh++),E_STYLE14,"Номер_района        "));
  //w->Add(BuildCell(T14,1,f14,1,&wh,w->w,E_STYLE14 ));

  w->Add(new _Static(1,(wh++),E_STYLE14,"Наименование_района "));
  w->Add(BuildCell(T14,2,f14,1,&wh,w->w,E_STYLE14 ));
  //wh++;
  w->AddCallBack(InsertF14Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void DeleteF14(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  fv->Delete();
  E->ClearEvent((_Window*)parent);
};

static void PrintF14(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  char name[16];
  Report(T14,fv->KeyNum,name);
  _ShowPrint *sp = new _ShowPrint(name,"Печать Справочник_районов");
  fv->Add(sp); sp->HelpID=100;
  sp->GotFocus();
  E->ClearEvent((_Window*)parent);
};

void AddF14( void (*CallBackFunc)(_Event *,void *), _Window * parent){
  if(!Dostup[14])
    return;
  _Window * tmp =parent;
  if(!tmp) tmp =GDt;
  if(tmp->Find("Справочник_районов")){
    if(tmp ==GDt){
      MessageBox("Окно 'Справочник_районов' уже доступно,",
                 "для переключения используйте: ",
                 "Alt + "
                );
    }else{
      MessageBox("Окно 'Справочник_районов' уже доступно,",
                 "его нельзя активизировать еще раз"
                );
    }
    return;
  }
  _FileView * fv=new _FileView (T14,0,0,0,WriteF14,2,2,75,20,1,V_STYLE14,"Справочник_районов");
  fv->HelpID=7;
  fv->Add(new _Static(2,0,V_STYLE14," Справочник_районов "));
  if(Dostup[14]==D_WRITE){
    fv->AddCallBack(InsertF14, evKey,INSERT);
    fv->AddCallBack(EditF14, evKey,ENTER);
    fv->AddCallBack(DeleteF14, evKey,DELETE);
  }
  fv->AddCallBack(PrintF14, evKey, CTRL_P);
  if(CallBackFunc)
        fv->AddCallBack(CallBackFunc, evKey, ESC);
  tmp->Add(fv);
  tmp->SwitchFocus(fv);
};


