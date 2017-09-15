/****************************************************
 *     ENDOFP.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"
#include "doc.h"
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>

void scan_eom(_Date *beg , _Date *end);
void EndOfPeriod(void){
  //if(MessageBox(
  //  "Обработать окончание месяца ?",
  //  " Да - [Enter]  Нет - [Esc]"))
  {
    _Date prev,d,t1,t2;
    char dbuf1[12],dbuf2[12];
    d.Today();

    int h = open("prevdate.dat",O_RDONLY|O_BINARY);
    if(h==-1){
      h=open("prevdate.dat",O_WRONLY|O_BINARY|O_CREAT|O_TRUNC,S_IWRITE);
      write(h,&d,4);
      close(h);
      prev.Today();
    }else{
      read(h,&prev,4);
    }
    t1=d;
    t2=prev;
    t1.EOM();
    t2.EOM();

    if(t1>t2){
      // обработать все концы месяца
    end_of_month:
      scan_eom(&t2,&d);
    }else{
      d.D2C(dbuf1);
      prev.D2C(dbuf2);
      if(t1<t2)
        if(MessageBox(
             "Проверьте устаноки даты на Вашей машине",
             "текущая дата:",
             dbuf1,
             "меньше даты последнего входа в программу:",
             dbuf2,
             "нажмите Enter - для перезаписи даты последнего входа",
             "или     Esc - для выхода"
         )){
           h=open("prevdate.dat",O_WRONLY|O_BINARY|O_CREAT|O_TRUNC,S_IWRITE);
           write(h,&d,4);
           close(h);
           t1.BOM();
           if(d==t1){
             if(MessageBox(
              "Обработать начало нового месяца ?",
              "  Да - [Enter]  Нет - [Esc]"))
              goto end_of_month;
           }
         }else{
           GDt->Q.AddMessage(evKey,ALT_X);
           return;
         }
    }
    h=open("prevdate.dat",O_WRONLY|O_BINARY|O_CREAT|O_TRUNC,S_IWRITE);
    write(h,&d,4);
    close(h);
  }

};

void scan_eom(_Date *prev , _Date *_cur){
  _Date beg,end,cur;
  end=*_cur;
  beg=*prev;
  beg.EOM();
  end.EOM();
  if(*_cur!=end){
    end=*_cur;
    end.BOM();
  }

  long g_cnt=0;
  _Gauge *g = new _Gauge(0,0,76,ST_GAUGE,T17->St.Records(T17));
  g->CenterScr();
  g->Show();
  GDt->Add(g);
  file_17 *r17 = new file_17;
  file_22 *r22 = new file_22;
  file_23 *r23 = new file_23;
  double cost;
  int res = T17->GFirst(r17,1);

  // просматриваем все договора
  while(!res){
    g->Update(g_cnt+1);
    if(r17->Sostoynie_dogovora == 1 &&
      (r17->Data_nahala   < end  ||
       r17->Data_okonhaniy> beg))
    {
      // читаем статьи договора
      file_22_0 k22;
      file_21   r21;
      int p_cnt;

      memcpy(&k22,r17->Nomer_dogovora,sizeof(k22));
      res=T22->GGE(r22,&k22,0);
      while(!res){
        if(memcmp(r22->Dogovor,r17->Nomer_dogovora,13))
          break;

        // специально для периодических статей
        cost =0.0;              // текущий размер платы за период

        // читаем историю статьи
        file_23_0 k23;
        cur=beg;
        do{
          memset(&k23,0,sizeof(k23));
          memcpy(k23.Dogovor,r22->Dogovor,13);
          k23.Statjy=r22->Stroka_dogovora;

          res = T23->GGE(r23,&k23,0);
          while(!res){
            if(memcmp(r23->Dogovor,r22->Dogovor,13))
              break;
            if(r23->Statjy!=r22->Stroka_dogovora)
              break;


            short op = r23->Operaciy;
            file_8   r8;
            file_8_0 k8;

            if(r22->Periodihnostj && op!=2 && op!=3 && op!=5 && op!=6){
                if(!cost)
                  cost = r23->Izmenyem_na;
                else{
                  _Date tmp = cur;
                  if(r22->Avansirovanie)
                    tmp.Add(1);
                  if(r23->Data_nahala_deystviy<=tmp)
                    cost = r23->Izmenyem_na;
                  else
                    break;

                }
            }

            res = T23->GNext(r23);
          }
          if(r22->Periodihnostj && cost ){
             memset(r23,0,sizeof(file_23));
             memcpy(r23->Dogovor,r22->Dogovor,13);
             r23->Statjy=r22->Stroka_dogovora;
             r23->Izmenyem_na=cost;
             // создаем записи на оплату
             if(!r22->Avansirovanie){
               r23->Data_nahala_deystviy=cur;
               r23->Operaciy=2;
             }else{
               r23->Data_nahala_deystviy=cur;
               r23->Data_nahala_deystviy.Add(1);
               r23->Operaciy=3;
             }
             switch(r22->Periodihnostj){
              case 1: // месяц
                T23->Insert(r23,&k23,0);
                break;
              case 2: // квартал
                {
                _Date tmp = cur;
                tmp.EOQ();
                if(tmp==cur)
                  T23->Insert(r23,&k23,0);
                break;
                }
              case 3: // год
                {
                _Date tmp = cur;
                tmp.EOY();
                if(tmp==cur)
                  T23->Insert(r23,&k23,0);
                break;
                }
             }
          }
          cur.Add(1);
          cur.EOM();
        }while(cur<end);
        res=T22->GNext(r22);
      }
    }
    g_cnt++;
    res = T17->GNext(r17);
  }

  delete r17;
  delete r22;
  delete r23;

  GDt->Del(g);
};



