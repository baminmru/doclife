/****************************************************
 *        F26.CPP                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#include "common.h"
#include "doc.h"


static short ShowMask26[]={
       1,   // ���
       2,   // ��������
       0    // end_of_list
};

file_26 * rf26;
Table *T26;
static char  * f26;

void Init26(void){
  T26= new Table("���_䨭���஢����",OWNER);
  if(!T26->opened){
	printf("Error: file ���_䨭���஢���� not opened\n");
        exit(0);
  }
  f26=new char [T26->max_rec_len];
  rf26 = (file_26*)f26;
};

void Close26(void){
  delete T26;
  delete f26;
};

static void WriteF26(void *rec, char *buf){
    int curlen=0,sz;
    buf[0]=0;
    char *buf2;
    int i=0;
    while(ShowMask26[i]){
      sz=T26->SizeForPrint(ShowMask26[i]);
      if(curlen+sz+1<MAX_STRING_WIDTH){
        buf2 = new char[sz+1];
        memset(buf2,0,sz+1);
        T26->PrintField(ShowMask26[i], rec ,buf2);
        strcat(buf,buf2);
        strcat(buf,"�");
        curlen=curlen+sz+1;
        delete buf2;
      }
      i++;
    }
};


static void EditF26Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Update(f26);
  E->msg.K=ESC;
};

static void InsertF26Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Insert(f26);
  E->msg.K=ESC;
};

static void EditF26(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f26)) return;
  _Window *w= new _Window(1,1,73,1,E_STYLE26,stBuffered | stFrame,"EditF26");
  fv->Add(w); short wh=1;   w->HelpID=19;

  //w->Add(new _Static(1,(wh++),E_STYLE26,"���                 "));
  //w->Add(BuildCell(T26,1,f26,1,&wh,w->w,E_STYLE26 ));

  w->Add(new _Static(1,(wh++),E_STYLE26,"��������            "));
  w->Add(BuildCell(T26,2,f26,1,&wh,w->w,E_STYLE26 ));
  //wh++;
  w->AddCallBack(EditF26Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void InsertF26(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  memset(f26,0,sizeof(f26));
  InitRecord(T26,f26);
  _Window *w= new _Window(1,1,73,1,E_STYLE26,stBuffered | stFrame,"InsertF26");
  fv->Add(w); short wh=1;   w->HelpID=19;

  //w->Add(new _Static(1,(wh++),E_STYLE26,"���                 "));
  //w->Add(BuildCell(T26,1,f26,1,&wh,w->w,E_STYLE26 ));

  w->Add(new _Static(1,(wh++),E_STYLE26,"��������            "));
  w->Add(BuildCell(T26,2,f26,1,&wh,w->w,E_STYLE26 ));
  //wh++;
  w->AddCallBack(InsertF26Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void DeleteF26(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f26)) return;
  if(!T24->GGE(rf24,rf26,0) && rf24->Kod_tipa==rf26->Kod){
    MessageBox(
      "����� 㤠���� ���ᠭ�� ���� 䨭���஢����",
      "���� ��ப� �ਮ��⮢ (F2)",
      "        ������ Esc ��� �த�������");

  }else
    fv->Delete();
  E->ClearEvent((_Window*)parent);
};

static void PrintF26(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  char name[16];
  Report(T26,fv->KeyNum,name);
  _ShowPrint *sp = new _ShowPrint(name,"����� ���_䨭���஢����");
  fv->Add(sp); sp->HelpID=100;
  sp->GotFocus();
  E->ClearEvent((_Window*)parent);
};
static void CB24(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f26)) return;
  AddF24(0,fv);
  E->ClearEvent((_Window*)parent);
};

void AddF26( void (*CallBackFunc)(_Event *,void *), _Window * parent){
  if(!Dostup[26])
    return;
  _Window * tmp =parent;
  if(!tmp) tmp =GDt;
  if(tmp->Find("���_䨭���஢����")){
    if(tmp ==GDt){
      MessageBox("���� '���_䨭���஢����' 㦥 ����㯭�,",
                 "��� ��४��祭�� �ᯮ����: ",
                 "Alt + "
                );
    }else{
      MessageBox("���� '���_䨭���஢����' 㦥 ����㯭�,",
                 "��� ����� ��⨢���஢��� �� ࠧ"
                );
    }
    return;
  }
  _FileView * fv=new _FileView (T26,0,0,0,WriteF26,2,2,75,20,1,V_STYLE26,"���_䨭���஢����");
  fv->HelpID=16;
  fv->Add(new _Static(2,0,V_STYLE26," ���_䨭���஢���� "));
  if(Dostup[26]==D_WRITE){
    fv->AddCallBack(InsertF26, evKey,INSERT);
    fv->AddCallBack(EditF26, evKey,ENTER);
    fv->AddCallBack(DeleteF26, evKey,DELETE);
  }
  fv->AddCallBack(PrintF26, evKey, CTRL_P);
  fv->AddCallBack(CB24, evKey, F2);
  if(CallBackFunc)
        fv->AddCallBack(CallBackFunc, evKey, ESC);
  tmp->Add(fv);
  tmp->SwitchFocus(fv);
};


