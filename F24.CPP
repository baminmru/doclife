/****************************************************
 *        F24.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"
#include "doc.h"
#include "f24.h"


static short ShowMask24[]={
      // 1,   // Код типа
       2,   // Приоритет
       3,   // Номер счета
       0    // end_of_list
};

file_24 * rf24;
Table *T24;
static char  * f24;

void Init24(void){
  T24= new Table("Тип_финансирования",OWNER);
  if(!T24->opened){
	printf("Error: file Тип_финансирования not opened\n");
        exit(0);
  }
  f24=new char [T24->max_rec_len];
  rf24 = (file_24*)f24;
};

void Close24(void){
  delete T24;
  delete f24;
};

static void WriteF24(void *rec, char *buf){
    int curlen=0,sz;
    buf[0]=0;
    char *buf2;
    int i=0;
    while(ShowMask24[i]){
      sz=T24->SizeForPrint(ShowMask24[i]);
      if(curlen+sz+1<MAX_STRING_WIDTH){
        buf2 = new char[sz+1];
        memset(buf2,0,sz+1);
        T24->PrintField(ShowMask24[i], rec ,buf2);
        strcat(buf,buf2);
        strcat(buf," ");
        curlen=curlen+sz+1;
        delete buf2;
      }
      i++;
    }
};


static void EditF24Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Update(f24);
  E->msg.K=ESC;
};

static void InsertF24Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Insert(f24);
  E->msg.K=ESC;
};

static void EditF24(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f24)) return;
  _Window *w= new _Window(1,1,73,1,E_STYLE24,stBuffered | stFrame,"EditF24");
  fv->Add(w); short wh=1;   w->HelpID=19;

  w->Add(new _Static(1,(wh++),E_STYLE24,"Приоритет           "));
  w->Add(BuildCell(T24,2,f24,1,&wh,w->w,E_STYLE24 ));

  w->Add(new _Static(1,(wh++),E_STYLE24,"Номер счета         "));
  w->Add(new _RelCCell(1,(wh++),40,E_STYLE13,15,rf24->Nomer_sheta,12,0,2));

  //wh++;
  w->AddCallBack(EditF24Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void InsertF24(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  memset(f24,0,sizeof(f24));
  InitRecord(T24,f24);
  _Window *w= new _Window(1,1,73,1,E_STYLE24,stBuffered | stFrame,"InsertF24");
  fv->Add(w); short wh=1;   w->HelpID=19;
  rf24->Kod_tipa=rf26->Kod;

  w->Add(new _Static(1,(wh),E_STYLE24,"Приоритет           "));
  w->Add(BuildCell(T24,2,f24,21,&wh,w->w,E_STYLE24 ));

  w->Add(new _Static(1,(wh++),E_STYLE24,"Номер счета         "));
  w->Add(new _RelCCell(1,(wh++),40,E_STYLE13,15,rf24->Nomer_sheta,12,0,2));

  //wh++;
  w->AddCallBack(InsertF24Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void DeleteF24(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  fv->Delete();
  E->ClearEvent((_Window*)parent);
};

static void PrintF24(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  char name[16];
  Report(T24,fv->KeyNum,name);
  _ShowPrint *sp = new _ShowPrint(name,"Печать Тип_финансирования");
  fv->Add(sp); sp->HelpID=100;
  sp->GotFocus();
  E->ClearEvent((_Window*)parent);
};

void AddF24( void (*CallBackFunc)(_Event *,void *), _Window * parent){
  if(!Dostup[24])
    return;
  _Window * tmp =parent;
  if(!tmp) tmp =GDt;
  if(tmp->Find("Тип_финансирования")){
    if(tmp ==GDt){
      MessageBox("Окно 'Тип_финансирования' уже доступно,",
                 "для переключения используйте: ",
                 "Alt + "
                );
    }else{
      MessageBox("Окно 'Тип_финансирования' уже доступно,",
                 "его нельзя активизировать еще раз"
                );
    }
    return;
  }
  _FileView * fv=new _FileView (T24,0,rf26,2,WriteF24,2,2,75,20,1,V_STYLE24,"Тип_финансирования");
  fv->HelpID = 17;
  fv->Add(new _Static(2,0,V_STYLE24," Тип_финансирования "));
  if(Dostup[24]==D_WRITE){
    fv->AddCallBack(InsertF24, evKey,INSERT);
    fv->AddCallBack(EditF24, evKey,ENTER);
    fv->AddCallBack(DeleteF24, evKey,DELETE);
  }
  fv->AddCallBack(PrintF24, evKey, CTRL_P);
  if(CallBackFunc)
        fv->AddCallBack(CallBackFunc, evKey, ESC);
  tmp->Add(fv);
  tmp->SwitchFocus(fv);
};


