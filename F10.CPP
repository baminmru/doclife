/****************************************************
 *        F10.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"
#include "doc.h"
#include "f10.h"


static short ShowMask10[]={
       1,   // Номер_статьи
       2,   // Название статьи
       0    // end_of_list
};

file_10 * rf10;
Table *T10;
static char  * f10;

void Init10(void){
  T10= new Table("Статьи_расходов",OWNER);
  if(!T10->opened){
	printf("Error: file Статьи_расходов not opened\n");
        exit(0);
  }
  f10=new char [T10->max_rec_len];
  rf10 = (file_10*)f10;
};

void Close10(void){
  delete T10;
  delete f10;
};

static void WriteF10(void *rec, char *buf){
    int curlen=0,sz;
    buf[0]=0;
    char *buf2;
    int i=0;
    while(ShowMask10[i]){
      sz=T10->SizeForPrint(ShowMask10[i]);
      if(curlen+sz+1<MAX_STRING_WIDTH){
        buf2 = new char[sz+1];
        memset(buf2,0,sz+1);
        T10->PrintField(ShowMask10[i], rec ,buf2);
        strcat(buf,buf2);
        strcat(buf,"│");;
        curlen=curlen+sz+1;
        delete buf2;
      }
      i++;
    }
};


static void EditF10Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Update(f10);
  E->msg.K=ESC;
};

static void InsertF10Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Insert(f10);
  E->msg.K=ESC;
};

static void EditF10(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f10)) return;
  _Window *w= new _Window(1,1,73,1,E_STYLE10,stBuffered | stFrame,"EditF10");
  fv->Add(w); short wh=1;   w->HelpID=19;
  w->Add(new _Static(1,(wh++),E_STYLE10,"Номер_статьи        "));
  w->Add(BuildCell(T10,1,f10,1,&wh,w->w,E_STYLE10 ));

  w->Add(new _Static(1,(wh++),E_STYLE10,"Название статьи     "));
  w->Add(BuildCell(T10,2,f10,1,&wh,w->w,E_STYLE10 ));
  w->AddCallBack(EditF10Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void InsertF10(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  memset(f10,0,sizeof(f10));
  InitRecord(T10,f10);
  _Window *w= new _Window(1,1,73,1,E_STYLE10,stBuffered | stFrame,"InsertF10");
  fv->Add(w); short wh=1;   w->HelpID=19;

  w->Add(new _Static(1,(wh++),E_STYLE10,"Номер_статьи        "));
  w->Add(BuildCell(T10,1,f10,1,&wh,w->w,E_STYLE10 ));

  w->Add(new _Static(1,(wh++),E_STYLE10,"Название статьи     "));
  w->Add(BuildCell(T10,2,f10,1,&wh,w->w,E_STYLE10 ));

  w->AddCallBack(InsertF10Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void DeleteF10(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  fv->Delete();
  E->ClearEvent((_Window*)parent);
};

static void PrintF10(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  char name[16];
  Report(T10,fv->KeyNum,name);
  _ShowPrint *sp = new _ShowPrint(name,"Печать Статьи_расходов");
  fv->Add(sp); sp->HelpID=100;
  sp->GotFocus();
  E->ClearEvent(fv);
};

void AddF10( void (*CallBackFunc)(_Event *,void *), _Window * parent){
  if(!Dostup[10])
    return;
  _Window * tmp =parent;
  if(!tmp) tmp =GDt;
  if(tmp->Find("Статьи_расходов")){
    if(tmp ==GDt){
      MessageBox("Окно 'Статьи_расходов' уже доступно,",
                 "для переключения используйте: ",
                 "Alt + "
                );
    }else{
      MessageBox("Окно 'Статьи_расходов' уже доступно,",
                 "его нельзя активизировать еще раз"
                );
    }
    return;
  }
  _FileView * fv=new _FileView (T10,0,0,0,WriteF10,2,2,75,20,1,V_STYLE10,"Статьи_расходов");
  fv->HelpID=7;
  fv->Add(new _Static(2,0,V_STYLE10," Статьи_расходов "));
  if(Dostup[10]==D_WRITE){
    fv->AddCallBack(InsertF10, evKey,INSERT);
    fv->AddCallBack(EditF10, evKey,ENTER);
    fv->AddCallBack(DeleteF10, evKey,DELETE);
  }
  fv->AddCallBack(PrintF10, evKey, CTRL_P);
  if(CallBackFunc)
        fv->AddCallBack(CallBackFunc, evKey, ESC);
  tmp->Add(fv);
  tmp->SwitchFocus(fv);
};


