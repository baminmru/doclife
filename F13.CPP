/****************************************************
 *        F13.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"
#include "doc.h"
#include "f13.h"


static short ShowMask13[]={
       1,   // Номер счета
       2,   // дата
       3,   // Сумма
       //4,   // Код_договора
       // 5,   // Расход
       0    // end_of_list
};

file_13 * rf13;
Table *T13;
static char  * f13;

void Init13(void){
  T13= new Table("Движение_средств",OWNER);
  if(!T13->opened){
	printf("Error: file Движение_средств not opened\n");
        exit(0);
  }
  f13=new char [T13->max_rec_len];
  rf13 = (file_13*)f13;
};

void Close13(void){
  delete T13;
  delete f13;
};

static void WriteF13(void *rec, char *buf){
    int curlen=0,sz;
    buf[0]=0;
    char *buf2;
    int i=0;
    while(ShowMask13[i]){
      sz=T13->SizeForPrint(ShowMask13[i]);
      if(curlen+sz+1<MAX_STRING_WIDTH){
        buf2 = new char[sz+1];
        memset(buf2,0,sz+1);
        T13->PrintField(ShowMask13[i], rec ,buf2);
        strcat(buf,buf2);
        strcat(buf,"│");;
        curlen=curlen+sz+1;
        delete buf2;
      }
      i++;
    }
};


static void EditF13Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Update(f13);
  E->msg.K=ESC;
};

static void InsertF13Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Insert(f13);
  E->msg.K=ESC;
};

static void EditF13(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f13)) return;
  _Window *w= new _Window(1,1,45,1,E_STYLE13,stBuffered | stFrame,"EditF13");
  fv->Add(w); short wh=1;   w->HelpID=19;

  w->Add(new _Static(1,(wh++),E_STYLE13,"Номер счета         "));
  w->Add(new _RelCCell(1,(wh++),40,E_STYLE13,15,rf13->Nomer_sheta,12,0,2));

  w->Add(new _Static(1,(wh),E_STYLE13,"дата                "));
  w->Add(BuildCell(T13,2,f13,21,&wh,w->w,E_STYLE13 ));

  w->Add(new _Static(1,(wh),E_STYLE13,"Сумма               "));
  w->Add(BuildCell(T13,3,f13,21,&wh,w->w,E_STYLE13 ));

  //w->Add(new _Static(1,(wh),E_STYLE13,"Расход              "));
  //w->Add(BuildCell(T13,5,f13,21,&wh,w->w,E_STYLE13 ));

  //w->Add(new _Static(1,(wh++),E_STYLE13,"Код_договора        "));
  //w->Add(new _RelCCell(1,(wh++),40,E_STYLE13,15,rf13->Kod_dogovora,17,0,11));

  //wh++;
  w->AddCallBack(EditF13Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void InsertF13(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  memset(f13,0,sizeof(f13));
  InitRecord(T13,f13);
  _Window *w= new _Window(1,1,45,1,E_STYLE13,stBuffered | stFrame,"InsertF13");
  fv->Add(w); short wh=1;   w->HelpID=19;

  w->Add(new _Static(1,(wh++),E_STYLE13,"Номер счета         "));
  w->Add(new _RelCCell(1,(wh++),40,E_STYLE13,15,rf13->Nomer_sheta,12,0,2));

  w->Add(new _Static(1,(wh),E_STYLE13,"дата                "));
  w->Add(BuildCell(T13,2,f13,21,&wh,w->w,E_STYLE13 ));

  w->Add(new _Static(1,(wh),E_STYLE13,"Сумма               "));
  w->Add(BuildCell(T13,3,f13,21,&wh,w->w,E_STYLE13 ));

  //w->Add(new _Static(1,(wh),E_STYLE13,"Расход              "));
  //w->Add(BuildCell(T13,5,f13,21,&wh,w->w,E_STYLE13 ));

  //w->Add(new _Static(1,(wh++),E_STYLE13,"Код_договора        "));
  //w->Add(new _RelCCell(1,(wh++),40,E_STYLE13,15,rf13->Kod_dogovora,17,0,11));

  //wh++;
  w->AddCallBack(InsertF13Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void DeleteF13(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  fv->Delete();
  E->ClearEvent((_Window*)parent);
};

static void PrintF13(_Event *E, void * parent);

void AddF13( void (*CallBackFunc)(_Event *,void *), _Window * parent){
  if(!Dostup[13])
    return;
  _Window * tmp =parent;
  if(!tmp) tmp =GDt;
  if(tmp->Find("Выписки_банка")){
    if(tmp ==GDt){
      MessageBox("Окно 'Выписки_банка' уже доступно,",
                 "для переключения используйте: ",
                 "Alt + "
                );
    }else{
      MessageBox("Окно 'Выписки_банка' уже доступно,",
                 "его нельзя активизировать еще раз"
                );
    }
    return;
  }
  _FileView * fv=new _FileView (T13,0,0,0,WriteF13,2,2,75,20,1,V_STYLE13,"Выписки_банка");
  fv->HelpID=14;
  fv->Add(new _Static(2,0,V_STYLE13," Выписки_банка "));
  if(Dostup[13]==D_WRITE){
    fv->AddCallBack(InsertF13, evKey,INSERT);
    fv->AddCallBack(EditF13, evKey,ENTER);
    fv->AddCallBack(DeleteF13, evKey,DELETE);
  }
  fv->AddCallBack(PrintF13, evKey, CTRL_P);
  if(CallBackFunc)
        fv->AddCallBack(CallBackFunc, evKey, ESC);
  tmp->Add(fv);
  tmp->SwitchFocus(fv);
};




static char *h[]=
{
  "         Справка о состоянии средств на счетах",
  "┌──────────────────────────────┬───────────────────┐",
  "│Номер счета                   │     Сумма         │",
  "├──────────────────────────────┼───────────────────┤"

};
static char *f=
{
  "└──────────────────────────────┴───────────────────┘"
};

static void Head(_LowPrint *lp ,int page){
 for(int i=0; i<4 ;i++)
  lp->PrintNL(h[i]);
}

static void Foot(_LowPrint *lp ,int page){
  char * pbuf = new char [strlen(f)+1];
  strcpy(pbuf,f);
  sprintf(pbuf+2," стр. %d ",page);
  pbuf[strlen(pbuf)]='─';
  lp->PrintNL(pbuf);
  delete pbuf;
}


static void PrintF13(_Event *E, void * parent)
{
  _FileView *fv= (_FileView*) parent;
  char name[16],pbuf[90];
  _LowPrint *lp = new _LowPrint(65,name);
  lp->SetHeader(Head);
  lp->SetFooter(1,Foot);
  char a[16];
  double sum,total=0.0;
  int res =T13->GFirst(rf13,0);
  if(!res){
    sum = 0;
    memcpy(a,rf13->Nomer_sheta,16);
  }
  while(!res){
    if(memcmp(a,rf13->Nomer_sheta,16)){
      sprintf(pbuf,"│%-30s│%19.2lf│", a ,sum);
      lp->PrintNL(pbuf);
      total+=sum;
      sum=0;
      memcpy(a,rf13->Nomer_sheta,16);
    }
    sum=rf13->Summa;

    res = T13->GNext(rf13);

  }
  if(sum){
    sprintf(pbuf,"│%-30s│%19.2lf│", a ,sum);
    lp->PrintNL(pbuf);
    total+=sum;
  }

  if(total){
    lp->PrintNL(h[3]);
    sprintf(pbuf,"│%-30s│%19.2lf│","Всего на счетах",total);
    lp->PrintNL(pbuf);
  }

   delete lp;
  _ShowPrint *sp = new _ShowPrint(name,"Печать Движение_средств");
  fv->Add(sp); sp->HelpID=100;
  sp->GotFocus();
  E->ClearEvent((_Window*)parent);
};

