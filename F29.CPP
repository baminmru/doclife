/****************************************************
 *        F29.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"
#include "doc.h"

extern char Coder[];
extern char Dostup[];
#define D_NOACCESS 0
#define D_READ     1
#define D_WRITE    2


static short ShowMask29[]={
       1,   // Код доступа
       //2,   // Пароль
       //3,   // Привелегии
       4,   // Примечание
       0    // end_of_list
};

file_29 * rf29;
Table *T29;
static char  * f29;



void Init29(void){
  T29= new Table("Уровни_доступа",OWNER);
  if(!T29->opened){
	printf("Error: file Уровни доступа not opened\n");
        exit(0);
  }
  f29=new char [T29->max_rec_len];
  rf29 = (file_29*)f29;
};

void Close29(void){
  delete T29;
  delete f29;
};

static void WriteF29(void *rec, char *buf){
    int curlen=0,sz;
    buf[0]=0;
    char *buf2;
    int i;

    unsigned char *mm=(char*) Coder;
    file_29 *r29=(file_29*)rec;
    for(i=0; i<10;i++){
      r29->Kod_dostupa[i]^=mm[i];
    }
    for(i=0; i<30;i++){
      r29->Parolj[i]^=mm[i+10];
    }
    for(i=0; i<50;i++){
      r29->Privelegii[i]^=mm[i+40];
    }

    i=0;
    while(ShowMask29[i]){
      sz=T29->SizeForPrint(ShowMask29[i]);
      if(curlen+sz+1<MAX_STRING_WIDTH){
        buf2 = new char[sz+1];
        memset(buf2,0,sz+1);
        T29->PrintField(ShowMask29[i], rec ,buf2);
        strcat(buf,buf2);
        strcat(buf,"│");
        curlen=curlen+sz+1;
        delete buf2;
      }
      i++;
    }
};


static void EditF29Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  ((_Window*) parent)->Hide(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);

  unsigned char *mm=(char*) Coder;
  for(int i=0; i<10;i++){
    rf29->Kod_dostupa[i]^=mm[i];
  }
  for(i=0; i<30;i++){
    rf29->Parolj[i]^=mm[i+10];
  }
  for(i=0; i<50;i++){
    rf29->Privelegii[i]^=mm[i+40];
  }

  fv->Update(rf29);
};

static void EditF29Cancel(_Event *E,void *parent){
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Unlock();
};

static void InsertF29Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  unsigned char *mm=(char*) Coder;
  for(int i=0; i<10;i++){
    rf29->Kod_dostupa[i]^=mm[i];
  }
  for(i=0; i<30;i++){
    rf29->Parolj[i]^=mm[i+10];
  }
  for(i=0; i<50;i++){
    rf29->Privelegii[i]^=mm[i+40];
  }

  fv->Insert(f29);
};

static long Out=0;
static long Access=0;
static _ListBox *cbc=0;
static _ListBox *lb=0;

static void AfterFile(void * parent){
  Access=rf29->Privelegii[Out];
  cbc->Load();
  cbc->Show(1);
};

static void AfterType(void *p){
  rf29->Privelegii[Out]=Access;
};

static void EditF29(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f29)){
    E->ClearEvent(fv);
    return;
  }

  unsigned char *mm=(char*) Coder;
  for(int i=0; i<10;i++){
    rf29->Kod_dostupa[i]^=mm[i];
  }
  for(i=0; i<30;i++){
    rf29->Parolj[i]^=mm[i+10];
  }

  for(i=0; i<50;i++){
    rf29->Privelegii[i]^=mm[i+40];
  }

  _Window *w= new _Window(1,1,73,1,E_STYLE29,stBuffered | stFrame,"EditF29");
  fv->Add(w);
  short wh=1;

  w->Add(new _Static(1,(wh)++,E_STYLE29,"Код доступа         "));
  w->Add(BuildCell(T29,1,f29,1,&wh,w->w,E_STYLE29 ));

  w->Add(new _Static(1,(wh)++,E_STYLE29,"Пароль              "));
  w->Add(BuildCell(T29,2,f29,1,&wh,w->w,E_STYLE29 ));

  w->Add(new _Static(1,(wh)++,E_STYLE29,"Примечание"));
  w->Add(BuildCell(T29,4,f29,1,&wh,w->w,E_STYLE29 ));

  w->Add(new _Static(1,(wh)++,E_STYLE29,"Тип доступа         "));

  Access=rf29->Privelegii[10];
  Out=10;

  w->Add(lb = new _ListBox(1,wh,20,7, E_STYLE29, 0,&Out));
  lb->Changed=AfterFile;
  file_ddf files;
  file_ddf_0 key;
  char n[21];
  key.id=10;
  do{
     if(!Station::main_file->GEQ(&files,&key,0)){
      memcpy(n,files.name,20);
      n[20]=0;
      lb->lbi.AddItem(n,key.id++);
     }else
      break;
   }while(key.id<=51);

  w->Add(cbc = new _ListBox(30,wh, 20,5, E_STYLE29,0,&Access ));
  cbc->Changed=AfterType;
  cbc->lbi.AddItem("Запрещен",0);
  cbc->lbi.AddItem("Чтение  ",1);
  cbc->lbi.AddItem("Запись  ",2);

  w->AddCallBack(EditF29Cancel,evKey,ESC);
  w->AddCallBack(EditF29Done,evKey,ENTER);
  w->HelpID=35;
  fv->SwitchFocus(w);
  E->ClearEvent((_Window*)parent);
};

static void InsertF29(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  InitRecord(T29,f29);
  memset(rf29->Privelegii,0,51);
  _Window *w= new _Window(1,1,73,1,E_STYLE29,stBuffered | stFrame,"InsertF29");
  fv->Add(w);
  short wh=1;

  w->Add(new _Static(1,(wh)++,E_STYLE29,"Код  "));
  w->Add(BuildCell(T29,1,f29,1,&wh,w->w,E_STYLE29 ));

  w->Add(new _Static(1,(wh)++,E_STYLE29,"Пароль"));
  w->Add(BuildCell(T29,2,f29,1,&wh,w->w,E_STYLE29 ));

  w->Add(new _Static(1,(wh)++,E_STYLE29,"Примечание"));
  w->Add(BuildCell(T29,4,f29,1,&wh,w->w,E_STYLE29 ));


  Access=rf29->Privelegii[10];
  Out=10;

  w->Add(new _Static(1,(wh)++,E_STYLE29,"Тип доступа "));
  w->Add(lb = new _ListBox(1,wh,20,7, E_STYLE29, 0, &Out));
  lb->Changed=AfterFile;
  file_ddf files;
  file_ddf_0 key;
  char n[21];
  key.id=10;

  do{
     if(!Station::main_file->GEQ(&files,&key,0)){
      memcpy(n,files.name,20);
      n[20]=0;
      lb->lbi.AddItem(n,key.id++);
     }else
      break;
   }while(key.id<=51);

  w->Add(cbc = new _ListBox(30,wh,20,5, E_STYLE29, 0, &Access ));
  cbc->Changed=AfterType;
  cbc->lbi.AddItem("Запрещен",0);
  cbc->lbi.AddItem("Чтение  ",1);
  cbc->lbi.AddItem("Запись  ",2);
  w->AddCallBack(InsertF29Done,evKey,ENTER);
  w->HelpID=34;
  fv->SwitchFocus(w);
  E->ClearEvent((_Window*)parent);
};

static void DeleteF29(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  fv->Delete();
  E->ClearEvent((_Window*)parent);
};

static void PrintF29(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  char name[16];
  Report(T29,fv->KeyNum,name);
  _ShowPrint *sp = new _ShowPrint(name,"Печать Уровни доступа");
  fv->Add(sp);
  fv->SwitchFocus(sp);
  E->ClearEvent((_Window*)parent);
};

void AddF29( void (*CallBackFunc)(_Event *,void *), _Window * parent){
  if(!Dostup[29])
    return;
  _Window * tmp =parent;
  if(!tmp) tmp =GDt;
  if(tmp->Find("Уровни доступа")){
    if(tmp ==GDt){
      MessageBox("Окно 'Уровни доступа' уже доступно,",
                 "для переключения используйте: ",
                 "Alt + "
                );
    }else{
      MessageBox("Окно 'Уровни доступа' уже доступно,",
                 "его нельзя активизировать еще раз"
                );
    }
    return;
  }
  _FileView * fv=new _FileView (T29,0,0,0,WriteF29,2,2,75,20,1,V_STYLE29,"Уровни доступа");
  fv->Add(new _Static(2,0,V_STYLE29," Уровни доступа "));
  fv->HelpID=33;
  if(Dostup[29]==2){
    fv->AddCallBack(InsertF29, evKey,INSERT);
    fv->AddCallBack(EditF29, evKey,ENTER);
    fv->AddCallBack(DeleteF29, evKey,DELETE);
  }
  fv->AddCallBack(PrintF29, evKey, CTRL_P);
  if(CallBackFunc)
    fv->AddCallBack(CallBackFunc, evKey, ESC);
  tmp->Add(fv);
  tmp->SwitchFocus(fv);
};


