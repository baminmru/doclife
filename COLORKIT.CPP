/****************************************************
 *   COLORKIT.CPP                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#include "common.h"

extern unsigned char ColorArray[16][2];
static _ColorCell *act, *pas;
static _Window *win;
static long style=1;
static long fcolor=0;
static long fback=0;
static long pcolor=0;
static long pback=0;
static _ListBox *l1,*l2,*l3,*l4;

void ChangedP(void *p){
  ColorArray[style-1][0]=pback*16+pcolor;
  pas->color=ColorArray[style-1][0];
  pas->Show(1);
};

void ChangedF(void *p){
  ColorArray[style-1][1]=fback*16+fcolor;
  act->color=ColorArray[style-1][1];
  act->Show(1);
};

void ChangedStyle(void *p){
  fcolor=ColorArray[style-1][1];
  fback=fcolor;
  fcolor&=0x0f;
  fback>>=4;
  fback&=7;
  pcolor=ColorArray[style-1][0];
  pback=pcolor;
  pcolor&=0x0f;
  pback>>=4;
  pback&=7;
  l1->Load();
  l2->Load();
  l3->Load();
  l4->Load();
  ChangedP(p);
  ChangedF(p);
  win->Show(1);
};


short ColorKit(void){
  _Window * w= new _Window(0,0,76,8,ST_OPTION,stFrame,"�롮� 梥⮢");
  win=w;
  w->Add(new _Static(1,1,ST_OPTION,"��� 梥�:"));
  _ListBox *l=new _ListBox(15,1,30,8,ST_OPTION,0,&style);
  l->lbi.sort=0;
  l->lbi.AddItem("������� ����", ST_DESKTOP    );
  l->lbi.AddItem("����饭��", ST_MESWIN     );
  l->lbi.AddItem("������", ST_HELP       );
  l->lbi.AddItem("���஥��� �����䨪����", ST_CLSVIEW    );
  l->lbi.AddItem("��ᬮ�� 䠩��", ST_FILEVIEW   );
  l->lbi.AddItem("��ࠢ��� ���� ������", ST_RELVIEW    );
  l->lbi.AddItem("����� ���㬥��", ST_SHOWPRINT  );
  l->lbi.AddItem("�ண��� ��������", ST_GAUGE      );
  l->lbi.AddItem("����", ST_MENU );
  l->lbi.AddItem("���⨪��쭮� ����", ST_VMENU );
  l->lbi.AddItem("���� ।���஢����", ST_WINDOW );
  l->lbi.AddItem("���� ��⠭����", ST_OPTION     );
  l->lbi.AddItem("���᮪ ��ਠ�⮢", ST_LISTWINDOW );
  //l->lbi.AddItem("���� ।���஢���� ��⠭����", ST_CELL_O     );
  //l->lbi.AddItem("���� ।���஢���� ", ST_CELL     );
  //l->lbi.AddItem("�����", ST_CELL_2    );
  l->Changed=ChangedStyle;
  w->Add(l);
  w->Add(new _Static(1,2,ST_OPTION,"��⨢�� �������:"));
  w->Add(new _Static(1,3,ST_OPTION,"���� 䮭�:"));
  l=new _ListBox(15,3,20,8,ST_OPTION,0,&fback);
  l->lbi.sort=0;
  l->lbi.AddItem("����",BLACK );
  l->lbi.AddItem("�����",BLUE  );
  l->lbi.AddItem("������",GREEN );
  l->lbi.AddItem("���㡮�",CYAN  );
  l->lbi.AddItem("����",RED   );
  l->lbi.AddItem("��������",MAGENTA);
  l->lbi.AddItem("���筥��",BROWN  );
  l->lbi.AddItem("���⫮ ���",LIGHTGRAY);

  l->Changed=ChangedF;
  w->Add(l);
  l1=l;
  w->Add(new _Static(1,4,ST_OPTION,"���� ᨬ����:"));
  l=new _ListBox(15,4,20,8,ST_OPTION,0,&fcolor);
  l->lbi.sort=0;
  l->lbi.AddItem("����",BLACK );
  l->lbi.AddItem("�����",BLUE  );
  l->lbi.AddItem("������",GREEN );
  l->lbi.AddItem("���㡮�",CYAN  );
  l->lbi.AddItem("����",RED   );
  l->lbi.AddItem("��������",MAGENTA);
  l->lbi.AddItem("���筥��",BROWN  );
  l->lbi.AddItem("���⫮ ���",LIGHTGRAY);

  l->lbi.AddItem("����� ���",DARKGRAY );
  l->lbi.AddItem("����� ᨭ��",LIGHTBLUE);
  l->lbi.AddItem("���⫮ ������",LIGHTGREEN);
  l->lbi.AddItem("���⫮ ���㡮�",LIGHTCYAN );
  l->lbi.AddItem("���⫮ ����",LIGHTRED  );
  l->lbi.AddItem("���⫮ ��������",LIGHTMAGENTA);
  l->lbi.AddItem("�����",YELLOW      );
  l->lbi.AddItem("����",WHITE       );
  l->Changed=ChangedF;
  w->Add(l);
  l2=l;
  w->Add(act=new _ColorCell(1,6,(fback*16+fcolor)&0xff,"�ਬ�� ��⨢���� �������",1));

  w->Add(new _Static(37,2,ST_OPTION,"���ᨢ�� �������:"));
  w->Add(new _Static(37,3,ST_OPTION,"���� 䮭�:"));
  l=new _ListBox(52,3,20,8,ST_OPTION,0,&pback);
  l->lbi.sort=0;
  l->lbi.AddItem("����",BLACK );
  l->lbi.AddItem("�����",BLUE  );
  l->lbi.AddItem("������",GREEN );
  l->lbi.AddItem("���㡮�",CYAN  );
  l->lbi.AddItem("����",RED   );
  l->lbi.AddItem("��������",MAGENTA);
  l->lbi.AddItem("���筥��",BROWN  );
  l->lbi.AddItem("���⫮ ���",LIGHTGRAY);

  l->Changed=ChangedP;
  l3=l;
  w->Add(l);
  w->Add(new _Static(37,4,ST_OPTION,"���� ᨬ����:"));
  l=new _ListBox(52,4,20,8,ST_OPTION,0,&pcolor);
  l->lbi.sort=0;
  l->lbi.AddItem("����",BLACK );
  l->lbi.AddItem("�����",BLUE  );
  l->lbi.AddItem("������",GREEN );
  l->lbi.AddItem("���㡮�",CYAN  );
  l->lbi.AddItem("����",RED   );
  l->lbi.AddItem("��������",MAGENTA);
  l->lbi.AddItem("���筥��",BROWN  );
  l->lbi.AddItem("���⫮ ���",LIGHTGRAY);

  l->lbi.AddItem("����� ���",DARKGRAY );
  l->lbi.AddItem("����� ᨭ��",LIGHTBLUE);
  l->lbi.AddItem("���⫮ ������",LIGHTGREEN);
  l->lbi.AddItem("���⫮ ���㡮�",LIGHTCYAN );
  l->lbi.AddItem("���⫮ ����",LIGHTRED  );
  l->lbi.AddItem("���⫮ ��������",LIGHTMAGENTA);
  l->lbi.AddItem("�����",YELLOW      );
  l->lbi.AddItem("����",WHITE       );
  l->Changed=ChangedP;
  l4=l;
  w->Add(l);
  w->Add(pas=new _ColorCell(37,6,(pback*16+pcolor)&0xff,"�ਬ�� ���ᨢ���� �������",1));
  w->CenterScr();
  ChangedStyle(w);
  short res=w->Exec();
  delete w;
  ShowBase->Show(1);
  return res;
};
