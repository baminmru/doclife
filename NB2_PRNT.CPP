/****************************************************
 *   NB2_PRNT.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include <conio.h>
#include <mem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "i_all.h"
#include "nb2_recs.h"
#include "nb2.h"

extern unsigned g_type;
extern unsigned g_f;
extern unsigned g_period;  // тип периода : день/месяц/квартал/год
extern unsigned g_quartal; // номер квартала
extern unsigned g_year;
extern unsigned g_month;
extern Date g_date;        // дата для которой все происходит
extern Date g_date2;        // дата для которой все происходит

static int boq[4]={ 1,4,7,10 };

int GetRGP( clsr_0 * InKey, clsr * R );

void print_main(void);
void print_sub(void);
void print_all(void);
static char rgpbuf[8];
char *RGPToNew(acc * r, char *buf);
void NewToRGP( acc *r,char *buf);


static char * doc_cap[]={
  "",
  "С П Р А В К А",
  "",
  "",
  "",
  "======================================================================================================================================",
  "! Бюдж.клас-│                     Hаименование доходов                         │   поступления   │     возврат     │      всего      !",
  "! сификация │                                                                  │                 │                 │                 !",
  "!───────────┼──────────────────────────────────────────────────────────────────┼─────────────────┼─────────────────┼─────────────────!",
  "!     1     │                               2                                  │        3        │        4        │        5        !",
  "!───┬───┬───┼──────────────────────────────────────────────────────────────────┼─────────────────┼─────────────────┼─────────────────!",
  "!───┼───┼───┼──────────────────────────────────────────────────────────────────┼─────────────────┼─────────────────┼─────────────────!",
  "!                                                                                                                                    !",
};

static char bottom[]="───┴───┴───┴──────────────────────────────────────────────────────────────────┼─────────────────┼─────────────────┼─────────────────";

Date b,e;
char *period[]={
  "За день   ",
  "За месяц  ",
  "За квартал",
  "За год    ",
  "За период "
};

char *month[]={
  "Январь  ",
  "Февраль ",
  "Март    ",
  "Апрель  ",
  "Май     ",
  "Июнь    ",
  "Июль    ",
  "Август  ",
  "Сентябрь",
  "Октябрь ",
  "Ноябрь  ",
  "Декабрь "
};


char *quartal[]={
  "  I квартал",
  " II квартал",
  "III квартал",
  " IV квартал"
};

int set_period(void){
  int out=1;
  _Help::Push(ReportSubM);
  PVMenuN * menu = new PVMenuN(5,period,1,WINDOW_CENTER,40,7,2);
  g_period=menu->GetChoice();
  _Help::Pop();
  if(!g_period){
    g_period=1;
    delete menu;
    return 0;
  }
  _Help::Push(ReportDate);
  switch(g_period){
   case 1:
   {
      Window * W1 = new Window( WINDOW_CENTER, 60, 12, 27, 3, 2 );
      Get * Get1 = new Get( W1, 1, KEYARR );
      GetElem *reserved;
      g_date.Today();
      reserved=Get1->NewDate( &(g_date), 1, 1,"Уточните дату : " );
      KeyMode=1;
      if(Get1->Read()==ESC) out=0;
      KeyMode=0;
      delete Get1;
      delete W1;
    }
    break;
   case 2:
    { g_date.Today();
      int chs=g_date.GetMonth();
      PVMenuN * menu = new PVMenuN(12,month,chs,WINDOW_CENTER,60,14,2);
      g_month=menu->GetChoice();
      if(!g_month){
        g_month=1;
        out=0;
      }
      delete menu;

    }
    break;

   case 3:
    {
      PVMenuN * menu = new PVMenuN(4,quartal,1,WINDOW_CENTER,60,12,2);
      g_quartal=menu->GetChoice();
      if(!g_quartal){
        g_quartal=1;
        out=0;
      }
      delete menu;
    }
    break;
   case 4:
   {
      Window * W1 = new Window( WINDOW_CENTER, 60, 12, 25, 3, 2, "За год" );
      Get * Get1 = new Get( W1,1,KEYARR );
      GetElem *reserved;
      g_date.Today();

      reserved=Get1->NewDate( &(g_date), 1, 1,              "По дату вкл. : " );
      KeyMode=1;
      if(Get1->Read()==ESC) out=0;
      KeyMode=0;
      delete Get1;
      delete W1;
    }
    break;

   default:
    {
      Window * W1 = new Window( WINDOW_CENTER, 60, 12, 27, 4, 2, "По заданному периоду" );
      Get * Get1 = new Get( W1,1,KEYARR );
      GetElem *reserved;
      g_date.Today();
      g_date.BOY(&g_date);
      g_date2.Today();

      reserved=Get1->NewDate( &(g_date), 1, 1,  "Начало периода : " );
      reserved=Get1->NewDate( &(g_date2), 1, 2, "Конец периода  : " );
      KeyMode=1;
      if(Get1->Read()==ESC) out=0;
      KeyMode=0;
      delete Get1;
      delete W1;
    }
    break;


  }
  _Help::Pop();
  delete menu;

  switch(g_period){

    //день
    case 1:
      b=g_date;
      e=g_date;
      break;

    // месяц
    case 2:
      b.Today();
      b.BOM(&b);
      b.SetMonth(g_month);
      e=b;
      e.EOM(&e);

      break;

    // квартал
    case 3:
      b.Today();
      b.SetMonth(boq[g_quartal-1]);
      b.BOQ(&b);
      e=b;
      e.EOQ(&e);
      break;

    // год
    case 4:
      b=g_date;
      b.BOY(&b);
      e=g_date;
      break;

    // период
    default:
      b=g_date;
      e=g_date2;
      break;
  }

  return out;
};

void CheckAcc( void );
void Print(void){
  if(!g_type){
    _Help::Push(ReportChoose);
    CheckAcc();
    _Help::Pop();
  }
  if(set_period())
  if(g_type)
    print_sub();
  else
    print_all();
};



void print_sub (void){

  doc_cap[0]=name1;
  char subtitle[2][151];
  char d1[11],d2[11];
  sprintf(subtitle[0],"о поступлениях на %s (%s)",an[g_type-1].name,an[g_type-1].acc);

  b.D2C(d1);
  e.D2C(d2);

  switch(g_period){
    case 1:
    sprintf(subtitle[1],"за %s.",d1);
    break;

    case 2:
    sprintf(subtitle[1],"за %s %d года.",month[g_month-1] ,b.GetYear());
    break;


    case 3:
    sprintf(subtitle[1],"за %s %d года.",quartal[g_quartal-1] ,b.GetYear());
    break;


    case 4:
    sprintf(subtitle[1],"за %d год (по %s включительно).",b.GetYear(),d2);
    break;

    case 5:
    sprintf(subtitle[1],"за период с %s по %s.",d1,d2);
    break;

  }
  AllTrim(subtitle[0]);
  AllTrim(subtitle[1]);


  doc_cap[3]=subtitle[1];
  doc_cap[2]=subtitle[0];

  HPrint * prn = new HPrint(1,0,"",13,doc_cap);

  int l=sizeof(acc);
  char pbuf[151];
  char prn_filename[120];
  acc_1 k,prev_k;
  acc rm,r,prev_r;
  long double r41p=0,r41m=0;

  long double i[2];
  i[0]=i[1]=0;


  memset(&k,0,sizeof(k));
  k.type=an[g_type-1].lnum;

  int err;
  FGauge *fg;
  err=acc_file->GGrtrEqual(&r,&l,&k,1);
  fg = new FGauge(acc_file,"Просмотр поступлений");
  long fg_cnt=0;
  int flag,first=1;

  memset(&prev_k,0,sizeof(k));
  memset(&prev_r,0,sizeof(r));
  memset(&rm,0,sizeof(r));

  while(!err ){
    if(r.type!=an[g_type-1].lnum)
      break;
    flag=1;

    if(r.date.Less(&b))
      flag=0;

    if(r.date.Greater(&e))
      flag=0;

    // отсечка досылок
    if(r.subtype==2)
      flag=0;

///////////////////////////////////////
    if(r.f[2])
      flag=0;

    if(r.f[3]!=New)
      flag=0;

    if(flag){

      if(memcmp(&prev_k,&k,8) ){
        if(!first){
          if(prev_r.r==41){
            r41p+=prev_r.sum;
            r41m-=rm.sum;
          }else{
            clsr cr;
            clsr_0 ck;
            ck.r=prev_r.r;
            ck.g=prev_r.g;
            ck.p=prev_r.p;
            memset(&cr,0,sizeof(cr));
            GetRGP(&ck,&cr);
            if(!New)
              sprintf(pbuf,"%3d│%3d│%3d│%66s│%17.2Lf│%17.2Lf│%17.2Lf",
                        prev_r.r,
                        prev_r.g,
                        prev_r.p,
                        cr.name,
                        prev_r.sum,
                        rm.sum,
                        prev_r.sum+rm.sum
              );
            else
              sprintf(pbuf,"%4s%7s│%66s│%17.2Lf│%17.2Lf│%17.2Lf",
                        "",
                        RGPToNew(&prev_r,rgpbuf),
                        cr.name,
                        prev_r.sum,
                        rm.sum,
                        prev_r.sum+rm.sum
              );

            i[0]+=prev_r.sum;
            i[1]-=rm.sum;
            prn->PrStr(pbuf);
          }
        }
        first=0;

        memset(&rm,0,sizeof(r));
        memcpy(&prev_r,&r,sizeof(r));
        memcpy(&prev_k,&k,sizeof(k));

        if(r.sum<0){
          prev_r.sum=0;
          rm.sum+=r.sum;
        }

      }else{

        if(r.sum>=0)
          prev_r.sum+=r.sum;
        else
          rm.sum+=r.sum;

      }

    }
    fg->Updt(fg_cnt++);
    err=acc_file->GNext(&r,&l,&k,1);
  }
  if(prev_r.sum || rm.sum){
    if(prev_r.r==41){
      r41p+=prev_r.sum;
      r41m-=rm.sum;
    }else{
      clsr cr;
      clsr_0 ck;
      ck.r=prev_r.r;
      ck.g=prev_r.g;
      ck.p=prev_r.p;
      memset(&cr,0,sizeof(cr));
      GetRGP(&ck,&cr);
      if(!New)
        sprintf(pbuf,"%3d│%3d│%3d│%66s│%17.2Lf│%17.2Lf│%17.2Lf",
                  prev_r.r,
                  prev_r.g,
                  prev_r.p,
                  cr.name,
                  prev_r.sum,
                  rm.sum,
                  prev_r.sum+rm.sum
        );
      else
        sprintf(pbuf,"%4s%7s│%66s│%17.2Lf│%17.2Lf│%17.2Lf",
                  "",
                  RGPToNew(&prev_r,rgpbuf),
                  cr.name,
                  prev_r.sum,
                  rm.sum,
                  prev_r.sum+rm.sum
        );
      i[0]+=prev_r.sum;
      i[1]-=rm.sum;
      prn->PrStr(pbuf);
    }
  }

  prn->PrStr(bottom);
  sprintf(pbuf,"%3s %3s %3s %66s│%17.2Lf│%17.2Lf│%17.2Lf",
              "",
              "",
              "",
              "Итого",
              i[0],
              -i[1],
              i[0]-i[1]
  );
  prn->PrStr(pbuf);
  sprintf(pbuf,"%3s %3s %3s %66s│%17.2Lf│%17.2Lf│%17.2Lf",
              "",
              "",
              "",
              "Невыясненные поступления",
              r41p,
              -r41m,
              r41p-r41m
  );
  prn->PrStr(pbuf);

  sprintf(pbuf,"%3s %3s %3s %66s│%17.2Lf│%17.2Lf│%17.2Lf",
              "",
              "",
              "",
              "В С Е Г О ",
              r41p+i[0],
              -r41m-i[1],
              r41p-r41m+i[0]-i[1]
  );
  prn->PrStr(pbuf);

  delete fg;
//------------------------------------------
  prn->EndPage();
  memset(&k,0,sizeof(k));
  k.type=an[g_type-1].lnum;
  err=acc_file->GGrtrEqual(&r,&l,&k,1);
  fg = new FGauge(acc_file,"Просмотр поступлений (досылки)");
  strcat(subtitle[0]," (досылки)");
  i[0]=i[1]=0;
  r41p=r41m=0;
  fg_cnt=0;
  first=1;

  memset(&k,0,sizeof(k));
  k.type=an[g_type-1].lnum;

  memset(&prev_k,0,sizeof(k));
  memset(&prev_r,0,sizeof(r));
  memset(&rm,0,sizeof(r));

  while(!err ){
    if(r.type!=an[g_type-1].lnum)
      break;
    flag=1;

    if(r.date.Less(&b))
      flag=0;

    if(r.date.Greater(&e))
      flag=0;

    // отсечка досылок
    if(r.subtype==2 || (r.f[2] && r.r==41) )
      flag=1;
    else
      flag=0;

    if(r.f[3]!=New)
      flag=0;

    if(flag){

      if(memcmp(&prev_k,&k,8) ){
        if(!first){
          clsr cr;
          clsr_0 ck;
          ck.r=prev_r.r;
          ck.g=prev_r.g;
          ck.p=prev_r.p;
          memset(&cr,0,sizeof(cr));
          GetRGP(&ck,&cr);
          if(!New)
            sprintf(pbuf,"%3d│%3d│%3d│%66s│%17.2Lf│%17.2Lf│%17.2Lf",
                      prev_r.r,
                      prev_r.g,
                      prev_r.p,
                      cr.name,
                      prev_r.sum,
                      rm.sum,
                      prev_r.sum+rm.sum
            );
          else
            sprintf(pbuf,"%4s%7s│%66s│%17.2Lf│%17.2Lf│%17.2Lf",
                      "",
                      RGPToNew(&prev_r,rgpbuf),
                      cr.name,
                      prev_r.sum,
                      rm.sum,
                      prev_r.sum+rm.sum
            );
            i[0]+=prev_r.sum;
            i[1]-=rm.sum;
            prn->PrStr(pbuf);
        }
        first=0;

        memset(&rm,0,sizeof(r));
        memcpy(&prev_r,&r,sizeof(r));
        memcpy(&prev_k,&k,sizeof(k));

        if(r.sum<0){
          prev_r.sum=0;
          rm.sum+=r.sum;
        }

      }else{

        if(r.sum>=0)
          prev_r.sum+=r.sum;
        else
          rm.sum+=r.sum;

      }

    }
    fg->Updt(fg_cnt++);
    err=acc_file->GNext(&r,&l,&k,1);
  }

  if(prev_r.sum || rm.sum){
    clsr cr;
    clsr_0 ck;
    ck.r=prev_r.r;
    ck.g=prev_r.g;
    ck.p=prev_r.p;
    memset(&cr,0,sizeof(cr));
    GetRGP(&ck,&cr);
      if(!New)
        sprintf(pbuf,"%3d│%3d│%3d│%66s│%17.2Lf│%17.2Lf│%17.2Lf",
                  prev_r.r,
                  prev_r.g,
                  prev_r.p,
                  cr.name,
                  prev_r.sum,
                  rm.sum,
                  prev_r.sum+rm.sum
        );
      else
        sprintf(pbuf,"%4s%7s│%66s│%17.2Lf│%17.2Lf│%17.2Lf",
                  "",
                  RGPToNew(&prev_r,rgpbuf),
                  cr.name,
                  prev_r.sum,
                  rm.sum,
                  prev_r.sum+rm.sum
        );
    i[0]+=prev_r.sum;
    i[1]-=rm.sum;
    prn->PrStr(pbuf);

  }

  prn->PrStr(bottom);
  sprintf(pbuf,"%3s %3s %3s %66s│%17.2Lf│%17.2Lf│%17.2Lf",
              "",
              "",
              "",
              "В С Е Г О  ",
              i[0],
              -i[1],
              i[0]-i[1]
  );
  prn->PrStr(pbuf);

  delete fg;

  // сохранить имя файла печати
  strcpy(prn_filename,prn->GetFileName());

  // закончить создание файла печати
  delete prn;

 _Help::Push(PrintHelp);
  //Показать и напечатать файл
  ShowPBFile *prn_show = new ShowPBFile(prn_filename);

  delete prn_show;

  //Удалить файл печати
  unlink(prn_filename);
  _Help::Pop();

};

extern int checkmark[];
extern int accounts;

void print_all (void){

  doc_cap[0]=name1;
  char subtitle[2][151];
  char d1[11],d2[11];
  sprintf(subtitle[0],"о поступлениях на счета");
  b.D2C(d1);
  e.D2C(d2);
  switch(g_period){
    case 1:
    sprintf(subtitle[1],"за %s.",d1);
    break;

    case 2:
    sprintf(subtitle[1],"за %s %d года.",month[g_month-1] ,b.GetYear());
    break;


    case 3:
    sprintf(subtitle[1],"за %s %d года.",quartal[g_quartal-1] ,b.GetYear());
    break;


    case 4:
    sprintf(subtitle[1],"за %d год (по %s включительно).",b.GetYear(),d2);
    break;

    case 5:
    sprintf(subtitle[1],"за период с %s по %s.",d1,d2);
    break;
  }
  AllTrim(subtitle[0]);
  AllTrim(subtitle[1]);


  doc_cap[2]=subtitle[0];
  doc_cap[3]=subtitle[1];
  HPrint * prn = new HPrint(1,0,"",13,doc_cap);

  int l=sizeof(acc);
  char pbuf[151];
  char prn_filename[120];
  acc_2 k,prev_k;
  acc rm,r,prev_r;
  long double i[2],r41p=0,r41m=0;
  i[0]=i[1]=0;


  memset(&k,0,sizeof(k));

  int err;
  FGauge *fg;
  err=acc_file->GFirst(&r,&l,&k,2);
  fg = new FGauge(acc_file,"Просмотр поступлений");
  long fg_cnt=0;
  int flag,first=1;

  memset(&prev_k,0,sizeof(k));
  memset(&prev_r,0,sizeof(r));
  memset(&rm,0,sizeof(r));

  while(!err ){
    flag=1;

    for(int iii=0; iii<accounts; iii++){
       if(r.type==an[iii].lnum) {
         if(!checkmark[iii])
            flag=0;
         else{
           flag=1;
           break;
         }
       }else
         flag=0;
    }

    if(r.date.Less(&b))
      flag=0;

    if(r.date.Greater(&e))
      flag=0;

    // отсечка досылок
    if(r.subtype==2)
      flag=0;
    if(r.f[3]!=New)
      flag=0;


    if(flag){

      if(memcmp(&prev_k,&k,6)){
        if(!first){
          if(prev_r.r==41){
            r41p+=prev_r.sum;
            r41m-=rm.sum;
          }else{
            clsr cr;
            clsr_0 ck;
            ck.r=prev_r.r;
            ck.g=prev_r.g;
            ck.p=prev_r.p;
            memset(&cr,0,sizeof(cr));
            GetRGP(&ck,&cr);
            if(!New)
              sprintf(pbuf,"%3d│%3d│%3d│%66s│%17.2Lf│%17.2Lf│%17.2Lf",
                        prev_r.r,
                        prev_r.g,
                        prev_r.p,
                        cr.name,
                        prev_r.sum,
                        rm.sum,
                        prev_r.sum+rm.sum
              );
            else
              sprintf(pbuf,"%4s%7s│%66s│%17.2Lf│%17.2Lf│%17.2Lf",
                        "",
                        RGPToNew(&prev_r,rgpbuf),
                        cr.name,
                        prev_r.sum,
                        rm.sum,
                        prev_r.sum+rm.sum
              );
            i[0]+=prev_r.sum;
            i[1]-=rm.sum;
            prn->PrStr(pbuf);
          }
        }
        first=0;

        memset(&rm,0,sizeof(r));
        memcpy(&prev_r,&r,sizeof(r));
        memcpy(&prev_k,&k,sizeof(k));

        if(r.sum<0){
          prev_r.sum=0;
          rm.sum+=r.sum;
        }

      }else{

        if(r.sum>=0)
          prev_r.sum+=r.sum;
        else
          rm.sum+=r.sum;

      }

    }
    fg->Updt(fg_cnt++);
    err=acc_file->GNext(&r,&l,&k,2);
  }
  if(prev_r.sum || rm.sum){
    if(prev_r.r==41){
      r41p+=prev_r.sum;
      r41m-=rm.sum;
    }else{
      clsr cr;
      clsr_0 ck;
      ck.r=prev_r.r;
      ck.g=prev_r.g;
      ck.p=prev_r.p;
      memset(&cr,0,sizeof(cr));
      GetRGP(&ck,&cr);
      if(!New)
        sprintf(pbuf,"%3d│%3d│%3d│%66s│%17.2Lf│%17.2Lf│%17.2Lf",
                  prev_r.r,
                  prev_r.g,
                  prev_r.p,
                  cr.name,
                  prev_r.sum,
                  rm.sum,
                  prev_r.sum+rm.sum
        );
      else
        sprintf(pbuf,"%4s%7s│%66s│%17.2Lf│%17.2Lf│%17.2Lf",
                  "",
                  RGPToNew(&prev_r,rgpbuf),
                  cr.name,
                  prev_r.sum,
                  rm.sum,
                  prev_r.sum+rm.sum
        );
      i[0]+=prev_r.sum;
      i[1]-=rm.sum;
      prn->PrStr(pbuf);
    }
  }

  prn->PrStr(bottom);
  sprintf(pbuf,"%3s %3s %3s %66s│%17.2Lf│%17.2Lf│%17.2Lf",
              "",
              "",
              "",
              "Итого",
              i[0],
              -i[1],
              i[0]-i[1]
  );
  prn->PrStr(pbuf);
  sprintf(pbuf,"%3s %3s %3s %66s│%17.2Lf│%17.2Lf│%17.2Lf",
              "",
              "",
              "",
              "Невыясненные поступления",
              r41p,
              -r41m,
              r41p-r41m
  );
  prn->PrStr(pbuf);

  sprintf(pbuf,"%3s %3s %3s %66s│%17.2Lf│%17.2Lf│%17.2Lf",
              "",
              "",
              "",
              "В С Е Г О ",
              r41p+i[0],
              -r41m-i[1],
              r41p-r41m+i[0]-i[1]
  );
  prn->PrStr(pbuf);

  delete fg;
//------------------------------------------
  prn->EndPage();
  memset(&k,0,sizeof(k));
  err=acc_file->GFirst(&r,&l,&k,2);
  fg = new FGauge(acc_file,"Просмотр поступлений (досылки)");
  strcat(subtitle[0]," (досылки)");
  i[0]=i[1]=0;
  fg_cnt=0;
  first=1;

  memset(&prev_k,0,sizeof(k));
  memset(&prev_r,0,sizeof(r));
  memset(&rm,0,sizeof(r));

  while(!err ){
    flag=1;
    if(r.date.Less(&b))
      flag=0;

    if(r.date.Greater(&e))
      flag=0;
    for(int iii=0; iii<accounts; iii++){
       if(r.type==an[iii].lnum) {
         if(!checkmark[iii])
            flag=0;
         else{
           flag=1;
           break;
         }
       }else
         flag=0;
    }
    // отсечка не досылок
    if(r.subtype!=2)
      flag=0;
    if(r.f[3]!=New)
      flag=0;

    if(flag){

      if(memcmp(&prev_k,&k,6) ){
        if(!first){

          clsr cr;
          clsr_0 ck;
          ck.r=prev_r.r;
          ck.g=prev_r.g;
          ck.p=prev_r.p;
          memset(&cr,0,sizeof(cr));
          GetRGP(&ck,&cr);
          if(!New)
            sprintf(pbuf,"%3d│%3d│%3d│%66s│%17.2Lf│%17.2Lf│%17.2Lf",
                      prev_r.r,
                      prev_r.g,
                      prev_r.p,
                      cr.name,
                      prev_r.sum,
                      rm.sum,
                      prev_r.sum+rm.sum
            );
          else
            sprintf(pbuf,"%4s%7s│%66s│%17.2Lf│%17.2Lf│%17.2Lf",
                      "",
                      RGPToNew(&prev_r,rgpbuf),
                      cr.name,
                      prev_r.sum,
                      rm.sum,
                      prev_r.sum+rm.sum
            );
            i[0]+=prev_r.sum;
            i[1]-=rm.sum;
            prn->PrStr(pbuf);
          }
        first=0;

        memset(&rm,0,sizeof(r));
        memcpy(&prev_r,&r,sizeof(r));
        memcpy(&prev_k,&k,sizeof(k));

        if(r.sum<0){
          prev_r.sum=0;
          rm.sum+=r.sum;
        }

      }else{

        if(r.sum>=0)
          prev_r.sum+=r.sum;
        else
          rm.sum+=r.sum;

      }

    }
    fg->Updt(fg_cnt++);
    err=acc_file->GNext(&r,&l,&k,2);
  }
  if(prev_r.sum || rm.sum){
    clsr cr;
    clsr_0 ck;
    ck.r=prev_r.r;
    ck.g=prev_r.g;
    ck.p=prev_r.p;
    memset(&cr,0,sizeof(cr));
    GetRGP(&ck,&cr);
    if(!New)
      sprintf(pbuf,"%3d│%3d│%3d│%66s│%17.2Lf│%17.2Lf│%17.2Lf",
                prev_r.r,
                prev_r.g,
                prev_r.p,
                cr.name,
                prev_r.sum,
                rm.sum,
                prev_r.sum+rm.sum
      );
    else
      sprintf(pbuf,"%4s%7s│%66s│%17.2Lf│%17.2Lf│%17.2Lf",
                "",
                RGPToNew(&prev_r,rgpbuf),
                cr.name,
                prev_r.sum,
                rm.sum,
                prev_r.sum+rm.sum
      );
    i[0]+=prev_r.sum;
    i[1]-=rm.sum;
    prn->PrStr(pbuf);

  }

  prn->PrStr(bottom);
  sprintf(pbuf,"%3s %3s %3s %66s│%17.2Lf│%17.2Lf│%17.2Lf",
              "",
              "",
              "",
              "В С Е Г О  ",
              i[0],
              -i[1],
              i[0]-i[1]
  );
  prn->PrStr(pbuf);

  delete fg;

  // сохранить имя файла печати
  strcpy(prn_filename,prn->GetFileName());

  // закончить создание файла печати
  delete prn;
  _Help::Push(PrintHelp);

  //Показать и напечатать файл
  ShowPBFile *prn_show = new ShowPBFile(prn_filename);

  delete prn_show;

  //Удалить файл печати
  unlink(prn_filename);
  _Help::Pop();

};


