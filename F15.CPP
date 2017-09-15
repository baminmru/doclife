/****************************************************
 *        F15.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"
#include "doc.h"
#include "f15.h"


static short ShowMask15[]={
       1,   // Код_типа
       2,   // Название
       0    // end_of_list
};

file_15 * rf15;
Table *T15;
static char  * f15;

void Init15(void){
  T15= new Table("Типы_договоров",OWNER);
  if(!T15->opened){
	printf("Error: file Типы_договоров not opened\n");
        exit(0);
  }
  f15=new char [T15->max_rec_len];
  rf15 = (file_15*)f15;
};

void Close15(void){
  delete T15;
  delete f15;
};

static void WriteF15(void *rec, char *buf){
    int curlen=0,sz;
    buf[0]=0;
    char *buf2;
    int i=0;
    while(ShowMask15[i]){
      sz=T15->SizeForPrint(ShowMask15[i]);
      if(curlen+sz+1<MAX_STRING_WIDTH){
        buf2 = new char[sz+1];
        memset(buf2,0,sz+1);
        T15->PrintField(ShowMask15[i], rec ,buf2);
        strcat(buf,buf2);
        strcat(buf,"│");;
        curlen=curlen+sz+1;
        delete buf2;
      }
      i++;
    }
};


static void EditF15Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Update(f15);
  E->msg.K=ESC;
};

static void InsertF15Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Insert(f15);
  E->msg.K=ESC;
};

static void EditF15(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f15)) return;
  _Window *w= new _Window(1,1,73,1,E_STYLE15,stBuffered | stFrame,"EditF15");
  fv->Add(w); short wh=1;   w->HelpID=19;

  //w->Add(new _Static(1,(wh++),E_STYLE15,"Код_типа            "));
  //w->Add(BuildCell(T15,1,f15,1,&wh,w->w,E_STYLE15 ));

  w->Add(new _Static(1,(wh++),E_STYLE15,"Название            "));
  w->Add(BuildCell(T15,2,f15,1,&wh,w->w,E_STYLE15 ));
  //wh++;
  w->AddCallBack(EditF15Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void InsertF15(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  memset(f15,0,sizeof(f15));
  InitRecord(T15,f15);
  _Window *w= new _Window(1,1,73,1,E_STYLE15,stBuffered | stFrame,"InsertF15");
  fv->Add(w); short wh=1;   w->HelpID=19;

  //w->Add(new _Static(1,(wh++),E_STYLE15,"Код_типа            "));
  //w->Add(BuildCell(T15,1,f15,1,&wh,w->w,E_STYLE15 ));

  w->Add(new _Static(1,(wh++),E_STYLE15,"Название            "));
  w->Add(BuildCell(T15,2,f15,1,&wh,w->w,E_STYLE15 ));
  //wh++;
  w->AddCallBack(InsertF15Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void DeleteF15(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  fv->Delete();
  E->ClearEvent((_Window*)parent);
};

static void PrintF15(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  char name[16];
  Report(T15,fv->KeyNum,name);
  _ShowPrint *sp = new _ShowPrint(name,"Печать Типы_договоров");
  fv->Add(sp); sp->HelpID=100;
  sp->GotFocus();
  E->ClearEvent((_Window*)parent);
};

void AddF15( void (*CallBackFunc)(_Event *,void *), _Window * parent){
  if(!Dostup[15])
    return;
  _Window * tmp =parent;
  if(!tmp) tmp =GDt;
  if(tmp->Find("Типы_договоров")){
    if(tmp ==GDt){
      MessageBox("Окно 'Типы_договоров' уже доступно,",
                 "для переключения используйте: ",
                 "Alt + "
                );
    }else{
      MessageBox("Окно 'Типы_договоров' уже доступно,",
                 "его нельзя активизировать еще раз"
                );
    }
    return;
  }
  _FileView * fv=new _FileView (T15,0,0,0,WriteF15,2,2,75,20,1,V_STYLE15,"Типы_договоров");
  fv->HelpID=7;
  fv->Add(new _Static(2,0,V_STYLE15," Типы_договоров "));
  if(Dostup[15]==D_WRITE){
    fv->AddCallBack(InsertF15, evKey,INSERT);
    fv->AddCallBack(EditF15, evKey,ENTER);
    fv->AddCallBack(DeleteF15, evKey,DELETE);
  }
  fv->AddCallBack(PrintF15, evKey, CTRL_P);
  if(CallBackFunc)
        fv->AddCallBack(CallBackFunc, evKey, ESC);
  tmp->Add(fv);
  tmp->SwitchFocus(fv);
};


