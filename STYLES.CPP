/****************************************************
 *     STYLES.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include "common.h"
/*
 1 - non Focused
 2 - Focused
*/
unsigned char ColorArray[16][2]={
     // 1
     {
      0x07,
      0x70
     },
     // 2
    {
      0x07,
      0x70
     },
    // 3
    {
      0x07,
      0x70
    },
    // 4
    {
      0x07,
      0x70
     },

    // 5
    {
      0x07,
      0x70
     },

    // 6
    {
      0x07,
      0x70
     },

    // 7
    {
      0x07,
      0x70
     },
    // 8
    {
      0x07,
      0x70
     },
    // 9
     {
      0x07,
      0x70
     },

    // 10
    {
      0x07,
      0x70
     },
     // 11
     {
      0x07,
      0x70
     },
     // 12
    {
      0x07,
      0x70
     },
    // 13
    {
      0x07,
      0x70
    },
    // 14
    {
      0x07,
      0x70
     },

    // 15
    {
      0x07,
      0x70
     },

    // 16
    {
      0x07,
      0x70
     }


};

unsigned char Style2Color(short style, short status){
 if(!style) style++;
 return ColorArray[(style - 1)&15][(status-1)&1];
}

void SaveColor(char *name){
  int h=open(name,O_WRONLY|O_BINARY|O_CREAT|O_TRUNC,S_IWRITE);
  if(h==-1)
    return;
  write(h,ColorArray,sizeof(ColorArray));
  close(h);
};

short LoadColor(char *name){
  int h=open(name,O_RDONLY|O_BINARY);
  if(h==-1)
    return 0;
  if(read(h,ColorArray,sizeof(ColorArray))!=sizeof(ColorArray)){
    close(h);
    return 0;
  }
  close(h);
  return 1;
};
