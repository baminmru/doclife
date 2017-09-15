/****************************************************
 *        F11.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"
#include "doc.h"

static short f11_year;

static short ShowMask11[]={
       1,   // Номер_статьи
       7,   // Дата уточнения
       6,   // Замена
       2,   // Сумма_на_1_кв
       3,   // Сумма_на_2_кв
       4,   // Сумма_на_3_кв
       5,   // Сумма_на_4_кв
       //8,   // Год
       0    // end_of_list
};

file_11 * rf11;
Table *T11;
static char  * f11;

void Init11(void){
  T11= new Table("Смета_из_Москвы",OWNER);
  if(!T11->opened){
	printf("Error: file Смета_из_Москвы not opened\n");
        exit(0);
  }
  f11=new char [T11->max_rec_len];
  rf11 = (file_11*)f11;
};

void Close11(void){
  delete T11;
  delete f11;
};

static void WriteF11(void *rec, char *buf){
    int curlen=0,sz;
    buf[0]=0;
    char *buf2;
    int i=0;
    while(ShowMask11[i]){
      sz=T11->SizeForPrint(ShowMask11[i]);
      if(curlen+sz+1<MAX_STRING_WIDTH){
        buf2 = new char[sz+1];
        memset(buf2,0,sz+1);
        T11->PrintField(ShowMask11[i], rec ,buf2);
        strcat(buf,buf2);
        strcat(buf,"│");
        curlen=curlen+sz+1;
        delete buf2;
      }
      i++;
    }
};


static void EditF11Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Update(f11);
  E->msg.K=ESC;
};

static void InsertF11Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Insert(f11);
  E->msg.K=ESC;
};

static void EditF11(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f11)) return;
  _Window *w= new _Window(1,1,73,1,E_STYLE11,stBuffered | stFrame,"EditF11");
  fv->Add(w); short wh=1;   w->HelpID=19;

  w->Add(new _Static(1,(wh),E_STYLE11,"Номер_статьи        "));
  //w->Add(BuildCell(T11,1,f11,21,&wh,w->w,E_STYLE11 ));
  w->Add(new _RelCCell(21,(wh++),50,E_STYLE21,5,(char*)(f11+T11->FieldOffset(1)),10,0,2 ));

  w->Add(new _Static(1,(wh),E_STYLE11,"Дата уточнения      "));
  w->Add(BuildCell(T11,7,f11,21,&wh,w->w,E_STYLE11 ));

  w->Add(new _Static(1,(wh),E_STYLE11,"Замена              "));
  w->Add(BuildCell(T11,6,f11,21,&wh,w->w,E_STYLE11 ));

  w->Add(new _Static(1,(wh),E_STYLE11,"Сумма_на_1_кв       "));
  w->Add(BuildCell(T11,2,f11,21,&wh,w->w,E_STYLE11 ));

  w->Add(new _Static(1,(wh),E_STYLE11,"Сумма_на_2_кв       "));
  w->Add(BuildCell(T11,3,f11,21,&wh,w->w,E_STYLE11 ));

  w->Add(new _Static(1,(wh),E_STYLE11,"Сумма_на_3_кв       "));
  w->Add(BuildCell(T11,4,f11,21,&wh,w->w,E_STYLE11 ));

  w->Add(new _Static(1,(wh),E_STYLE11,"Сумма_на_4_кв       "));
  w->Add(BuildCell(T11,5,f11,21,&wh,w->w,E_STYLE11 ));

  ////wh++;
  w->AddCallBack(EditF11Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void InsertF11(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  memset(f11,0,sizeof(f11));
  InitRecord(T11,f11);
  rf11->God=f11_year;
  _Window *w= new _Window(1,1,73,1,E_STYLE11,stBuffered | stFrame,"InsertF11");
  fv->Add(w); short wh=1;   w->HelpID=19;

  w->Add(new _Static(1,(wh),E_STYLE11,"Номер_статьи        "));
  w->Add(new _RelCCell(21,(wh++),50,E_STYLE21,5,(char*)(f11+T11->FieldOffset(1)),10,0,2 ));
  //w->Add(BuildCell(T11,1,f11,21,&wh,w->w,E_STYLE11 ));

  w->Add(new _Static(1,(wh),E_STYLE11,"Дата уточнения      "));
  w->Add(BuildCell(T11,7,f11,21,&wh,w->w,E_STYLE11 ));

  w->Add(new _Static(1,(wh),E_STYLE11,"Замена              "));
  w->Add(BuildCell(T11,6,f11,21,&wh,w->w,E_STYLE11 ));

  w->Add(new _Static(1,(wh),E_STYLE11,"Сумма_на_1_кв       "));
  w->Add(BuildCell(T11,2,f11,21,&wh,w->w,E_STYLE11 ));

  w->Add(new _Static(1,(wh),E_STYLE11,"Сумма_на_2_кв       "));
  w->Add(BuildCell(T11,3,f11,21,&wh,w->w,E_STYLE11 ));

  w->Add(new _Static(1,(wh),E_STYLE11,"Сумма_на_3_кв       "));
  w->Add(BuildCell(T11,4,f11,21,&wh,w->w,E_STYLE11 ));

  w->Add(new _Static(1,(wh),E_STYLE11,"Сумма_на_4_кв       "));
  w->Add(BuildCell(T11,5,f11,21,&wh,w->w,E_STYLE11 ));

  w->AddCallBack(InsertF11Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void DeleteF11(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  fv->Delete();
  E->ClearEvent((_Window*)parent);
};

static void PrintF11(_Event *E, void * parent);

void AddF11( void (*CallBackFunc)(_Event *,void *), _Window * parent){
  if(!Dostup[11])
    return;
  _Window * tmp =parent;
  if(!tmp) tmp =GDt;
  if(tmp->Find("Смета_из_Москвы")){
    if(tmp ==GDt){
      MessageBox("Окно 'Смета_из_Москвы' уже доступно,",
                 "для переключения используйте: ",
                 "Alt + "
                );
    }else{
      MessageBox("Окно 'Смета_из_Москвы' уже доступно,",
                 "его нельзя активизировать еще раз"
                );
    }
    return;
  }

  _Date td;
  td.Today();
  f11_year = td.y;
  _Window *y_win = new _Window(0,0,30,3,ST_WINDOW, stBuffered | stFrame ,"Задание года");
  y_win->CenterScr();
  y_win->Add(new _Static (1,1,ST_WINDOW,"Задайте год "));
  y_win->Add(new _ShortCell(15,1,ST_WINDOW,4,&f11_year,0));
  y_win->Exec();
  delete y_win;


  _FileView * fv=new _FileView (T11,0,&f11_year,2,WriteF11,2,2,75,20,1,V_STYLE11,"Смета_из_Москвы");
  fv->HelpID = 13;
  fv->Add(new _Static(2,0,V_STYLE11," Смета_из_Москвы "));
  if(Dostup[11]==D_WRITE){
    fv->AddCallBack(InsertF11, evKey,INSERT);
    fv->AddCallBack(EditF11, evKey,ENTER);
    fv->AddCallBack(DeleteF11, evKey,DELETE);
    fv->AddCallBack(PrintF11, evKey, CTRL_P);
  }
  if(CallBackFunc)
        fv->AddCallBack(CallBackFunc, evKey, ESC);
  tmp->Add(fv);
  tmp->SwitchFocus(fv);
};


static char *h[]=
{
  "",
  "┌──────────┬───────────────────┬───────────────────┬───────────────────┬───────────────────┬───────────────────┐",
  "│ Статья   │   I квартал       │   II квартал      │   III квартал     │   IV квартал      │        Год        │",
  "├──────────┼───────────────────┼───────────────────┼───────────────────┼───────────────────┼───────────────────┤"
};
static char *f=
{
  "└──────────┴───────────────────┴───────────────────┴───────────────────┴───────────────────┴───────────────────┘"
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


static void PrintF11(_Event *E, void * parent)
{

   char h0[50];
   sprintf(h0,"                   Смета расходов на %d год.",f11_year);
   h[0]=h0;
  _FileView *fv= (_FileView*) parent;
  char name[16],pbuf[150];
  _LowPrint *lp = new _LowPrint(65,name);
  lp->SetHeader(Head);
  lp->SetFooter(1,Foot);
  char a[6];
  file_11 sum,total;
  file_11_0 k11;
  memset(&k11,0,sizeof(k11));
  k11.God=f11_year;
  int res =T11->GGE(rf11,&k11,0);
  if(!res && rf11->God==f11_year){
    memset(&sum,0,sizeof(sum));
    memset(&total,0,sizeof(sum));
    memcpy(a,rf11->Nomer_statji,6);
  }
  while(!res && rf11->God==f11_year){
    if(memcmp(a,rf11->Nomer_statji,6)){
      sprintf(pbuf,"│%-10s│%19.2lf│%19.2lf│%19.2lf│%19.2lf│%19.2lf│"
                  , a
                  ,sum.Summa_na_1_kv
                  ,sum.Summa_na_2_kv
                  ,sum.Summa_na_3_kv
                  ,sum.Summa_na_4_kv
                  ,sum.Summa_na_1_kv
                  +sum.Summa_na_2_kv
                  +sum.Summa_na_3_kv
                  +sum.Summa_na_4_kv
                  );
      lp->PrintNL(pbuf);
      total.Summa_na_1_kv+=sum.Summa_na_1_kv;
      total.Summa_na_2_kv+=sum.Summa_na_2_kv;
      total.Summa_na_3_kv+=sum.Summa_na_3_kv;
      total.Summa_na_4_kv+=sum.Summa_na_4_kv;
      memset(&sum,0,sizeof(sum));
      memcpy(a,rf11->Nomer_statji,6);
    }
    if(rf11->Zamena){
      if(rf11->Summa_na_1_kv)
        sum.Summa_na_1_kv=rf11->Summa_na_1_kv;
      if(rf11->Summa_na_2_kv)
        sum.Summa_na_2_kv=rf11->Summa_na_2_kv;
      if(rf11->Summa_na_3_kv)
        sum.Summa_na_3_kv=rf11->Summa_na_3_kv;
      if(rf11->Summa_na_4_kv)
        sum.Summa_na_4_kv=rf11->Summa_na_4_kv;

    }else{
      sum.Summa_na_1_kv+=rf11->Summa_na_1_kv;
      sum.Summa_na_2_kv+=rf11->Summa_na_2_kv;
      sum.Summa_na_3_kv+=rf11->Summa_na_3_kv;
      sum.Summa_na_4_kv+=rf11->Summa_na_4_kv;
    }

    res = T11->GNext(rf11);

  }
  if(
      sum.Summa_na_1_kv
      +sum.Summa_na_2_kv
      +sum.Summa_na_3_kv
      +sum.Summa_na_4_kv
  ){
    sprintf(pbuf,"│%-10s│%19.2lf│%19.2lf│%19.2lf│%19.2lf│%19.2lf│"
                  , a
                  ,sum.Summa_na_1_kv
                  ,sum.Summa_na_2_kv
                  ,sum.Summa_na_3_kv
                  ,sum.Summa_na_4_kv
                  ,sum.Summa_na_1_kv
                  +sum.Summa_na_2_kv
                  +sum.Summa_na_3_kv
                  +sum.Summa_na_4_kv
                  );
    lp->PrintNL(pbuf);
    total.Summa_na_1_kv+=sum.Summa_na_1_kv;
    total.Summa_na_2_kv+=sum.Summa_na_2_kv;
    total.Summa_na_3_kv+=sum.Summa_na_3_kv;
    total.Summa_na_4_kv+=sum.Summa_na_4_kv;

  }

  if(
      total.Summa_na_1_kv
      +total.Summa_na_2_kv
      +total.Summa_na_3_kv
      +total.Summa_na_4_kv
  ){
    lp->PrintNL(h[3]);
    sprintf(pbuf,"│%-10s│%19.2lf│%19.2lf│%19.2lf│%19.2lf│%19.2lf│"
                ,"Всего"
                ,total.Summa_na_1_kv
                ,total.Summa_na_2_kv
                ,total.Summa_na_3_kv
                ,total.Summa_na_4_kv
                ,total.Summa_na_1_kv
                +total.Summa_na_2_kv
                +total.Summa_na_3_kv
                +total.Summa_na_4_kv
                );
    lp->PrintNL(pbuf);
  }

   delete lp;
  _ShowPrint *sp = new _ShowPrint(name,"Печать Движение_средств");
  fv->Add(sp); sp->HelpID=100;
  sp->GotFocus();
  E->ClearEvent((_Window*)parent);
};




