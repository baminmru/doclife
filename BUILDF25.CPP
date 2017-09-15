/****************************************************
 *   BUILDF25.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"
#include "doc.h"
void BuildF25(void){
// 1 - создаем пустой вспомогательный файл
  char path[65];
  T25->St.GetFilePath("Промежуточный_итог",path);
  T25->St.Stat(T25);
  Close25();
  T10->St.Create(path);
  Init25();

// 2 - формируем список задолженностей
  long g_cnt=0;
  _Gauge *g = new _Gauge(0,0,76,ST_GAUGE,T17->St.Records(T17));
  g->CenterScr();
  g->Add(new _Static(10,0,ST_GAUGE,"Выбираем не оплаченные суммы ..."));
  g->Show();
  GDt->Add(g);
  file_17 *r17 = new file_17;
  file_22 *r22 = new file_22;
  file_23 *r23 = new file_23;
  file_16_0 k16;
  file_18_0 k18;
  file_24_1 k24;

  double sum,opl;
  short bal;
  _Date t;
  t.Today();
  int res = T17->GFirst(r17,1);

  // просматриваем все договора
  while(!res){
    sum  =0.0;
    bal=0;
    g->Update(g_cnt+1);
    if(r17->Sostoynie_dogovora == 1){
      //подсчет оплаченных сумм
      {
        opl =0.0;
        memcpy(k18.Nomer_dogovora,r17->Nomer_dogovora,13);
        res = T18->GGE(rf18 ,&k18,0);
        while(!res){
           if(memcmp(k18.Nomer_dogovora,r17->Nomer_dogovora,13))
            break;
           if(rf18->Data_oplati <= t)
            opl+= rf18->Summa;
           res = T18->GNext(rf18,&k18);
        }
        //Debug("Opl = %lf (%s) \n",opl,r17->Nomer_dogovora);
      }

      // читаем статьи договора
      file_22_0 k22;
      memcpy(&k22,r17->Nomer_dogovora,sizeof(k22));
      res=T22->GGE(r22,&k22,0);
      while(!res){
        if(memcmp(r22->Dogovor,r17->Nomer_dogovora,13))
          break;

        // не включаем безакцептные статьи в предложение
        if(!r22->Ne_vkluhatj_v_predl_){

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
              if(!r22->Periodihnostj){
                if(op == 2 || op ==3 || op==6 || op ==7 || op == 8){
                  if(op==6 && r23->Izmenyem_na>0.0)
                    r23->Izmenyem_na*=-1;
                  sum += r23->Izmenyem_na;
                  bal+=(t.D2L()- r23->Data_nahala_deystviy.D2L());
                }
              }else{
                if(op==2 || op==3 || op==5 || op==6){
                  sum += r23->Izmenyem_na;
                  bal+=(t.D2L()- r23->Data_nahala_deystviy.D2L());
                }
              }
            }
            res = T23->GNext(r23);
          }

          k16.Pokazatelj=1;
          k16.Znahenie_pokazately=r22->Kod_raboti;
          if(!T16->GEQ(rf16,&k16,0)){
            bal+=rf16->Kolihestvo_ballov;
          }

        }
        res=T22->GNext(r22);

      }

      sum-=opl;

      // итог по договору
      if(sum>0){
        memset(rf25,0,sizeof(file_25));
        rf25->Summa=sum;
        rf25->Ball=bal;
        memcpy(rf25->Kod_dogovora,r17->Nomer_dogovora,13);

        // учитываем настраиваемые приоритеты
        k16.Pokazatelj=2;
        k16.Znahenie_pokazately=r17->Tip_finansirovaniy;
        if(!T16->GEQ(rf16,&k16,0)){
          rf25->Ball+=rf16->Kolihestvo_ballov;
        }
        k16.Pokazatelj=3;
        k16.Znahenie_pokazately=r17->Kod_partnera;
        if(!T16->GEQ(rf16,&k16,0)){
          rf25->Ball+=rf16->Kolihestvo_ballov;
        }
        k16.Pokazatelj=4;
        k16.Znahenie_pokazately=r17->Rayon;
        if(!T16->GEQ(rf16,&k16,0)){
          rf25->Ball+=rf16->Kolihestvo_ballov;
        }

        // расшивка по типу финансирования
        if(r17->Tip_finansirovaniy){
          k24.Kod_tipa=r17->Tip_finansirovaniy;
          k24.Prioritet=-32766;
          res = T24->GGE(rf24,&k24,1);
          if(res){
            T25->Insert(rf25,0);
            /*Debug("!res %s %s %lf %d R1=%d R2=%d\n",rf25->Nomer_sheta
                                 ,rf25->Kod_dogovora
                                 ,rf25->Summa
                                 ,rf25->Ball
                                 ,res
                                 ,T25->Insert(rf25,0));
           */
          }

          while(!res){
            if(rf24->Kod_tipa!=r17->Tip_finansirovaniy)
              break;
            memcpy(rf25->Nomer_sheta,rf24->Nomer_sheta,16);
            rf25->Ball+=rf24->Prioritet;
            /*Debug("%s %s %lf %d R1=%d R2=%d\n",rf25->Nomer_sheta
                                 ,rf25->Kod_dogovora
                                 ,rf25->Summa
                                 ,rf25->Ball
                                 ,res
                                 ,T25->Insert(rf25,0));
            */
            T25->Insert(rf25,0);
            rf25->Ball-=rf24->Prioritet;
            res = T24->GNext(rf24);
          }
        }else{
            /*Debug("%s %s %lf %d R1=%d R2=%d\n",rf25->Nomer_sheta
                                 ,rf25->Kod_dogovora
                                 ,rf25->Summa
                                 ,rf25->Ball
                                 ,res
                                 ,T25->Insert(rf25,0));
            */
          T25->Insert(rf25,0);
        }
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
