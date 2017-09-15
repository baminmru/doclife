/****************************************************
 *        F16.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"
#include "doc.h"
#include "f16.h"



file_16 * rf16;
Table *T16;
static char  * f16;

void Init16(void){
  T16= new Table("Матрица_приоритетов",OWNER);
  if(!T16->opened){
	printf("Error: file Матрица_приоритетов not opened\n");
        exit(0);
  }
  f16=new char [T16->max_rec_len];
  rf16 = (file_16*)f16;
};

void Close16(void){
  delete T16;
  delete f16;
};

static void WriteF16(void *rec, char *buf){
  file_16 * r16 = (file_16*)rec;

  // что такое
  Table *T8 = T8->St.FindTable("X$Classificator");
  file_8 r8;
  file_8_0 k8;
  k8.id=10;
  k8.item=r16->Pokazatelj;
  if(!T8 || T8->GEQ(&r8,&k8,0)){
    StrClear(r8.name,30);
  }
  strcpy(buf,r8.name);
  strcat(buf,"│");

  // сколько стоит
  char d[8];
  sprintf(d,"%6d",r16->Kolihestvo_ballov);
  strcat(buf,d);
  strcat(buf,"│");

  // значение
  switch(r16->Pokazatelj){
    case 1:
     if(T21->GEQ(rf21,&r16->Znahenie_pokazately,0)){
       StrClear(rf21->Naimenovanie,70);
     }
     rf21->Naimenovanie[70]=0;
     strcat(buf,rf21->Naimenovanie);
    break;
    case 2:
     if(T26->GEQ(rf26,&r16->Znahenie_pokazately,0)){
       StrClear(rf26->Nazvanie,70);
     }
     strcat(buf,rf26->Nazvanie);
    break;
    case 3:
     if(T20->GEQ(rf20,&r16->Znahenie_pokazately,0)){
       StrClear(rf20->Naimenovanie,70);
     }
     rf20->Naimenovanie[70]=0;
     strcat(buf,rf20->Naimenovanie);
    break;
    case 4:
     if(T14->GEQ(rf14,&r16->Znahenie_pokazately,0)){
       StrClear(rf14->Naimenovanie_rayona,70);
     }
     strcat(buf,rf14->Naimenovanie_rayona);
    break;

  }

};


static void EditF16Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Update(f16);
  E->msg.K=ESC;
};

static void InsertF16Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  if(fv->Insert(f16))
    GDt->Q.AddMessage(evKey,ENTER);
  E->msg.K=ESC;
};

static void EditF16(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f16)) return;
  _Window *w1,*w= new _Window(1,1,73,1,E_STYLE16,stBuffered | stFrame,"EditF16");
  fv->Add(w); short wh=1;   w->HelpID=19;

  w->Add(new _Static(1,(wh),E_STYLE16,"Показатель :  "));
  w->Add(w1=new _ClsrCell (15,(wh++),30,E_STYLE16,(short*)(f16+T16->FieldOffset(1)),10));
  w1->st|=stLocked;

  w->Add(new _Static(1,(wh++),E_STYLE16,"Значение показателя "));
  switch(rf16->Pokazatelj ){
    case 1:
      w->Add(new _RelSCell(1,(wh++),50,E_STYLE16,5,(short*)(f16+T16->FieldOffset(2)),0,21,0,2 ));
    break;

    case 2:
      w->Add(new _RelSCell(1,(wh++),50,E_STYLE16,5,(short*)(f16+T16->FieldOffset(2)),0,26,0,2 ));
    break;

    case 3:
      w->Add(new _RelSCell(1,(wh++),50,E_STYLE16,5,(short*)(f16+T16->FieldOffset(2)),0,20,0,2 ));
    break;

    case 4:
      w->Add(new _RelSCell(1,(wh++),50,E_STYLE16,5,(short*)(f16+T16->FieldOffset(2)),0,14,0,2 ));
    break;


  }

  w->Add(new _Static(1,(wh++),E_STYLE16,"Количество баллов   "));
  w->Add(BuildCell(T16,3,f16,1,&wh,w->w,E_STYLE16 ));
  w->AddCallBack(EditF16Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void InsertF16(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  memset(f16,0,sizeof(f16));
  InitRecord(T16,f16);
  _Window *w= new _Window(1,1,50,1,E_STYLE16,stBuffered | stFrame,"InsertF16");
  fv->Add(w); short wh=1;   w->HelpID=19;

  w->Add(new _Static(1,(wh),E_STYLE16,"Определите показатель "));
  w->Add(new _ClsrCell (24,(wh++),30,E_STYLE16,(short*)(f16+T16->FieldOffset(1)),10));
  w->AddCallBack(InsertF16Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void DeleteF16(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  fv->Delete();
  E->ClearEvent((_Window*)parent);
};

static void PrintF16(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  char name[16];
  Report(T16,fv->KeyNum,name);
  _ShowPrint *sp = new _ShowPrint(name,"Печать Матрица_приоритетов");
  fv->Add(sp); sp->HelpID=100;
  sp->GotFocus();
  E->ClearEvent((_Window*)parent);
};

void AddF16( void (*CallBackFunc)(_Event *,void *), _Window * parent){
  if(!Dostup[16])
    return;
  _Window * tmp =parent;
  if(!tmp) tmp =GDt;
  if(tmp->Find("Приоритеты по показателям")){
    if(tmp ==GDt){
      MessageBox("Окно 'Приоритеты по показателям' уже доступно,",
                 "для переключения используйте: ",
                 "Alt + "
                );
    }else{
      MessageBox("Окно 'Приоритеты по показателям' уже доступно,",
                 "его нельзя активизировать еще раз"
                );
    }
    return;
  }
  _FileView * fv=new _FileView (T16,0,0,0,WriteF16,2,2,75,20,1,V_STYLE16,"Приоритеты по показателям");
  fv->HelpID=15;
  fv->Add(new _Static(2,0,V_STYLE16," Приоритеты по показателям "));
  if(Dostup[16]==D_WRITE){
    fv->AddCallBack(InsertF16, evKey,INSERT);
    fv->AddCallBack(EditF16, evKey,ENTER);
    fv->AddCallBack(DeleteF16, evKey,DELETE);
  }
  fv->AddCallBack(PrintF16, evKey, CTRL_P);
  if(CallBackFunc)
        fv->AddCallBack(CallBackFunc, evKey, ESC);
  tmp->Add(fv);
  tmp->SwitchFocus(fv);
};


