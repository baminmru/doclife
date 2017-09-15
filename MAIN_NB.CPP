/****************************************************
 *    MAIN_NB.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include <malloc.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <string.h>
#include <process.h>
#include "common.h"
#include "doc.h"
#include "option.h"
#include "mail.h"
#include "dial.h"
#include "password.h"
#include "logo.h"

char Dostup[51];

#define D_NOACCESS 0
#define D_READ     1
#define D_WRITE    2

char Coder[]="/,m/lzmfphoadfb,mczxb'klamfe]pogqmfd'vbmzcba[fjobvaoygfWEOFPUHWET]JABFG[AEIRHG  [OFNCXKV;HjbcxVLJKsdV\"cxkfg[sfdg]";

extern void (*ModemFunc)(void);
void ModemIdle(void);

extern unsigned _stklen=30000;

short ColorKit(void);

short CurentAccount;
short CurentAccServ;
char *CurentAccName;
_Date CurentDate;
short MayStart=0;
short Loader=0,Administrator=0,FastAndBad=0,ColorDefined=0,StartMailer=0,NoLogo=0;

void MainMenu(void);
void ColorOption(_Event *E, void * parent);
void ServeMainChoice(_Event *E, void * parent);
void ServeAccChoice(_Event *E, void * parent);
void ServeOptionChoice();
void ServeOrdersChoice();
void ServeReportChoice(_Event *E, void * parent);
void ServeReportChoice2(_Event *E, void * parent);
void ServeConnectChoice(int);
void BuildDoc01(short _state=0);   // создание отчета
void BuildDoc02_G(short _month, short _Year);
void LoadOption(void);
void Option(void);

int LoadBank(short AccCode, _Date *date, char *fname);
void ModemParam(void);

extern _Window *ShowBase;

short style=1;
short main_choice=0;
short choice=0;

struct Accs{
  char name[17];
  short code;
  unsigned char yes;
} AccArr[22];

void LoadAccounts(void){
    file_12_0 k12;
    k12.flag=0;
    k12.Kod=0;
    int res=T12->GGE(rf12,&k12);
    int p_num=0;
    while(!res && p_num <= 20){
      if(rf12->flag)
        break;
      strncpy(AccArr[p_num].name,rf12->Nazvanie,16);
      AccArr[p_num].code=rf12->Kod;
      AccArr[p_num].name[16]=0;
      p_num++;
      res=T12->GNext(rf12);
    }
    AccArr[p_num].name[0]=0;
};


void MainMenu(void){
  _Window * w;

  char * main_menu[]={
  "Счета ",
         " Доходы ",
                  " Расходы ",
                            " Мемориальные ордера ",
                                                  " Настройка ",
                                                              " Модем ",0
  };
  w=new _HMenu(40,1,main_menu,&main_choice);
  w->HelpID=2;
  w->name = "Главное меню";
  w->st |= stNoMove;
  w->Center();
  w->HelpID=1;
  //w->st |= stFrame;
  w->AddCallBack(ServeMainChoice,evKey,ESC);
  w->AddCallBack(ServeMainChoice,evKey,ENTER);
  GDt->Add(w);
  GDt->SwitchFocus(w);
};


void LoadCls(void){
  FILE * h=fopen("dohod.dif","r");
  if(!h)
    return;
   file_11 r;
   char buf[301],*ch,clsbuf[15];
   short s1,s2,s3,s4;
   short cnt=-1;
   while(!feof(h) && fgets(buf,300,h) ){
    if(strstr(buf,"BOT"))
      cnt=0;
    if(buf[0]=='-' && buf[1]=='1'){
      if(cnt > 12){
        sprintf(clsbuf,"%02d%02d%02d%02d",s1,s2,s3,s4);
        memcpy(r.s1,clsbuf,8);
        T11->Insert(&r);
      }
      cnt=-1;
    }
    switch(cnt){
      case -1:
        break;

      case 0:
      case 2:
      case 4:
      case 6:
      case 8:
      case 9:
      case 11:
        cnt++;
        break;
      case 1:
        sscanf(buf+2,"%d",&s2);
        cnt++;
        break;
      case 3:
        sscanf(buf+2,"%d",&s3);
        cnt++;
        break;
      case 5:
        sscanf(buf+2,"%d",&s4);
        cnt++;
        break;
      case 7:
        sscanf(buf+2,"%d",&s1);
        cnt++;
        break;
      case 10:
        ch=strchr(buf+1,'"');
        if(ch) *ch=0;
        StrClear(r.Naimenovanie,200);
        strncpy(r.Naimenovanie,buf+1,strlen(buf+1));
        cnt++;
        break;
      case 12:
        ch=strchr(buf+1,'"');
        if(ch) *ch=0;
        StrClear(r.Kratkoe_naimenovanie,45);
        strncpy(r.Kratkoe_naimenovanie,buf+1,strlen(buf+1));
        cnt++;
        break;
    }


   }

  fclose(h);
};

int DateOK;


void ColorOption(_Event *E, void * parent){
  SaveColor("current.pal");
  if(ColorKit())
    SaveColor("current.pal");
  else
    LoadColor("current.pal");
};


void ServeMainChoice(_Event *E, void * parent){
  short ax=0,ay=0;
  _Window *p=(_Window*) parent;
  p->Coord(&ax,&ay);
  if(ax+2>75) ax-=10;
  if(ay+1>24) ay-=8;
  switch(main_choice){
    case 0:
      if(MessageBox(
        "Вы хотите завершить работу с программой ?",
        "      Да -  [Enter]   Нет - [Esc]")){

        if(Loader)
          LoadCls();

        GDt->Q.AddMessage(evKey,ALT_X);
        return;
      }
      break;
    case 1:
      if(Dostup[10]==D_READ || Dostup[10]==D_WRITE)
      {
        MayStart=0;
        _Window * w;
        LoadAccounts();
        char *names[30];
        short p_num=0;
        while(AccArr[p_num].name[0]){
          names[p_num]=AccArr[p_num++].name;
        }
        names[p_num]=0;
        w=new _VMenu(ax+1,ay+2,names,&choice);
        w->HelpID=3;
        w->AddCallBack(ServeAccChoice,evKey,ENTER);
        w->AddCallBack(ServeAccChoice,evKey,ESC);
        w->HelpID=2;
        w->Exec();
        delete w;
        if(MayStart)
          AddF10(0);
      }
      break;

    // отчеты
    case 2:
      {
        _Window * w;
        DateOK=0;
        LoadAccounts();
        char *names[30];
        short p_num=0;
        names[0]="По всем счетам";
        while(AccArr[p_num].name[0]){
          names[p_num+1]=AccArr[p_num++].name;
        }
        names[p_num+1]=0;
        w=new _VMenu(ax+8,ay+2,names,&choice);
        w->HelpID=3;
        w->AddCallBack(ServeReportChoice,evKey,ESC);
        w->AddCallBack(ServeReportChoice,evKey,ENTER);
        w->Exec();
        delete w;
        if(DateOK)
          BuildDoc01();

      }

      break;

    case 3:
      {
        _Window * w;
        DateOK=0;
        LoadAccounts();
        char *names[30];
        short p_num=0;
        names[0]="По всем счетам";
        while(AccArr[p_num].name[0]){
          names[p_num+1]=AccArr[p_num++].name;
        }
        names[p_num+1]=0;
        w=new _VMenu(ax+17,ay+2,names,&choice);
        w->HelpID=4;
        w->AddCallBack(ServeReportChoice2,evKey,ESC);
        w->AddCallBack(ServeReportChoice2,evKey,ENTER);
        w->Exec();
        delete w;
        if(DateOK)
          BuildDoc01(10);

      }

      break;

    case 4:
      {
        _Window * w;

        static char * order_menu[]={
          "Работа с ордерами",
          "Журнал-Главная",
          0
        };
        w=new _VMenu(ax+27,ay+2,order_menu,&choice);
        //w->HelpID=5;
        w->Exec();
        delete w;
        ServeOrdersChoice();

      }
      break;

    // настройки
    case 5:
      {
        _Window * w;

        static char * help_menu[]={
          "Общая информация",
          "Классификатор",
          "Справочник счетов",
          "План счетов",
          "Справочник проводок",
          "Права доступа",
          "Сменить пароль",
          "О программе ...",
          0
        };
        w=new _VMenu(ax+48,ay+2,help_menu,&choice);
        w->HelpID=5;
        w->Exec();
        delete w;
        ServeOptionChoice();

      }
      break;

    // модем
    case 6:
      {
        _Window * w;

        static char * help_menu[]={
          "Настройки модема",
          "Ожидание сообщений",
          "Отключиться от линии",
          "Партнеры",
          0
        };
        w=new _VMenu(ax+47,ay+2,help_menu,&choice);
        w->HelpID=6;
        w->Exec();
        delete w;
        ServeConnectChoice(choice);
      }
      break;

  };
  E->ClearEvent(0);
};

void ServeOptionChoice(){
  switch(choice){

    case 1:
      Option();
      break;

    case 2:
      if(Dostup[11]==D_READ || Dostup[11]==D_WRITE)
        AddF11(0);
      break;

    case 3:
      if(Dostup[12]==D_READ || Dostup[12]==D_WRITE)
        AddF12(0);
      break;


    case 4:
      if(Dostup[13]==D_READ || Dostup[13]==D_WRITE)
        AddF13(0);
      break;
    case 5:
      if(Dostup[13]==D_READ || Dostup[13]==D_WRITE)
        AddF14(0);
      break;

    case 6:
      if(Dostup[13]==D_READ || Dostup[13]==D_WRITE)
        AddF23(0);
      break;

   case 7:
    {
      file_23_0 k23;
      int i;
      StrClear(k23.Kod_dostupa,10);
      StrClear(k23.Parolj,30);
      char NewPass[31];
      StrClear(NewPass,30);
      _Window * w=new _Window(20,5,45,4,ST_OPTION,stFrame,"ChPass");
      w->Add(new _Static(1,1,ST_OPTION,"Код    : "));
      w->Add(new _Password(15,1,10,ST_OPTION,k23.Kod_dostupa));
      w->Add(new _Static(1,2,ST_OPTION,"Пароль : "));
      w->Add(new _Password(15,2,30,ST_OPTION,k23.Parolj));
      w->Add(new _Static(1,3,ST_OPTION,"Новый пароль : "));
      w->Add(new _Password(15,3,30,ST_OPTION,NewPass));
      w->Exec();
      delete w;
      unsigned char *mm=(char*) Coder;
      for(i=0; i<10;i++){
        k23.Kod_dostupa[i]^=mm[i];
      }
      for(i=0; i<30;i++){
        k23.Parolj[i]^=mm[i+10];
      }
      if(!T23->GEQ(rf23,&k23,0)){
        // Блокируем
        if(T23->GEQ(rf23,&k23,0,200)){
          MessageBox(
            "Запись занята другим пользователем ???",
            "Не возможно получить доступ на обновление !!!",
            "Если вы редактируете запись в окне \"Уровень доступа\"",
            "Сначала закройте это окно..."
            );
        }else{
          // шифруем пароль
          for(i=0; i<30;i++){
            NewPass[i]^=mm[i+10];
          }

          // записываем в базу данных
          memcpy(rf23->Parolj,NewPass,31);
          if(!T23->Update(rf23)){
            MessageBox("Новый проль занесен в базу данных");
          }else{
            MessageBox(
              "Ошибка записи,",
              "обратитесь к администратору базы данных..."
            );
          }
        }
      }else{
        MessageBox(
          "Вы ошиблись в наборе кода или пароля");
      }
    }
    break;
   case 8:
    GDt->M.hide();
    GDt->S.Lock();
    Logo320x200("LME2.pal","LME2.xt");
    GDt->S.Paste();
    GDt->S.SetCursor(0);
    GDt->M.show();
    break;

  };
};

void ServeOrdersChoice(void){
  switch(choice){
    case 1:
      if(Dostup[21]==D_READ || Dostup[21]==D_WRITE)
        AddF21(0);
      break;

     case 2:
      {
         extern char *month[];
         short g_month;
         short g_year;
         _Date td;
         td.Today();
         g_year=td.y;
         _Window * w = new _Window(0,0,1,1,ST_OPTION,stBuffered|stFrame,"Выбор года");
         w->Add( new _Static (1,1,ST_OPTION,"Определите год : "));
         w->Add( new _ShortCell (20,1,ST_OPTION,4,&g_year));
         w->CenterScr();
         if(!w->Exec()){
          delete w;
          break;
         }
         delete w;

         g_month=td.m;
        _VMenu * menu = new _VMenu(0,0,month,&g_month);
        menu->CenterScr();
        if(!menu->Exec()){
          delete menu;
          break;
        }
        delete menu;
        BuildDoc02_G(g_month,g_year);
      }
      break;
  }
};

void EnterDate(_Event *E,void *p){
  DateOK=1;
}

void DateLoader(_Event *E,void *p){
  _Window * w;
  DateOK=0;

  switch(CurentAccServ){
    case 0:
    return;

    case 7:
    case 4:
      DateOK=1;
      break;

    case 1:
    case 2:
    case 3:
    case 5:
    case 6:
      w = new _Window(75,20,1,1,ST_OPTION,stBuffered|stFrame,"Выбор даты");
      w->Add( new _Static (1,1,ST_OPTION,"Определите дату : "));
      w->Add( new _DateCell (20,1,ST_OPTION,&CurentDate));
      w->AddCallBack(EnterDate,evKey,ENTER);
      w->BottomRight();
      w->HelpID=8;
      w->Exec();
      delete w;
    break;


  }
  if(DateOK){

    CurentAccName=AccArr[CurentAccount-1].name;
    CurentAccount=AccArr[CurentAccount-1].code;

    if(CurentAccServ==5){
      MayStart=LoadBank(CurentAccount, &CurentDate, "*.bnk");
    }else
      MayStart=1;
  }
};

void ServeAccChoice(_Event *E, void *parent){
  MayStart=0;
  if(choice){
    CurentAccount=choice;
    _Window * w;

    static char * menu[]={
      "Основные поступления",
      "Досылки",
      "Переброски",
      "Полный просмотр доходов",
      "Загрузить выписку",
      "Расходы",
      "Полный просмотр расходов",
      0
    };
    w=new _VMenu(40,12,menu,&CurentAccServ);
    w->Add(new _Static(2,0,ST_VMENU,AccArr[choice-1].name));
    w->Center();
    w->HelpID=7;
    w->AddCallBack(DateLoader,evKey,ESC);
    w->AddCallBack(DateLoader,evKey,ENTER);
    w->Exec();
    delete w;
  }
};

extern ModemOption     O;

void ServeConnectChoice(int choice){
  int h=open("setup.id",O_RDONLY|O_BINARY);
  if(h!=-1){
    read(h,&O,sizeof(O));
    close(h);
  }

  switch(choice){
    case 0:
    break;

    case 1:
    ModemParam();
    break;

    case 2:
     if(Mailer) Mailer->WaitFor();
     break;

    case 3:
     if(Mailer) Mailer->StopWait();
     if(Mailer) Mailer->HangUp();
     break;

    case 4: // Клиенты
     if(Dostup[10]==D_READ || Dostup[10]==D_WRITE)
      AddF16(0);
     break;

  };
};

void ServeReportChoice(_Event *E, void * parent){
  for(int i=0;i<=20;i++)
    AccArr[i].yes=0;
  switch(choice){
    case 0:
      return;

    // по всем счетам
    case 1:
      DateOK=0;
      _Window *w =new _Window(40,12,0,3,ST_OPTION,stFrame|stBuffered,"Выбор счетов");
      w->Add(new _Static(1,1,ST_OPTION,"Определите счета для включения в отчет:"));
      for(int i=0;i<=20 && AccArr[i].name[0]!=0;i++){
        _Cell *c;
        w->Add(c=new _Cell(1,2+i,16,1,ST_OPTION,AccArr[i].name));
        c->st|=stLocked;
        w->Add(new _YNCell(18,2+i,ST_OPTION,&AccArr[i].yes));
        w->AddCallBack(EnterDate,evKey,ENTER);
      }
      w->HelpID=29;
      w->Center();
      w->Exec();
      delete w;
      break;

    default:
      DateOK=1;
      AccArr[choice-2].yes=1;
  };
};

void ServeReportChoice2(_Event *E, void * parent){
  for(int i=0;i<=20;i++)
    AccArr[i].yes=0;
  switch(choice){
    case 0:
      return;

    // по всем счетам
    case 1:
      DateOK=0;
      _Window *w =new _Window(40,12,0,3,ST_OPTION,stFrame|stBuffered,"Выбор счетов");
      w->Add(new _Static(1,1,ST_OPTION,"Определите счета для включения в отчет:"));
      for(int i=0;i<=20 && AccArr[i].name[0]!=0;i++){
        _Cell *c;
        w->Add(c=new _Cell(1,2+i,16,1,ST_OPTION,AccArr[i].name));
        c->st|=stLocked;
        w->Add(new _YNCell(18,2+i,ST_OPTION,&AccArr[i].yes));
        w->AddCallBack(EnterDate,evKey,ENTER);
      }
      w->HelpID=30;
      w->Center();
      w->Exec();
      delete w;
      break;

    default:
      DateOK=1;
      AccArr[choice-2].yes=1;
  };
};



extern int LPT;

int main(int argc, char * argv[]){
  printf("MB V2.01 betta (c) LME Reserches 1995\n");
  {
    _Date td;
    _Time tt;
    tt.Now();
    td.Today();
    char b[50];
    printf(td.PrtDate(b,1));
    printf(" ");
    printf(tt.T2C(b));
  }
  CurentDate.Today();
  printf("\nИнициализация...\n");
  unlink("trace.");
  for(int i=1;i<argc;i++){
    /*
    if(argv[i][0]=='l' || argv[i][0]=='L'){
      Loader=1;
    }
    */
    if(argv[i][0]=='a' || argv[i][0]=='A'){
      Administrator=1;
    }
    if(argv[i][0]=='f' || argv[i][0]=='F'){
      FastAndBad=1;
    }
    if(argv[i][0]=='c' || argv[i][0]=='C'){
      ColorDefined=LoadColor(argv[i]+1);
    }
    if(argv[i][0]=='m' || argv[i][0]=='M'){
      StartMailer=1;
    }
    if(argv[i][0]=='l' || argv[i][0]=='L'){
      NoLogo=1;
    }

  }

  Init10();
  Init11();
  Init12();
  Init13();
  Init14();
  Init15();
  Init16();
  Init17();
  Init18();
  Init19();
  Init20();
  Init21();
  Init22();
  Init23();

  if(!ColorDefined)
     LoadColor("current.pal");

  if(!NoLogo)
    Logo320x200("lme.pal","lme.xt");

  _Desktop *D = new _Desktop("NB V2.0");

  D->Show();
  _Help Help("nb.hlp");

  if(!Administrator){
    file_23_0 k23;
    StrClear(k23.Kod_dostupa,10);
    StrClear(k23.Parolj,30);
    _Window * w=new _Window(40,13,45,4,ST_OPTION,stFrame,"Password");
    w->Add(new _Static(1,1,ST_OPTION,"Код    : "));
    w->Add(new _Password(10,1,10,ST_OPTION,k23.Kod_dostupa));
    w->Add(new _Static(1,2,ST_OPTION,"Пароль : "));
    w->Add(new _Password(10,2,30,ST_OPTION,k23.Parolj));
    w->Exec();
    delete w;
    unsigned char *mm=(char*) Coder;
    for(int i=0; i<10;i++){
      k23.Kod_dostupa[i]^=mm[i];
    }
    for(i=0; i<30;i++){
      k23.Parolj[i]^=mm[i+10];
    }
    if(!T23->GEQ(rf23,&k23,0)){
        for(i=0; i<50;i++){
          rf23->Privelegii[i]^=mm[i+40];
        }
        memcpy(Dostup,rf23->Privelegii,51);
    }else{
      MessageBox(
        "Вы ошиблись в наборе кода или пароля",
        "Извините, увидимся в следующий раз ...");
      goto bye;
    }
  }else{
    memset(Dostup,D_WRITE,51);
  }

  //D->AddCallBack(Order,evKey,CTRL_M);
  D->AddCallBack(ColorOption,evKey,CTRL_O);

  LoadOption();
  if(StartMailer){
    //printf("Start BMM's Maier support\n");
    Mailer= new MAILER(cfg.r_number);
    if(Mailer && Mailer->OK){
      //printf("Init Mail Idle \n");
      ModemFunc=ModemIdle;
    }
  }


  MainMenu();

  D->Run();
  if(StartMailer){
    if(Mailer){
      delete Mailer;
      Mailer=0;
    }
  }
bye:
  Close23();
  Close22();
  Close21();
  Close20();
  Close19();
  Close18();
  Close17();
  Close16();
  Close15();
  Close14();
  Close13();
  Close12();
  Close11();
  Close10();
  if(!NoLogo)
    Logo320x200("lme.pal","lme.xt");

  delete D;
  return 0;
};
