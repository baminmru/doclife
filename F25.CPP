/****************************************************
 *        F25.CPP                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#include "common.h"
#include "doc.h"


static short ShowMask25[]={
       1,   // ����� ���
       2,   // ��� �������
       3,   // �����
       4,   // �㬬�
       5,   // ����
       6,   // ��ࠡ�⠭
       0    // end_of_list
};

file_25 * rf25;
Table *T25;
static char  * f25;

void Init25(void){
  T25= new Table("�஬������_�⮣",OWNER);
  if(!T25->opened){
	printf("Error: file �஬������_�⮣ not opened\n");
        exit(0);
  }
  f25=new char [T25->max_rec_len];
  rf25 = (file_25*)f25;
};

void Close25(void){
  delete T25;
  delete f25;
};

static void WriteF25(void *rec, char *buf){
    int curlen=0,sz;
    buf[0]=0;
    char *buf2;
    int i=0;
    while(ShowMask25[i]){
      sz=T25->SizeForPrint(ShowMask25[i]);
      if(curlen+sz+1<MAX_STRING_WIDTH){
        buf2 = new char[sz+1];
        memset(buf2,0,sz+1);
        T25->PrintField(ShowMask25[i], rec ,buf2);
        strcat(buf,buf2);
        strcat(buf,"�");
        curlen=curlen+sz+1;
        delete buf2;
      }
      i++;
    }
};


static void EditF25Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Update(f25);
  E->msg.K=ESC;
};

static void InsertF25Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Insert(f25);
  E->msg.K=ESC;
};
/*
static void EditF25(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f25)) return;
  _Window *w= new _Window(1,1,73,1,E_STYLE25,stBuffered | stFrame,"EditF25");
  fv->Add(w); short wh=1;   w->HelpID=19;

  w->Add(new _Static(1,(wh++),E_STYLE25,"����� ���         "));
  w->Add(BuildCell(T25,1,f25,1,&wh,w->w,E_STYLE25 ));

  w->Add(new _Static(1,(wh++),E_STYLE25,"��� �������        "));
  w->Add(BuildCell(T25,2,f25,1,&wh,w->w,E_STYLE25 ));

  w->Add(new _Static(1,(wh++),E_STYLE25,"�����              "));
  w->Add(BuildCell(T25,3,f25,1,&wh,w->w,E_STYLE25 ));

  w->Add(new _Static(1,(wh++),E_STYLE25,"�㬬�               "));
  w->Add(BuildCell(T25,4,f25,1,&wh,w->w,E_STYLE25 ));

  w->Add(new _Static(1,(wh++),E_STYLE25,"����                "));
  w->Add(BuildCell(T25,5,f25,1,&wh,w->w,E_STYLE25 ));

  w->Add(new _Static(1,(wh++),E_STYLE25,"��ࠡ�⠭           "));
  w->Add(BuildCell(T25,6,f25,1,&wh,w->w,E_STYLE25 ));
  //wh++;
  w->AddCallBack(EditF25Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void InsertF25(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  memset(f25,0,sizeof(f25));
  InitRecord(T25,f25);
  _Window *w= new _Window(1,1,73,1,E_STYLE25,stBuffered | stFrame,"InsertF25");
  fv->Add(w); short wh=1;   w->HelpID=19;

  w->Add(new _Static(1,(wh++),E_STYLE25,"����� ���         "));
  w->Add(BuildCell(T25,1,f25,1,&wh,w->w,E_STYLE25 ));

  w->Add(new _Static(1,(wh++),E_STYLE25,"��� �������        "));
  w->Add(BuildCell(T25,2,f25,1,&wh,w->w,E_STYLE25 ));

  w->Add(new _Static(1,(wh++),E_STYLE25,"�����              "));
  w->Add(BuildCell(T25,3,f25,1,&wh,w->w,E_STYLE25 ));

  w->Add(new _Static(1,(wh++),E_STYLE25,"�㬬�               "));
  w->Add(BuildCell(T25,4,f25,1,&wh,w->w,E_STYLE25 ));

  w->Add(new _Static(1,(wh++),E_STYLE25,"����                "));
  w->Add(BuildCell(T25,5,f25,1,&wh,w->w,E_STYLE25 ));

  w->Add(new _Static(1,(wh++),E_STYLE25,"��ࠡ�⠭           "));
  w->Add(BuildCell(T25,6,f25,1,&wh,w->w,E_STYLE25 ));
  //wh++;
  w->AddCallBack(InsertF25Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void DeleteF25(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  fv->Delete();
  E->ClearEvent((_Window*)parent);
};

static void PrintF25(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  char name[16];
  Report(T25,fv->KeyNum,name);
  _ShowPrint *sp = new _ShowPrint(name,"����� �஬������_�⮣");
  fv->Add(sp); sp->HelpID=100;
  sp->GotFocus();
  E->ClearEvent((_Window*)parent);
};

void AddF25( void (*CallBackFunc)(_Event *,void *), _Window * parent){
  if(!Dostup[25])
    return;
  _Window * tmp =parent;
  if(!tmp) tmp =GDt;
  if(tmp->Find("�஬������_�⮣")){
    if(tmp ==GDt){
      MessageBox("���� '�஬������_�⮣' 㦥 ����㯭�,",
                 "��� ��४��祭�� �ᯮ����: ",
                 "Alt + "
                );
    }else{
      MessageBox("���� '�஬������_�⮣' 㦥 ����㯭�,",
                 "��� ����� ��⨢���஢��� �� ࠧ"
                );
    }
    return;
  }
  _FileView * fv=new _FileView (T25,0,0,0,WriteF25,2,2,75,20,1,V_STYLE25,"�஬������_�⮣");
  fv->Add(new _Static(2,0,V_STYLE25," �஬������_�⮣ "));
  if(Dostup[25]==D_WRITE){
    fv->AddCallBack(InsertF25, evKey,INSERT);
    fv->AddCallBack(EditF25, evKey,ENTER);
    fv->AddCallBack(DeleteF25, evKey,DELETE);
  }
  fv->AddCallBack(PrintF25, evKey, CTRL_P);
  if(CallBackFunc)
        fv->AddCallBack(CallBackFunc, evKey, ESC);
  tmp->Add(fv);
  tmp->SwitchFocus(fv);
};
*/

