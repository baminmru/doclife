/****************************************************
 *        F17.CPP                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#include "common.h"
#include "doc.h"

void ChooseTheDocs(_FileView *fv);

void DocReport(char *name);

static short ShowMask17[]={
       1,   // �����_�������
       //2,   // ���_�������
       3,   // ���_�����祭��
       //4,   // ���_���⭥�
       //5,   // ���_��砫�
       6,   // ���_����砭��
       //7,   // ���_�����⥫�
       //8,   // ����ঠ���
       //9,   // ����ﭨ�_�������
       //10,   // �ਬ�砭��
       11,  //��⪮�_���ᠭ��
       //12 �ਧ��� �������
       //13 �ਧ��� ���
       //14 �����
       //15 ��� 䨭���஢����
       0    // end_of_list
};
static file_17 prev;
file_17 * rf17;
Table *T17;
static char  * f17;
short DocType=0;
static short Filter = 0;
static file_17_2 fff;
_Static *fltr;

void Init17(void){
  T17= new Table("�������",OWNER);
  if(!T17->opened){
	printf("Error: file ������� not opened\n");
        exit(0);
  }
  f17=new char [T17->max_rec_len];
  rf17 = (file_17*)f17;
};

void Close17(void){
  delete T17;
  delete f17;
};

static void WriteF17(void *rec, char *buf){
    int curlen=0,sz;
    buf[0]=0;
    char *buf2;
    int i=0;
    while(ShowMask17[i]){
      sz=T17->SizeForPrint(ShowMask17[i]);
      if(curlen+sz+1<MAX_STRING_WIDTH){
        buf2 = new char[sz+1];
        memset(buf2,0,sz+1);
        T17->PrintField(ShowMask17[i], rec ,buf2);
        strcat(buf,buf2);
        strcat(buf,"�");;
        curlen=curlen+sz+1;
        delete buf2;
      }
      i++;
    }
};


static void EditF17Done(_Event *E,void *parent){
  int OK=1;
  ((_Window*) parent)->LostFocus(1);
  if(rf17->Data_zakluheniy.Empty())  OK=0;
  if(rf17->Data_nahala.Empty()) OK=0;
  if(rf17->Data_okonhaniy.Empty()) OK=0;
  if(OK && rf17->Data_okonhaniy < rf17->Data_nahala)
    OK=0;
  if(!OK){
    MessageBox(
      "�஢���� �ࠢ��쭮��� ��⠭�������� ���:",
      "1. ���� �� ᫥��� ��⠢���� ����묨",
      "2. ��� ����砭�� �� ������ ���� �����",
      "   祬 ��� ��砫� �������"
      );
     E->ClearEvent((_Window*)parent);
    ((_Window*) parent)->GotFocus(1);
     return;
  }

  if(rf17->Sostoynie_dogovora>1){
    if(MessageBox("��ॢ��� ������� � ࠧ�� �����襭��� ?",
               "       �� - [Enter] ��� - [Esc]"))
      rf17->Priznak_zakritiy=1;
    else
     rf17->Sostoynie_dogovora=1;
  }

  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);

  // �஢��塞 ����� �� ������ �����
  if(memcmp(prev.Nomer_dogovora,rf17->Nomer_dogovora,13)){
    if(!T22->GEQ(rf22,prev.Nomer_dogovora,0) ||
       !T18->GEQ(rf18,prev.Nomer_dogovora,0) ||
       !T27->GEQ(rf27,prev.Nomer_dogovora,0)
       ){
         MessageBox( "         ����� ������ ����� �������!",
                     "���� ��ப� ��⥩ (F2) ��� ��ப� ����� (F3)",
                     "��� ����窨 ������� (F4)",
                     "          ������ Esc ��� �த�������"
                     );
         memcpy(rf17->Nomer_dogovora,prev.Nomer_dogovora,13);
       }
  }
  if(!rf17->Priznak_zakritiy)
    fv->Update(f17);
  else{
    long ptr=fv->CurRecPos();
    T17->GDir(&prev,&ptr,3);
    T17->Update(rf17);
    fv->Repaint();
  }
  E->msg.K=ESC;
};

static void InsertF17Done(_Event *E,void *parent){
  int OK=1;
  ((_Window*) parent)->LostFocus(0);
  if(rf17->Data_zakluheniy.Empty())  OK=0;
  if(rf17->Data_nahala.Empty()) OK=0;
  if(rf17->Data_okonhaniy.Empty()) OK=0;
  if(OK && rf17->Data_okonhaniy < rf17->Data_nahala)
    OK=0;
  if(!OK){
    MessageBox(
      "�஢���� �ࠢ��쭮��� ��⠭�������� ���:",
      "1. ���� �� ᫥��� ��⠢���� ����묨",
      "2. ��� ����砭�� �� ����� ���� �����",
      "   祬 ��� ��砫� �������"
      );
  }

  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Insert(f17);
  E->msg.K=ESC;
};

static void EditF17(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f17)) return;
  memcpy(&prev,rf17,sizeof(file_17));
  _Window *w= new _Window(1,1,74,1,E_STYLE17,stBuffered | stFrame,"InsertF17");
  fv->Add(w); short wh=1;   w->HelpID=19;
  w->Add(new _Static(1,(wh),E_STYLE17,"�����_�������      "));
  w->Add(BuildCell(T17,1,f17,21,&wh,w->w,E_STYLE17 ));

  w->Add(new _Static(1,(wh),E_STYLE17,"���_�������        "));
  w->Add(new _RelSCell(21,(wh++),50,E_STYLE17,5,(short*)(f17+T17->FieldOffset(2)),0,15,0,2 ));

  w->Add(new _Static(1,(wh),E_STYLE17,"���_�����祭��     "));
  w->Add(BuildCell(T17,3,f17,21,&wh,w->w,E_STYLE17 ));

  w->Add(new _Static(1,(wh),E_STYLE17,"���_��砫�         "));
  w->Add(BuildCell(T17,5,f17,21,&wh,w->w,E_STYLE17 ));

  w->Add(new _Static(1,(wh),E_STYLE17,"���_����砭��      "));
  w->Add(BuildCell(T17,6,f17,21,&wh,w->w,E_STYLE17 ));

  w->Add(new _Static(1,(wh),E_STYLE17,"����ﭨ�_�������  "));
  w->Add(new _ClsrCell (21,(wh++),30,E_STYLE17,(short*)(f17+T17->FieldOffset(9)),6));

  w->Add(new _Static(1,(wh),E_STYLE17,"���_���⭥�        "));
  w->Add(new _RelSCell(21,(wh++),50,E_STYLE17,5,(short*)(f17+T17->FieldOffset(4)),0,20,0,2 ));

  w->Add(new _Static(1,(wh),E_STYLE17,"���_�����⥫�      "));
  w->Add(new _RelSCell(21,(wh++),50,E_STYLE17,5,(short*)(f17+T17->FieldOffset(7)),0,20,0,2 ));

  w->Add(new _Static(1,(wh),E_STYLE17,"�����               "));
  w->Add(new _RelSCell(21,(wh++),50,E_STYLE17,5,(short*)(f17+T17->FieldOffset(14)),0,14,0,2 ));

  w->Add(new _Static(1,(wh),E_STYLE17,"��� 䨭���஢����  "));
  w->Add(new _RelSCell(21,(wh++),50,E_STYLE17,5,(short*)(f17+T17->FieldOffset(15)),0,26,0,2 ));

  w->Add(new _Static(1,(wh++),E_STYLE17,"��⪮�_���ᠭ��    "));
  w->Add(BuildCell(T17,11,f17,1,&wh,w->w,E_STYLE17 ));

  w->Add(new _Static(1,(wh++),E_STYLE17,"����ঠ���          "));
  w->Add(BuildCell(T17,8,f17,1,&wh,w->w,E_STYLE17 ));

  w->Add(new _Static(1,(wh++),E_STYLE17,"�ਬ�砭��          "));
  w->Add(BuildCell(T17,10,f17,1,&wh,w->w,E_STYLE17 ));


  //wh++;
  if(!DocType)
    w->AddCallBack(EditF17Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void InsertF17(_Event *E, void * parent){
  if(DocType==1){
   E->ClearEvent((_Window*)parent);
   return;
  }
  _FileView *fv= (_FileView*) parent;
  memset(f17,0,sizeof(f17));
  InitRecord(T17,f17);
  _Window *w= new _Window(1,1,74,1,E_STYLE17,stBuffered | stFrame,"InsertF17");
  fv->Add(w); short wh=1;   w->HelpID=19;
  w->Add(new _Static(1,(wh),E_STYLE17,"�����_�������      "));
  w->Add(BuildCell(T17,1,f17,21,&wh,w->w,E_STYLE17 ));

  w->Add(new _Static(1,(wh),E_STYLE17,"���_�������        "));
  w->Add(new _RelSCell(21,(wh++),50,E_STYLE17,5,(short*)(f17+T17->FieldOffset(2)),0,15,0,2 ));

  w->Add(new _Static(1,(wh),E_STYLE17,"���_�����祭��     "));
  w->Add(BuildCell(T17,3,f17,21,&wh,w->w,E_STYLE17 ));

  w->Add(new _Static(1,(wh),E_STYLE17,"���_��砫�         "));
  w->Add(BuildCell(T17,5,f17,21,&wh,w->w,E_STYLE17 ));

  w->Add(new _Static(1,(wh),E_STYLE17,"���_����砭��      "));
  w->Add(BuildCell(T17,6,f17,21,&wh,w->w,E_STYLE17 ));

  w->Add(new _Static(1,(wh),E_STYLE17,"����ﭨ�_�������  "));
  w->Add(new _ClsrCell (21,(wh++),30,E_STYLE17,(short*)(f17+T17->FieldOffset(9)),6));

  w->Add(new _Static(1,(wh),E_STYLE17,"���_���⭥�        "));
  w->Add(new _RelSCell(21,(wh++),50,E_STYLE17,5,(short*)(f17+T17->FieldOffset(4)),0,20,0,2 ));


  w->Add(new _Static(1,(wh),E_STYLE17,"���_�����⥫�      "));
  w->Add(new _RelSCell(21,(wh++),50,E_STYLE17,5,(short*)(f17+T17->FieldOffset(7)),0,20,0,2 ));

  w->Add(new _Static(1,(wh),E_STYLE17,"�����               "));
  w->Add(new _RelSCell(21,(wh++),50,E_STYLE17,5,(short*)(f17+T17->FieldOffset(14)),0,14,0,2 ));

  w->Add(new _Static(1,(wh),E_STYLE17,"��� 䨭���஢����  "));
  w->Add(new _RelSCell(21,(wh++),50,E_STYLE17,5,(short*)(f17+T17->FieldOffset(15)),0,26,0,2 ));

  w->Add(new _Static(1,(wh++),E_STYLE17,"��⪮�_���ᠭ��    "));
  w->Add(BuildCell(T17,11,f17,1,&wh,w->w,E_STYLE17 ));

  w->Add(new _Static(1,(wh++),E_STYLE17,"����ঠ���          "));
  w->Add(BuildCell(T17,8,f17,1,&wh,w->w,E_STYLE17 ));

  w->Add(new _Static(1,(wh++),E_STYLE17,"�ਬ�砭��          "));
  w->Add(BuildCell(T17,10,f17,1,&wh,w->w,E_STYLE17 ));

  //wh++;
  w->AddCallBack(InsertF17Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void DeleteF17(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(f17)) return;
  if( !T22->GEQ(rf22,rf17->Nomer_dogovora,0) ||
      !T18->GEQ(rf18,rf17->Nomer_dogovora,0) ||
      !T27->GEQ(rf27,rf17->Nomer_dogovora,0)
    ){
        MessageBox( "����� 㤠���� ��ப� �������",
                    "���� ��ப� ��⥩ (F2)",
                    "��� ��ப� ����� (F3)",
                    "��� ����窨 ������� (F4)",
                    "������ Esc ��� �த�������"
                    );
     }else
        fv->Delete();
  E->ClearEvent((_Window*)parent);
};

static void PrintF17(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  fv->Lock(rf17);
  char name[16];
  DocReport(name);
  _ShowPrint *sp = new _ShowPrint(name,"����� �������");
  fv->Add(sp); sp->HelpID=100;
  sp->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void CB18(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(rf17)) return;
  AddF18(0,fv);
  E->ClearEvent((_Window*)parent);
};
static void CB22(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(rf17)) return;
  AddF22(0,fv);
  E->ClearEvent((_Window*)parent);
};

static void CB27(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  if(!fv->Lock(rf17)) return;
  AddF27(0,fv);
  E->ClearEvent((_Window*)parent);
};
static void FlipFilter(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  Filter=1-Filter;
  fff.Priznak_zakritiy=DocType;
  if(fv->Lock(rf17))
    fff.Tip_dogovora=rf17->Tip_dogovora;
  else
    fff.Tip_dogovora=0;

  if(Filter){
    {
      _Window *w= new _Window(0,0,53,1,E_STYLE17,stBuffered | stFrame,"FilterF17");
      short wh=1;
      w->CenterScr();
      w->Add(new _Static(1,(wh++),E_STYLE17,"��p������ ⨯ ������p� ��� ������� 䨫��p�"));
      w->Add(new _RelSCell(1,(wh),50,E_STYLE17,5,&(fff.Tip_dogovora),0,15,0,2 ));
      w->Exec();
      delete w;
    }
    fv->Del(fltr);
    fv->Add(fltr=new _Static(12,0,V_STYLE17,"/䨫���/"));
    fv->ChangeFilter(2, &fff, 4);
  }else{
    fv->Del(fltr);
    fv->Add(fltr=new _Static(12,0,V_STYLE17,"/  �� /"));
    fv->ChangeFilter(2, &fff, 2);
  }
  E->ClearEvent((_Window*)parent);
};
static void Choose(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  ChooseTheDocs(fv);
  E->ClearEvent((_Window*)parent);
};


void AddF17( void (*CallBackFunc)(_Event *,void *), _Window * parent){
  if(!Dostup[17])
    return;
  _Window * tmp =parent;
  if(!tmp) tmp =GDt;
  if(tmp->Find("�������")){
      tmp->Del(tmp->Find("�������"));
  }
  fff.Priznak_zakritiy=DocType;
  Filter=0;
  _FileView * fv=new _FileView (T17,2,&fff,2,WriteF17,2,2,75,20,1,V_STYLE17,"�������");
  fv->HelpID=8;
  fv->Add(new _Static(2,0,V_STYLE17," ������� "));
  fv->Add(fltr=new _Static(12,0,V_STYLE17,"/  �� /"));
  if(Dostup[17]==D_WRITE){
    fv->AddCallBack(EditF17, evKey,ENTER);
    if(!DocType){
      fv->AddCallBack(InsertF17, evKey,INSERT);
      fv->AddCallBack(DeleteF17, evKey,DELETE);
    }
  }
  fv->AddCallBack(PrintF17, evKey, CTRL_P);
  fv->AddCallBack(CB18, evKey,F3);
  fv->AddCallBack(CB22, evKey,F2);
  fv->AddCallBack(CB27, evKey,F4);
  fv->AddCallBack(FlipFilter, evKey,F6);
  fv->AddCallBack(Choose, evKey,F7);
  if(CallBackFunc)
        fv->AddCallBack(CallBackFunc, evKey, ESC);
  tmp->Add(fv);
  tmp->SwitchFocus(fv);
};


