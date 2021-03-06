/****************************************************
 *     CHOOSE.CPP                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#include "common.h"
#include "doc.h"
#include "clsctrs.h"

static void BuildDoc(_Window *fv );

struct choose_type {
  short field;
  short type;
              // 1 - �ᯮ������ ���祭��
              // 2 - �ᯮ������ �஬���⮪ ���祭��
              // 3 - �ᯮ������ �宦�����
              // 4 - �ᯮ������ ���祭�� �� �����䨪����
              // 5 - �ᯮ������ ���祭�� �� �ࠢ�筨��

  short relation; // - ����� 䠩�� �ࠢ�筨�� / �����䨪���
  short relkey;   // - ���� ��� �裡 � �ࠢ�筨���
  short vfield;   // - ���� ��� ������ � �ࠢ�筨��
};

static void *l,*r;
static unsigned char *yn;
static int cnt=0;

static choose_type mask[]={
    1 , 5, 17, 0, 1, // �����_�������
    2 , 5, 15, 0, 2, // ���_�������
    3 , 2, 0 , 0, 0, // ���_�����祭��
    4 , 5, 20, 0, 2, // ���_���⭥�
    5 , 2, 0 , 0, 0, // ���_��砫�
    6 , 2, 0 , 0, 0, // ���_����砭��
    7 , 5, 20, 0, 2, // ���_�����⥫�
    8 , 3, 0 , 0, 0, // ����ঠ���
    9 , 4, 6 , 0, 0, // ����ﭨ�_�������
    10, 3, 0 , 0, 0, // �ਬ�砭��
    11, 3, 0 , 0, 0, // ��⪮�_���ᠭ��
    14, 5, 14, 0, 2, // �����
    15, 5, 26, 0, 2, // ��� 䨭���஢����
    0 , 0, 0 , 0, 0  //------
};
void ChooseTheDocs(_FileView *fv){
  Table *t= fv->T;
  l=new char[t->max_rec_len];
  r=new char[t->max_rec_len];
  memset(l,0,t->max_rec_len);
  memset(r,0,t->max_rec_len);
  char **items;
  while(mask[cnt].field){
    cnt++;
  };

  items =  new char*[cnt];
  yn =  new unsigned char[cnt];
  for(int i=0;i<cnt;i++){
    items[i]=new char[23];
    t->FieldName(mask[i].field,items[i]);
    strcat(items[i],"  ");
  }
  memset(yn,0,cnt);
  short res;
  do{
    _VMenu * vm= new _VMenu(0,0,cnt,items,&res);
    vm->st|=stFrame;
    vm->HelpID=21;
    vm->Add(new _Static(5,0,ST_VMENU,"��ࠬ���� ���᪠"));
    vm->CenterScr();
    vm->Exec();
    if(res){
      _Window * blank = new _Window(0,0,0,0,ST_VMENU,stFrame);
      blank->Add(new _Static(1,1,ST_VMENU,"�ᯮ�짮���� ��� �롮�"));
      blank->Add(new _YNCell(25,1,ST_VMENU,&(yn[res-1])));
      items[res-1][20]=0;
      short y=3;
      char * off =(char*)l;
      off+=t->FieldOffset(mask[res-1].field);
      switch(mask[res-1].type){
        // ���⮥ ���祭��
        case 1:
        case 3:
          blank->Add(new  _Static(1,2,ST_VMENU,items[res-1]));
          blank->Add(BuildCell(t, mask[res-1].field, l, 1,&y ,70, ST_VMENU));
          break;
        case 2:
          blank->Add(new  _Static(1,2,ST_VMENU,items[res-1]));
          blank->Add(new  _Static(1,3,ST_VMENU,"�� "));
          blank->Add(BuildCell(t, mask[res-1].field, l, 4,&y ,60, ST_VMENU));
          blank->Add(new  _Static(1,4,ST_VMENU,"�� "));
          blank->Add(BuildCell(t, mask[res-1].field, r, 4,&y ,60, ST_VMENU));
          break;
        case 4:
          blank->Add(new  _Static(1,2,ST_VMENU,items[res-1]));
          blank->w=32;
          blank->Add(new _ClsrCell(1,3, 30, ST_VMENU,(short*)off, mask[res-1].relation));
          break;
        case  5:
          blank->Add(new  _Static(1,2,ST_VMENU,items[res-1]));
          blank->w=72;
          switch(t->FieldType(mask[res-1].field)){
            case 0:
            case 11:
               blank->Add(new _RelCCell(1,3,65,ST_VMENU,t->FieldSize(mask[res-1].field)-1,off,
                              mask[res-1].relation,mask[res-1].relkey,mask[res-1].vfield));
               break;
            case 1:
            case 15:
            case 14:
              if(t->FieldSize(mask[res-1].field)==2)
                blank->Add(new _RelSCell(1,3,65,ST_VMENU,6,(short*)off,0,
                           mask[res-1].relation,mask[res-1].relkey,mask[res-1].vfield));
              if(t->FieldSize(mask[res-1].field)==4)
                blank->Add(new _RelLCell(1,3,65,ST_VMENU,10,(long*)off,0,
                           mask[res-1].relation,mask[res-1].relkey,mask[res-1].vfield));
              break;
          }
          break;

      }
      blank->CenterScr();
      blank->HelpID=22;
      blank->Exec();
      delete blank;

      items[res-1][20]=' ';
      if(yn[res-1])
        items[res-1][21]='�';
      else
        items[res-1][21]=' ';
    }
    delete vm;
  }while(res);
  res=MessageBox(
       "�������� �ࠢ�� �� ��࠭�� ��ࠬ��ࠬ ?",
       "        �� - [Enter]  ��� - [Esc]"
      );
  if(res){
    // ��諠 ���� ...
    BuildDoc(fv);
  }

  for(i=0;i<cnt;i++){
    delete(items[i]);
  }
  delete items;
  delete yn;
  delete l;
  delete r;
};


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

static void BuildDoc(_Window *fv ){
  T8 = T8->St.FindTable("X$Classificator");
  _Date b_date,e_date,date,prevdate,lastdate;
  b_date.Set(1,1,1900);
  e_date.Set(1,1,2000);

  long g_cnt=0;
  _Gauge *g = new _Gauge(0,0,76,ST_GAUGE,T17->St.Records(T17));
  g->CenterScr();
  g->Show();
  GDt->Add(g);
  file_17 *r17 = new file_17;
  file_22 *r22 = new file_22;
  file_23 *r23 = new file_23;
  double msum,sum,plan,cost,total,stotal,mtotal;
  double stype[3];
  int res = T17->GFirst(r17,1);
  char f_name[15];
  char pbuf[201];
  _LowPrint *lp = new _LowPrint(65,f_name);
  lp->SetHeader(Head);
  lp->SetFooter(1,Foot);
  char h_buf[80];
  char ebuf[12],bbuf[12];
  total = 0.0;
  stotal = 0.0;
  mtotal = 0.0;
  stype[0]=stype[1]=stype[2]=0;
  // ��ᬠ�ਢ��� �� �������
  int first=1;
  short prev_tip=0;
  short flag;
  short off,sz;
  while(!res){

    flag=1;


    // ��ࠡ��뢠�� �᫮��� ��� �ਭ��� �����
    for(int iii=0;iii<cnt;iii++){
      if(yn[iii]){
        off=T17->FieldOffset(mask[iii].field);
        sz=T17->FieldSize(mask[iii].field);
        switch(mask[iii].type){
          case 1:
          case 4:
          case 5:
           if(memcmp(((char*)r17)+off,((char*)l)+off,sz))
             flag=0;
          break;
          case 3:
            {
            StrTrim(((char*)l)+off);
            StrUpper(((char*)l)+off);
            char *tmpc=new char[sz+1];
            memcpy(tmpc,((char*)r17)+off,sz);
            tmpc[sz]=0;
            StrTrim(tmpc);
            StrUpper(tmpc);
            if(!strstr(tmpc,((char*)l)+off))
              flag=0;
            }
          break;
          case 2:
          switch(T17->FieldType(mask[iii].field)){
                case 1:
                case 15:
                case 14:
                  switch(sz){
                  case 1:
                    {
                      int t1=*((unsigned char*)r17+off);
                      int t2=*((unsigned char*)l+off);
                      int t3=*((unsigned char*)r+off);
                      if(t2>t1 || t1>t3)
                        flag=0;
                    }
                    break;
                  case 2:
                    {
                      int t1=*(int*)((char*)r17+off);
                      int t2=*(int*)((char*)l+off);
                      int t3=*(int*)((char*)r+off);
                      if(t2>t1 || t1>t3)
                        flag=0;
                    }
                    break;

                  case 4:
                    {
                      int t1=*(long*)((char*)r17+off);
                      int t2=*(long*)((char*)l+off);
                      int t3=*(long*)((char*)r+off);
                      if(t2>t1 || t1>t3)
                        flag=0;
                    }
                    break;
                  }
                  break;
                case 2:
                  if(sz==4){
                    float t1 = *(float*)((char*)r17+off);
                    float t2 = *(float*)((char*)l+off);
                    float t3 = *(float*)((char*)r+off);
                    if(t2>t1 || t1>t3)
                      flag=0;

                  }
                  if(sz==8){
                    double t1 = *(double*)((char*)r17+off);
                    double t2 = *(double*)((char*)l+off);
                    double t3 = *(double*)((char*)r+off);
                    if(t2>t1 || t1>t3)
                      flag=0;

                  }
                  break;
                case 16:
                  {
                    long double t1 = *(long double*)((char*)r17+off);
                    long double t2 = *(long double*)((char*)l+off);
                    long double t3 = *(long double*)((char*)r+off);
                    if(t2>t1 || t1>t3)
                      flag=0;
                  }
                  break;

                case 3:
                  {
                    _Date t1 = *(_Date*)((char*)r17+off);
                    _Date t2 = *(_Date*)((char*)l+off);
                    _Date t3 = *(_Date*)((char*)r+off);
                    if(t2>t1 || t1>t3)
                      flag=0;
                  }
                  break;

                case 4:
                  {
                    _Time t1 = *(_Time*)((char*)r17+off);
                    _Time t2 = *(_Time*)((char*)l+off);
                    _Time t3 = *(_Time*)((char*)r+off);
                    if(t2>t1 || t1>t3)
                      flag=0;
                  }
                  break;
          }// switch �� �஬�����
        } // switch(mask[iii].type)

        }
    }



    sum  =0.0;
    plan =0.0;
    msum=0;

    g->Update(g_cnt+1);


    if(flag){
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
    _ShowPrint *sp= (_ShowPrint*)fv->Find("����� �롮ન");
    fv->Del(sp);
    sp = new _ShowPrint(f_name, "����� �롮ન");
    fv->Add(sp); sp->HelpID=100;
    fv->SwitchFocus(sp);
  }else{
    MessageBox( "���� ������஢ ���� ?!",
                "������ ESC ��� �த�������");
  }
};




