/****************************************************
 *        SUM.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#pragma pack(1);
struct CHECK_SUM {
  char Array[16];
  unsigned long FileSum;
  unsigned long FileSumInvert;
  unsigned long BiosSum;
  unsigned long BiosSumInvert;
};
union CHECK {
  char Array[sizeof(CHECK_SUM)];
  CHECK_SUM CheckSumPack;
};
extern CHECK Check;
//=
//  {'П','о','ш','е','л',' ','т','ы',' ','в',' ','п','е','н','ь','!',
//   0,0,0,0, 0,0,0,0, -1,-1,-1,-1, -1,-1,-1,-1};
#pragma pack();
static unsigned long CheckSum;
void InitSum() { CheckSum=0; }
void AddSum(void *buf,long Length) {
  unsigned long *lp=(unsigned long *)buf;
  Length>>=2;
  while (Length--) CheckSum+=*(lp++);
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
  close(h);
  Check.CheckSumPack.FileSum      = CheckSum;
  Check.CheckSumPack.FileSumInvert=~CheckSum;
  InitSum();
  AddSum((void *)(0xFF000000),0xFFF);
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
