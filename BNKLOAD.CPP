/****************************************************
 *    BNKLOAD.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"
#include "doc.h"

extern void (*ModemFunc)(void);

void ScrollWrite(_Window *Win, char *f,...);

struct BankLine{
  char f1[3];
  char f2[6];
  char f3[10];
  char f4[12];
  char f5[22];
  char f6[2];
} bnk;

static QuitByUser=0;
void QbyU(_Event *E,void * parent){
  QuitByUser=1;

};

int LoadBank(short AccCode, _Date *date,char *proto){
  QuitByUser=0;
  _Window *w=new _Window(75,17,30,3,ST_OPTION,stFrame,"Имя файла выписки");
  long index;
  ListItems *li=MakeFileList(proto,0);
  w->Add(new _Static(1,1,ST_OPTION,"Файл: "));
  w->Add(new _ListBox(10,1,15,8, ST_OPTION, li->top,&index));
  w->BottomRight();
  w->AddCallBack(QbyU,evKey,ENTER);
  w->Exec();
  if(!QuitByUser){
    delete w;
    li->top=0;
    li->top->next=0;
    delete li;
    return 0;
  }
  FILE *f=fopen((li->GetItem(index))->text,"r");
  if(!f){
    MessageBox("    Файл"     ,
               (li->GetItem(index))->text,
               "не найден..." );
    delete w;
    return 0;
  }
  delete w;
  li->top=0;
  li->top->next=0;
  delete li;
  w=new _Window(0,0,30,3,ST_OPTION,0,"Загрузка выписки");
  w->CenterScr();
  GDt->Add(w);
  w->Show(1);
  char buffer[101];
  int p,i;
  long cnt=0;
  long prev=0;
  T12->GEQ(rf12,&AccCode,2);

  //T10->St.TBegin();

  while(!feof(f)){
    if(fgets(buffer,100,f)){
      if(cnt++<2) continue;
      if(StrEmpty(buffer))
       break;
      if(prev<clock()){
        prev=clock();
        if(ModemFunc)
          ModemFunc();
      }
      ScrollWrite(w,"Запись %d",cnt);
      memcpy(&bnk,buffer,sizeof(bnk));
      StrTrim(buffer);
      if(buffer[0]==0)
        break;
      bnk.f1[2]=0;
      bnk.f2[5]=0;
      bnk.f3[9]=0;
      bnk.f4[11]=0;

      // сохранить знак платежа
      bnk.f6[0]=bnk.f5[21];

      bnk.f5[21]=0;
      bnk.f6[1]=0;

      //срезаем ','
      memset(buffer,0,100);
      strcpy(buffer,bnk.f5);
      p=i=0;
      for(;i<=strlen(buffer);i++)
        if(buffer[i]!=','){
          bnk.f5[p++]=buffer[i];
        }
      memset(rf10,0,sizeof(file_10));
      memset(rf10->cls,'0',8);
      rf10->date=*date;
      rf10->date_2=*date;
      rf10->account=AccCode;
      // state = 0
      StrR(bnk.f1,rf10->Kod_platewa,5);   //6

      StrR(bnk.f2,rf10->N_platewnogo_dok_,5);        //6

      StrR(bnk.f3,rf10->MFO,15);    //16

      StrR(bnk.f4,rf10->Ras__shet ,15);    //16


      sscanf(bnk.f5,"%lf",&(rf10->Summa));
      if(bnk.f6[0]=='-')  rf10->Summa*=-1;
      if(rf10->Summa>=0){
        rf10->Buh__provodka=rf12->Provodka_ne_viysn_;
        if(T10->Insert(rf10)){
          //T10->St.TAbort();
          break;
        }
      }else{
        rf10->state=10;
        rf10->Buh__provodka=0;
        if(T10->Insert(rf10)){
          //T10->St.TAbort();
          break;
        }
      }
    }
  }
  //T10->St.TEnd();
  fclose(f);
  GDt->Del(w);
  return 1;
};
