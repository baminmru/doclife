/****************************************************
 *      DOC04.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"
#include "doc.h"
#include "clsctrs.h"

static char *h[]=
{
  "",
  "┌──────────┬───────────────────┬───────────────────┬───────────────────┬───────────────────┬───────────────────┐",
  "│          │     план I кв     │    план II кв.    │    план III кв.   │     план IV кв.   │     план Год      │",
  "│ Статья   │  исполнение I кв. │ исполнение II кв. │ исполнение III кв.│ исполнение IV кв. │   исполнение Год  │",
  "│          │    исполнение %   │    исполнение %   │    исполнение %   │    исполнение %   │    исполнение %   │",
  "├──────────┼───────────────────┼───────────────────┼───────────────────┼───────────────────┼───────────────────┤"
};
static char *f=
{
  "└──────────┴───────────────────┴───────────────────┴───────────────────┴───────────────────┴───────────────────┘"
};

static void Head(_LowPrint *lp ,int page){
 for(int i=0; i<6 ;i++)
  lp->PrintNL(h[i]);
}

static void Foot(_LowPrint *lp ,int page){
  char * pbuf = new char [strlen(f)+1];
  strcpy(pbuf,f);
  sprintf(pbuf+2," стр. %d ",page);
  pbuf[strlen(pbuf)]='─';
  lp->PrintNL(pbuf);
  delete pbuf;
}

void CalcDoc(file_11 *t, file_11 *s,char *statja, short year);

void BuildDoc04(void)
{
  _Date td;
  td.Today();
  short year = td.y;
  _Window *y_win = new _Window(0,0,30,3,ST_WINDOW, stBuffered | stFrame ,"Задание года");
  y_win->CenterScr();
  y_win->Add(new _Static (1,1,ST_WINDOW,"Задайте год "));
  y_win->Add(new _ShortCell(15,1,ST_WINDOW,4,&year,0));
  y_win->Exec();
  delete y_win;

  td.BOY();
  td.y=year;

  char h0[50];
  sprintf(h0,"            Исполнение сметы на %d год.",year);
  h[0]=h0;
  char name[16],pbuf[150];
  _LowPrint *lp = new _LowPrint(65,name);
  lp->SetHeader(Head);
  lp->SetFooter(1,Foot);
  char a[6];
  file_11 sum[2],total[2];
  file_11_0 k11;
  memset(&k11,0,sizeof(k11));
  memset(sum,0,sizeof(sum));
  memset(total,0,sizeof(total));
  k11.God=year;
  int res =T11->GGE(rf11,&k11,0);
  if(!res && rf11->God==year){
    memset(sum,0,sizeof(sum));
    memset(total,0,sizeof(total));
    memcpy(a,rf11->Nomer_statji,6);
  }
  while(!res && rf11->God==year){
    if(memcmp(a,rf11->Nomer_statji,6)){
      sprintf(pbuf,"│%-10s│%19.2lf│%19.2lf│%19.2lf│%19.2lf│%19.2lf│"
                  , a
                  ,sum[0].Summa_na_1_kv
                  ,sum[0].Summa_na_2_kv
                  ,sum[0].Summa_na_3_kv
                  ,sum[0].Summa_na_4_kv
                  ,sum[0].Summa_na_1_kv
                  +sum[0].Summa_na_2_kv
                  +sum[0].Summa_na_3_kv
                  +sum[0].Summa_na_4_kv
                  );
      lp->PrintNL(pbuf);
      total[0].Summa_na_1_kv+=sum[0].Summa_na_1_kv;
      total[0].Summa_na_2_kv+=sum[0].Summa_na_2_kv;
      total[0].Summa_na_3_kv+=sum[0].Summa_na_3_kv;
      total[0].Summa_na_4_kv+=sum[0].Summa_na_4_kv;
      CalcDoc(&total[1],&sum[1],a,year);

      sprintf(pbuf,"│%-10s│%19.2lf│%19.2lf│%19.2lf│%19.2lf│%19.2lf│"
                  , ""
                  ,sum[1].Summa_na_1_kv
                  ,sum[1].Summa_na_2_kv
                  ,sum[1].Summa_na_3_kv
                  ,sum[1].Summa_na_4_kv
                  ,sum[1].Summa_na_1_kv
                  +sum[1].Summa_na_2_kv
                  +sum[1].Summa_na_3_kv
                  +sum[1].Summa_na_4_kv
                  );
      lp->PrintNL(pbuf);
      sprintf(pbuf,"│%-10s│%19.2lf│%19.2lf│%19.2lf│%19.2lf│%19.2lf│"
                  , ""
                  ,100*((sum[0].Summa_na_1_kv)?sum[1].Summa_na_1_kv/sum[0].Summa_na_1_kv:1)
                  ,100*((sum[0].Summa_na_2_kv)?sum[1].Summa_na_2_kv/sum[0].Summa_na_2_kv:1)
                  ,100*((sum[0].Summa_na_3_kv)?sum[1].Summa_na_3_kv/sum[0].Summa_na_3_kv:1)
                  ,100*((sum[0].Summa_na_4_kv)?sum[1].Summa_na_4_kv/sum[0].Summa_na_4_kv:1)
                  ,100*((sum[0].Summa_na_1_kv+sum[0].Summa_na_2_kv+
                         sum[0].Summa_na_3_kv+sum[0].Summa_na_4_kv )?
                          (sum[1].Summa_na_1_kv+sum[1].Summa_na_2_kv+
                           sum[1].Summa_na_3_kv+sum[1].Summa_na_4_kv )/
                          (sum[0].Summa_na_1_kv+sum[0].Summa_na_2_kv+
                           sum[0].Summa_na_3_kv+sum[0].Summa_na_4_kv )
                         :1)

                  );
      lp->PrintNL(pbuf);
      lp->PrintNL(h[5]);

      memset(&sum,0,sizeof(sum));
      memcpy(a,rf11->Nomer_statji,6);
    }
    if(rf11->Zamena){
      if(rf11->Summa_na_1_kv)
        sum[0].Summa_na_1_kv=rf11->Summa_na_1_kv;
      if(rf11->Summa_na_2_kv)
        sum[0].Summa_na_2_kv=rf11->Summa_na_2_kv;
      if(rf11->Summa_na_3_kv)
        sum[0].Summa_na_3_kv=rf11->Summa_na_3_kv;
      if(rf11->Summa_na_4_kv)
        sum[0].Summa_na_4_kv=rf11->Summa_na_4_kv;

    }else{
      sum[0].Summa_na_1_kv+=rf11->Summa_na_1_kv;
      sum[0].Summa_na_2_kv+=rf11->Summa_na_2_kv;
      sum[0].Summa_na_3_kv+=rf11->Summa_na_3_kv;
      sum[0].Summa_na_4_kv+=rf11->Summa_na_4_kv;
    }

    res = T11->GNext(rf11);

  }
  if(
      sum[0].Summa_na_1_kv
      +sum[0].Summa_na_2_kv
      +sum[0].Summa_na_3_kv
      +sum[0].Summa_na_4_kv
  ){
    sprintf(pbuf,"│%-10s│%19.2lf│%19.2lf│%19.2lf│%19.2lf│%19.2lf│"
                  , a
                  ,sum[0].Summa_na_1_kv
                  ,sum[0].Summa_na_2_kv
                  ,sum[0].Summa_na_3_kv
                  ,sum[0].Summa_na_4_kv
                  ,sum[0].Summa_na_1_kv
                  +sum[0].Summa_na_2_kv
                  +sum[0].Summa_na_3_kv
                  +sum[0].Summa_na_4_kv
                  );
    lp->PrintNL(pbuf);
    total[0].Summa_na_1_kv+=sum[0].Summa_na_1_kv;
    total[0].Summa_na_2_kv+=sum[0].Summa_na_2_kv;
    total[0].Summa_na_3_kv+=sum[0].Summa_na_3_kv;
    total[0].Summa_na_4_kv+=sum[0].Summa_na_4_kv;
    CalcDoc(&total[1],&sum[1],a,year);
    sprintf(pbuf,"│%-10s│%19.2lf│%19.2lf│%19.2lf│%19.2lf│%19.2lf│"
                , ""
                ,sum[1].Summa_na_1_kv
                ,sum[1].Summa_na_2_kv
                ,sum[1].Summa_na_3_kv
                ,sum[1].Summa_na_4_kv
                ,sum[1].Summa_na_1_kv
                +sum[1].Summa_na_2_kv
                +sum[1].Summa_na_3_kv
                +sum[1].Summa_na_4_kv
                );
    lp->PrintNL(pbuf);
    sprintf(pbuf,"│%-10s│%19.2lf│%19.2lf│%19.2lf│%19.2lf│%19.2lf│"
                , ""
                ,100*((sum[0].Summa_na_1_kv)?sum[1].Summa_na_1_kv/sum[0].Summa_na_1_kv:1)
                ,100*((sum[0].Summa_na_2_kv)?sum[1].Summa_na_2_kv/sum[0].Summa_na_2_kv:1)
                ,100*((sum[0].Summa_na_3_kv)?sum[1].Summa_na_3_kv/sum[0].Summa_na_3_kv:1)
                ,100*((sum[0].Summa_na_4_kv)?sum[1].Summa_na_4_kv/sum[0].Summa_na_4_kv:1)
                ,100*((sum[0].Summa_na_1_kv+sum[0].Summa_na_2_kv+
                        sum[0].Summa_na_3_kv+sum[0].Summa_na_4_kv )?
                        (sum[1].Summa_na_1_kv+sum[1].Summa_na_2_kv+
                          sum[1].Summa_na_3_kv+sum[1].Summa_na_4_kv )/
                        (sum[0].Summa_na_1_kv+sum[0].Summa_na_2_kv+
                          sum[0].Summa_na_3_kv+sum[0].Summa_na_4_kv )
                        :1)

                );
    lp->PrintNL(pbuf);
    lp->PrintNL(h[5]);

  }

  if(
      total[0].Summa_na_1_kv
      +total[0].Summa_na_2_kv
      +total[0].Summa_na_3_kv
      +total[0].Summa_na_4_kv
  ){
    sprintf(pbuf,"│%-10s│%19.2lf│%19.2lf│%19.2lf│%19.2lf│%19.2lf│"
                ,"Всего"
                ,total[0].Summa_na_1_kv
                ,total[0].Summa_na_2_kv
                ,total[0].Summa_na_3_kv
                ,total[0].Summa_na_4_kv
                ,total[0].Summa_na_1_kv
                +total[0].Summa_na_2_kv
                +total[0].Summa_na_3_kv
                +total[0].Summa_na_4_kv
                );
    lp->PrintNL(pbuf);
    sprintf(pbuf,"│%-10s│%19.2lf│%19.2lf│%19.2lf│%19.2lf│%19.2lf│"
                , ""
                ,total[1].Summa_na_1_kv
                ,total[1].Summa_na_2_kv
                ,total[1].Summa_na_3_kv
                ,total[1].Summa_na_4_kv
                ,total[1].Summa_na_1_kv
                +total[1].Summa_na_2_kv
                +total[1].Summa_na_3_kv
                +total[1].Summa_na_4_kv
                );
    lp->PrintNL(pbuf);
    sprintf(pbuf,"│%-10s│%19.2lf│%19.2lf│%19.2lf│%19.2lf│%19.2lf│"
                , ""
                ,100*((total[0].Summa_na_1_kv)?total[1].Summa_na_1_kv/total[0].Summa_na_1_kv:1)
                ,100*((total[0].Summa_na_2_kv)?total[1].Summa_na_2_kv/total[0].Summa_na_2_kv:1)
                ,100*((total[0].Summa_na_3_kv)?total[1].Summa_na_3_kv/total[0].Summa_na_3_kv:1)
                ,100*((total[0].Summa_na_4_kv)?total[1].Summa_na_4_kv/total[0].Summa_na_4_kv:1)
                ,100*((total[0].Summa_na_1_kv+total[0].Summa_na_2_kv+
                        total[0].Summa_na_3_kv+total[0].Summa_na_4_kv )?
                        (total[1].Summa_na_1_kv+total[1].Summa_na_2_kv+
                          total[1].Summa_na_3_kv+total[1].Summa_na_4_kv )/
                        (total[0].Summa_na_1_kv+total[0].Summa_na_2_kv+
                          total[0].Summa_na_3_kv+total[0].Summa_na_4_kv )
                        :1)

                );
    lp->PrintNL(pbuf);
  }

   delete lp;
    _ShowPrint *sp =(_ShowPrint*) GDt->Find("Печать Исполнения сметы");
    if(sp)
      GDt->Del(sp);
    sp = new _ShowPrint(name, "Печать Исполнения сметы");
    GDt->Add(sp); sp->HelpID=100;
    GDt->SwitchFocus(sp);
};


void CalcDoc(file_11 *total, file_11 *sum,char *statja, short year){
  char _tmp[21];
  memset(_tmp,0,21);
  strcpy(_tmp,"Статья ");
  strncat(_tmp,statja,5);
  strcat(_tmp," ...");
  long g_cnt=0;
  _Gauge *g = new _Gauge(0,0,76,ST_GAUGE,T17->St.Records(T17));
  g->CenterScr();
  g->Add( new _Static(10,0,ST_GAUGE,_tmp));
  g->Show();
  GDt->Add(g);
  file_17 *r17 = new file_17;
  file_22 *r22 = new file_22;
  file_23 *r23 = new file_23;
  _Date q[5];
  q[0].Set(1,1,year); //начало года

  q[1]=q[0];
  q[1].EOQ();
  q[1]+=1;    // начало 2 квартала

  q[2]=q[1];
  q[2].EOQ();
  q[2]+=1;    // начало 3 квартала

  q[3]=q[2];
  q[3].EOQ();
  q[3]+=1;    // начало 4 квартала

  q[4]=q[0];
  q[4].EOY();
  q[4]+=1;    // начало следующего года


  // очищаем значения sum
  memset(sum,0,sizeof(file_11));

  int res = T17->GFirst(r17,1);
  // просматриваем все договора
  while(!res){
    g->Update(g_cnt+1);
    if(r17->Sostoynie_dogovora == 1 || r17->Sostoynie_dogovora == 2){
      // читаем статьи договора
      file_22_0 k22;
      file_21   r21;
      memcpy(&k22,r17->Nomer_dogovora,sizeof(k22));
      res=T22->GGE(r22,&k22,0);
      while(!res){
        if(memcmp(r22->Dogovor,r17->Nomer_dogovora,13))
          break;

        res=T21->GEQ(&r21,&r22->Kod_raboti,0);
        if(!res && !memcmp(r21.Nomer_statji,statja,6)){

          // читаем историю статьи
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

            // определяем попадает ли сумма в наши периоды
            if(r23->Data_nahala_deystviy>q[0] && r23->Data_nahala_deystviy<q[4]){
              short kv;
              for(kv=1;kv<4;kv++)
                if(r23->Data_nahala_deystviy<q[kv])
                  break;
              short op = r23->Operaciy;
              if(!r22->Periodihnostj){
                if(op == 2 || op ==3 || op==6 || op ==7 ){
                  if(op==6 && r23->Izmenyem_na>0.0)
                    r23->Izmenyem_na*=-1;
                  switch(kv){
                    case 1:
                     sum->Summa_na_1_kv += r23->Izmenyem_na;
                     total->Summa_na_1_kv += r23->Izmenyem_na;
                     break;
                    case 2:
                     sum->Summa_na_2_kv += r23->Izmenyem_na;
                     total->Summa_na_2_kv += r23->Izmenyem_na;
                     break;
                    case 3:
                     sum->Summa_na_3_kv += r23->Izmenyem_na;
                     total->Summa_na_3_kv += r23->Izmenyem_na;
                     break;
                    case 4:
                     sum->Summa_na_4_kv += r23->Izmenyem_na;
                     total->Summa_na_4_kv += r23->Izmenyem_na;
                     break;
                  }
                }
              }else{
                if(op==2 || op==3 || op==5 || op==6){
                  switch(kv){
                    case 1:
                     sum->Summa_na_1_kv += r23->Izmenyem_na;
                     total->Summa_na_1_kv += r23->Izmenyem_na;
                     break;
                    case 2:
                     sum->Summa_na_2_kv += r23->Izmenyem_na;
                     total->Summa_na_2_kv += r23->Izmenyem_na;
                     break;
                    case 3:
                     sum->Summa_na_3_kv += r23->Izmenyem_na;
                     total->Summa_na_3_kv += r23->Izmenyem_na;
                     break;
                    case 4:
                     sum->Summa_na_4_kv += r23->Izmenyem_na;
                     total->Summa_na_4_kv += r23->Izmenyem_na;
                     break;
                  }
                }
              }
            }
            res = T23->GNext(r23);
          }
        }
        res=T22->GNext(r22);
      }
    }
    res=T17->GNext(r17);
  }
  delete r17;
  delete r22;
  delete r23;

  GDt->Del(g);


  // учитываем платы по разовым счетам
  g = new _Gauge(0,0,76,ST_GAUGE,T28->St.Records(T28));
  g->CenterScr();
  g->Show();
  GDt->Add(g);
  res=T28->GFirst(rf28,0);
  while(!res){
    g->Update(g_cnt++);
    // определяем попадает ли сумма в наши периоды
    if(!memcmp(rf28->Statjy,statja,6) &&rf28->Data_postupleniy>q[0] && rf28->Data_postupleniy<q[4]){
      short kv;
      for(kv=1;kv<4;kv++)
        if(rf28->Data_postupleniy<q[kv])
          break;
      switch(kv){
        case 1:
          sum->Summa_na_1_kv += rf28->Summa;
          total->Summa_na_1_kv += rf28->Summa;
          break;
        case 2:
          sum->Summa_na_2_kv += rf28->Summa;
          total->Summa_na_2_kv += rf28->Summa;
          break;
        case 3:
          sum->Summa_na_3_kv += rf28->Summa;
          total->Summa_na_3_kv += rf28->Summa;
          break;
        case 4:
          sum->Summa_na_4_kv += rf28->Summa;
          total->Summa_na_4_kv += rf28->Summa;
          break;
      }
    }
    res=T28->GNext(rf28);
  }
  GDt->Del(g);
};
