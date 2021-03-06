/****************************************************
 *     EXPERT.CPP                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#include "common.h"
#include "doc.h"
struct account {
  char    Nomer_sheta[16] ; //�����_���
  double sum;
  double cur;
};

int FindAcc( char * num);


void Expert(account *acc, int AccNum){


//1 - ���ᮧ���� 䠩� �।������� � �����

  // �᫨ ���� 㦥 �맢��� �� ��࠭,  ��� �ਤ���� 㤠����
  if(GDt->Find("�।�������_�_�����"))
      GDt->Del(GDt->Find("�।�������_�_�����"));
  char path[65];
  T19->St.GetFilePath("�।�������_�_�����",path);
  T19->St.Stat(T19);
  Close19();
  T19->St.Create(path);
  Init19();

//2 - �ந����� ������ �����������⥩
  file_25_1 k25a,k25b;
  int res,cur;
  double s=0;

  long g_cnt=0;
  _Gauge *g = new _Gauge(0,0,76,ST_GAUGE,T25->St.Records(T25));
  g->CenterScr();
  g->Add(new _Static(10,0,ST_GAUGE,"��ࠡ��뢠�� �롮�� ..."));
  g->Show();
  GDt->Add(g);

  res = T25->GLast(rf25,0);
  while(!res){
    g->Update(g_cnt++);
    /*
    Debug("Scan %s %s %15.2lf Ball=%d  Obr=%d\n"
                          ,rf25->Nomer_sheta
                          ,rf25->Kod_dogovora
                          ,rf25->Summa
                          ,rf25->Ball
                          ,rf25->Obrabotan
                          );
    */
    cur=FindAcc(rf25->Nomer_sheta);
    s=acc[cur].cur;
    Debug("Money: %s->%15.2lf\n",rf25->Nomer_sheta,s);

    // �� �뤠�� �� ��� ���죨 �� �⮬� �������� ?
    if(s>=rf25->Summa && rf25->Obrabotan==0){
      s-=rf25->Summa;
      rf25->Obrabotan=1;
      T25->Update(rf25);
      long pos;
      T25->GPos(&pos);

      // �⫮���� �� �宦����� ��� ��㣨� ��⮢
      memcpy(k25a.Kod_dogovora,rf25->Kod_dogovora,13);
      k25a.Statjy=rf25->Statjy;
      memcpy(&k25b,&k25a,sizeof(k25b));
      res = T25->GGE(rf25,&k25a,1);
      while(!res){
        if(memcmp(&k25a,&k25b,sizeof(k25b)))
          break;
        rf25->Obrabotan=1;
        T25->Update(rf25);
        res = T25->GNext(rf25,&k25a);
      }

      // �������� � �०���� ��������� � 䠩��
      T25->GDir(rf25,&pos,0);

      //������� �।������� �� ������ � 䠩� �।�������
      rf19->Razoviy_shet=0;
      rf19->Data.Today();
      rf19->Summa = rf25->Summa;
      memcpy(rf19->Nomer_dogovora,rf25->Kod_dogovora,13);
      memcpy(rf19->Nomer_sheta,rf25->Nomer_sheta,16);
      /*
      Debug("Insert %s %s %15.2lf (Bal=%d) R=%d\n"
                            ,rf19->Nomer_sheta
                            ,rf19->Nomer_dogovora
                            ,rf19->Summa
                            ,rf25->Ball
                            ,T19->Insert(rf19,0));
       */
      T19->Insert(rf19,0);

    }


    acc[cur].cur=s;
    res = T25->GPrev(rf25);
  }
  GDt->Del(g);

  // ������塞 ࠧ��� ���
  g = new _Gauge(0,0,76,ST_GAUGE,T28->St.Records(T28));
  g->CenterScr();
  g->Add(new _Static(10,0,ST_GAUGE,"������塞 ࠧ��� ��� ..."));
  g->Show();
  GDt->Add(g);
  res=T28->GFirst(rf28,0);
  g_cnt=0;
  while(!res){
    g->Update(g_cnt++);
    if(!rf28->Priznak_oplati){
      int i=FindAcc(rf28->Shet_dly_oplati );
      if(acc[i].sum>=rf28->Summa){
        rf19->Razoviy_shet=1;
        rf19->Data.Today();
        rf19->Summa = rf28->Summa;
        memcpy(rf19->Nomer_dogovora,rf28->Nomer_sheta,16);
        memcpy(rf19->Nomer_sheta,rf28->Shet_dly_oplati,16);
        T19->Insert(rf19,0);
      }
    }
    res=T28->GNext(rf28);
  }
  GDt->Del(g);
};
