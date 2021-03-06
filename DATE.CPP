/****************************************************
 *       DATE.CPP                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#include "common.h"

char * DayName[]={
  "?",
  "�������쭨�",
  "��୨�",
  "�।�",
  "��⢥�",
  "��⭨�",
  "�㡡��",
  "����ᥭ�"
};

char *MonthName1[]={
  "?",
  "������",
  "���ࠫ�",
  "����",
  "��५�",
  "���",
  "���",
  "���",
  "������",
  "�������",
  "������",
  "�����",
  "�������"
};

char *MonthName2[]={
  "?",
  "������",
  "���ࠫ�",
  "����",
  "��५�",
  "���",
  "���",
  "���",
  "������",
  "�������",
  "������",
  "�����",
  "�������"
};

static int ml[12]={31,28,31,30,31,30,31,31,30,31,30,31};

int _Date::LeapYear(void){
  int leap=0;
  if(y/4*4==y) leap=1;
  if(y/100*100==y) leap=0;
  if(y/400*400==y) leap=1;
  return leap;
};



_Date::_Date(int _d,int _m,int _y){
  Set(_d,_m,_y);
};

long _Date::D2L(void){
  long e = 0;
  long y1;
  if (y > 1 ){
    e = (y - 1)/400 * 146097;
    y1 = (y - 1)%400;
    e = e + y1*365 + (y1/4) - (y1/100);
  }

  for(int i = 1;i<12;i++){
    if( i < m ){
      e = e + ml[i - 1];
      if( i == 2) {
        e = e + LeapYear();
      }
    }
  }
  e = e + d;
  return e;
};

void _Date::L2D(long e){
  long ee=0;
  long y1;
  d = 0;
  m = 1;
  y = (e / 146097)*400 + (e%146097)/366 + 1;
  ee = D2L();
  y1 = e - ee;
  if (y1 > 365) {
    y = y + 1;
    ee = D2L();
    y1 = e - ee;
  }
  ml[1] = ml[1] + LeapYear();
  for(int i = 0 ;i<=11;i++){
    if (y1 > ml[i]) {
      m = m + 1;
      y1 = y1 - ml[i];
    }else{
      break;
    }
  }
  ml[1] = ml[1] - LeapYear();
  d = y1;
};

char * _Date::D2C(char *string){
  sprintf(string,"%02d.%02d.%04d",d,m,y);
  return string;
};
void _Date::C2D(char *string){
 sscanf(string,"%d.%d.%d",&d,&m,&y);
 if(!OK()){
  d=m=0; y=0;
 }else
  if(y<100)
    y+=1900;
};

void _Date::Today(void){
  struct date ddd;
  getdate(&ddd);
  d=ddd.da_day;
  m=ddd.da_mon;
  y=ddd.da_year;
};

void _Date::BOY(void){
  d=1;
  m=1;
};
void _Date::BOQ(void){
  d=1;
  if(m<4) m=1;
  else
    if(m<7) m=4;
    else
      if(m<10) m=7;
      else m=10;
};

void _Date::BOM(void){
  d=1;
};

void _Date::EOY(void){
  d=31;
  m=12;
};

void _Date::EOQ(void){
  if(m<4) m=3;
  else
    if(m<7) m=6;
    else
      if(m<10) m=9;
      else m=12;
  EOM();
};

void _Date::EOM(void){
  if( m){
    ml[1]+=LeapYear();
    d=ml[m-1];
    ml[1]-=LeapYear();
  }
};

void _Date::Clear(void){
  d=m=0;
  y=0;
};

void _Date::Set(int _d,int _m,int _y){
  d=_d;
  m=_m;
  y=_y;
  if(!OK()){
    d=m=0;
    y=0;
  }
};

int _Date::OK(){
  int best=0;
  if(y>=0)
    if(m>0 && m<13){
      ml[1]+=LeapYear();
      if(d>0 && d<=ml[m-1])
        best=1;
      ml[1]-=LeapYear();
    }
  return best;
};

int _Date::EQL(_Date * Anothe_Date){
  if(d==Anothe_Date->d &&
     m==Anothe_Date->m &&
     y==Anothe_Date->y )
      return 1;
  else
    return 0;
};

int _Date::GT(_Date * Anothe_Date){
  if(D2L()>Anothe_Date->D2L())
    return 1;
  else
    return 0;
};

int _Date::LT(_Date * Anothe_Date){
  if(D2L()<Anothe_Date->D2L())
    return 1;
  else
    return 0;
};

int _Date::GE(_Date * Anothe_Date){
  if(D2L()>=Anothe_Date->D2L())
    return 1;
  else
    return 0;
};

int _Date::LE(_Date * Anothe_Date){
  if(D2L()<=Anothe_Date->D2L())
    return 1;
  else
    return 0;
};

void _Date::Add(_Date * Anothe_Date){
  Add(Anothe_Date->D2L());
};
void _Date::Sub(_Date * Anothe_Date){
  Sub(Anothe_Date->D2L());
};
void _Date::Add(long days){
  long cur=D2L();
  cur+=days;
  if(cur<0)
    cur=0;
  L2D(cur);
};

void _Date::Sub(long days){
  long cur=D2L();
  cur-=days;
  if(cur<0)
    cur=0;
  L2D(cur);
};

int  operator ==(_Date a,_Date b){
  return (a.d==b.d && a.m==b.m && a.y==b.y);
};

int  operator !=(_Date a,_Date b){
  return !(a.d==b.d && a.m==b.m && a.y==b.y);
};

int  operator >(_Date a,_Date b){
  return (a.D2L()>b.D2L());
};

int  operator >=(_Date a,_Date b){
  return (a.D2L()>=b.D2L());
};

int  operator <(_Date a,_Date b){
  return (a.D2L()<b.D2L());
};

int  operator <=(_Date a,_Date b){
  return (a.D2L()<=b.D2L());
};


long _Time::T2L(void){
  return (((hour*60+min)*60+sec)*100+hsec);
};
void _Time::L2T(long l){
  long l1;
  hsec =l%100;
  l1=l/100;
  sec=l1%60;
  l1/=60;
  min=l1%60;
  hour=l1/60;
  hour=hour%24;
};
void _Time::C2T(char *str){
  short h=0,m=0,s=0,hs=0;
  sscanf(str,"%d:%d:%d.%d",&h,&m,&s,&hs);
  hour=h%24;
  min=m%60;
  sec=s%60;
  hsec=hs%100;
};
char *_Time::T2C(char *s){
  sprintf(s,"%02d:%02d:%02d.%02d", hour,min,sec,hsec);
  return s;
};
void _Time::Set(char h,char m,char s, char hs ){
  hour= h%24;
  min = m%60;
  sec = s%60;
  hsec= hs%100;
};

void _Time::Now(void){
  struct time t;
  gettime(&t);
  hour=t.ti_hour;
  min=t.ti_min;
  sec=t.ti_sec;
  hsec=t.ti_hund;
};

_Time&  _Time::operator = (_Time& b){
  hour= b.hour%24;
  min = b.min%60;
  sec = b.sec%60;
  hsec= b.hsec%100;
  return *this;
};

void  _Time::operator +=(_Time& b){
  long l=T2L()+b.T2L();
  L2T(l);
};
void  _Time::operator -=(_Time& b){
  long l=T2L()-b.T2L();
  L2T(l);
};
_Time::_Time(char h,char m,char s, char hs ){
  Set(h,m,s,hs);
};

int  operator !=(_Time a,_Time b){
  return a.T2L()!=b.T2L();
};
int  operator ==(_Time a,_Time b){
  return a.T2L()==b.T2L();
};
int  operator > (_Time a,_Time b){
  return a.T2L()>b.T2L();
};
int  operator >=(_Time a,_Time b){
  return a.T2L()>=b.T2L();
};
int  operator < (_Time a,_Time b){
  return a.T2L()<b.T2L();
};
int  operator <=(_Time a,_Time b){
  return a.T2L()<=b.T2L();
};


int Periods(_Date * bdate ,_Date *edate, short type, short b_e){
  _Date b=*bdate,e=*edate,t;
  int cnt;
  if(e <= b)
    return 0;
  if(b_e){
    // ����� �� ��砫� ��ਮ��
    switch(type){
      case 1: // �����
        t=b;
        t.BOM();
        if(b!=t){
          b.EOM();
          b.Add(1);
        }
        e.BOM();
        cnt=0;
        while(b<=e){
         cnt++;
         b.EOM();
         b.Add(1);
        }
        return cnt;
      case 2: // ����⠫
        t=b;
        t.BOQ();
        if(b!=t){
          b.EOQ();
          b.Add(1);
        }
        e.BOQ();
        cnt=0;
        while(b<=e){
         cnt++;
         b.EOQ();
         b.Add(1);
        }
        return cnt;
      case 3: // ���
        t=b;
        t.BOY();
        if(b!=t){
          b.EOY();
          b.Add(1);
        }
        e.BOY();
        cnt=0;
        while(b<=e){
         cnt++;
         b.EOY();
         b.Add(1);
        }
        return cnt;
    }
  }else{
    // ����� �� ����� ��ਮ��
    switch(type){
      case 1: // �����
        cnt=0;
        t=e;
        t.EOM();
        if(e!=t){
          e.BOM();
          e.Sub(1);
        }
        b.EOM();
        while(b<=e){
         cnt++;
         b.Add(1);
         b.EOM();
        }
        return cnt;
      case 2: // ����⠫
        cnt=0;
        t=e;
        t.EOQ();
        if(e!=t){
          e.BOQ();
          e.Sub(1);
        }
        b.EOQ();
        while(b<=e){
         cnt++;
         b.Add(1);
         b.EOQ();
        }
        return cnt;

      case 3: // ���
        cnt=0;
        t=e;
        t.EOY();
        if(e!=t){
          e.BOY();
          e.Sub(1);
        }
        b.EOY();
        while(b<=e){
         cnt++;
         b.Add(1);
         b.EOY();
        }
        return cnt;
    }
  }
  return 0;
};
short _Date::DayOfWeek(void)
{
    short century, yr, dw, y2=y, m2=m, d2=d;

    if(m2 < 3)
        {
        m2 += 10;
        --y2;
        }
    else
        m2 -= 2;

    century = y2 / 100;
    yr = y2 % 100;
    dw = (((26 * m2 - 2) / 10) + d2 + yr + (yr / 4) + (century / 4) -
                (2 * century)) % 7;

    if(dw < 0)
        dw += 7;

    if(!dw) dw=7;

    return dw;
};

char *_Date::PrtDate(char *buf,short week){
  if(week)
    sprintf(buf,"%d %s %04d, %s",d,MonthName2[m],y,DayName[DayOfWeek()]);
  else
    sprintf(buf,"%d %s %04d",d,MonthName2[m],y);

  return buf;
}

