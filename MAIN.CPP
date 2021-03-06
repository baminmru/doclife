/****************************************************
 *       MAIN.CPP                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <process.h>
#include "common.h"
#include "doc.h"
#include "hmenu.h"
#include "protect.h"
#include "password.h"
#include "logo.h"

char Dostup[51];
char Coder[]="/,m/lzmfphoadfb,mczxb'klamfe]pogqmfd'vbmzcba[fjobvaoygfWEOFPUHWET]JABFG[AEIRHG  [OFNCXKV;HjbcxVLJKsdV\"cxkfg[sfdg]";
short Administrator=0,NoLogo=0;

extern unsigned _stklen=30000;

char *OWNER="MIKE";

short ColorKit(void);

void MainMenu(void);
void ServeMainChoice(_Event *E, void * parent);
void ServeHelpChoice(void);
void ServeDogChoice(void);
void ServeSmetChoice(void);
void BuildDoc01(void);   // ᮧ����� ���� � ������������ �� ������ࠬ
void BuildDoc02(void);   // ᮧ����� 䠩�� �।������� � �����
void BuildDoc03(void);   // ᮧ����� ���� � ����� �� ������ࠬ
void BuildDoc04(void);   // ᮧ����� ���� �� �ᯮ������ ᬥ��
void EndOfPeriod(void);  // ��ࠡ���� ����� �����
void ExportToParus(void);
void CheckParusDoc(void);
void SearchForPartners(void);


short style=1;
short main_choice=0;
short help_choice=0;


void MainMenu(void){
  _Window * w;

  char * main_menu[]={
    "�������",
    "���� ��室��",
    "�믨᪨ ����� ",
    "�ਮ����",
    "��ࠢ�筨��",
    "DL������",
    0
  };
  w=new _HMenu(3,1,main_menu,&main_choice);
  w->HelpID=2;
  w->name = "������� ����";
  w->st |= stNoMove;
  w->st |= stFrame;
  w->AddCallBack(ServeMainChoice,evKey,ESC);
  w->AddCallBack(ServeMainChoice,evKey,ENTER);
  GDt->Add(w);
};


void ColorOption(_Event *E, void * parent){
  SaveColor("current.pal");
  if(ColorKit())
    SaveColor("current.pal");
  else
    LoadColor("current.pal");
};

void ServeMainChoice(_Event *E, void * parent){
  short ax=0,ay=0;
  _Window *p=(_Window*) parent;
  p->Coord(&ax,&ay);
  if(ax+2>75) ax-=10;
  if(ay+1>24) ay-=8;
  switch(main_choice){
    case 0:
      if(MessageBox(
        "�� ��� �������� ࠡ��� � �ணࠬ��� ?",
        "      �� -  [Enter]   ��� - [Esc]")){
        GDt->Q.AddMessage(evKey,ALT_X);
        return;
      }
      break;
    case 1:
      {
        _Window * w;

        static char * dog_menu[]={
          "������� ������஢",
          "������� �������",
          "���",
          "���� � ��������������", //doc01
          "���� �� ������ࠬ",      //doc03
          "��ନ஢��� �।������� � �����",   //doc02
          "����� � �।�������� � �����",
          0
        };


        w=new _VMenu(ax+1,ay+2,dog_menu,&help_choice);
        w->HelpID=3;
        //w->Add( new _Static(2,0,2," ������� "));
        w->Exec();
        delete w;
        ServeDogChoice();
      }
      break;

    case 2:
      {
        _Window * w;

        static char * dog_menu[]={
          "����",
          "�ᯮ������ ᬥ��",
          //"���� �ᯮ������",
          0
        };
        w=new _VMenu(ax+10,ay+2,dog_menu,&help_choice);
        w->HelpID=4;
        //w->Add( new _Static(2,0,2," ���� "));
        w->Exec();
        delete w;
        ServeSmetChoice();
      }
      break;

    case 3:
        AddF13(0);
      break;
    case 4:
      {
        _Window * w;

        static char * pr_menu[]={
          "�ਮ���� �� ������⥫�",
          "��� 䨭���஢����",
          0
        };


        w=new _VMenu(ax+40,ay+2,pr_menu,&help_choice);
        w->HelpID=5;
        //w->Add( new _Static(2,0,2," �ਮ���� "));
        w->Exec();
        delete w;
        switch(help_choice){
          case 1:
              AddF16(0);
            break;
          case 2:
              AddF26(0);
            break;
        };
      }
      break;


    case 5:
      {
        _Window * w;

        static char * help_menu[]={
          "���� ��室��",            //10
          "����� ��⮢ ���",          //12
          "��ࠢ�筨� ࠩ����",         //14
          "���� ������஢",             //15
          "��ࠢ�筨� ���⭥஢",       //20
          "��ࠢ�筨� ࠡ��",           //21
          "������� ��஫�",
          "�஢�� ����㯠",             //29
          0
        };


        w=new _VMenu(ax+51,ay+2,help_menu,&help_choice);
        w->HelpID=6;
        //w->Add( new _Static(2,0,2," ��ࠢ�筨�� "));
        w->Exec();
        delete w;
        ServeHelpChoice();
      }
      break;
    case 6:
      {
        _Window * w;

        static char * parus_menu[]={
          "��ᯮ�� ���⥦��",            //10
          "�஢�ઠ ������",            //12
          "����㧪� ���⭥஢",         //14
          0
        };


        w=new _VMenu(ax+53,ay+2,parus_menu,&help_choice);
        w->HelpID=32;
        w->Exec();
        delete w;
        switch(help_choice){
          case 1:
            ExportToParus();
            break;
          case 2:
            CheckParusDoc();
            break;
          case 3:
            SearchForPartners();
            break;
        }
      }
      break;
  };
  E->ClearEvent(0);
};

void ServeHelpChoice(void){
  switch(help_choice){

    case 1:
      AddF10(0);
      break;

    case 2:
      AddF12(0);
      break;

    case 3:
      AddF14(0);
      break;

    case 4:
      AddF15(0);
      break;
    case 5:
      AddF20(0);
      break;
    case 6:
      AddF21(0);
      break;
    case 7:
      {
      file_29_0 k29;
      int i;
      StrClear(k29.Kod_dostupa,10);
      StrClear(k29.Parolj,30);
      char NewPass[31];
      StrClear(NewPass,30);
      _Window * w=new _Window(20,5,45,4,ST_OPTION,stFrame,"ChPass");
      w->Add(new _Static(1,1,ST_OPTION,"���    : "));
      w->Add(new _Password(15,1,10,ST_OPTION,k29.Kod_dostupa));
      w->Add(new _Static(1,2,ST_OPTION,"��஫� : "));
      w->Add(new _Password(15,2,30,ST_OPTION,k29.Parolj));
      w->Add(new _Static(1,3,ST_OPTION,"���� ��஫� : "));
      w->Add(new _Password(15,3,30,ST_OPTION,NewPass));
      w->HelpID=31;

      w->Exec();
      delete w;
      unsigned char *mm=(char*) Coder;
      for(i=0; i<10;i++){
        k29.Kod_dostupa[i]^=mm[i];
      }
      for(i=0; i<30;i++){
        k29.Parolj[i]^=mm[i+10];
      }
      if(!T29->GEQ(rf29,&k29,0)){
        // ������㥬
        if(T29->GEQ(rf29,&k29,0,200)){
          MessageBox(
            "������ ����� ��㣨� ���짮��⥫�� ???",
            "�� �������� ������� ����� �� ���������� !!!",
            "�᫨ �� ।������ ������ � ���� \"�஢��� ����㯠\"",
            "���砫� ���ன� �� ����..."
            );
        }else{
          // ���㥬 ��஫�
          for(i=0; i<30;i++){
            NewPass[i]^=mm[i+10];
          }

          // �����뢠�� � ���� ������
          memcpy(rf29->Parolj,NewPass,31);
          if(!T29->Update(rf29)){
            MessageBox("���� �஫� ����ᥭ � ���� ������");
          }else{
            MessageBox(
              "�訡�� �����,",
              "������� � ������������ ���� ������..."
            );
          }
        }
      }else{
        MessageBox(
          "�� �訡���� � ����� ���� ��� ��஫�");
      }
    }
    break;

    case 8:
      AddF29(0);
      break;


  };
};

extern short DocType;

void ServeDogChoice(void){
  switch(help_choice){

    case 1:
      DocType=0;
      AddF17(0);
      break;

    case 2:
      DocType=1;
      AddF17(0);
      break;

    case 3:
      AddF28(0);
      break;

    case 4:
      BuildDoc01();
      break;

    case 5:
      BuildDoc03();
      break;


    case 6:
      if(Dostup[19])
        BuildDoc02();
      break;

    case 7:
      AddF19(0);
      break;

  };
};
void ServeSmetChoice(void){
  switch(help_choice){

    case 1:
      AddF11(0);
      break;

    case 2:
      BuildDoc04();
      break;

    case 3:
      break;
  };
};

extern int LPT;
short SetOwner=1;
short PassChecked=0;

int main(int argc, char * argv[]){
  int changepas=0;
  for(int i =1; i<argc; i++){
    StrUpper(argv[i]);
    switch(argv[i][0]){
    case '-':
    case '/':
      if(argv[i][1]!='?')
        break;
    case '?':
      printf("\n����� �ணࠬ�� DocLife:\n"
             "doc [-?] [/?] [?]  [Pn] [L]\n"
             "Pn    - ����� ���� �ਭ�� 1 ��� 2\n"
             "L  �⬥���� �뢮� ���⠢�� \n"
             );
             exit(0);

    case 'P':
      LPT=argv[i][1]-'1';
      if(LPT>2 || LPT<0)
        LPT=0;
    break;
    case 'L':
      NoLogo=1;
      break;
    case 'A':
      Administrator=1;
    }

  }

  printf("DocLife V2.0 for PARUS users (c) LME Researches 1995 \n");
  {
    _Date td;
    _Time tt;
    tt.Now();
    td.Today();
    char b[50];
    printf(td.PrtDate(b,1));
    printf(" ");
    printf(tt.T2C(b));
  }
  printf("\n���樠������...\n");
  unlink("trace.");
  Init10();
  Init11();
  Init12();
  Init13();
  Init14();
  Init15();
  Init16();
  Init17();
  Init18();
  Init19();
  Init20();
  Init21();
  Init22();
  Init23();
  Init24();
  Init25();
  Init26();
  Init27();
  Init28();
  Init29();
  Init30();

  LoadColor("current.pal");
  if(!NoLogo)
    ViewFli("test.fli",2,0);

  _Desktop *D = new _Desktop("DocLife V1.1");
  _Help Help("doc.hlp");
  D->Show();
  EndOfPeriod();



  {
    if(!Administrator){
      file_29_0 k29;
      StrClear(k29.Kod_dostupa,10);
      StrClear(k29.Parolj,30);
      _Window * w=new _Window(40,13,45,4,ST_OPTION,stFrame,"Password");
      w->Add(new _Static(1,1,ST_OPTION,"���    : "));
      w->Add(new _Password(10,1,10,ST_OPTION,k29.Kod_dostupa));
      w->Add(new _Static(1,2,ST_OPTION,"��஫� : "));
      w->Add(new _Password(10,2,30,ST_OPTION,k29.Parolj));
      w->Exec();
      delete w;
      unsigned char *mm=(char*) Coder;
      for(int i=0; i<10;i++){
        k29.Kod_dostupa[i]^=mm[i];
      }
      for(i=0; i<30;i++){
        k29.Parolj[i]^=mm[i+10];
      }
      if(!T29->GEQ(rf29,&k29,0)){
          for(i=0; i<50;i++){
            rf29->Privelegii[i]^=mm[i+40];
          }
          memcpy(Dostup,rf29->Privelegii,51);
      }else{
        MessageBox(
          "�� �訡���� � ����� ���� ��� ��஫�",
          "�������, 㢨����� � ᫥���騩 ࠧ ...");
        goto quit;
      }
    }else{
      memset(Dostup,D_WRITE,51);
    }
    D->AddCallBack(ColorOption,evKey,CTRL_O);
    MainMenu();

  }


  D->Run();
 quit:

  if(SetOwner)
  {
    T10->St.main_file->SetOwner("BMMDDF");
    T10->St.field_file->SetOwner("BMMDDF");
    T10->St.index_file->SetOwner("BMMDDF");
    T10->St.winsize_file->SetOwner("BMMDDF");
    T10->SetOwner(OWNER);
    T11->SetOwner(OWNER);
    T12->SetOwner(OWNER);
    T13->SetOwner(OWNER);
    T14->SetOwner(OWNER);
    T15->SetOwner(OWNER);
    T16->SetOwner(OWNER);
    T17->SetOwner(OWNER);
    T18->SetOwner(OWNER);
    T19->SetOwner(OWNER);
    T20->SetOwner(OWNER);
    T21->SetOwner(OWNER);
    T22->SetOwner(OWNER);
    T23->SetOwner(OWNER);
    T24->SetOwner(OWNER);
    T25->SetOwner(OWNER);
    T26->SetOwner(OWNER);
    T27->SetOwner(OWNER);
    T28->SetOwner(OWNER);
    T29->SetOwner(OWNER);
  }

  if(!NoLogo)
    Logo320x200("lme.pal","lme.xt");

  delete D;
  Close10();
  Close11();
  Close12();
  Close13();
  Close14();
  Close15();
  Close16();
  Close17();
  Close18();
  Close19();
  Close20();
  Close21();
  Close22();
  Close23();
  Close24();
  Close25();
  Close26();
  Close27();
  Close28();
  Close29();
  Close30();
  return 0;
};
