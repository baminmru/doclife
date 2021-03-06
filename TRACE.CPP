/****************************************************
 *      TRACE.CPP                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
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
