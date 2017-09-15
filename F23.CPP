/****************************************************
 *        F23.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"
#include "doc.h"
#include "clsctrs.h"



static short ShowMask23[]={
       //1,   // Договор
       //2,   // Статья
       3,   // Дата начала действия
       4,   // Операция
       5,   // Изменяем на
       6,   // Примечание
      // 7,   // Авансовый платеж
      // 8,   // Что изменяется
       0    // end_of_list
};

file_23 * rf23;
Table *T23;
static char  * f23;

void Init23(void){
  T23= new Table("История_статьи",OWNER);
  if(!T23->opened){
	printf("Error: file История_статьи not opened\n");
        exit(0);
  }
  f23=new char [T23->max_rec_len];
  rf23 = (file_23*)f23;
};

void Close23(void){
  delete T23;
  delete f23;
};

static void WriteF23(void *rec, char *buf){
    int curlen=0,sz;
    buf[0]=0;
    char *buf2;
    int i=0;
    file_23 * r23 =(file_23 *) rec;
    while(ShowMask23[i]){
      switch(ShowMask23[i]){
       default:
        sz=T23->SizeForPrint(ShowMask23[i]);
        if(curlen+sz+1<MAX_STRING_WIDTH){
          buf2 = new char[sz+1];
          memset(buf2,0,sz+1);
          T23->PrintField(ShowMask23[i], rec ,buf2);
          strcat(buf,buf2);
          strcat(buf,"│");;
          curlen=curlen+sz+1;
          delete buf2;
        }
        break;
       case 4:
        if(curlen+31<MAX_STRING_WIDTH){
          Table *T8 = T8->St.FindTable("X$Classificator");
          file_8 r8;
          file_8_0 k8;

          if(rf22->Periodihnostj)
            k8.id=5;
          else
            k8.id=4;

          k8.item=r23->Operaciy;

          if(!T8 || T8->GEQ(&r8,&k8,0)){
            StrClear(r8.name,30);
          }
          strcat(buf,r8.name);
          strcat(buf,"│");;
          curlen=curlen+31;
        }
        break;
       case 6:
        sz=T23->SizeForPrint(ShowMask23[i]);
        {
          buf2 = new char[sz+1];
          memset(buf2,0,sz+1);
          T23->PrintField(ShowMask23[i], rec ,buf2);
          buf2[MAX_STRING_WIDTH-curlen-2]=0;
          strcat(buf,buf2);
          curlen=MAX_STRING_WIDTH;
          delete buf2;
        }
        break;

      }
      i++;
    }
};


static void EditF23Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Update(f23);
  E->msg.K=ESC;
};

static void InsertF23Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Insert(f23);
  E->msg.K=ESC;
};

static void EditF23(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f23)) return;
  _Window *w= new _Window(1,1,72,1,E_STYLE23,stBuffered | stFrame,"EditF23");
  fv->Add(w); short wh=1;   w->HelpID=19;

  w->Add(new _Static(1,(wh),E_STYLE23,"Дата_начала_действия"));
  w->Add(BuildCell(T23,3,f23,21,&wh,w->w,E_STYLE23 ));

  w->Add(new _Static(1,(wh),E_STYLE23,"Операция            "));
  w->Add(new _ClsrCell (21,(wh++),30,E_STYLE23,(short*)(f23+T23->FieldOffset(4)),(rf22->Periodihnostj)?5:4));

  w->Add(new _Static(1,(wh),E_STYLE23,"Сумма "));
  w->Add(BuildCell(T23,5,f23,21,&wh,w->w,E_STYLE23 ));

  w->Add(new _Static(1,(wh++),E_STYLE23,"Примечание          "));
  w->Add(BuildCell(T23,6,f23,1,&wh,w->w,E_STYLE23 ));

  if(!rf17->Priznak_zakritiy)
    w->AddCallBack(EditF23Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void InsertF23(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  memset(f23,0,sizeof(f23));
  InitRecord(T23,f23);
  memcpy(rf23->Dogovor,rf17->Nomer_dogovora,13);
  rf23->Statjy=rf22->Stroka_dogovora;

  _Window *w= new _Window(1,1,72,1,E_STYLE23,stBuffered | stFrame,"InsertF23");
  fv->Add(w); short wh=1;   w->HelpID=19;

  w->Add(new _Static(1,(wh),E_STYLE23,"Дата_начала_действия"));
  w->Add(BuildCell(T23,3,f23,21,&wh,w->w,E_STYLE23 ));

  w->Add(new _Static(1,(wh),E_STYLE23,"Операция            "));
  w->Add(new _ClsrCell (21,(wh++),30,E_STYLE23,(short*)(f23+T23->FieldOffset(4)),(rf22->Periodihnostj)?5:4));

  w->Add(new _Static(1,(wh),E_STYLE23,"Сумма "));
  w->Add(BuildCell(T23,5,f23,21,&wh,w->w,E_STYLE23 ));

  w->Add(new _Static(1,(wh++),E_STYLE23,"Примечание          "));
  w->Add(BuildCell(T23,6,f23,1,&wh,w->w,E_STYLE23 ));

  w->AddCallBack(InsertF23Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void DeleteF23(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  fv->Delete();
  E->ClearEvent((_Window*)parent);
};

static void PrintF23(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  char name[16];
  Report(T23,fv->KeyNum,name);
  _ShowPrint *sp = new _ShowPrint(name,"Печать История_статьи");
  fv->Add(sp); sp->HelpID=100;
  sp->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static file_23_0 filter;

void AddF23( void (*CallBackFunc)(_Event *,void *), _Window * parent){
  if(!Dostup[23])
    return;
  _Window * tmp =parent;
  if(!tmp) tmp =GDt;
  if(tmp->Find("История статьи")){
    if(tmp ==GDt){
      MessageBox("Окно 'История статьи' уже доступно,",
                 "для переключения используйте: ",
                 "Alt + "
                );
    }else{
      MessageBox("Окно 'История статьи' уже доступно,",
                 "его нельзя активизировать еще раз"
                );
    }
    return;
  }
  memcpy(filter.Dogovor , rf17->Nomer_dogovora,13);
  filter.Statjy=rf22->Stroka_dogovora;
  _FileView * fv=new _FileView (T23,0,&filter,17,WriteF23,2,2,75,20,1,V_STYLE23,"История статьи");
  fv->HelpID = 10;
  //fv->st &= ~stMaySwitch;
  fv->Add(new _Static(2,0,V_STYLE23," История_статьи "));
  if(Dostup[17]==D_WRITE){
    fv->AddCallBack(EditF23, evKey,ENTER);
    if(!rf17->Priznak_zakritiy){
      fv->AddCallBack(InsertF23, evKey,INSERT);
      fv->AddCallBack(DeleteF23, evKey,DELETE);
    }
  }
  fv->AddCallBack(PrintF23, evKey, CTRL_P);
  if(CallBackFunc)
        fv->AddCallBack(CallBackFunc, evKey, ESC);
  tmp->Add(fv);
  tmp->SwitchFocus(fv);
};


