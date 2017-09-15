/****************************************************
 *        F30.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"
#include "doc.h"


static short ShowMask30[]={
       //1,   // Код_партнера
       2,   // Номер счета
       3,   // Псевдоним
       0    // end_of_list
};

file_30 * rf30;
Table *T30;
static char  * f30;
file_30_0 filter;

void Init30(void){
  T30= new Table("Счета_партнеров");
  if(!T30->opened){
	printf("Error: file Счета_партнеров not opened\n");
        exit(0);
  }
  f30=new char [T30->max_rec_len];
  rf30 = (file_30*)f30;
};

void Close30(void){
  delete T30;
  delete f30;
};

static void WriteF30(void *rec, char *buf){
    int curlen=0,sz;
    buf[0]=0;
    char *buf2;
    int i=0;
    while(ShowMask30[i]){
      sz=T30->SizeForPrint(ShowMask30[i]);
      if(curlen+sz+1<MAX_STRING_WIDTH){
        buf2 = new char[sz+1];
        memset(buf2,0,sz+1);
        T30->PrintField(ShowMask30[i], rec ,buf2);
        strcat(buf,buf2);
        strcat(buf,"│");
        curlen=curlen+sz+1;
        delete buf2;
      }
      i++;
    }
};


static void EditF30Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Update(f30);
  E->msg.K=ESC;
};

static void InsertF30Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Insert(f30);
  E->msg.K=ESC;
};

static void EditF30(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  fv->Lock(f30);
  _Window *w= new _Window(1,1,73,1,E_STYLE30,stBuffered | stFrame,"EditF30");
  fv->Add(w);
  short wh=1;
  _Cell *cell;
  w->Add(new _Static(1,(wh)++,E_STYLE30,"Номер счета         "));
  w->Add(cell=new _RelCCell(1,(wh++),40,E_STYLE30,15,rf30->Nomer_sheta,12,0,2));
  //w->Add(BuildCell(T30,2,f30,1,&wh,w->w,E_STYLE30 ));
  cell->st|=stLocked;

  w->Add(new _Static(1,(wh)++,E_STYLE30,"Псевдоним           "));
  w->Add(BuildCell(T30,3,f30,1,&wh,w->w,E_STYLE30 ));

  w->AddCallBack(EditF30Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void InsertF30(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  memset(f30,0,sizeof(f30));
  InitRecord(T30,f30);
  _Window *w= new _Window(1,1,73,1,E_STYLE30,stBuffered | stFrame,"InsertF30");
  fv->Add(w);
  short wh=1;

  rf30->Kod_partnera=filter.Kod_partnera;

  w->Add(new _Static(1,(wh)++,E_STYLE30,"Номер счета         "));
  w->Add(new _RelCCell(1,(wh++),40,E_STYLE30,15,rf30->Nomer_sheta,12,0,2));
  //w->Add(BuildCell(T30,2,f30,1,&wh,w->w,E_STYLE30 ));

  w->Add(new _Static(1,(wh)++,E_STYLE30,"Псевдоним           "));
  w->Add(BuildCell(T30,3,f30,1,&wh,w->w,E_STYLE30 ));

  w->AddCallBack(InsertF30Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void DeleteF30(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  fv->Delete();
  E->ClearEvent((_Window*)parent);
};

static void PrintF30(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  char name[16];
  Report(T30,fv->KeyNum,name);
  _ShowPrint *sp = new _ShowPrint(name,"Печать Псевдонимы");
  fv->Add(sp);
  sp->GotFocus();
  E->ClearEvent((_Window*)parent);
};

//127-61-40
void AddF30( void (*CallBackFunc)(_Event *,void *), _Window * parent){
  _Window * tmp =parent;
  if(!tmp) tmp =GDt;
  if(tmp->Find("Псевдонимы")){
    if(tmp ==GDt){
      MessageBox("Окно 'Псевдонимы' уже доступно,",
                 "для переключения используйте: ",
                 "Alt + "
                );
    }else{
      MessageBox("Окно 'Псевдонимы' уже доступно,",
                 "его нельзя активизировать еще раз"
                );
    }
    return;
  }
  _FileView * fv=new _FileView (T30,0,&filter,2,WriteF30,2,2,75,20,1,V_STYLE30,"Псевдонимы");
  fv->Add(new _Static(2,0,V_STYLE30," Псевдонимы "));
  fv->AddCallBack(InsertF30, evKey,INSERT);
  fv->AddCallBack(EditF30, evKey,ENTER);
  fv->AddCallBack(DeleteF30, evKey,DELETE);
  fv->AddCallBack(PrintF30, evKey, CTRL_P);
  if(CallBackFunc)
        fv->AddCallBack(CallBackFunc, evKey, ESC);
  tmp->Add(fv);
  tmp->SwitchFocus(fv);
};


