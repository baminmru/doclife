/****************************************************
 *       LOGO.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include <dos.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <io.h>
#include <sys\stat.h>
#include <fcntl.h>
#include <time.h>

#define FADE_DELAY 5
#define PICTURE_DELAY 5000

unsigned char *video=(unsigned char*) 0xA0000000UL;


void Logo320x200(char *pal_file, char *pic_file){


  unsigned char pal[768];
  REGS r;
  int color;

  // читаем палетку
  outp(0x3c7,0);
  for(color=0;color<768; color++){
    pal[color]=inp(0x3c9);
  }

  // гасим текстовый экран
  for(int fade=64; fade>=0;fade--){
    outp(0x3c8,0);
    for(color=0;color<768; color++){
      outp(0x3c9,((long)pal[color])*fade/64);
    }
    delay(FADE_DELAY);
  }

  int ph=open(pal_file,O_RDONLY|O_BINARY);
  read(ph,pal,768);
  close(ph);

  r.x.ax=0x13;
  int86(0x10,&r,&r);

  outp(0x3c8,0);
  for(color=0;color<768; color++){
    outp(0x3c9,0);
  }

  ph=open(pic_file,O_RDONLY|O_BINARY);
  lseek(ph,12,SEEK_SET);
  read(ph,video,filelength(ph)-12);
  close (ph);

  for(fade=1; fade<=64;fade++){
    outp(0x3c8,0);
    for(color=0;color<768; color++){
      outp(0x3c9,((long)pal[color])*fade/64);
    }
    delay(FADE_DELAY);
  }

  for(fade=PICTURE_DELAY; !kbhit()  && fade>0; fade-=10)
    delay(10);


  for(fade=64; fade>0;fade--){
    outp(0x3c8,0);
    for(color=0;color<768; color++){
      outp(0x3c9,((long)pal[color])*fade/64);
    }
    delay(FADE_DELAY);
  }

  outp(0x3c8,0);
  for(color=0;color<768; color++){
    outp(0x3c9,0);
  }


  r.x.ax=3;
  int86(0x10,&r,&r);
  while(kbhit())
    getch();
  return;
};

static unsigned char TextPal[768];
static short Saver=0;

void SaverON(void){
  int color;
  if(Saver)
    return;
  Saver=1;
  // читаем палетку
  outp(0x3c7,0);
  for(color=0;color<768; color++){
    TextPal[color]=inp(0x3c9);
  }

  // гасим текстовый экран
  for(int fade=64; fade>=20;fade--){
    outp(0x3c8,0);
    for(color=0;color<768; color++){
      outp(0x3c9,((long)TextPal[color])*fade/64);
    }
    delay(FADE_DELAY*2);
  }
};

void SaverOFF(void){
  if(!Saver)
    return;
  Saver=0;
  for(int fade=20; fade<=64;fade++){
    outp(0x3c8,0);
    for(int color=0;color<768; color++){
      outp(0x3c9,((long)TextPal[color])*fade/64);
    }
    delay(FADE_DELAY/2);
  }
};

static long InitAddr=0;

struct TAG {
  long Length;
  short Type;
};
struct HEADER {
  long Size;                 // The size of the entire animation file,
                             // including this file header.
  short Magic;               // File format identifier. Always hex AF12.
  short LastCadr;            // Number of frames in the flic. This count does
                             // not include the ring frame. Flic files have a
                             // maximum length of 4000 frames.
  short dx;                  // Screen width in pixels.
  short dy;                  // Screen height in pixels.
  short Depth;               // Bits per pixel (always 8).
  short Flags;               // Set to hex 0003 after ring frame is written
                             // and flic header is updated. This indicates
                             // that the file was properly finished and
                             // closed.
  long Speed;                // Number of milliseconds to delay between each
                             // frame during playback.
  short Reserved;            // Unused word, set to 0.
  unsigned long Created;     // The MSDOS-formatted date and time of the
                             // file's creation.
  unsigned long Creator;     // The serial number of the Autodesk Animator
                             // Pro program used to create the file. If the
                             // file was created by some other program using
                             // FlicLib, this value is hex 464C4942 ("FLIB").
  unsigned long Updated;     // The MSDOS-formatted date and time of the
                             // file's most recent update.
  unsigned long Updater;     // Indicates who last updated the file. See the
                             // description of creator.
  short AspectX;             // The x-axis aspect ratio at which the file was
                             // created.
  short AspectY;             // The y-axis aspect ratio at which the file was
                             // created. Most often, the x:y aspect ratio is
                             // 1:1. A 320x200 flic has a ratio of 6:5.
  char reserved1[38];        // Unused space, set to zeroes.
  long Start;                // Offset from the beginning of the file to the
                             // first animation frame chunk.
  long Restart;              // Offset from the beginning of the file to the
                             // second animation frame chunk. This value is
                             // used when looping from the ring frame back to
                             // the second frame during playback.
  char reserved3[40];        // Unused space, set to zeroes.
};
struct FRAME_HEADER {
  long Size;                 // The size of the frame chunk, including this
                             // header and all subordinate chunks that
                             // follow.
  short Type;                // Frame chunk identifier. Always hex F1FA.
  short nElt;                // Number of subordinate chunks in the frame chunk.
  char reserved[8];          // Unused space, set to zeroes.
};
static HEADER Header;
static FRAME_HEADER *FrameHeaderPtr;
static TAG *TagPtr;

#define TRUE_SCREEN_WIDTH 320
#define TRUE_SCREEN_HEIGHT 200
char *ScreenBuffer=0;

void SetPalette(short first,short cnt,char *Ptr){
  outp(0x3c8,first);
  for(int color=0;color<cnt; color++){
    outp(0x3c9,Ptr[color]);
  }
};

void OutBuf(short Addr,char * Pointer, short cnt){
  memcpy(video+Addr,Pointer,cnt);
};

void OutChar(short Addr,char Pointer, short cnt){
  memset(video+Addr,Pointer,cnt);
};

short ViewFli(char *FileName ,short Time=2, short AfterTime=0) {
  short h=open(FileName,O_RDONLY|O_BINARY);
  if (h==-1)  return 0;

  // читаем заголовок
  read(h,&Header,sizeof(Header));
  if (Header.dx>TRUE_SCREEN_WIDTH || Header.dy> TRUE_SCREEN_HEIGHT){
    close(h);
    return 0;
  }

  unsigned char pal[768];
  REGS r;
  int color;

  // читаем палетку
  outp(0x3c7,0);
  for(color=0;color<768; color++){
    pal[color]=inp(0x3c9);
  }

  // гасим текстовый экран
  for(int fade=64; fade>=0;fade--){
    outp(0x3c8,0);
    for(color=0;color<768; color++){
      outp(0x3c9,((long)pal[color])*fade/64);
    }
    delay(FADE_DELAY);
  }

  // переключаем режим экрана
  r.x.ax=0x13;
  int86(0x10,&r,&r);

  //
  outp(0x3c8,0);
  for(color=0;color<768; color++){
    outp(0x3c9,0);
  }

  ScreenBuffer= new char [Header.dx*Header.dy+16];

  InitAddr+=(TRUE_SCREEN_WIDTH-Header.dx)/2;
  InitAddr+=(long)((TRUE_SCREEN_HEIGHT-Header.dy)/2)*TRUE_SCREEN_WIDTH;

  if (Header.Start) lseek(h,Header.Start,SEEK_SET);

  long Addr,BlockLength; short i,ly,y,t,ktw,ksw;
  unsigned char *Pointer,*p,ksc,ColorIndex,*PrevPointer; char ktc;
  unsigned long CadrTime=clock();

 ViewAgain:
  for (short cadr=0; cadr<Header.LastCadr; cadr++) {
    if (kbhit()) {
      while(kbhit())
        getch();
      close(h);
      delete ScreenBuffer;
      r.x.ax=3;
      int86(0x10,&r,&r);
      return 0;
    }

    read(h,FrameHeaderPtr=(FRAME_HEADER *)ScreenBuffer,16);

    BlockLength = FrameHeaderPtr->Size-16;

    read(h,Pointer=(unsigned char *)(ScreenBuffer+16),BlockLength);

    while (BlockLength>0) {

      TagPtr=(TAG *)Pointer;
      BlockLength-=TagPtr->Length;
      PrevPointer=Pointer;
      Pointer+=6;
      switch (TagPtr->Type) {
        case 0x04:   // 8 битовая палитра
          t=*((short *)(Pointer)); Pointer+=2;
          ColorIndex=0;
          for (i=0; i<t; i++) {
            ColorIndex+=*(Pointer++);
            ly=*(Pointer++); if (!ly) ly=256*3; else ly*=3;
            for (y=0,p=Pointer; y<ly; y++,p++) *p>>=2;
            SetPalette(ColorIndex,ly,Pointer);
            ColorIndex+=ly/3;
            Pointer+=ly;
          }
          if ((Pointer-ScreenBuffer)&1) Pointer++;
          break;
        case 0x0B:   // 6 битовая палитра
          t=*((short *)(Pointer)); Pointer+=2;
          ColorIndex=0;
          for (i=0; i<t; i++) {
            ColorIndex+=*(Pointer++);
            ly=*(Pointer++); if (!ly) ly=256*3; else ly*=3;
            SetPalette(ColorIndex,ly,Pointer);
            ColorIndex+=ly/3;
            Pointer+=ly;
          }
          if ((Pointer-ScreenBuffer)&1) Pointer++;
          break;

        case 0x0F:   // первый кадр
          ly=Header.dy;
          for (y=0;  y<ly; y++) {
            Addr=(long)y*TRUE_SCREEN_WIDTH+InitAddr;
            ksc=*(Pointer++);
            for (t=0; t<ksc; t++) {
              ktc=(char)*(Pointer++);
              if (ktc<0)
                { OutBuf(Addr,Pointer,-ktc); Pointer-=ktc; Addr-=ktc; }
              else
                { OutChar(Addr,*(Pointer++),ktc); Addr+=ktc; }
            }
          }
          break;

        case 0x0C:   // байтовый ТЭГ
          y = *(short*)(Pointer);
          ly = *(short*)(Pointer+2)+y;
          Pointer+=4;
          for(; y<ly; y++) {
            Addr=(long)y*TRUE_SCREEN_WIDTH+InitAddr;
            ksc = *(Pointer++);
            for (t=0; t<ksc; t++) {
              Addr+=*(Pointer++);
              ktc=(char)(*(Pointer++));
              if (ktc>0)
                { OutBuf(Addr,Pointer,ktc); Pointer+=ktc; Addr+=ktc; }
              else
                { OutChar(Addr,*(Pointer++),-ktc); Addr-=ktc; }
            }
          }
          break;

        case 0x07:   // словный ТЭГ
          y=0; ly=*((short*)Pointer); Pointer+=2;
          if (ly<0) { y-=ly; ly=*((short*)Pointer); Pointer+=2; }
          for(; y<ly; y++) {
MoreWords:  ksw=*(short*)Pointer; Pointer+=2;
            if ((ksw&0xC000)==0xC000) { y-=ksw; ly-=ksw; goto MoreWords; }
            Addr=(long)y*TRUE_SCREEN_WIDTH+InitAddr;
            if (ksw&0x8000) { // последний байт в строке
              OutChar(Addr+Header.dx-1,ksw&0xFF,1);
              ksw=*(short*)Pointer; Pointer+=2; // сразу за ним к-во пакетов
            }
            for(t=0; t<ksw; t++) {
              Addr+=*(Pointer++);
              ktw=2*((short)((char)*(Pointer++)));
              if (ktw>0) { OutBuf(Addr,  Pointer, ktw); Pointer+=ktw; Addr+=ktw; }
              else       { OutChar(Addr,*Pointer,-ktw); Pointer+=2;   Addr-=ktw; }
            }
          }
          break;

        case 0x10:   // полный кадр
          BlockLength=0;
          for (y=0,Addr=InitAddr;
               y<Header.dy;
               y++,Addr+=TRUE_SCREEN_WIDTH,Pointer+=Header.dx)
            OutBuf(Addr,Pointer,Header.dx);
          goto PointerMoved;

        default:     // остальное пропускаем
          break;
      }
      Pointer=PrevPointer+TagPtr->Length;
PointerMoved:;
    }                                            // цикл по тегам
    // задержка по времени
    while (clock()<CadrTime+Time);
    CadrTime=clock();
  }                                                // цикл по кадрам

  if(!AfterTime){
    lseek(h,Header.Start,SEEK_SET);
    goto ViewAgain;
  }else{
    while (clock()<CadrTime+AfterTime){
     if(kbhit()) break;
    }
  }

  while(kbhit())
    getch();
  delete ScreenBuffer;
  close(h);

  // переключаем режим экрана
  r.x.ax=0x3;
  int86(0x10,&r,&r);

  return 1;
}



