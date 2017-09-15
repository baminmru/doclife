/****************************************************
 *       SCREEN.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef SCREEN
#define SCREEN

class _Screen{
 private:
  char *Buf,*Clone;
  short Col,Raw;
  unsigned char X,Y,Mode;
  char  *Video;


 public:

  void Get      (short x, short y, short w, short h,char *buf);
  void Put      (short x, short y, short w, short h,char *buf);

  void Move     (short x, short y, short w, short h, short x1, short y1){
                 Get(x,y,w,h,Clone);
                 Put(x1,y1,w,h,Clone);
  };

  void Fill     (short x, short y, short w, short h, char ch, char attr);
  void ClearScr (char attr=0x07){ Fill(0,0,Raw+1,Col+1,' ',attr); };
  void Lock     (void);
  void Paste    (void);
  void ScrollUp (short x, short y, short w, short h, short lines, char attr);
  void ScrollDn (short x, short y, short w, short h, short lines, char attr);
  void Bar      (short x, short y, short w, short h, short type,char attr);

  void Write    (short x, short y, short w, short h, char *buf);
  void AWrite   (short x, short y, short w, short h, char *buf, char attr);
  void Write    (short x, short y, short w,  char *buf);
  void AWrite   (short x, short y, short w,  char *buf, char attr);
  void Write    (short x, short y, char ch);
  void AWrite   (short x, short y, char ch, char attr);

  void Putc     (char ch);
  void Putc     (unsigned char x, unsigned char y, char ch);
  void Goto     (unsigned char x, unsigned char y);
  void SetCursor(unsigned char OnOff);

  _Screen(int _Col, int _Raw) ;
  ~_Screen();
};

#endif
