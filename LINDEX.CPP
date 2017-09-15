/****************************************************
 *     LINDEX.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>

#include "lindex.h"

LIndex::LIndex(char *_name){
  h=-1;
  counter=0;
  name=_name;
  closed=0;
};

LIndex::~LIndex(){
  if(h!=-1)
    close(h);
};

int LIndex::NewList(void){
  if(h!=-1) close(h);
  h=open(name,O_RDWR|O_BINARY|O_CREAT|O_TRUNC);
  counter=0;
  closed=0;
  if(h==-1) return 0;

  if(write(h,&counter,4)==4) return 1;

  return 0;
};


void LIndex::CloseList(void){
  if(h!=-1){
    closed=1;
    lseek(h,0,SEEK_SET);
    write(h,&counter,4);
  }
};


int LIndex::AddPosition(long pos){
  if(h!=-1 && !closed){
    lseek(h,0,SEEK_END);
    if(write(h,&pos,4)==4){
      counter++;
      return 1;
    }else
      return 0;
  }else{
    return 0;
  }
};

long LIndex::GetPosition(long num){
  long newpos;
  if(h!=-1 && num <= counter && num >0){
    lseek(h,num*4,SEEK_SET);
    if(read(h,&newpos,4)==4)
      return newpos;
    else
      return 0;
  }
    return 0;
};


