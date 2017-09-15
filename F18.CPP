/****************************************************
 *        F18.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"
#include "doc.h"
#include "f18.h"


static short ShowMask18[]={
      // 1,   // Номер_договора
       2,   // Дата_оплаты
       3,   // Сумма
       4,   // Номер_счета
       0    // end_of_list
};

file_18 * rf18;
Table *T18;
static char  * f18;
double prevsum=0;

void Init18(void){
  T18= new Table("Оплаты_по_договору",OWNER);
  if(!T18->opened){
	printf("Error: file Оплаты_по_договору not opened\n");
        exit(0);
  }
  f18=new char [T18->max_rec_len];
  rf18 = (file_18*)f18;
};

void Close18(void){
  delete T18;
  delete f18;
};

void InsertCorelation(double sum);

static void WriteF18(void *rec, char *buf){
    int curlen=0,sz;
    buf[0]=0;
    char *buf2;
    int i=0;
    while(ShowMask18[i]){
      sz=T18->SizeForPrint(ShowMask18[i]);
      if(curlen+sz+1<MAX_STRING_WIDTH){
        buf2 = new char[sz+1];
        memset(buf2,0,sz+1);
        T18->PrintField(ShowMask18[i], rec ,buf2);
        strcat(buf,buf2);
        strcat(buf,"│");;
        curlen=curlen+sz+1;
        delete buf2;
      }
      i++;
    }
};


static void EditF18Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  InsertCorelation(prevsum-rf18->Summa);
  fv->Update(f18);
  E->msg.K=ESC;
};

static void InsertF18Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  InsertCorelation(-rf18->Summa);
  fv->Insert(f18);
  E->msg.K=ESC;
};

static void EditF18(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f18)) return;
  prevsum=rf18->Summa;
  _Window *w= new _Window(1,1,73,1,E_STYLE18,stBuffered | stFrame,"EditF18");
  fv->Add(w); short wh=1;   w->HelpID=19;

  //w->Add(new _Static(1,(wh++),E_STYLE18,"Номер_договора      "));
  //w->Add(BuildCell(T18,1,f18,1,&wh,w->w,E_STYLE18 ));

  w->Add(new _Static(1,(wh++),E_STYLE18,"Дата_оплаты         "));
  w->Add(BuildCell(T18,2,f18,1,&wh,w->w,E_STYLE18 ));

  w->Add(new _Static(1,(wh++),E_STYLE18,"Сумма               "));
  w->Add(BuildCell(T18,3,f18,1,&wh,w->w,E_STYLE18 ));

  w->Add(new _Static(1,(wh++),E_STYLE18,"Номер_счета         "));
  //w->Add(BuildCell(T18,4,f18,1,&wh,w->w,E_STYLE18 ));
  w->Add(new _RelCCell(1,(wh++),40,E_STYLE13,15,rf18->Nomer_sheta,12,0,2));

  //wh++;
  w->AddCallBack(EditF18Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void InsertF18(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  memset(f18,0,sizeof(f18));
  InitRecord(T18,f18);
  memcpy(rf18->Nomer_dogovora,rf17->Nomer_dogovora,13);
  _Window *w= new _Window(1,1,73,1,E_STYLE18,stBuffered | stFrame,"InsertF18");
  fv->Add(w); short wh=1;   w->HelpID=19;


  w->Add(new _Static(1,(wh++),E_STYLE18,"Дата_оплаты         "));
  w->Add(BuildCell(T18,2,f18,1,&wh,w->w,E_STYLE18 ));

  w->Add(new _Static(1,(wh++),E_STYLE18,"Сумма               "));
  w->Add(BuildCell(T18,3,f18,1,&wh,w->w,E_STYLE18 ));

  w->Add(new _Static(1,(wh++),E_STYLE18,"Номер_счета         "));
  //w->Add(BuildCell(T18,4,f18,1,&wh,w->w,E_STYLE18 ));
  w->Add(new _RelCCell(1,(wh++),40,E_STYLE13,15,rf18->Nomer_sheta,12,0,2));

  //wh++;
  w->AddCallBack(InsertF18Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void DeleteF18(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f18)) return;
  InsertCorelation(rf18->Summa);
  fv->Delete();
  E->ClearEvent((_Window*)parent);
};

static void PrintF18(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  char name[16];
  Report(T18,fv->KeyNum,name);
  _ShowPrint *sp = new _ShowPrint(name,"Печать Оплаты_по_договору");
  fv->Add(sp); sp->HelpID=100;
  sp->GotFocus();
  E->ClearEvent((_Window*)parent);
};

void AddF18( void (*CallBackFunc)(_Event *,void *), _Window * parent){
  if(!Dostup[18])
    return;
  _Window * tmp =parent;
  if(!tmp) tmp =GDt;
  if(tmp->Find("Оплаты_по_договору")){
    if(tmp ==GDt){
      MessageBox("Окно 'Оплаты_по_договору' уже доступно,",
                 "для переключения используйте: ",
                 "Alt + "
                );
    }else{
      MessageBox("Окно 'Оплаты_по_договору' уже доступно,",
                 "его нельзя активизировать еще раз"
                );
    }
    return;
  }
  _FileView * fv=new _FileView (T18,0,rf17->Nomer_dogovora,13,WriteF18,2,2,75,20,1,V_STYLE18,"Оплаты_по_договору");
  fv->HelpID=11;
  //fv->st &= ~stMaySwitch;
  fv->Add(new _Static(2,0,V_STYLE18," Оплаты_по_договору "));
  if(Dostup[18]==D_WRITE){
    fv->AddCallBack(InsertF18, evKey,INSERT);
    fv->AddCallBack(EditF18, evKey,ENTER);
    fv->AddCallBack(DeleteF18, evKey,DELETE);
  }
  fv->AddCallBack(PrintF18, evKey, CTRL_P);
  if(CallBackFunc)
        fv->AddCallBack(CallBackFunc, evKey, ESC);
  tmp->Add(fv);
  tmp->SwitchFocus(fv);
};


void InsertCorelation(double sum){
  /*
     memcpy(rf13->Nomer_sheta,rf18->Nomer_sheta,16);
     memcpy(rf13->Kod_dogovora,rf18->Nomer_dogovora,13);
     rf13->data=rf18->Data_oplati;
     rf13->Summa=sum;
     rf13->Rashod=1;
     T13->Insert(rf13,0);
  */
}

