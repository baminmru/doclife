/****************************************************
 *      DOC01.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"
#include "doc.h"
#include "clsctrs.h"

static Table *T8;

static char * h[] = {
 "┌────────────────────────────────────────────────────────────┬────────────────────┐",
 "│ Наименование документа, статьи                             │  Сумма             │",
 "├────────────────────────────────────────────────────────────┼────────────────────┤",
};
static char *blank =
 "│                                                            │                    │";

static char *f=
 "└────────────────────────────────────────────────────────────┴────────────────────┘";
static char * grp=
 "╞════════════════════════════════════════════════════════════╪════════════════════╡";


static void Head(_LowPrint *lp ,int page){
 for(int i=0; i<3 ;i++)
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

void BuildDoc01(void){
  T8 = T8->St.FindTable("X$Classificator");
  if(!MessageBox(
    "Создать отчет о задолженностях ?",
    "  Да - [Enter]  Нет - [Esc]"      )) return;
  long g_cnt=0;

  file_17 *r17 = new file_17;
  file_22 *r22 = new file_22;
  file_23 *r23 = new file_23;
  double sum=0,total=0.0;
  _Date t;
  t.Today();
  int res;
  char f_name[15];
  char pbuf[201];
  _LowPrint *lp = new _LowPrint(65,f_name);
  lp->SetHeader(Head);
  lp->SetFooter(1,Foot);

  short first =1,prev_tip=0;
  double stype[2];
  stype[0]=stype[1]=0.0;

  _Gauge *g = new _Gauge(0,0,76,ST_GAUGE,T28->St.Records(T28));
  g->CenterScr();
  g->Show();
  GDt->Add(g);
  res=T28->GFirst(rf28,0);
  sum=0;
  while(!res){
    g->Update(g_cnt++);

    // безакцептные счета считаем оплаченными в момент ввода !!!!
    if(!rf28->Priznak_oplati || rf28->Ne_vkluhatj_v_predl_)
      sum+=rf28->Summa;
    res=T28->GNext(rf28);
  }
  GDt->Del(g);
  if(sum){
    sprintf(pbuf,"│%-60s│%20.2lf│","Задолженность по разовым счетам",sum);
    lp->PrintNL(pbuf);
    total+=sum;
    lp->PrintNL(grp);
  }


  g_cnt=0;
  g = new _Gauge(0,0,76,ST_GAUGE,T17->St.Records(T17));
  g->CenterScr();
  g->Show();
  GDt->Add(g);

  res = T17->GFirst(r17,1);

  // просматриваем все договора
  while(!res){
    sum  =0.0;
    g->Update(g_cnt+1);
    if(r17->Sostoynie_dogovora == 1){

      // напечатать итог по типу договоров
      if(first || r17->Tip_dogovora != prev_tip){
        if(first){
          first = 0;
          prev_tip = r17->Tip_dogovora;
        }else{
          if(!T15->GEQ(rf15,&prev_tip)){
            rf15->Nazvanie[50]=0;
          }else
            strcpy(rf15->Nazvanie,"Тип не известен");

          prev_tip = r17->Tip_dogovora;
          lp->PrintNL(h[2]);
          lp->PrintNL(blank);
          sprintf(pbuf,"│Итог    : %-50s│%20s│",rf15->Nazvanie,"");
          lp->PrintNL(pbuf);
          sprintf(pbuf,"│%-60s│%20.2lf│","Уже выдвинуто на оплату",stype[0]);
          lp->PrintNL(pbuf);
          sprintf(pbuf,"│%-60s│%20.2lf│","  Из них оплачено",stype[1]);
          lp->PrintNL(pbuf);
          sprintf(pbuf,"│%-60s│%20.2lf│","Еще планируется оплатить",stype[0]-stype[1]);
          lp->PrintNL(pbuf);
          stype[0]=stype[1]=0;
        }

        if(!T15->GEQ(rf15,&prev_tip)){
          rf15->Nazvanie[50]=0;
        }else
          strcpy(rf15->Nazvanie,"Тип не известен");
        if(g_cnt)
          lp->PrintNL(grp);

        sprintf(pbuf,"│Группа  : %-50s│%20s│",rf15->Nazvanie,"");
        lp->PrintNL(pbuf);
      }

      lp->PrintNL(h[2]);

                     r17->Kratkoe_opisanie[57]=0;
      sprintf(pbuf,"│Договор %-16s%56s│",r17->Nomer_dogovora,r17->Kratkoe_opisanie);
      lp->PrintNL(pbuf);


      // читаем статьи договора
      file_22_0 k22;
      file_21   r21;
      memcpy(&k22,r17->Nomer_dogovora,sizeof(k22));
      res=T22->GGE(r22,&k22,0);
      while(!res){
        if(memcmp(r22->Dogovor,r17->Nomer_dogovora,13))
          break;

        res=T21->GEQ(&r21,&r22->Kod_raboti,0);

        if(res)
          memset(r21.Naimenovanie,'?',52);

        r21.Naimenovanie[52]=0;

        sprintf(pbuf,"│Статья: %52s│%20s│",r21.Naimenovanie,"");
        lp->PrintNL(pbuf);

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
          if(r23->Data_nahala_deystviy<=t){
            short op = r23->Operaciy;
            file_8   r8;
            file_8_0 k8;

            if(!r22->Periodihnostj){
              if(op == 2 || op ==3 || op==6 || op ==7 || op == 8){
                if(op==6 && r23->Izmenyem_na>0.0)
                  r23->Izmenyem_na*=-1;
                sum += r23->Izmenyem_na;
                k8.id=4;
                k8.item=op;
                if(!T8 || T8->GEQ(&r8,&k8,0)){
                  StrClear(r8.name,30);
                }
                sprintf(pbuf,"│Операция  %-50s│%20.2lf│",r8.name,r23->Izmenyem_na);
                lp->PrintNL(pbuf);
              }
            }else{
              if(op==2 || op==3 || op==5 || op==6){
                sum += r23->Izmenyem_na;
                k8.id=5;
                k8.item=op;

                if(!T8 || T8->GEQ(&r8,&k8,0)){
                  StrClear(r8.name,30);
                }

                sprintf(pbuf,"│Операция  %-50s│%20.2lf│",r8.name,r23->Izmenyem_na);
                lp->PrintNL(pbuf);
              }
            }
          }
          res = T23->GNext(r23);
        }
        res=T22->GNext(r22);
      }

      {
        //подсчет и печать оплаченных сумм
        file_18 r18;
        file_18_0 k18;
        double opl ;
        opl =0.0;
        memcpy(k18.Nomer_dogovora,r17->Nomer_dogovora,13);
        res = T18->GGE(&r18 ,&k18,0);
        while(!res){
           if(memcmp(k18.Nomer_dogovora,r17->Nomer_dogovora,13))
            break;
           if(r18.Data_oplati <= t)
             opl+= r18.Summa;
           res = T18->GNext(&r18,&k18);
        }
        sprintf(pbuf,"│%-60s│%20.2lf│","Оплачено",opl);
        lp->PrintNL(pbuf);
        stype[0]+=sum;
        stype[1]+=opl;
        sum-=opl;
      }
      // печать итога по договору
      if(sum){
        lp->PrintNL(blank);
        sprintf(pbuf,"│%-60s│%20.2lf│","Итого задолженность по договору",sum);
        lp->PrintNL(pbuf);
      }else{
        lp->PrintNL(blank);
        sprintf(pbuf,"│%-60s│%20s│","По оплате договора задолженностей нет ","");
        lp->PrintNL(pbuf);

      }
      total+=sum;

    }
    g_cnt++;
    res = T17->GNext(r17);
  }

  if(stype[0]||stype[1]){

    if(!T15->GEQ(rf15,&prev_tip)){
      rf15->Nazvanie[50]=0;
    }else
      strcpy(rf15->Nazvanie,"Тип не известен");
    lp->PrintNL(h[2]);
    lp->PrintNL(blank);
    sprintf(pbuf,"│Итог    : %-50s│%20s│",rf15->Nazvanie,"");
    lp->PrintNL(pbuf);
    sprintf(pbuf,"│%-60s│%20.2lf│","Уже выдвинуто на оплату",stype[0]);
    lp->PrintNL(pbuf);
    sprintf(pbuf,"│%-60s│%20.2lf│","  Из них оплачено",stype[1]);
    lp->PrintNL(pbuf);
    sprintf(pbuf,"│%-60s│%20.2lf│","Еще планируется оплатить",stype[0]-stype[1]);
    lp->PrintNL(pbuf);
    stype[0]=stype[1]=0.0;

  }



  lp->PrintNL(grp);
  sprintf(pbuf,"│%-60s %20.2lf│","Итого на оплату требуется ",total);
  lp->PrintNL(pbuf);

  delete r17;
  delete r22;
  delete r23;

  GDt->Del(g);
  delete lp;

  if(g_cnt){
    _ShowPrint *sp= (_ShowPrint*)GDt->Find("Печать Задолженностей");
    GDt->Del(sp);
    sp = new _ShowPrint(f_name, "Печать Задолженностей");
    GDt->Add(sp); sp->HelpID=100;
    GDt->SwitchFocus(sp);
  }else{
    MessageBox( "Файл договоров пуст ?!",
                "Нажмите ESC для продолжения");
  }
};
