/****************************************************
 *        KEY.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/

#include <bios.h>
#include <conio.h>
#include "common.h"

short HelpLock =0;

unsigned GetKey(int no_wait){
  unsigned int code;
  unsigned int shift;
  while(1){
    if(bioskey(0x11)){
      shift = bioskey(0x12)&0x0f;
      code  = bioskey(0x10);

      if(shift & 0x8)
        return (code & 0xff00)+8;

      if(shift & 0x4)
        return (code & 0xff00)+4;

      if(code & 0xff)
        if((code & 0xff)== 0xe0){
          if(code == 0x00e0)
            return code & 0xff;
        }else
          return code & 0xff;

      if(shift & 0x3)
        return (code & 0xff00)+3;

      //контекстная помощь
      if((code&0xff00)==0x3b00  ){
        if(!HelpLock){
          HelpLock= GDt->Q.AddMessage(sysHelp);
        }
        return 0;
      }
      return code&0xff00;
    }else{
      if(no_wait)
        return 0;
    }
  }

};

unsigned CheckKey(unsigned key,unsigned *list,int listlen){
  for(int i=0;i<listlen;i++){
    if(key==*(list+i))
      return key;
  }
  return 0;
};

unsigned GetKeyArray( unsigned s, ... ){
  unsigned *arg=&s;
  unsigned key;
  do{
    key=GetKey();
    for(int i=0;i<s;i++)
      if(arg[i+1]==key) return key;
  }while(1);
};


unsigned GKArr( unsigned s, ... ){
  unsigned key;
  unsigned char *r=(unsigned char*)&s;
  unsigned *arg=&s;
  unsigned char *b;
  unsigned char c;
  unsigned int i=0;

// работает для Boralnd в старших моделях памяти !!!
  r-=4;
  b=*((unsigned char**)r);

  c=*b;
  if(c==0x59){
    i+=2;
    c=*(b+1);
    if(c==0x59) i+=2;
  }else{
    c=*(b);
    if(c==0x83){
      c=*(b+1);
      if(c==0xC4){
        i=*(b+2);
      }
    }
  }

  do{
    key=GetKey();
    for(int j=0;j<i/2;j++)
      if(arg[j]==key) return key;
  }while(1);
};

