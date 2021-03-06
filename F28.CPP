/****************************************************
 *        F28.CPP                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#include "common.h"
#include "doc.h"


static short ShowMask28[]={
       1,   // ����� ���
       2,   // �ਧ���_������
       3,   // ��� ����㯫����
       4,   // ��� ������
       //5,   // ��� ���⭥�
       6,   // �㬬�
       //7,   // �����
       8,   // �ਬ�砭��
       //9,   // ��� ��� ������
       0    // end_of_list
};

file_28 * rf28;
Table *T28;
static char  * f28;
static unsigned char op=0;

void Init28(void){
  T28= new Table("�६����_���",OWNER);
  if(!T28->opened){
	printf("Error: file �६����_��� not opened\n");
        exit(0);
  }
  f28=new char [T28->max_rec_len];
  rf28 = (file_28*)f28;
};

void Close28(void){
  delete T28;
  delete f28;
};

static void WriteF28(void *rec, char *buf){
    int curlen=0,sz;
    buf[0]=0;
    char *buf2;
    int i=0;
    while(ShowMask28[i]){
      sz=T28->SizeForPrint(ShowMask28[i]);
      if(curlen+sz+1<MAX_STRING_WIDTH){
        buf2 = new char[sz+1];
        memset(buf2,0,sz+1);
        T28->PrintField(ShowMask28[i], rec ,buf2);
        strcat(buf,buf2);
        strcat(buf,"�");
        curlen=curlen+sz+1;
        delete buf2;
      }
      i++;
    }
};


static void EditF28Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  if(op && !rf28->Priznak_oplati)
    rf28->Priznak_oplati=op;
  if(!op  && rf28->Priznak_oplati){

    if(!MessageBox("��䨪�஢��� ������ ���?",
                  "�� - [Enter]    ��� - [Esc]")){
      /*
        memcpy(rf13->Nomer_sheta,rf28->Nomer_sheta,16);
        rf13->data=rf28->Data_oplati;
        strcpy(rf13->Kod_dogovora,"������ ���   ");
        rf13->Rashod=1;
        rf13->Summa=-rf28->Summa;
        T13->Insert(rf13);
    }else{*/
      rf28->Priznak_oplati=0;
    }
  }
  fv->Update(f28);
  E->msg.K=ESC;
};

static void InsertF28Done(_Event *E,void *parent){
  ((_Window*) parent)->LostFocus(0);
  _FileView *fv= (_FileView*)(((_Window *) parent)->parent);
  fv->Insert(f28);
  E->msg.K=ESC;
};

static void EditF28(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  fv->Lock(f28);
  op=rf28->Priznak_oplati;
  _Window *w= new _Window(1,1,77,1,E_STYLE28,stBuffered | stFrame,"EditF28");
  fv->Add(w);    w->HelpID=19;
  short wh=1;

  w->Add(new _Static(1,(wh),E_STYLE28,"����� ���         "));
  w->Add(BuildCell(T28,1,f28,21,&wh,w->w,E_STYLE28 ));

  w->Add(new _Static(1,(wh),E_STYLE28,"�ਧ���_������      "));
  w->Add(BuildCell(T28,2,f28,21,&wh,w->w,E_STYLE28 ));

  w->Add(new _Static(1,(wh),E_STYLE28,"��� ����㯫����    "));
  w->Add(BuildCell(T28,3,f28,21,&wh,w->w,E_STYLE28 ));

  w->Add(new _Static(1,(wh),E_STYLE28,"��� ������         "));
  w->Add(BuildCell(T28,4,f28,21,&wh,w->w,E_STYLE28 ));

  w->Add(new _Static(1,(wh)++,E_STYLE28,"��� ���⭥�        "));
  w->Add(new _RelSCell(1,(wh++),70,E_STYLE28,5,(short*)(f28+T28->FieldOffset(5)),0,20,0,2 ));

  w->Add(new _Static(1,(wh),E_STYLE28,"�㬬�               "));
  w->Add(BuildCell(T28,6,f28,21,&wh,w->w,E_STYLE28 ));

  w->Add(new _Static(1,(wh)++,E_STYLE28,"�����              "));
  w->Add(new _RelCCell(1,(wh++),70,E_STYLE28,5,(f28+T28->FieldOffset(7)),10,0,2 ));

  w->Add(new _Static(1,(wh)++,E_STYLE28,"��� ��� ������     "));
  w->Add(new _RelCCell(1,(wh++),70,E_STYLE28,15,(f28+T28->FieldOffset(9)),12,0,2));

  w->Add(new _Static(1,(wh),E_STYLE28,"��� ��楯�           "));
  w->Add(BuildCell(T28,10,f28,21,&wh,w->w,E_STYLE28 ));

  w->Add(new _Static(1,(wh)++,E_STYLE28,"�ਬ�砭��          "));
  w->Add(BuildCell(T28,8,f28,1,&wh,w->w,E_STYLE28 ));


  w->AddCallBack(EditF28Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void InsertF28(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  memset(f28,0,sizeof(f28));
  InitRecord(T28,f28);
  _Window *w= new _Window(1,1,77,1,E_STYLE28,stBuffered | stFrame,"InsertF28");
  fv->Add(w);  w->HelpID=19;
  short wh=1;
  w->Add(new _Static(1,(wh),E_STYLE28,"����� ���         "));
  w->Add(BuildCell(T28,1,f28,21,&wh,w->w,E_STYLE28 ));

  //w->Add(new _Static(1,(wh),E_STYLE28,"�ਧ���_������      "));
  //w->Add(BuildCell(T28,2,f28,21,&wh,w->w,E_STYLE28 ));

  w->Add(new _Static(1,(wh),E_STYLE28,"��� ����㯫����    "));
  w->Add(BuildCell(T28,3,f28,21,&wh,w->w,E_STYLE28 ));

  //w->Add(new _Static(1,(wh),E_STYLE28,"��� ������         "));
  //w->Add(BuildCell(T28,4,f28,21,&wh,w->w,E_STYLE28 ));

  w->Add(new _Static(1,(wh)++,E_STYLE28,"��� ���⭥�        "));
  w->Add(new _RelSCell(1,(wh++),70,E_STYLE28,5,(short*)(f28+T28->FieldOffset(5)),0,20,0,2 ));

  w->Add(new _Static(1,(wh),E_STYLE28,"�㬬�               "));
  w->Add(BuildCell(T28,6,f28,21,&wh,w->w,E_STYLE28 ));

  w->Add(new _Static(1,(wh)++,E_STYLE28,"�����              "));
  w->Add(new _RelCCell(1,(wh++),70,E_STYLE28,5,(f28+T28->FieldOffset(7)), 10,0,2 ));

  w->Add(new _Static(1,(wh)++,E_STYLE28,"��� ��� ������     "));
  w->Add(new _RelCCell(1,(wh++),70,E_STYLE28,15,(f28+T28->FieldOffset(9)),12,0,2));

  w->Add(new _Static(1,(wh),E_STYLE28,"��� ��楯�           "));
  w->Add(BuildCell(T28,10,f28,21,&wh,w->w,E_STYLE28 ));

  w->Add(new _Static(1,(wh)++,E_STYLE28,"�ਬ�砭��          "));
  w->Add(BuildCell(T28,8,f28,1,&wh,w->w,E_STYLE28 ));

  w->AddCallBack(InsertF28Done,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent((_Window*)parent);
};

static void DeleteF28(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  fv->Delete();
  E->ClearEvent((_Window*)parent);
};

static void PrintF28(_Event *E, void * parent){
  _FileView *fv= (_FileView*) parent;
  char name[16];
  char buf[200];
  char dbuf[13];
  _LowPrint *lp =new _LowPrint(65,name);
  int res,first=1,pr;
  file_28_2 key;
  res=T28->GFirst(rf28,&key,2);
  double opl=0,nopl=0;
  lp->PrintNL("������ ���");
  lp->PrintNL("---------------------------------------------------------------------------------");
  lp->PrintNL("");
  while(!res){
    if(first){
      first=0;
      pr=rf28->Priznak_oplati;
      if(rf28->Priznak_oplati)
        lp->PrintNL("����祭�� ���");
      else
        lp->PrintNL("�� ����祭�� ���");
    }

    if(rf28->Priznak_oplati!=pr){
      if(rf28->Priznak_oplati){
        lp->PrintNL("---------------------------------------------------------------------------------");
        sprintf(buf,"�⮣� �� �� ����祭�� ��⠬ : %15.2lf",nopl);
        lp->PrintNL(buf);
        lp->PrintNL("");
        lp->PrintNL("����祭�� ���");
      }
    }
    if(rf28->Priznak_oplati)
      opl+=rf28->Summa;
    else
      nopl+=rf28->Summa;

    // � ��� �����祭
    StrClear(rf20->Naimenovanie,210);
    T20->GEQ(rf20,&rf28->Kod_partnera ,0);
    StrTrim(rf20->Naimenovanie);
    rf20->Naimenovanie[30]=0;
              // ��� ��� �㬬�
    sprintf(buf,"%-12s  %-16s  %15.2lf (%-30s)",
                rf28->Data_postupleniy.D2C(dbuf),
                rf28->Nomer_sheta,
                rf28->Summa,
                rf20->Naimenovanie);
    lp->PrintNL(buf);
    res=T28->GNext(rf28);
  };
  lp->PrintNL("---------------------------------------------------------------------------------");

  if(pr){
    sprintf(buf,"�⮣� ��    ����祭�� ��⠬ : %15.2lf",opl);
  }else{
    sprintf(buf,"�⮣� �� �� ����祭�� ��⠬ : %15.2lf",nopl);
  }
  lp->PrintNL(buf);
  lp->PrintNL("");
  lp->PrintNL("=================================================================================");
  sprintf(buf,"�ᥣ� �� ࠧ��� ��⠬       : %15.2lf",nopl+opl);
  lp->PrintNL(buf);


  delete lp;
  _ShowPrint *sp = new _ShowPrint(name,"����� ������ ���");
  fv->Add(sp);
  sp->GotFocus();
  E->ClearEvent((_Window*)parent);
};

void AddF28( void (*CallBackFunc)(_Event *,void *), _Window * parent){
  if(!Dostup[28])
    return;
  _Window * tmp =parent;
  if(!tmp) tmp =GDt;
  if(tmp->Find("������ ���")){
    if(tmp ==GDt){
      MessageBox("���� '������ ���' 㦥 ����㯭�,",
                 "��� ��४��祭�� �ᯮ����: ",
                 "Alt + "
                );
    }else{
      MessageBox("���� '������ ���' 㦥 ����㯭�,",
                 "��� ����� ��⨢���஢��� �� ࠧ"
                );
    }
    return;
  }
  _FileView * fv=new _FileView (T28,2,0,0,WriteF28,2,2,75,20,1,V_STYLE28,"������ ���");
  fv->Add(new _Static(2,0,V_STYLE28," ������ ��� "));
  if(Dostup[28]==D_WRITE){
    fv->AddCallBack(InsertF28, evKey,INSERT);
    fv->AddCallBack(EditF28, evKey,ENTER);
    fv->AddCallBack(DeleteF28, evKey,DELETE);
  }
  fv->AddCallBack(PrintF28, evKey, CTRL_P);
  if(CallBackFunc)
        fv->AddCallBack(CallBackFunc, evKey, ESC);
  fv->HelpID=23;
  tmp->Add(fv);
  tmp->SwitchFocus(fv);
};


