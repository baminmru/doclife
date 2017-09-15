/****************************************************
 *        F19.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"
#include "doc.h"
#include "f19.h"

struct account {
  char    Nomer_sheta[16] ; //Номер_счета
  double sum;
  double cur;
};

extern int AccNum;
extern account *acc;

double CountAll(void);
int FindAcc( char * num);
int  CalculateChanges(void);


static short ShowMask19[]={
       1,   // Номер_договора
       2,   // Сумма
       3,   // Номер_счета
       4,   // Дата
       0    // end_of_list
};

file_19 * rf19;
Table *T19;
static char  * f19;

void Init19(void){
  T19= new Table("Предложение_к_оплате",OWNER);
  if(!T19->opened){
	printf("Error: file Предложение_к_оплате not opened\n");
        exit(0);
  }
  f19=new char [T19->max_rec_len];
  rf19 = (file_19*)f19;
};

void Close19(void){
  delete T19;
  delete f19;
};

static void WriteF19(void *rec, char *buf){
    file_19 * rr=(file_19*)rec;
    int curlen=0,sz;
    buf[0]=0;
    char *buf2;
    int i=0;
    if(rr->Razoviy_shet)
      strcpy(buf,"Счет     ");
    else
      strcpy(buf,"Договор: ");

    while(ShowMask19[i]){
      sz=T19->SizeForPrint(ShowMask19[i]);
      if(curlen+sz+1<MAX_STRING_WIDTH){
        buf2 = new char[sz+1];
        memset(buf2,0,sz+1);
        T19->PrintField(ShowMask19[i], rec ,buf2);
        strcat(buf,buf2);
        strcat(buf,"│");;
        curlen=curlen+sz+1;
        delete buf2;
      }
      i++;
    }
};


static void EditF19Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Update(f19);
  CalculateChanges();
  E->msg.K=ESC;
};

static void InsertF19Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Insert(f19);
  CalculateChanges();
  E->msg.K=ESC;
};

static void EditF19(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f19)) return;
  _Window *w= new _Window(1,1,50,1,E_STYLE19,stBuffered | stFrame,"EditF19");
  fv->Add(w); short wh=1;   w->HelpID=19;

  //w->Add(new _Static(1,(wh++),E_STYLE19,"Разовый счет      "));
  //w->Add(BuildCell(T19,5,f19,21,&wh,w->w,E_STYLE19 ));

  if(!rf19->Razoviy_shet){
    w->Add(new _Static(1,(wh++),E_STYLE19,"Номер договора      "));
    w->Add(new _RelCCell(1,(wh++),40,E_STYLE19,12,rf19->Nomer_dogovora,17,0,11));
  }else{
    w->Add(new _Static(1,(wh++),E_STYLE19,"Номер счета      "));
    w->Add(new _RelCCell(1,(wh++),40,E_STYLE19,12,rf19->Nomer_dogovora,28,0,1));
  }

  w->Add(new _Static(1,(wh),E_STYLE19,"Сумма               "));
  w->Add(BuildCell(T19,2,f19,21,&wh,w->w,E_STYLE19 ));

  w->Add(new _Static(1,(wh++),E_STYLE19,"Номер счета для оплаты"));
  w->Add(new _RelCCell(1,(wh++),40,E_STYLE19,15,rf19->Nomer_sheta,12,0,2));

  w->Add(new _Static(1,(wh),E_STYLE19,"Дата                "));
  w->Add(BuildCell(T19,4,f19,21,&wh,w->w,E_STYLE19 ));
  //wh++;
  w->AddCallBack(EditF19Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void InsertF19(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  memset(f19,0,sizeof(f19));
  InitRecord(T19,f19);

  _Window *w= new _Window(1,1,73,1,E_STYLE19,stBuffered | stFrame,"InsertF19");
  fv->Add(w); short wh=1;   w->HelpID=19;


  w->Add(new _Static(1,(wh),E_STYLE19,"Разовый счет      "));
  w->Add(BuildCell(T19,5,f19,21,&wh,w->w,E_STYLE19 ));

  //w->Add(new _Static(1,(wh++),E_STYLE19,"Номер_договора      "));
  //w->Add(new _RelCCell(1,(wh++),40,E_STYLE19,15,rf19->Nomer_dogovora,17,0,11));

  //w->Add(new _Static(1,(wh++),E_STYLE19,"Сумма               "));
  //w->Add(BuildCell(T19,2,f19,21,&wh,w->w,E_STYLE19 ));

  w->Add(new _Static(1,(wh++),E_STYLE19,"Номер счета для оплаты"));
  w->Add(new _RelCCell(1,(wh++),40,E_STYLE19,15,rf19->Nomer_sheta,12,0,2));

  w->Add(new _Static(1,(wh),E_STYLE19,"Дата                "));
  w->Add(BuildCell(T19,4,f19,21,&wh,w->w,E_STYLE19 ));
  w->AddCallBack(InsertF19Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void DeleteF19(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  fv->Delete();
  E->ClearEvent((_Window*)parent);
};

static void PrintF19(_Event *E, void * parent);

void AddF19( void (*CallBackFunc)(_Event *,void *), _Window * parent){
  if(!Dostup[19])
    return;
  _Window * tmp =parent;
  if(!tmp) tmp =GDt;
  if(tmp->Find("Предложение_к_оплате")){
    if(tmp ==GDt){
      MessageBox("Окно 'Предложение_к_оплате' уже доступно,",
                 "для переключения используйте: ",
                 "Alt + "
                );
    }else{
      MessageBox("Окно 'Предложение_к_оплате' уже доступно,",
                 "его нельзя активизировать еще раз"
                );
    }
    return;
  }
  _FileView * fv=new _FileView (T19,0,0,0,WriteF19,2,2,75,20,1,V_STYLE19,"Предложение_к_оплате");
  fv->HelpID=18;
  fv->Add(new _Static(2,0,V_STYLE19," Предложение_к_оплате "));
  if(Dostup[19]==D_WRITE){
    fv->AddCallBack(InsertF19, evKey,INSERT);
    fv->AddCallBack(EditF19, evKey,ENTER);
    fv->AddCallBack(DeleteF19, evKey,DELETE);
  }
  fv->AddCallBack(PrintF19, evKey, CTRL_P);
  if(CallBackFunc)
        fv->AddCallBack(CallBackFunc, evKey, ESC);
  tmp->Add(fv);
  tmp->SwitchFocus(fv);
};

int  CalculateChanges(void){
  double *sum2;
  CountAll();

  sum2 =  new double [AccNum+1];
  memset(sum2,0,sizeof(double)*(AccNum+1));
  int res;
  res = T19->GFirst(rf19);
  while(!res){
    sum2[FindAcc(rf19->Nomer_sheta)]+=rf19->Summa;
    res = T19->GNext(rf19);
  }
  res=0;
  for(int i=0;i<=AccNum;i++){
    if(sum2[i]>acc[i].sum){
      res=i+1;
      break;
    }
  }
  if(res){
    char buf[25];
    sprintf(buf,"%20.2lf",-acc[res-1].sum+sum2[res-1]);
    if(res>1){
      MessageBox(
        "                 Внимание !!!",
        "Сумма предпологаемая к оплате превышает  имеющиеся",
        "средства, например по счету :",
        acc[res-1].Nomer_sheta,
        "превышение состовляет:",
        buf,
        "          нажмите Esc для продолжения");
    }else{
      MessageBox(
        "                 Внимание !!!",
        "Сумма предпологаемая к оплате превышает  имеющиеся",
        "средства, например по счету, с номером, почему-то",
        "не включенным в справочник счетов ",
        "превышение состовляет:",
        buf,
        "          нажмите Esc для продолжения");

    }

  }
  delete acc;
  acc=0;
  delete sum2;
  return res;
};


static char *h[]=
{
  "                       Исходя из наличия средств на счетах ",
  "┌──────────────────────────────────────────────────────────┬───────────────────┐",
  "│Договор, партнер, реквизиты                               │     Сумма         │",
  "├──────────────────────────────────────────────────────────┼───────────────────┤"

};
static char *f=
{
  "└──────────────────────────────────────────────────────────┴───────────────────┘"
};
static char *blank=
{
  "│                                                          │                   │"
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


static void PrintF19(_Event *E, void * parent)
{
   file_17 *r17= new file_17;
  _FileView *fv= (_FileView*) parent;
  char name[16],pbuf[90];
  _LowPrint *lp = new _LowPrint(65,name);
  lp->SetHeader(Head);
  lp->SetFooter(1,Foot);

  int res =T19->GFirst(rf19,1);
  static char a[16];
  file_12 r12;

  double sum,total=0.0;
  _Date td;
  td.Today();
  if(!res){
    sum = 0;
    memcpy(a,rf19->Nomer_sheta,16);
    sprintf(pbuf,"│%-15s %16s %-25s│%19s│","Со счета ",a,"оплатить следующие суммы:","");
    lp->PrintNL(pbuf);
   }
  while(!res){
    if(memcmp(a,rf19->Nomer_sheta,16)){
      sprintf(pbuf,"│%-20s %16s %-20s│%19.2lf│","Всего со счета", a , "следует оплатить",sum);
      lp->PrintNL(pbuf);
      sum=0;
      lp->PrintNL(h[3]);
      memcpy(a,rf19->Nomer_sheta,16);
      sprintf(pbuf,"│%-15s %16s %-25s│%19s│","Со счета ",a,"оплатить следующие суммы:","");
      lp->PrintNL(pbuf);
    }
    if(!rf19->Razoviy_shet){
      if(!T17->GEQ(r17,rf19->Nomer_dogovora,0)){
        if(T20->GEQ(rf20,&r17->Kod_partnera,0))
          StrClear(rf20->Naimenovanie,35);
        rf20->Naimenovanie[35]=0;
        sprintf(pbuf,"│Дог.%16s c %35s│%19.2lf│",rf19->Nomer_dogovora,rf20->Naimenovanie,rf19->Summa);
        lp->PrintNL(pbuf);

        r17->Kratkoe_opisanie[58]=0;
        sprintf(pbuf,"│%58s│%19s│",r17->Kratkoe_opisanie,"");
        lp->PrintNL(pbuf);

        sum+=rf19->Summa;
        total+=rf19->Summa;

        if(!T20->GEQ(rf20,&r17->Kod_poluhately,0)){
          lp->PrintNL(pbuf);
        }
        lp->PrintNL(blank);
      }
    }else{
      if(!T28->GEQ(rf28,rf19->Nomer_dogovora,0)){
        if(T20->GEQ(rf20,&rf28->Kod_partnera,0))
          StrClear(rf20->Naimenovanie,35);
        rf20->Naimenovanie[35]=0;
        sprintf(pbuf,"│Сч. %16s c %35s│%19.2lf│",rf19->Nomer_dogovora,rf20->Naimenovanie,rf19->Summa);
        sum+=rf19->Summa;
        total+=rf19->Summa;
        lp->PrintNL(pbuf);
        lp->PrintNL(blank);
      }
    }
    res = T19->GNext(rf19);
  }
  if(sum){
    sprintf(pbuf,"│%-20s %16s %-20s│%19.2lf│","Всего со счета", a , "следует оплатить",sum);
    lp->PrintNL(pbuf);
  }
  if(total){
    lp->PrintNL(h[3]);
    sprintf(pbuf,"│%-58s│%19.2lf│","Всего к оплате",total);
    lp->PrintNL(pbuf);
  }

  delete lp;
  delete r17;
  _ShowPrint *sp = new _ShowPrint(name,"Печать Предложение_к_оплате");
  fv->Add(sp); sp->HelpID=100;
  sp->GotFocus();
  E->ClearEvent((_Window*)parent);
};

