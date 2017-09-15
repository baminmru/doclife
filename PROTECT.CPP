/****************************************************
 *    PROTECT.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include <conio.h>
#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <new.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys\types.h>
#include <sys\stat.h>

#include "protect.h"
#include "common.h"

/*
#pragma pack(1);
struct CHECK_SUM {
  char Array[16];
  unsigned long FileSum;
  unsigned long FileSumInvert;
  unsigned long BiosSum;
  unsigned long BiosSumInvert;
  unsigned long Length;
  unsigned long LengthInvert;
};
union CHECK {
  char Array[sizeof(CHECK_SUM)];
  CHECK_SUM CheckSumPack;
};
static CHECK Check=
  {'П','о','ш','е','л',' ','т','ы',' ','в',' ','п','е','н','ь','!',
   0,0,0,0, 0,0,0,0, 0,0,0,0, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1};
#pragma pack();
static unsigned long CheckSum;
static void InitSum() { CheckSum=0; }
static void AddSum(void *buf,long Length) {
  unsigned long *lp=(unsigned long *)buf;
  Length>>=2; while (Length--) CheckSum+=*(lp++);
}
static unsigned long CheckSum1;
static void InitSum1() { CheckSum=0; }
static void AddSum1(void *buf,long Length) {
  unsigned long *lp=(unsigned long *)buf;
  Length>>=2; while (Length--) CheckSum1+=*(lp++);
}

// возвращает TRUE если программе можно продолжаться
BOOL ProtectOK(char *Name) {

  short h=open(Name,O_RDONLY|O_BINARY);
  short i; char buf[1024];
  InitSum(); do AddSum(buf,i=read(h,buf,1024)); while (i==1024);
  if (Check.CheckSumPack.FileSum!=CheckSum){
     close(h);
     return FALSE; //сумма файла
  }
  if(Check.CheckSumPack.Length!=filelength(h)){
    close(h);
    return FALSE;
  }
  close(h);

  if (Check.CheckSumPack.FileSumInvert!=~CheckSum) return TRUE; //сумма файла
  void *biosptr=MK_FP(0xf000,0xf000);
  InitSum(); AddSum(biosptr,0xFFF);
  if (Check.CheckSumPack.BiosSum!=CheckSum) return FALSE; //сумма BIOS

  return TRUE;
}

// возвращает TRUE если программе нельзя продолжаться (нелегальная копия)
BOOL ProtectNotOK(char *Name) {

    short h=open(Name,O_RDONLY|O_BINARY);
    short i; char buf[1000];
    InitSum(); do AddSum(buf,i=read(h,buf,1000)); while (i==1000);
    if (Check.CheckSumPack.FileSum!=CheckSum){
       close(h);
       return FALSE; //сумма файла
    }
    if(Check.CheckSumPack.Length!=filelength(h)){
      close(h);
      return FALSE; //длинна файла
    }
    close(h);
    void *biosptr=MK_FP(0xf000,0xf000);
    InitSum(); AddSum(biosptr,0xFFF);
    if (Check.CheckSumPack.BiosSumInvert!=~CheckSum) return TRUE; //сумма BIOS

    return FALSE;
}

short InstallEXE(char *Name) {

  short h=open(Name,O_RDONLY|O_BINARY);
  if (h==-1) { printf("Can't %s\n",Name); return 0; }
  short s; char buf[1024];
  InitSum();
  do {
    s=read(h,buf,1024);
    AddSum(buf,s);
  } while (s==1024);
  Check.CheckSumPack.FileSum      = CheckSum;
  Check.CheckSumPack.FileSumInvert=~CheckSum;
  Check.CheckSumPack.Length=filelength(h);
  Check.CheckSumPack.LengthInvert=~filelength(h);
  close(h);
  InitSum();
  void *biosptr=MK_FP(0xf000,0xf000);
  InitSum(); AddSum(biosptr,0xFFF);
  Check.CheckSumPack.BiosSum      = CheckSum;
  Check.CheckSumPack.BiosSumInvert=~CheckSum;
  FILE *f=fopen(Name,"r+b");
  short Ptr=0; char c;
  while (!feof(f)) {
    c=fgetc(f);
    aaa:if (c==Check.Array[Ptr]) { if (++Ptr==16) goto eee; }
    else if (Ptr) { Ptr=0; goto aaa; }
  }
  printf("Mask not found\n");
  fclose(f);
  return 0;
eee:
  fseek(f,0L,SEEK_CUR);
  fwrite(Check.Array+16,sizeof(CHECK_SUM)-16,1,f);
  fclose(f);
  return 1;
}
static unsigned char *coder="ql╪[,sdaf,poqer;lawg";
void SavePasswd(unsigned char *pwd){
  int h = open("password.dl",O_WRONLY|O_BINARY|O_CREAT|O_TRUNC,S_IWRITE);
  if(h==-1)
    return;
  unsigned char *buf=new unsigned char[strlen(pwd)+1];
  strcpy(buf,pwd);
  StrUpper(buf);
  Debug("-> %s\n",buf);
  for(int i=0;i<strlen(pwd);i++){
    buf[i]=buf[i]^coder[i%20];
  }
  write(h,buf,strlen(pwd));
  delete buf;
  close(h);
};

int CmpPasswd(unsigned char *pwd){
  int h = open("password.dl",O_RDONLY|O_BINARY);
  if(h==-1)
    return 1;
  long l=filelength(h);
  unsigned char *buf=new unsigned char[l];
  read(h,buf,l);
  StrUpper(pwd);
  Debug("? %s\n",pwd);
  for(int i=0;i<l;i++){
    if(buf[i]!=(pwd[i]^coder[i%20])){
      delete buf;
      close(h);
      return 0;
    }
  }
  delete buf;
  close(h);
  return 1;
};
*/
