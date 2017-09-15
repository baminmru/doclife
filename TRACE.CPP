/****************************************************
 *      TRACE.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include <stdio.h>
#include <stdarg.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <string.h>

int TraceFlag=1;

void  SetTrace(int x){TraceFlag=x;};
void  Trace (char *File, int line ,char *message){
  if(TraceFlag){
    FILE*h=fopen("trace.","at");
    fprintf(h,"#%12s:%05d -> %s\n",File,line,message);
    fclose(h);
  }
};

void Debug(char *format,...){
  if(TraceFlag){
    FILE* h=fopen("trace.","at");
    va_list vl; va_start(vl,format); vfprintf(h,format,vl); va_end(vl);
    fclose(h);
  }
}


void Error(char *format,...){
    FILE* h=fopen("trace.","at");
    va_list vl; va_start(vl,format); vfprintf(h,format,vl); va_end(vl);
    fclose(h);
}
