/****************************************************
 *   MYSTRING.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include <string.h>
#include <stdio.h>
#include <io.h>
#include <dos.h>

#define NO_TRACE
#include "common.h"
#undef NO_TRACE

void StrClear(char * s,int l,char f){
  TRACE("StrClear");
  memset(s,f,l);
  s[l]=0;
};

int upper(unsigned char ch){
  if(ch<97) return ch;
  if(ch>=97 && ch <=122) return ch-32;
  if(ch>=160 && ch<=175) return ch-32;
  if(ch >=224 && ch <=239) return ch-80;
	return ch;
};

char *StrUpper(char *source,char *dest){
	TRACE("StrUpper");
  unsigned char *s=(unsigned char *)source;
  unsigned char *buf=(unsigned char *)dest;
	if(!buf)
		buf=s;
	for(int i=0; i<strlen(s);i++)
		buf[i]=upper(s[i]);
	return buf;
};

char* StrTrim(char *s,char *buf){
	TRACE("StrTrim");
	if(!buf)
    buf=s;
  int i,j;
  i=j=0;

  for(j=0;j<strlen(s);j++){
    if(s[j]!=' ' && s[j] !='\t' && s[j] !=10 && s[j] !=13)
      buf[i++]=s[j];
    else
      if(i!=0 && buf[i-1]!=' ')
        buf[i++]=' ';
  }

  if(i && buf[i-1]==' ') i--;

  buf[i]=0;

  return buf;
};

int   StrWrap(char *s, int l){
   TRACE("StrWrap");

    if(strlen(s)<=l)
      return strlen(s)-1;

    for(int i=l-1 ;i>=0;i--)
      if( s[i]==' ' ||
          s[i]=='.' ||
          s[i]==',' ||
          s[i]=='+' ||
          s[i]=='-' ||
          s[i]=='/' ||
          s[i]=='*' ||
          s[i]==')' ||
          s[i]==']' ||
          s[i]==0)
            return i;

   return 0;

};


char* StrL(char *s,char *buf, int l,int mode){
  TRACE("StrL");
  StrClear(buf,l);
  if(strlen(s)<l){
    strncpy(buf,s,strlen(s));
  }else{
    switch ( mode )
    {
    case 0:
      strncpy(buf,s,l);
      break;

    case 1:
      {
      int g=StrWrap(s,l);
      if(g){
        //g++;
        strncpy(buf,s,g);
      }else
        strncpy(buf,s,l);
      break;
      }
    default:
      StrClear(buf,l,mode);

    }
  }
  return buf;
};

char* StrR(char *s,char *buf, int l,int mode){
  TRACE("StrR");

  StrClear(buf,l);
  if(strlen(s)<l){
    strncpy((buf+l-strlen(s)),s,strlen(s));
  }else{
    switch ( mode )
    {
    case 0:
      strncpy(buf,s,l);
      break;

    case 1:
      {
      int g=StrWrap(s,l);
      if(g){
        //g++;
        strncpy(buf+l-g,s,g);
      }else
        strncpy(buf,s,l);
      break;
      }
    default:
      StrClear(buf,l,mode);

    }
  }
  return buf;
};

char* StrC(char *s,char *buf, int l,int mode){
  TRACE("StrC");
  StrClear(buf,l);
  if(strlen(s)<l){
    strncpy((buf+(l-strlen(s))/2),s,strlen(s));
  }else{
    switch ( mode )
    {
    case 0:
      strncpy(buf,s,l);
      break;

    case 1:
      {
      int g=StrWrap(s,l);
      if(g){
        //g++;
        strncpy(buf+(l-g)/2,s,g);
      }else
        strncpy(buf,s,l);
      break;
      }
    default:
      StrClear(buf,l,mode);

    }
  }
  return buf;
};
char* StrJ(char *s,char *buf, int l,int mode){
  TRACE("StrJ");
  StrClear(buf,l);
  char *c=new char [strlen(s)+1];
  StrTrim(s,c);
  int sp=0;
  int mult=0;
  int mod=0;
  int j=0;

  for(int i=0 ;i< strlen(c);i++)
    if(c[i]==' ') sp++;

  if(strlen(c)<l){
    if(sp){
      mult=(l-strlen(c))/sp;
      mod=(l-strlen(c))%sp;
      for(i=0;i<strlen(c);i++){
        if(c[i]==' '){
          if(mod){ buf[j++]=' '; mod--;}
          for(int k=0;k<=mult;k++)
            buf[j++]=' ';
        }else{
          buf[j++]=c[i];
        }
      }
    }else{
      strncpy(buf,c,strlen(c));
    }

  }else{
    switch ( mode )
    {
    case 0:
      strncpy(buf,s,l);
      break;

    case 1:
      {
      int g=StrWrap(c,l);
      if(g){
          //g++;
          sp=0;
          for(int i=0 ;i< g;i++)
            if(c[i]==' ') sp++;
          if(sp){
            mult=(l-g)/sp;
            mod=(l-g)%sp;
            for(i=0;i<g;i++){
              if(c[i]==' '){
                if(mod){ buf[j++]=' '; mod--;}
                for(int k=0;k<=mult;k++)
                  buf[j++]=' ';
              }else
                buf[j++]=c[i];
            }
          }else{
            strncpy(buf,c,g);
          }


      }else
        strncpy(buf,s,l);
      break;
      }
    default:
      StrClear(buf,l,mode);

    }
  }
  delete c;
  return buf;
};





int   StrForm(char *s,char * buf[], int l,int array){
  TRACE("StrForm");
  int curl=0,ll;
  for(int i=0;i<array;i++){
    if(curl<strlen(s)){
      ll=StrWrap(s+curl,l);
      StrClear(buf[i],l);

      if(!ll)
        ll=l;
      else
        ll++;
      strncpy(buf[i],s+curl,ll);
      curl+=ll;
      if(s[curl]==' ')
        curl++;
    }else
      break;
  }
 return i;
};

int   StrEmpty(char *s){
    TRACE("StrEmpty");
    for(int i=0 ;i<=strlen(s);i++)
      if( !(s[i]==' ' || s[i]==0 || s[i]=='\r' || s[i] =='\t' || s[i]=='\n' ))
            return 0;

   return 1;

};

int StrRead(char *buf ,char *format, ...){
  TRACE("StrRead");
  int p[100],fp[100];
  char **ptr=&format;
  char *b=new char[strlen(buf)+1];
  char *f=new char[strlen(format)+1];
  strcpy(b,buf);
  strcpy(f,format);
  StrTrim(b);
  StrTrim(f);

  int fj=1,j=1,k=strlen(b);
  memset(p,0,sizeof(int)*100);
  memset(fp,0,sizeof(int)*100);

  // разбиваем на строки
  for(int i=0;i<k ;i++){
    if(b[i]==' '){
       p[j++]=i+1;
       b[i]=0;
       if(j==100) break;
    }
  }

  k=strlen(f);
  for(i=0;i<k ;i++){
    if(f[i]==' '){
       fp[fj++]=i+1;
       f[i]=0;
       if(fj==100) break;
    }
  }

  int res,out=0;
  for(i=0;i<fj;i++){
    res=0;
    if(f[fp[i]]=='['){
      fp[i]++;
      k=strlen(&(f[fp[i]]));
      f[fp[i]+k]=0;
      for(k=0;k<j;k++){
			 if(res= sscanf(&(b[p[k]]),&(f[fp[i]]),ptr[i+1]) ) break;
      }
    }else
      res=sscanf(&(b[p[i]]),&(f[fp[i]]),ptr[i+1]);
    if(res) out++;
  }
  delete b;
  delete f;
  return out;
};

static char TempFileBuf[14];
char *MakeTemp (char *ext){
 TRACE("MakeTemp");
 struct time T;
 while(1){
  gettime(&T);
  sprintf(TempFileBuf,"%02d%02d%02d%02d.%c%c%c",T.ti_hour,T.ti_min,T.ti_sec,T.ti_hund,ext[0],ext[1],ext[2]);
  if(access(TempFileBuf,0)){
    return TempFileBuf;
  }
 }
};



