/****************************************************
 *        F20.CPP                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#include "common.h"
#include "doc.h"
#include "f20.h"

extern file_30_0 filter;

static short ShowMask20[]={
       1,   // ��� ���⭥�
       2,   // ������������
       4,   // ����
       3,   // ����䮭�
       0    // end_of_list
};

file_20 * rf20;
Table *T20;
static char  * f20;

void Init20(void){
  T20= new Table("��ࠢ�筨�_���⭥஢",OWNER);
  if(!T20->opened){
	printf("Error: file ��ࠢ�筨�_���⭥஢ not opened\n");
        exit(0);
  }
  f20=new char [T20->max_rec_len];
  rf20 = (file_20*)f20;
};

void Close20(void){
  delete T20;
  delete f20;
};

static void WriteF20(void *rec, char *buf){
    file_20 *r20=(file_20*)rec;
    sprintf(buf,"%5d�%s",r20->Kod_partnera,r20->Naimenovanie);
};


static void EditF20Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  StrTrim(rf20->Naimenovanie);
  StrUpper(rf20->Naimenovanie);
  fv->Update(f20);
  E->msg.K=ESC;
};

static void InsertF20Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  StrTrim(rf20->Naimenovanie);
  StrUpper(rf20->Naimenovanie);
  fv->Insert(f20);
  E->msg.K=ESC;
};

static void EditF20(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f20)) return;
  _Window *w= new _Window(1,1,73,1,E_STYLE20,stBuffered | stFrame,"EditF20");
  fv->Add(w); short wh=1;   w->HelpID=19;

  w->Add(new _Static(1,(wh++),E_STYLE20,"������������        "));
  w->Add(BuildCell(T20,2,f20,1,&wh,w->w,E_STYLE20 ));

  w->Add(new _Static(1,(wh++),E_STYLE20,"����䮭�            "));
  w->Add(BuildCell(T20,3,f20,1,&wh,w->w,E_STYLE20 ));

  w->Add(new _Static(1,(wh),E_STYLE20,"����                "));
  w->Add(BuildCell(T20,4,f20,21,&wh,w->w,E_STYLE20 ));

  //wh++;
  w->AddCallBack(EditF20Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void InsertF20(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  memset(f20,0,sizeof(f20));
  InitRecord(T20,f20);
  _Window *w= new _Window(1,1,73,1,E_STYLE20,stBuffered | stFrame,"InsertF20");
  fv->Add(w); short wh=1;   w->HelpID=19;

  w->Add(new _Static(1,(wh++),E_STYLE20,"������������        "));
  w->Add(BuildCell(T20,2,f20,1,&wh,w->w,E_STYLE20 ));

  w->Add(new _Static(1,(wh++),E_STYLE20,"����䮭�            "));
  w->Add(BuildCell(T20,3,f20,1,&wh,w->w,E_STYLE20 ));

  w->Add(new _Static(1,(wh),E_STYLE20,"����                "));
  w->Add(BuildCell(T20,4,f20,21,&wh,w->w,E_STYLE20 ));
  //wh++;
  w->AddCallBack(InsertF20Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void DeleteF20(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f20)) return;
  file_30_0 k0;
  memset(&k0,0,sizeof(k0));
  k0.Kod_partnera=rf20->Kod_partnera;

  if(!T30->GGE(rf30,&k0,0) && rf30->Kod_partnera==rf20->Kod_partnera){

    MessageBox("���砫� ���� 㤠���� ����� � �ᥢ������� ���⭥஢");
    fv->Unlock();
    E->ClearEvent((_Window*)parent);
    return;

  }else
    fv->Delete();

  E->ClearEvent((_Window*)parent);
};

static void ViewAcc(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f20)) return;
  filter.Kod_partnera=rf20->Kod_partnera;
  AddF30(0,fv);
  E->ClearEvent((_Window*)parent);
};

static void PrintF20(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  char name[16];
  Report(T20,fv->KeyNum,name);
  _ShowPrint *sp = new _ShowPrint(name,"����� ��ࠢ�筨�_���⭥஢");
  fv->Add(sp); sp->HelpID=100;
  sp->GotFocus();
  E->ClearEvent((_Window*)parent);
};

void AddF20( void (*CallBackFunc)(_Event *,void *), _Window * parent){
  if(!Dostup[20])
    return;
  _Window * tmp =parent;
  if(!tmp) tmp =GDt;
  if(tmp->Find("��ࠢ�筨�_���⭥஢")){
    if(tmp ==GDt){
      MessageBox("���� '��ࠢ�筨�_���⭥஢' 㦥 ����㯭�,",
                 "��� ��४��祭�� �ᯮ����: ",
                 "Alt + "
                );
    }else{
      MessageBox("���� '��ࠢ�筨�_���⭥஢' 㦥 ����㯭�,",
                 "��� ����� ��⨢���஢��� �� ࠧ"
                );
    }
    return;
  }
  _FileView * fv=new _FileView (T20,1,0,0,WriteF20,2,2,75,20,1,V_STYLE20,"��ࠢ�筨�_���⭥஢");
  fv->HelpID=24;
  fv->Add(new _Static(2,0,V_STYLE20," ��ࠢ�筨�_���⭥஢ "));
  if(Dostup[20]==D_WRITE){
    fv->AddCallBack(InsertF20, evKey,INSERT);
    fv->AddCallBack(EditF20, evKey,ENTER);
    fv->AddCallBack(DeleteF20, evKey,DELETE);
    fv->AddCallBack(ViewAcc, evKey,F2);
  }
  fv->AddCallBack(PrintF20, evKey, CTRL_P);
  if(CallBackFunc)
        fv->AddCallBack(CallBackFunc, evKey, ESC);
  tmp->Add(fv);
  tmp->SwitchFocus(fv);
};


