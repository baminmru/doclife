/****************************************************
 *       TEST.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include "date.h"


int main(void){
  _Date b;
  char cb[80];
  printf("Введите дату (dd.mm.yy) :");
  scanf("%s",cb);
  b.C2D(cb);
  int h= open("prevdate.dat",O_WRONLY|O_BINARY|O_CREAT|O_TRUNC,S_IWRITE);
  if(h==-1) return 0;
  write(h,&b,4);
  close(h);
  return 0;
}
