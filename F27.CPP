/****************************************************
 *        F27.CPP                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#include "common.h"
#include "doc.h"


static short ShowMask27[]={
       //1,   // ��� �������
       2,   // ���
       3,   // ��������
       //4,   // ���祭��1
       //5,   // ���祭��2
       //6,   // ���祭��3
       //7,   // ���祭��4
       //8,   // ��� 2
       0    // end_of_list
};

file_27 * rf27;
Table *T27;
static char  * f27;

void Init27(void){
  T27= new Table("����窠_�������",OWNER);
  if(!T27->opened){
	printf("Error: file ����窠_������� not opened\n");
        exit(0);
  }
  f27=new char [T27->max_rec_len];
  rf27 = (file_27*)f27;
};

void Close27(void){
  delete T27;
  delete f27;
};

static void WriteF27(void *rec, char *buf){
    int curlen=0,sz;
    buf[0]=0;
    char *buf2;
    int i=0;
    while(ShowMask27[i]){
      sz=T27->SizeForPrint(ShowMask27[i]);
      if(curlen+sz+1<MAX_STRING_WIDTH){
        buf2 = new char[sz+1];
        memset(buf2,0,sz+1);
        T27->PrintField(ShowMask27[i], rec ,buf2);
        strcat(buf,buf2);
        strcat(buf,"�");
        curlen=curlen+sz+1;
        delete buf2;
      }
      i++;
    }
};


static void EditF27Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Update(f27);
  E->msg.K=ESC;
};

static void InsertF27Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Insert(f27);
  E->msg.K=ESC;
};

static void EditF27(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f27)) return;
  _Window *w= new _Window(1,1,73,1,E_STYLE27,stBuffered | stFrame,"EditF27");
  fv->Add(w); short wh=1;  w->HelpID=19;

  //w->Add(new _Static(1,(wh++),E_STYLE27,"��� �������        "));
  //w->Add(BuildCell(T27,1,f27,1,&wh,w->w,E_STYLE27 ));

  w->Add(new _Static(1,(wh++),E_STYLE27,"���                "));
  w->Add(BuildCell(T27,2,f27,1,&wh,w->w,E_STYLE27 ));

  w->Add(new _Static(1,(wh++),E_STYLE27,"��������            "));
  w->Add(BuildCell(T27,3,f27,1,&wh,w->w,E_STYLE27 ));

  w->Add(new _Static(1,(wh++),E_STYLE27,"���祭��1           "));
  w->Add(BuildCell(T27,4,f27,1,&wh,w->w,E_STYLE27 ));

  w->Add(new _Static(1,(wh++),E_STYLE27,"���祭��2           "));
  w->Add(BuildCell(T27,5,f27,1,&wh,w->w,E_STYLE27 ));

  w->Add(new _Static(1,(wh++),E_STYLE27,"���祭��3           "));
  w->Add(BuildCell(T27,6,f27,1,&wh,w->w,E_STYLE27 ));

  w->Add(new _Static(1,(wh++),E_STYLE27,"���祭��4           "));
  w->Add(BuildCell(T27,7,f27,1,&wh,w->w,E_STYLE27 ));

  w->Add(new _Static(1,(wh++),E_STYLE27,"��� 2              "));
  w->Add(BuildCell(T27,8,f27,1,&wh,w->w,E_STYLE27 ));
  //wh++;
  w->AddCallBack(EditF27Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void InsertF27(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  memset(f27,0,sizeof(f27));
  InitRecord(T27,f27);
  memcpy(rf27->Kod_dogovora,rf17->Nomer_dogovora,13);

  _Window *w= new _Window(1,1,73,1,E_STYLE27,stBuffered | stFrame,"InsertF27");
  fv->Add(w); short wh=1;  w->HelpID=19;

  //w->Add(new _Static(1,(wh++),E_STYLE27,"��� �������        "));
  //w->Add(BuildCell(T27,1,f27,1,&wh,w->w,E_STYLE27 ));

  w->Add(new _Static(1,(wh++),E_STYLE27,"���                "));
  w->Add(BuildCell(T27,2,f27,1,&wh,w->w,E_STYLE27 ));

  w->Add(new _Static(1,(wh++),E_STYLE27,"��������            "));
  w->Add(BuildCell(T27,3,f27,1,&wh,w->w,E_STYLE27 ));

  w->Add(new _Static(1,(wh++),E_STYLE27,"���祭��1           "));
  w->Add(BuildCell(T27,4,f27,1,&wh,w->w,E_STYLE27 ));

  w->Add(new _Static(1,(wh++),E_STYLE27,"���祭��2           "));
  w->Add(BuildCell(T27,5,f27,1,&wh,w->w,E_STYLE27 ));

  w->Add(new _Static(1,(wh++),E_STYLE27,"���祭��3           "));
  w->Add(BuildCell(T27,6,f27,1,&wh,w->w,E_STYLE27 ));

  w->Add(new _Static(1,(wh++),E_STYLE27,"���祭��4           "));
  w->Add(BuildCell(T27,7,f27,1,&wh,w->w,E_STYLE27 ));

  w->Add(new _Static(1,(wh++),E_STYLE27,"��� 2              "));
  w->Add(BuildCell(T27,8,f27,1,&wh,w->w,E_STYLE27 ));
  //wh++;
  w->AddCallBack(InsertF27Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void DeleteF27(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  fv->Delete();
  E->ClearEvent((_Window*)parent);
};

static void PrintF27(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  char name[16];
  Report(T27,fv->KeyNum,name);
  _ShowPrint *sp = new _ShowPrint(name,"����� ����窠_�������");
  fv->Add(sp);
  sp->GotFocus();
  E->ClearEvent((_Window*)parent);
};

void AddF27( void (*CallBackFunc)(_Event *,void *), _Window * parent){
  if(!Dostup[27])
    return;
  _Window * tmp =parent;
  if(!tmp) tmp =GDt;
  if(tmp->Find("����窠_�������")){
    if(tmp ==GDt){
      MessageBox("���� '����窠_�������' 㦥 ����㯭�,",
                 "��� ��४��祭�� �ᯮ����: ",
                 "Alt + "
                );
    }else{
      MessageBox("���� '����窠_�������' 㦥 ����㯭�,",
                 "��� ����� ��⨢���஢��� �� ࠧ"
                );
    }
    return;
  }
  _FileView * fv=new _FileView (T27,0,&rf17->Nomer_dogovora,13,WriteF27,2,2,75,20,1,V_STYLE27,"����窠_�������");
  fv->Add(new _Static(2,0,V_STYLE27," ����窠_������� "));
  if(Dostup[27]==D_WRITE){
    fv->AddCallBack(InsertF27, evKey,INSERT);
    fv->AddCallBack(EditF27, evKey,ENTER);
    fv->AddCallBack(DeleteF27, evKey,DELETE);
  }
  fv->AddCallBack(PrintF27, evKey, CTRL_P);
  if(CallBackFunc)
        fv->AddCallBack(CallBackFunc, evKey, ESC);
  //fv->st &=~stMaySwitch;
  tmp->Add(fv); fv->HelpID=20;
  tmp->SwitchFocus(fv);
};


