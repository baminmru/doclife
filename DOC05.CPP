/****************************************************
 *      DOC05.CPP                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#include "common.h"
#include "doc.h"
#include "clsctrs.h"
char *wrap[3];


void DocReport(char *name){
   wrap[0] = new char [81];
   wrap[1] = new char [81];
   wrap[2] = new char [81];


  file_8 r8;
  file_8_0 key8;
  Table * T8 = T8->St.FindTable("X$Classificator");
  char pbuf[150],dbuf[11][2];
  _LowPrint *lp = new _LowPrint(65,name);

  // �������� � ��� �����祭��
  sprintf(pbuf,"������� N %s �� %s"
              ,rf17->Nomer_dogovora
              ,(rf17->Data_zakluheniy).D2C(dbuf[0]));
  lp->PrintNL(pbuf);
  sprintf(pbuf,"(%s)",rf17->Kratkoe_opisanie);
  lp->PrintNL(pbuf);

  // ��� �������
  T15->GEQ(rf15,&rf17->Tip_dogovora,0);
  sprintf(pbuf,"��� : %s", StrTrim(rf15->Nazvanie));
  lp->PrintNL(pbuf);

  // �������� �ਭ����������
  T14->GEQ(rf14,&rf17->Rayon,0);
  sprintf(pbuf,"�����: %s", StrTrim(rf14->Naimenovanie_rayona));
  lp->PrintNL(pbuf);



  // ���ﭨ� � ����
  key8.id=6;
  key8.item=rf17->Sostoynie_dogovora;
  T8->GEQ(&r8,&key8,0);

  sprintf(pbuf,"��砫� :  %s    ����砭��: %s  ����ﭨ� �������: %s"
                  ,(rf17->Data_nahala).D2C(dbuf[0])
                  ,(rf17->Data_okonhaniy).D2C(dbuf[1])
                  ,StrTrim(r8.name)
                  );
  lp->PrintNL(pbuf);

  // � ��� �����祭
  StrClear(rf20->Naimenovanie,210);
  T20->GEQ(rf20,&rf17->Kod_partnera ,0);
  StrTrim(rf20->Naimenovanie);

  int ar = StrForm(rf20->Naimenovanie,wrap,70,3);
  sprintf(pbuf,"���⭥�:  %s",wrap[0]);
  lp->PrintNL(pbuf);
  if(ar) ar--;
  if(ar){
    sprintf(pbuf,"          %s",wrap[1]);
    lp->PrintNL(pbuf);
    ar--;
  }
  if(ar){
    sprintf(pbuf,"          %s",wrap[2]);
    lp->PrintNL(pbuf);
  }

  // �����⥫�
  if(rf17->Kod_partnera!=rf17->Kod_poluhately){

    StrClear(rf20->Naimenovanie,210);
    T20->GEQ(rf20,&rf17->Kod_partnera ,0);
    StrTrim(rf20->Naimenovanie);

    ar = StrForm(rf20->Naimenovanie,wrap,70,3);
    sprintf(pbuf,"�����⥫�: %s",wrap[0]);
    lp->PrintNL(pbuf);
    if(ar) ar--;
    if(ar){
      sprintf(pbuf,"          %s",wrap[1]);
      lp->PrintNL(pbuf);
      ar--;
    }
    if(ar){
      sprintf(pbuf,"          %s",wrap[2]);
      lp->PrintNL(pbuf);
    }
  }

  lp->PrintNL("");

  // ᮤ�ঠ���
  ar=StrForm(StrTrim(rf17->Soderwanie),wrap,80,3);
  if(ar){
    lp->PrintNL(wrap[0]);
    ar--;
  }
  if(ar){
    lp->PrintNL(wrap[1]);
    ar--;
  }
  if(ar){
    lp->PrintNL(wrap[2]);
  }
  lp->PrintNL("");

  // ������� ...
  double sum=0.0,cost=0.0,plan=0.0,msum=0.0;
  short p_cnt;

  // �⠥� ���� �������
  file_22_0 k22;
  file_21   r21;
  _Date prevdate,lastdate;
  memcpy(&k22,rf17->Nomer_dogovora,sizeof(k22));
  int res=T22->GGE(rf22,&k22,0);

  while(!res){
    if(memcmp(rf22->Dogovor,rf17->Nomer_dogovora,13))
      break;

    // ᯥ樠�쭮 ��� ��ਮ���᪨� ��⥩
    cost =0.0;              // ⥪�騩 ࠧ��� ����� �� ��ਮ�
    p_cnt = 0;
    prevdate=rf17->Data_nahala;
    lastdate=rf17->Data_okonhaniy;

    // �⠥� ����� ����
    file_23_0 k23;
    memset(&k23,0,sizeof(k23));
    memcpy(k23.Dogovor,rf22->Dogovor,13);
    k23.Statjy=rf22->Stroka_dogovora;

    res = T23->GGE(rf23,&k23,0);
    while(!res){
      if(memcmp(rf23->Dogovor,rf22->Dogovor,13))
        break;
      if(rf23->Statjy!=rf22->Stroka_dogovora)
        break;

      {
        short op = rf23->Operaciy;

        if(!rf22->Periodihnostj){
          if(op == 2 || op ==3 || op==6 || op ==7 || op == 8){
            if(op==6 && rf23->Izmenyem_na>0.0)
              rf23->Izmenyem_na*=-1;

            // ����� �㬬������� ������
            sum += rf23->Izmenyem_na;
          }else{
            // � �� 㦥 �����
            if(op!=5){
              plan += rf23->Izmenyem_na;
            }else{
              plan = sum + rf23->Izmenyem_na;
            }
          }
        }else{
          if(op==2 || op==3 || op==5 || op==6){
            // ��ﬠ� �㬬� �� ������
            sum += rf23->Izmenyem_na;
          }else{
            if(!cost)
              // �⮨����� ��ਮ���᪮�� ������
              cost = rf23->Izmenyem_na;
            else{
              if(rf23->Data_nahala_deystviy<lastdate){
                p_cnt=Periods(&prevdate
                              ,&(rf23->Data_nahala_deystviy)
                              ,rf22->Periodihnostj
                              ,rf22->Avansirovanie);
                // ������뢠�� ᪮�쪮 �������� �� �।��饩 業�
                plan+= cost*p_cnt;
                cost = rf23->Izmenyem_na;
                prevdate = rf23->Data_nahala_deystviy;

              }
            }

          }

        }
      }
      res = T23->GNext(rf23);
    }
    if(rf22->Periodihnostj){
      // ��⠥� ᪮�쪮 �� ������� �� ��᫥���� ��⠭��������
      // 業�
      p_cnt=Periods( &prevdate
                    ,&lastdate
                    ,rf22->Periodihnostj
                    ,rf22->Avansirovanie);
      plan+= cost*p_cnt;
    }
    res=T22->GNext(rf22);
  }


  //������ � ����� ����祭��� �㬬
  file_18 r18;
  file_18_0 k18;
  memcpy(k18.Nomer_dogovora,rf17->Nomer_dogovora,13);
  res = T18->GGE(&r18 ,&k18,0);
  while(!res){
      if(memcmp(k18.Nomer_dogovora,rf17->Nomer_dogovora,13))
      break;
      msum+= r18.Summa;
      res = T18->GNext(&r18,&k18);
  }



  // ����� �⮣� �� ��������
  if(sum || plan || msum){
    sprintf(pbuf,"%-60s%20.2lf","�ᥣ� �����஢�����",plan);
    lp->PrintNL(pbuf);
    sprintf(pbuf,"%-60s%20.2lf","��� �뤢���� �� ������",sum);
    lp->PrintNL(pbuf);
    sprintf(pbuf,"%-60s%20.2lf","  �� ��� ����祭�",msum);
    lp->PrintNL(pbuf);
    plan=plan-sum;
    sprintf(pbuf,"%-60s%20.2lf","�� ��� �뤢������ � �����",plan);
    lp->PrintNL(pbuf);
  }


  delete lp;
  delete wrap[0];
  delete wrap[1];
  delete wrap[2];
};
