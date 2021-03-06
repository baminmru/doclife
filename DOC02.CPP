/****************************************************
 *      DOC02.CPP                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#include "common.h"
#include "doc.h"
struct account {
  char    Nomer_sheta[16] ; //�����_���
  double sum;
  double cur;
};

void Expert(account *acc, int AccNum);
void BuildF25(void);

int AccNum=0;
account *acc=0;


int FindAcc( char * num){
  for(int i=1;i<=AccNum;i++){
    if(!memcmp(num,acc[i].Nomer_sheta,13))
      return i;
  }
  return 0;
}

double CountAll(void){
   AccNum =  T12->St.Records(T12);
   if(acc){
     delete acc;
     acc=0;
   }
   acc= new account [AccNum+1];
   memset(acc,0,sizeof(account)*(AccNum+1));
   int res,cnt;
   res=T12->GFirst(rf12,0);
   cnt=1;
   while(!res && cnt <= AccNum){
     memcpy(acc[cnt].Nomer_sheta,rf12->Nomer_sheta,16);
     cnt++;
     res=T12->GNext(rf12);
   }

// 2 - ��⠥� ᪮�쪮 ����� �� ����� ���
   double sum=0.0;
   res = T13->GFirst(rf13,0);
   while(!res){
     Debug("%s %d\n",rf13->Nomer_sheta,FindAcc(rf13->Nomer_sheta));
     acc[FindAcc(rf13->Nomer_sheta)].sum=rf13->Summa;
     acc[FindAcc(rf13->Nomer_sheta)].cur=rf13->Summa;
     res=T13->GNext(rf13);
   }
   for(int iii=0; iii<=AccNum;iii++)
     sum+=acc[iii].sum;
  return sum;
};

void BuildDoc02(void){
  if(!MessageBox(
    "     ��ନ஢��� �।������� �� ������ ?     ",
    "(�।���� ����� �।������� �㤥� ��������)",
    "         �� - [Enter]  ��� - [Esc]")) return;


   _Window * ww = new _Window(0,0,36,3,ST_WINDOW,stBuffered,"������ �।��");
   ww->CenterScr();
   ww->Add(new _Static(1,1,ST_WINDOW, "���� ������ �।�� �� ����..."));
   ww->Show();
   int res,cnt;

// 1 - ���뢠�� ����� ��⮢
// 2 - ��⠥� ᪮�쪮 ����� �� ����� ���
   double sum=CountAll();

   delete ww;

   if(acc[0].sum)
    Error("�㬬� ����� �� ����।������� ��� %lf\n",acc[0].sum);
   char buf1[21],buf2[21];
   sprintf(buf1,"%20.2lf",sum);
   sprintf(buf2,"%20.2lf",acc[0].sum);
   if(!MessageBox(
          "�� ����� �ணࠬ�� ������� ᫥���饥 ������⢮ �।��:",
          buf1,
          "� ⮬ �᫥ �� �訡��� ������ (� ��������� ��⮬):",
          buf2,
          "                   �த������ ࠡ��� ?",
          "               �� - [Enter]   ��� - [Esc]")){

         if(acc){
           delete acc;
           acc=0;
         }
         return;
   }

// 3 - �ய�᪠�� ���। �� ࠧ��� ���
  _Gauge *g = new _Gauge(0,0,76,ST_GAUGE,T28->St.Records(T28));
  long g_cnt=0;
  g->CenterScr();
  g->Show();
  GDt->Add(g);
  res=T28->GFirst(rf28,0);
  while(!res){
    g->Update(g_cnt++);
    if(!rf28->Priznak_oplati){
      int i=FindAcc(rf28->Shet_dly_oplati );
      if(acc[i].sum>=rf28->Summa){
        acc[i].sum-=rf28->Summa;
        sum-=rf28->Summa;
      }
    }
    res=T28->GNext(rf28);
  }
  GDt->Del(g);
  sprintf(buf1,"%20.2lf",sum);

  if(!MessageBox(
       "��᫥ �뤥����� �।�� �� ������ ࠧ���� ��⮢ ��⠥���:",
        buf1,
        "                   �த������ ࠡ��� ?",
        "               �� - [Enter]   ��� - [Esc]")
   ){
    if(acc){
        delete acc;
        acc=0;
    }
    return;
  }


// 4 - ��⠥� ����� ��� �㬬 ������஢
   BuildF25();
// 5 - �蠥� �� �蠡����� ������
   Expert(acc,AccNum);

   if(acc){
     delete acc;
     acc=0;
   }
// 5 - �����뢠�� १����
  if(Dostup[19])
    AddF19(0);
};
