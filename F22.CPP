/****************************************************
 *        F22.CPP                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#include "common.h"
#include "doc.h"
#include "f22.h"


file_22 * rf22;
Table *T22;
static char  * f22;
static file_22 * prev;

void Init22(void){
  T22= new Table("�����_�������",OWNER);
  if(!T22->opened){
	printf("Error: file �����_������� not opened\n");
        exit(0);
  }
  f22=new char [T22->max_rec_len];
  rf22 = (file_22*)f22;
};

void Close22(void){
  delete T22;
  delete f22;
};

static void WriteF22(void *rec, char *buf){
    file_22 *r22 =(file_22*)rec;
    Table *T8 = T8->St.FindTable ("X$Classificator");
    file_8 r8;
    file_8_0 k8;
    if(T21->GEQ(rf21,&r22->Kod_raboti,0))
      StrClear(rf21->Naimenovanie,30);
    rf21->Naimenovanie[30]=0;
    strcpy(buf,rf21->Naimenovanie);
    strcat(buf,"�");;
    k8.id=7;
    k8.item = r22->Periodihnostj;
    if(!T8 || T8->GEQ(&r8,&k8,0))
      StrClear(r8.name,30);
    strcat(buf,r8.name);
    strcat(buf,"�");
    if(r22->Avansirovanie)
      strcat(buf,"�� ");
    else
      strcat(buf,"���");
};


static void EditF22Done(_Event *E,void *parent){
  if(rf22->Periodihnostj!=prev->Periodihnostj){
    file_23_0 k23;
    memset(&k23,0,sizeof(k23));
    memcpy(k23.Dogovor,rf22->Dogovor,13);
    k23.Statjy=rf22->Stroka_dogovora;

    if(!T23->GGE(rf23,&k23,0)
       && !memcmp(k23.Dogovor,rf22->Dogovor,13)
       && k23.Statjy==rf22->Stroka_dogovora){
        MessageBox(" ����� �������� �ਧ��� ��ਮ��筮��!",
                   "㦥 ���� ��ப� ���ਨ �⮩ ���� (F2)",
                   "      ������ Esc ��� �த�������"
                   );
     rf22->Periodihnostj=prev->Periodihnostj;
   };

  }

  delete prev;
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Update(f22);
  E->msg.K=ESC;
};

static void InsertF22Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Insert(f22);
  E->msg.K=ESC;
};


static void EditF22(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f22)) return;
  prev=new file_22;
  memcpy(prev,f22,sizeof(file_22));
  _Window *w= new _Window(1,1,72,1,E_STYLE22,stBuffered | stFrame,"EditF22");
  fv->Add(w); short wh=1;   w->HelpID=19;

  w->Add(new _Static(1,(wh),E_STYLE22,"���_ࠡ���          "));
  w->Add(new _RelSCell(21,(wh++),50,E_STYLE22,5,(short*)(f22+T22->FieldOffset(3)),0,21,0,2 ));

  w->Add(new _Static(1,(wh),E_STYLE22,"��ਮ��筮���       "));
  w->Add(new _ClsrCell (21,(wh++),30,E_STYLE22,(short*)(f22+T22->FieldOffset(5)),7));

  w->Add(new _Static(1,(wh),E_STYLE22,"�����஢����       "));
  w->Add(BuildCell(T22,6,f22,21,&wh,w->w,E_STYLE22 ));

  w->Add(new _Static(1,(wh),E_STYLE22,"��� ��楯�         "));
  w->Add(BuildCell(T22,4,f22,21,&wh,w->w,E_STYLE22 ));

  //wh++;
  if(!rf17->Priznak_zakritiy)
    w->AddCallBack(EditF22Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void InsertF22(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  memset(f22,0,sizeof(f22));
  InitRecord(T22,f22);
  memcpy(rf22->Dogovor,rf17->Nomer_dogovora,13);

  _Window *w= new _Window(1,1,72,1,E_STYLE22,stBuffered | stFrame,"InsertF22");
  fv->Add(w); short wh=1;   w->HelpID=19;

  w->Add(new _Static(1,(wh),E_STYLE22,"���_ࠡ���          "));
  w->Add(new _RelSCell(21,(wh++),50,E_STYLE22,5,(short*)(f22+T22->FieldOffset(3)),0,21,0,2 ));

  //w->Add(new _Static(1,(wh++),E_STYLE22,"������ ����७��   "));
  //w->Add(new _ClsrCell (21,(wh++),30,E_STYLE22,(short*)(f22+T22->FieldOffset(4)),8));

  w->Add(new _Static(1,(wh),E_STYLE22,"��ਮ��筮���       "));
  w->Add(new _ClsrCell (21,(wh++),30,E_STYLE22,(short*)(f22+T22->FieldOffset(5)),7));

  w->Add(new _Static(1,(wh),E_STYLE22,"�����஢����       "));
  w->Add(BuildCell(T22,6,f22,21,&wh,w->w,E_STYLE22 ));

  w->Add(new _Static(1,(wh),E_STYLE22,"��� ��楯�         "));
  w->Add(BuildCell(T22,4,f22,21,&wh,w->w,E_STYLE22 ));

  //wh++;
  w->AddCallBack(InsertF22Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void DeleteF22(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f22)) return;
  file_23_0 k23;
  memset(&k23,0,sizeof(k23));
  memcpy(k23.Dogovor,rf22->Dogovor,13);
  k23.Statjy=rf22->Stroka_dogovora;
  if( !T23->GGE(rf23,&k23,0) &&
      !memcmp(k23.Dogovor,rf22->Dogovor,13) &&
      k23.Statjy==rf22->Stroka_dogovora
    ){
        MessageBox( "����� 㤠���� ������ � ���� ",
                    "���� ��ப� ���ਨ ���� (F2)",
                    "������ Esc ��� �த�������"
                    );
     }else
        fv->Delete();
  E->ClearEvent((_Window*)parent);
};

static void PrintF22(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  char name[16];
  Report(T22,fv->KeyNum,name);
  _ShowPrint *sp = new _ShowPrint(name,"����� �����_�������");
  fv->Add(sp); sp->HelpID=100;
  sp->GotFocus();
  E->ClearEvent((_Window*)parent);
};
static void CB23 (_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  fv->Lock(rf22);
  AddF23(0,fv);
  E->ClearEvent((_Window*)parent);
};

void AddF22( void (*CallBackFunc)(_Event *,void *), _Window * parent){
  if(!Dostup[22])
    return;
  _Window * tmp =parent;
  if(!tmp) tmp =GDt;
  if(tmp->Find("����� �������")){
    if(tmp == GDt){
      MessageBox("���� '�����_�������' 㦥 ����㯭�,",
                 "��� ��४��祭�� �ᯮ����: ",
                 "Alt + "
                );
    }else{
      MessageBox("���� '�����_�������' 㦥 ����㯭�,",
                 "��� ����� ��⨢���஢��� �� ࠧ"
                );
    }
    return;
  }
  _FileView * fv=new _FileView (T22,0,&rf17->Nomer_dogovora,13,WriteF22,2,2,75,20,1,V_STYLE22,"����� �������");
  fv->HelpID = 9;
  //fv->st &=~stMaySwitch;
  fv->Add(new _Static(2,0,V_STYLE22," �����_������� "));
  fv->AddCallBack(EditF22, evKey,ENTER);

  if(!rf17->Priznak_zakritiy && Dostup[22]==D_WRITE){
    fv->AddCallBack(InsertF22, evKey,INSERT);
    fv->AddCallBack(DeleteF22, evKey,DELETE);
  }

  fv->AddCallBack(PrintF22, evKey, CTRL_P);
  fv->AddCallBack(CB23, evKey,F2);
  if(CallBackFunc)
        fv->AddCallBack(CallBackFunc, evKey, ESC);
  tmp->Add(fv);
  tmp->SwitchFocus(fv);
};


