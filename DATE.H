/****************************************************
 *         DATE.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef DATE
#define DATE

extern char *DayName[];
extern char *MonthName1[];
extern char *MonthName2[];

class _Date
{
  int LeapYear(void);


public:
  char d;
  char m;
  int  y;


  _Date(int d=0,int m=0,int y=0);

  long D2L(void);
  void L2D(long NofDays);
  char * D2C(char *string);
  void C2D(char *string);

  void Today(void);
  void BOY(void);
  void BOQ(void);
  void BOM(void);
  void EOY(void);
  void EOQ(void);
  void EOM(void);
  short DayOfWeek(void);

  void Clear(void);
  void Set(int d,int m,int y);
  int OK();
  int Empty(void){
    if(D2L()==0)
      return -1;
    else
      return 0;
  };

  int EQL(_Date * Anothe_Date);
  int GE(_Date * Anothe_Date);
  int GT(_Date * Anothe_Date);
  int LE(_Date * Anothe_Date);
  int LT(_Date * Anothe_Date);

  void Add(_Date * Anothe_Date);
  void Sub(_Date * Anothe_Date);

  void Add(long days);
  void Sub(long days);
  _Date& operator =(_Date &b){
    d=b.d;
    m=b.m;
    y=b.y;
    return *this;
  };
  void operator +=(long b){
    Add(b);
  };
  void operator -=(long b){
    Sub(b);
  };

  char * PrtDate(char *buf, short week = 0);
};


int  operator !=(_Date a,_Date b);
int  operator ==(_Date a,_Date b);
int  operator > (_Date a,_Date b);
int  operator >=(_Date a,_Date b);
int  operator < (_Date a,_Date b);
int  operator <=(_Date a,_Date b);


class _Time{
 public:
  char hsec;
  char sec;
  char min;
  char hour;
  void C2T(char *s);
  char *T2C(char *s);
  long T2L(void);
  void L2T(long l);
  void Clear(void) { Set(); };
  void Set(char h=0,char m=0,char s=0, char hs=0 );
  void Now(void);

  _Time&  operator = (_Time& b);
  void operator +=(_Time& b);
  void operator -=(_Time& b);
  _Time(char h=0,char m=0,char s=0, char hs=0 );
  ~_Time(){};
};

int  operator !=(_Time a,_Time b);
int  operator ==(_Time a,_Time b);
int  operator > (_Time a,_Time b);
int  operator >=(_Time a,_Time b);
int  operator < (_Time a,_Time b);
int  operator <=(_Time a,_Time b);

int Periods(_Date * bdate ,_Date *edate, short type, short b_e=1);


#endif


