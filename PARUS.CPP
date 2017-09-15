/****************************************************
 *      PARUS.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"
#include "doc.h"
#include "dbf.h"

#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>


// типы документов на оплату
char * plattypes[]={
 "П/П  "                //"платежное поручение"
,"П/Т  "                //"платежное требование"

/*
,"НАКЛ."                //"накладная"
,"РКО  "                //"расходный кассовый ордер"
,"ЧЕК  "                  //"чек"
,"ПКО  "                //"приходный кассовый ордер"
,"АКТ  "                  //"акт"
,"ДОГ. "                 //"договор"
,"А/О  "                  //"авансовый отчет"
*/
};

char *dogtypes[]={
 "СЧЕТ "                 //"счет"
,"ДОГ. "                 //"договор"
/*
,"HАКЛ."                //"накладная"
,"АКТ  "                  //"акт"
,"ПРИК."                //"приказ"
,"ДЕП  "                  //"депонент"
,"чек  "                  //"чек"
,"ВЕДОМ"                //"ведомость"
*/
 };



void ClearExportFile(char *exportfile){
  DBFFile d(exportfile);
  Byte *r= new Byte [d.RecSize()+1];
  d.Top(r);

  // удаляем записи файла экспорта
  do{
    if(!d.Error()){
        d.Delete();
    }else
      break;

    d.Next(r);
  }while(1);
 delete r;
};


long ExportToDbf( char* mainfile,char *exportfile,char *part1, char *part2, char * dognum,_Date dogdate,double Summa,short type ,long ID){
  DBFFile m(mainfile);
  Byte *r= new Byte [m.RecSize()+1];
  r[m.RecSize()]=0;
  char buf [15];
  int i;


  long tmp,max;
  if(!ID){
    // запись с максимальным номером
    max=0;
    m.Top(r);

    // ищем платежку с максимальным номером
    do{
      if(!m.Error()){
        tmp=0;
        sscanf(m.GetField(r,"pdnum"),"%ld",&tmp);
        if(tmp > max && !strnicmp(m.GetField(r,"pdtyp"),"П/П",3)){
          max=tmp;
        }
      }else
        break;

      m.Next(r);
    }while(1);
  }else{
    max=ID;
  }

  DBFFile d(exportfile);

  d.today.Today();
  _Date boy=d.today;
  boy.BOY();

  d.LockHeader();
  max+=d.Records();
  max++;

  d.Blank(r);

  // количество дней с начала месяца
  sprintf(buf,"%03d",d.today.D2L()-boy.D2L()+1);
  d.SetField(r,buf,"pddat");

  // номер платежки
  sprintf(buf,"%12ld",max+1);
  d.SetField(r,buf,"pdnum");

  // тип платежки
  d.SetField(r,plattypes[0],"pdtyp");

  // тип документа по которому платим
  d.SetField(r,dogtypes[type],"odtyp");

  // дата этого документа
  sprintf(buf,"%04d%02d%02d",dogdate.y,dogdate.m,dogdate.d);
  d.SetField(r,buf,"oddat");

  //номер документа
  d.SetField(r,dognum,"odnum");

  //мнемокоды организаций
  d.SetField(r,part1,"orgfr");
  d.SetField(r,part2,"orgto");

  //сумма
  sprintf(buf,"%14.2lf",Summa);
  d.SetField(r,buf,"itsum");
  d.Insert(r);

  delete r;
  return ID;
};



void ExportToParus(void){

  if(!MessageBox(
         "Вы действительно готовы к экспорту 'предложения к оплате' в ПАРУС ?",
         "",
          "1) Убедитесь что предыдущие данные уже приняты в бухгалтерии !!!",
          "2) Убедитесь что в бухгалтерии в это время никто не работает с ",
          "   банковскими документами !!!",
          "",
          "   Если все готово, нажмите Enter, или нажмите Esc для выхода"))
          return;
  long recs;
  if(!(recs=T19->St.Records(T19))){
    MessageBox("      Нет информации для экспорта,",
               " не сформировано предложение к оплате.");
    return;
  }

  _Gauge *g= new _Gauge(1,21,76,ST_GAUGE,recs,"Export gauge");
  GDt->Add(g);
  GDt->SwitchFocus(g);
  // обнуляем файлы экспорта
  file_12 r12;
  int res = T12->GFirst(&r12);
  _Date td;
  td.Today();
  static char a[16],expdir[100],maindir[130];
  long ID=0;
  recs=0;
  while(!res){
    g->Update(++recs);
    StrTrim(r12.Direktoriy_aksporta);
    sprintf(expdir,"%s\\prdoc%02d.dfl"  ,r12.Direktoriy_aksporta,td.y-1900);
    sprintf(maindir,"%s\\dbf\\prdoc.dbf",r12.Direktoriy_aksporta);
    int h1=open(maindir,O_RDONLY|O_BINARY);
    int h2=open(expdir,O_WRONLY|O_BINARY|O_CREAT|O_TRUNC,S_IWRITE);
    long cur,l=filelength(h1);
    while(l){
      if(l>100)
        cur=100;
      else
        cur=l;
      read(h1,maindir,cur);
      write(h2,maindir,cur);
      l-=cur;
    }
    close(h2);
    close(h1);
    res=T12->GNext(&r12);
  };

  file_17 *r17= new file_17;

  res =T19->GFirst(rf19,1);

  double sum,total=0.0;
  if(!res){
    sum = 0;
    memcpy(a,rf19->Nomer_sheta,16);
    if(T12->GEQ(&r12,a,0))
      Debug("Error in accountcode (%s)\n",a);
    StrTrim(r12.Direktoriy_aksporta);
    sprintf(expdir,"%s\\prdoc%02d.dfl",r12.Direktoriy_aksporta,td.y-1900);
    sprintf(maindir,"%s\\dbf\\prdoc%02d.dbf",r12.Direktoriy_aksporta,td.y-1900);
   }
  while(!res){
    if(memcmp(a,rf19->Nomer_sheta,16)){
      sum=0;
      memcpy(a,rf19->Nomer_sheta,16);
      if(T12->GEQ(&r12,a,0))
        Debug("Error in accountcode (%s)\n",a);
      StrTrim(r12.Direktoriy_aksporta);
      sprintf(expdir,"%s\\prdoc%02d.dfl",r12.Direktoriy_aksporta,td.y-1900);
      sprintf(maindir,"%s\\dbf\\prdoc%02d.dbf",r12.Direktoriy_aksporta,td.y-1900);
      //ClearExportFile(expdir);
    }
    if(!rf19->Razoviy_shet){
      if(!T17->GEQ(r17,rf19->Nomer_dogovora,0)){
        if(T20->GEQ(rf20,&r17->Kod_partnera,0))
          StrClear(rf20->Naimenovanie,35);
        rf20->Naimenovanie[35]=0;
        sum+=rf19->Summa;
        total+=rf19->Summa;
        file_30_0 key30;
        key30.Kod_partnera=r17->Kod_poluhately;
        memcpy(key30.Nomer_sheta,r12.Nomer_sheta,16);

        if(T30->GEQ(rf30,&key30,0))
          memcpy(rf30->Psevdonim,"НЕТ МНЕНМОКОДА!",16);

        ID=ExportToDbf(
          maindir,
          expdir,
          r12.Psevdonim,
          rf30->Psevdonim,
          rf19->Nomer_dogovora,
          r17->Data_zakluheniy,
          rf19->Summa,
          1, //  договор
          ID
        );
      }
    }else{
      if(!T28->GEQ(rf28,rf19->Nomer_dogovora,0)){
        if(T20->GEQ(rf20,&rf28->Kod_partnera,0))
          StrClear(rf20->Naimenovanie,35);
        rf20->Naimenovanie[35]=0;
        sum+=rf19->Summa;
        total+=rf19->Summa;
        file_30_0 key30;
        key30.Kod_partnera=r17->Kod_poluhately;
        memcpy(key30.Nomer_sheta,r12.Nomer_sheta,16);

        if(T30->GEQ(rf30,&key30,0))
          memcpy(rf30->Psevdonim,"НЕТ МНЕНМОКОДА!",16);

        ID=ExportToDbf(
          maindir,
          expdir,
          r12.Psevdonim,
          rf30->Psevdonim,
          rf19->Nomer_dogovora,
          r17->Data_zakluheniy,
          rf19->Summa,
          0, //  счет
          ID
        );
      }
    }
    T19->Delete();
    res = T19->GFirst(rf19,1);
  }
  delete r17;
  GDt->Del(g);

};

void CheckParusDoc(void){
  // проходим по всем счетам с целью поиска информации об оплате документов
  file_12 r12;
  int res = T12->GFirst(&r12);
  _Date td;
  td.Today();
  static char maindir[130];

  while(!res){
    StrTrim(r12.Direktoriy_aksporta);
    sprintf(maindir,"%s\\dbf\\prdoc%02d.dbf",r12.Direktoriy_aksporta,td.y-1900);



    // сканируем данные по платежкам для этого счета
    DBFFile *d=new DBFFile(maindir);
    Byte *r= new Byte [d->RecSize()+1];
    long recs;
    recs=0;
    _Gauge *g= new _Gauge(1,21,76,ST_GAUGE,d->Records(),"Export gauge");
    g->Add(new _Static(5,0,ST_GAUGE,"Счет: "));
    g->Add(new _Static(11,0,ST_GAUGE,r12.Nomer_sheta));
    GDt->Add(g);
    GDt->SwitchFocus(g);

    d->Top(r);

    do{
      g->Update(recs++);
      if(!d->Error()){
        if(*(d->GetField(r,"Primbank")) == 251){
          if(!strcmp(d->GetField(r,"odtyp"),dogtypes[1])){
            // ищем не зафиксирована ли эта запись для соответствующего договора
            file_18_2 k18;
            short days;
            days=0;
            sscanf(d->GetField(r,"pddat"),"%d",&days);
            char dd[13];
            k18.Data_oplati.Today();
            k18.Data_oplati.BOY();
            //Debug("BOM=%ld days=%d\n",k18.Data_oplati.D2L(),days);
            k18.Data_oplati.Add(days-1);
            //Debug("ADD=%ld\n",k18.Data_oplati.D2L());
            k18.Nomer_platewki[12]=0;
            memcpy(k18.Nomer_platewki,d->GetField(r,"pdnum"),12);
            k18.Nomer_dogovora[12]=0;
            memcpy(k18.Nomer_dogovora,d->GetField(r,"odnum"),12);

            //такого платежа не существует пока ?
            if(T18->GEQ(rf18,&k18,2)){

              // добавляем его в файл (если конечно существует такой договор)
              if(!T17->GEQ(rf17,k18.Nomer_dogovora,0)){
                // строим запись по оплатам
                memcpy(rf18->Nomer_dogovora,k18.Nomer_dogovora,13);
                rf18->Data_oplati   =k18.Data_oplati;
                sscanf(d->GetField(r,"itsum"),"%lf",&rf18->Summa);
                memcpy(rf18->Nomer_sheta   ,r12.Nomer_sheta,16);
                memcpy(rf18->Nomer_platewki,k18.Nomer_platewki,13);

                // добавляем ее в файл оплат
                T18->Insert(rf18);

              }

            }

          }else{
            // ищем не зафиксирована ли эта запись для соответствующего счета
            file_28_1 k28;
            short days;
            days=0;
            sscanf(d->GetField(r,"pddat"),"%d",&days);
            char dd[13];
            _Date Data_oplati;
            Data_oplati.Today();
            Data_oplati.BOY();
            Data_oplati.Add(days-1);
            k28.Priznak_oplati=0;
            k28.Nomer_sheta[12]=0;
            memcpy(k28.Nomer_sheta,d->GetField(r,"odnum"),12);

            // не заплачено пока ?
            if(!T28->GEQ(rf28,&k28,1) &&
               !strcmp(rf28->Shet_dly_oplati,r12.Nomer_sheta)){

              // фиксируем оплату по счету
              rf28->Priznak_oplati=1;
              rf28->Data_oplati=Data_oplati;
              T28->Update(rf28);



            }


          }
        }
      }else
        break;

      d->Next(r);
    }while(1);
    delete r;
    delete d;
    GDt->Del(g);
   res=T12->GNext(&r12);
  };

};


void SearchForPartners(void){

  // проходим по всем счетам с целью поиска информации об оплате документов
  file_12 r12;
  int res = T12->GFirst(&r12);
  static char maindir[130];

  while(!res){
    StrTrim(r12.Direktoriy_aksporta);
    sprintf(maindir,"%s\\dbf\\ORBASE.dbf",r12.Direktoriy_aksporta);



    // сканируем данные по платежкам для этого счета
    DBFFile *d=new DBFFile(maindir);
    Byte *r= new Byte [d->RecSize()+1];
    long recs;
    recs=0;
    _Gauge *g= new _Gauge(1,21,76,ST_GAUGE,d->Records(),"Export gauge");
    g->Add(new _Static(5,0,ST_GAUGE,"Счет: "));
    g->Add(new _Static(11,0,ST_GAUGE,r12.Nomer_sheta));
    GDt->Add(g);
    GDt->SwitchFocus(g);

    d->Top(r);

    do{
      g->Update(recs++);
      if(!d->Error()){
        if(*(d->GetField(r,"Orpri")) == '1'){

           file_30_1 key;
           StrClear(key.Psevdonim,15);
           strncpy(key.Psevdonim,d->GetField(r,"Ormne"),15);
           memcpy(key.Nomer_sheta,r12.Nomer_sheta,16);

           // отсекаем название собственного счета
           if(strcmp(key.Psevdonim,r12.Psevdonim)){


             // проверяем нет ли уже организации с таким мнемокодом
             if(T30->GEQ(rf30,&key,1)){

               // ищем организацию по названию
               static file_20_1 k20;
               StrClear(k20.Naimenovanie,210);
               strncpy(k20.Naimenovanie,d->GetField(r,"Ornam"),120);
               StrTrim(k20.Naimenovanie);
               StrUpper(k20.Naimenovanie);

               // возможно организация есть а записи по счету нет
               if(T20->GEQ(rf20,&k20,1)){

                 // нет ничего
                 // добавляем новую организацию
                 strcpy(rf20->Naimenovanie,k20.Naimenovanie);
                 StrClear(rf20->Telefoni,100);
                 StrClear(rf20->Faks,30);
                 rf20->Kod_partnera=0;
                 T20->Insert(rf20);
               }

               // добовляем запись на счет
               memcpy(rf30->Psevdonim,key.Psevdonim,16);
               memcpy(rf30->Nomer_sheta,key.Nomer_sheta,16);
               rf30->Kod_partnera=rf20->Kod_partnera;
               T30->Insert(rf30);
             }
           }
        }
      }else
        break;
      d->Next(r);
    }while(1);
    delete r;
    delete d;
    GDt->Del(g);
   res=T12->GNext(&r12);
  };
};
