/****************************************************
 *      DOC03.CPP                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#include "common.h"
#include "doc.h"
#include "clsctrs.h"

static Table *T8;

static char * h[] = {
 "",
 "���������������������������������������������������������������������������������Ŀ",
 "� ������������ ���㬥��, ����                             �  �㬬�             �",
 "���������������������������������������������������������������������������������Ĵ",
};
static char *blank =
 "�                                                            �                    �";

static char *f=
 "�����������������������������������������������������������������������������������";

static char * grp=
 "���������������������������������������������������������������������������������͵";

static void Head(_LowPrint *lp ,int page){
 for(int i=0; i<4 ;i++)
  lp->PrintNL(h[i]);
}

static void Foot(_LowPrint *lp ,int page){
  char * pbuf = new char [strlen(f)+1];
  strcpy(pbuf,f);
  sprintf(pbuf+2," ���. %d ",page);
  pbuf[strlen(pbuf)]='�';
  lp->PrintNL(pbuf);
  delete pbuf;
}

void BuildDoc03(void){
  T8 = T8->St.FindTable("X$Classificator");
  if(!MessageBox(
    "������� ���� �� ����� ������஢ ?",
    "  �� - [Enter]  ��� - [Esc]"      )) return;
  _Date b_date,e_date,date,prevdate,lastdate;
  b_date.Today();
  e_date.Today();

  // ᮧ���� ���� ����� ��ਮ�� (⨯� �������???)
  _Window *qw = new _Window(0,0,40,4,ST_WINDOW,stFrame,"����� ��ਮ��");
  qw->CenterScr();
  qw->Add(new _Static(1,1,ST_WINDOW,"��� ��砫� ��ந�� : "));
  qw->Add(new _DateCell(26,1,ST_WINDOW,&b_date));
  qw->Add(new _Static(1,2,ST_WINDOW,"��� ����砭�� ��ந�� : "));
  qw->Add(new _DateCell(26,2,ST_WINDOW,&e_date));
  qw->Exec();
  delete qw;

  long g_cnt=0;
  file_17 *r17 = new file_17;
  file_22 *r22 = new file_22;
  file_23 *r23 = new file_23;
  double msum=0,sum=0,plan=0,cost=0,total=0,stotal=0,mtotal=0;
  double stype[3]={0,0,0};
  char f_name[15];
  char pbuf[201];
  _LowPrint *lp = new _LowPrint(65,f_name);
  lp->SetHeader(Head);
  lp->SetFooter(1,Foot);
  char h_buf[80];
  char ebuf[12],bbuf[12];
  sprintf(h_buf
         ,"  ������㥬� �㬬� �� ������ ������஢ �� ��ਮ� c %s �� %s"
         ,b_date.D2C(bbuf)
         ,e_date.D2C(ebuf)
  );


  h[0]=h_buf;
  total = 0.0;
  stotal = 0.0;
  mtotal = 0.0;
  stype[0]=stype[1]=stype[2]=0;
  //��ᬠ�ਢ��� ࠧ��� ���
  _Gauge *g = new _Gauge(0,0,76,ST_GAUGE,T28->St.Records(T28));
  g->CenterScr();
  g->Show();
  GDt->Add(g);
  int res=T28->GFirst(rf28,0);
  sum=0;
  while(!res){
    g->Update(g_cnt++);
    if(rf28->Data_postupleniy>=b_date && rf28->Data_postupleniy<=e_date){

      // �����楯�� ��� ������� ����祭�묨 �� ������ �����
      if(!rf28->Priznak_oplati || rf28->Ne_vkluhatj_v_predl_)
        sum+=rf28->Summa;
      plan+=rf28->Summa;
    }
    res=T28->GNext(rf28);
  }
  GDt->Del(g);
  if(sum||plan){
    sprintf(pbuf,"�%-60s�%20.2lf�","���������� �� ࠧ��� ��⠬",plan);
    lp->PrintNL(pbuf);
    sprintf(pbuf,"�%-60s�%20.2lf�","�� ��� 㦥 ����祭� ",plan-sum);
    lp->PrintNL(pbuf);
    sprintf(pbuf,"�%-60s�%20.2lf�","�⮣� ������������� �� ࠧ��� ��⠬ ",sum);
    lp->PrintNL(pbuf);

    total+=plan;
    stotal+=plan;
    mtotal+=sum;
  }



  // ��ᬠ�ਢ��� �� �������
  g = new _Gauge(0,0,76,ST_GAUGE,T17->St.Records(T17));
  g->CenterScr();
  g->Show();
  GDt->Add(g);
  res = T17->GFirst(r17,1);

  int first=1;
  short prev_tip=0;
  while(!res){

    sum  =0.0;
    plan =0.0;
    msum=0;

    g->Update(g_cnt+1);

    // ������� �����祭�� � �室�騥 � ��ਮ�
    if(r17->Sostoynie_dogovora == 1 &&
       (r17->Data_nahala    < e_date  &&
        r17->Data_okonhaniy > b_date)
    ){
      // �������� �⮣ �� ⨯� ������஢
      if(first || r17->Tip_dogovora != prev_tip){
        if(first){
          first = 0;
          prev_tip = r17->Tip_dogovora;
        }else{
          if(!T15->GEQ(rf15,&prev_tip)){
            rf15->Nazvanie[50]=0;
          }else
            strcpy(rf15->Nazvanie,"��� �� �����⥭");

          prev_tip = r17->Tip_dogovora;
          lp->PrintNL(h[3]);
          lp->PrintNL(blank);
          sprintf(pbuf,"��⮣    : %-50s�%20s�",rf15->Nazvanie,"");
          lp->PrintNL(pbuf);
          sprintf(pbuf,"�%-60s�%20.2lf�","�ᥣ� �����஢�����",stype[0]);
          lp->PrintNL(pbuf);
          sprintf(pbuf,"�%-60s�%20.2lf�","��� �뤢���� �� ������",stype[1]);
          lp->PrintNL(pbuf);
          sprintf(pbuf,"�%-60s�%20.2lf�","  �� ��� ����祭�",stype[2]);
          lp->PrintNL(pbuf);
          sprintf(pbuf,"�%-60s�%20.2lf�","�� ���������� �������",stype[0]-stype[1]);
          lp->PrintNL(pbuf);
          stype[0]=stype[1]=stype[2]=0;
        }

        if(!T15->GEQ(rf15,&prev_tip)){
          rf15->Nazvanie[50]=0;
        }else
          strcpy(rf15->Nazvanie,"��� �� �����⥭");
        if(g_cnt)
          lp->PrintNL(grp);

        sprintf(pbuf,"���㯯�  : %-50s�%20s�",rf15->Nazvanie,"");
        lp->PrintNL(pbuf);
      }

      lp->PrintNL(h[3]);

      // �����⠫� ����� �������
                     r17->Kratkoe_opisanie[57]=0;
      sprintf(pbuf,"�������� %-16s%56s�",r17->Nomer_dogovora,r17->Kratkoe_opisanie);
      lp->PrintNL(pbuf);

      // �⠥� ���� �������
      file_22_0 k22;
      file_21   r21;
      int p_cnt;

      memcpy(&k22,r17->Nomer_dogovora,sizeof(k22));
      res=T22->GGE(r22,&k22,0);

      while(!res){
        if(memcmp(r22->Dogovor,r17->Nomer_dogovora,13))
          break;

        // ᯥ樠�쭮 ��� ��ਮ���᪨� ��⥩
        cost =0.0;              // ⥪�騩 ࠧ��� ����� �� ��ਮ�
        p_cnt = 0;
        prevdate=b_date;

        if(prevdate<r17->Data_nahala){
          prevdate=r17->Data_nahala;
        }
        lastdate=e_date;
        if(lastdate>r17->Data_okonhaniy){
          lastdate=r17->Data_okonhaniy;
        }

        // �⠥� ����� ����
        file_23_0 k23;
        memset(&k23,0,sizeof(k23));
        memcpy(k23.Dogovor,r22->Dogovor,13);
        k23.Statjy=r22->Stroka_dogovora;

        res = T23->GGE(r23,&k23,0);
        while(!res){
          if(memcmp(r23->Dogovor,r22->Dogovor,13))
            break;
          if(r23->Statjy!=r22->Stroka_dogovora)
            break;

          {
            short op = r23->Operaciy;
            file_8   r8;
            file_8_0 k8;

            if(!r22->Periodihnostj){
              if(op == 2 || op ==3 || op==6 || op ==7 || op == 8){
                if(op==6 && r23->Izmenyem_na>0.0)
                  r23->Izmenyem_na*=-1;

                // ����� �㬬������� ������
                sum += r23->Izmenyem_na;
              }else{
                // � �� 㦥 �����
                if(op!=5){
                  plan += r23->Izmenyem_na;
                }else{
                  plan = sum + r23->Izmenyem_na;
                }
              }
            }else{
              // ��� ���� ᨫ쭮 ���㬠�� !!!
              if(op==2 || op==3 || op==5 || op==6){
                // ��ﬠ� �㬬� �� ������
                sum += r23->Izmenyem_na;
              }else{
                if(!cost)
                  // �⮨����� ��ਮ���᪮�� ������
                  cost = r23->Izmenyem_na;
                else{
                  if(r23->Data_nahala_deystviy<lastdate){
                    p_cnt=Periods(&prevdate
                                  ,&r23->Data_nahala_deystviy
                                  ,r22->Periodihnostj
                                  ,r22->Avansirovanie);
                    // ������뢠�� ᪮�쪮 �������� �� �।��饩 業�
                    plan+= cost*p_cnt;
                    cost = r23->Izmenyem_na;
                    prevdate = r23->Data_nahala_deystviy;

                  }
                }

              }

            }
          }
          res = T23->GNext(r23);
        }
        if(r22->Periodihnostj){
          // ��⠥� ᪮�쪮 �� ������� �� ��᫥���� ��⠭��������
          // 業�
          p_cnt=Periods( &prevdate
                        ,&lastdate
                        ,r22->Periodihnostj
                        ,r22->Avansirovanie);
          plan+= cost*p_cnt;
        }
        res=T22->GNext(r22);
      }

      {
        //������ � ����� ����祭��� �㬬
        file_18 r18;
        file_18_0 k18;
        memcpy(k18.Nomer_dogovora,r17->Nomer_dogovora,13);
        res = T18->GGE(&r18 ,&k18,0);
        while(!res){
           if(memcmp(k18.Nomer_dogovora,r17->Nomer_dogovora,13))
            break;
           msum+= r18.Summa;
           res = T18->GNext(&r18,&k18);
        }
      }

      stype[0]+=plan;
      stype[1]+=sum;
      stype[2]+=msum;

      total+=plan;
      stotal+=sum;
      mtotal+=msum;

      // ����� �⮣� �� ��������
      if(sum || plan){
        sprintf(pbuf,"�%-60s�%20.2lf�","�ᥣ� �����஢�����",plan);
        lp->PrintNL(pbuf);
        sprintf(pbuf,"�%-60s�%20.2lf�","��� �뤢���� �� ������",sum);
        lp->PrintNL(pbuf);
        sprintf(pbuf,"�%-60s�%20.2lf�","  �� ��� ����祭�",msum);
        lp->PrintNL(pbuf);
        plan=plan-sum;
        sprintf(pbuf,"�%-60s�%20.2lf�","�� ���������� �������",plan);
        lp->PrintNL(pbuf);
      }
    }
    g_cnt++;
    res = T17->GNext(r17);
  }

  if(stype[0]||stype[1]||stype[2]){

    if(!T15->GEQ(rf15,&prev_tip)){
      rf15->Nazvanie[50]=0;
    }else
      strcpy(rf15->Nazvanie,"��� �� �����⥭");
    lp->PrintNL(h[3]);
    lp->PrintNL(blank);
    sprintf(pbuf,"��⮣    : %-50s�%20s�",rf15->Nazvanie,"");
    lp->PrintNL(pbuf);
    sprintf(pbuf,"�%-60s�%20.2lf�","�ᥣ� �����஢�����",stype[0]);
    lp->PrintNL(pbuf);
    sprintf(pbuf,"�%-60s�%20.2lf�","��� �뤢���� �� ������",stype[1]);
    lp->PrintNL(pbuf);
    sprintf(pbuf,"�%-60s�%20.2lf�","  �� ��� ����祭�",stype[2]);
    lp->PrintNL(pbuf);
    sprintf(pbuf,"�%-60s�%20.2lf�","�� ���������� �������",stype[0]-stype[1]);
    lp->PrintNL(pbuf);
    stype[0]=stype[1]=stype[2]=0;
  }

  lp->PrintNL(grp);
  sprintf(pbuf,"�%-60s %20.2lf�","�⮣� ���������� �� ������祭�� ��ਮ� ",total);
  lp->PrintNL(pbuf);
  sprintf(pbuf,"�%-60s %20.2lf�","�뤢���� � ����� ",stotal);
  lp->PrintNL(pbuf);
  sprintf(pbuf,"�%-60s %20.2lf�","  �� ��� 㦥 ����祭� ",mtotal);
  lp->PrintNL(pbuf);
  sprintf(pbuf,"�%-60s %20.2lf�","�� ���������� �������",total-stotal);
  lp->PrintNL(pbuf);

  delete r17;
  delete r22;
  delete r23;

  GDt->Del(g);
  delete lp;

  if(g_cnt){
    _ShowPrint *sp= (_ShowPrint*)GDt->Find("����� �����");
    GDt->Del(sp);
    sp = new _ShowPrint(f_name, "����� �����");
    GDt->Add(sp); sp->HelpID=100;
    GDt->SwitchFocus(sp);
  }else{
    MessageBox( "���� ������஢ ���� ?!",
                "������ ESC ��� �த�������");
  }
};
