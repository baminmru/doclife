/****************************************************
 *        F21.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"
#include "doc.h"
#include "f21.h"


static short ShowMask21[]={
       1,   // Код_строки
       2,   // Наименование
       3,   // Номер статьи
       0    // end_of_list
};

file_21 * rf21;
Table *T21;
static char  * f21;

void Init21(void){
  T21= new Table("Справочник_работ",OWNER);
  if(!T21->opened){
	printf("Error: file Справочник_работ not opened\n");
        exit(0);
  }
  f21=new char [T21->max_rec_len];
  rf21 = (file_21*)f21;
};

void Close21(void){
  delete T21;
  delete f21;
};

static void WriteF21(void *rec, char *buf){
    int curlen=0,sz;
    buf[0]=0;
    char *buf2;
    int i=0;
    while(ShowMask21[i]){
      sz=T21->SizeForPrint(ShowMask21[i]);
      if(curlen+sz+1<MAX_STRING_WIDTH){
        buf2 = new char[sz+1];
        memset(buf2,0,sz+1);
        T21->PrintField(ShowMask21[i], rec ,buf2);
        strcat(buf,buf2);
        strcat(buf,"│");;
        curlen=curlen+sz+1;
        delete buf2;
      }
      i++;
    }
};


static void EditF21Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Update(f21);
  E->msg.K=ESC;
};

static void InsertF21Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Insert(f21);
  E->msg.K=ESC;
};

static void EditF21(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f21)) return;
  _Window *w= new _Window(1,1,73,1,E_STYLE21,stBuffered | stFrame,"EditF21");
  fv->Add(w); short wh=1;   w->HelpID=19;


  w->Add(new _Static(1,(wh++),E_STYLE21,"Наименование        "));
  w->Add(BuildCell(T21,2,f21,1,&wh,w->w,E_STYLE21 ));

  w->Add(new _Static(1,(wh),E_STYLE21,"Номер статьи        "));
  w->Add(new _RelCCell(21,(wh++),50,E_STYLE21,5,(char*)(f21+T21->FieldOffset(3)),10,0,2 ));

  //wh++;
  w->AddCallBack(EditF21Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void InsertF21(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  memset(f21,0,sizeof(f21));
  InitRecord(T21,f21);
  _Window *w= new _Window(1,1,73,1,E_STYLE21,stBuffered | stFrame,"InsertF21");
  fv->Add(w); short wh=1;   w->HelpID=19;

  w->Add(new _Static(1,(wh++),E_STYLE21,"Наименование        "));
  w->Add(BuildCell(T21,2,f21,1,&wh,w->w,E_STYLE21 ));

  w->Add(new _Static(1,(wh),E_STYLE21,"Номер статьи        "));
  w->Add(new _RelCCell(21,(wh++),50,E_STYLE21,5,(char*)(f21+T21->FieldOffset(3)),10,0,2 ));

  w->AddCallBack(InsertF21Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void DeleteF21(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  fv->Delete();
  E->ClearEvent((_Window*)parent);
};

static void PrintF21(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  char name[16];
  Report(T21,fv->KeyNum,name);
  _ShowPrint *sp = new _ShowPrint(name,"Печать Справочник_работ");
  fv->Add(sp); sp->HelpID=100;
  sp->GotFocus();
  E->ClearEvent((_Window*)parent);
};

void AddF21( void (*CallBackFunc)(_Event *,void *), _Window * parent){
  if(!Dostup[21])
    return;
  _Window * tmp =parent;
  if(!tmp) tmp =GDt;
  if(tmp->Find("Справочник_работ")){
    if(tmp ==GDt){
      MessageBox("Окно 'Справочник_работ' уже доступно,",
                 "для переключения используйте: ",
                 "Alt + "
                );
    }else{
      MessageBox("Окно 'Справочник_работ' уже доступно,",
                 "его нельзя активизировать еще раз"
                );
    }
    return;
  }
  _FileView * fv=new _FileView (T21,0,0,0,WriteF21,2,2,75,20,1,V_STYLE21,"Справочник_работ");
  fv->HelpID=7;
  fv->Add(new _Static(2,0,V_STYLE21," Справочник_работ "));
  if(Dostup[21]==D_WRITE){
    fv->AddCallBack(InsertF21, evKey,INSERT);
    fv->AddCallBack(EditF21, evKey,ENTER);
    fv->AddCallBack(DeleteF21, evKey,DELETE);
  }
  fv->AddCallBack(PrintF21, evKey, CTRL_P);

  if(CallBackFunc)
        fv->AddCallBack(CallBackFunc, evKey, ESC);
  tmp->Add(fv);
  tmp->SwitchFocus(fv);
};


