/****************************************************
 *   LOWPRINT.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include <stdio.h>
#include <io.h>
#include <sys\stat.h>
#include <fcntl.h>
#include <dos.h>
#include <string.h>
#include <bios.h>

#define NO_TRACE
#include "common.h"
#undef NO_TRACE

#define PRN_BUF 1024
int LPT=0;

#define PRN_TIMEOUT     0x01
#define PRN_ERROR       0x08
#define PRN_SELECT      0x10
#define PRN_NO_PAPER    0x20
#define PRN_ACK         0x40
#define PRN_BUSY        0x80

#define NO_TEST 0



/*
  file format:
  long - position of index table
	int  - pages in doc
	int - lines per page
	[
	int - line len   (if > 0 print at next line)
	char - text style
	]

	index:
	[
  int   - lines on previous page
	long  - begin of page
	] * pages
*/

_LowPrint::_LowPrint(int _LinesPerPage, char *name){
 LinesPerPage=_LinesPerPage;
 lines=0;
 pages=0;
 pos=0;
 footerCnt=0;
 HPrint=0;
 FPrint=0;
 heading=0;
 d.Today();
 t.Now();
 char * n;
 cnt=0;
 while(++cnt!=1000){
   //Debug("Make temp file name (prn) \n");
	 n=MakeTemp("PRN");
   f=open(n,O_WRONLY|O_BINARY|O_CREAT|O_TRUNC,S_IWRITE);
   if(f!=-1){
     if(name) strcpy(name,n);
     break;
   }
 }
 if(cnt==1000){
    MessageBox("У нас проблеммы с созданием файла печати...",
               "Попробуйте изменить значение в строке FILES=...",
               "в фале config.sys, на большее");
    return;
 }
 cnt=0;
 while(++cnt!=1000){
   //Debug("Make temp file name (pix) \n") ;

  n=MakeTemp("PIX");
  idx=open(n,O_RDWR|O_BINARY|O_CREAT|O_TRUNC,S_IWRITE);
  if(idx!=-1) break;
 };
 if(cnt==1000){
    MessageBox("У нас проблеммы с созданием файла печати...",
               "Попробуйте изменить значение в строке FILES=...",
               "в фале config.sys, на большее");
    return;
 }
 strcpy(idxname,n);
 //Debug("index %s\n",idxname);
 write(f,&pos,4);
 write(f,&pages,2);
 write(f,&LinesPerPage,2);
 buffer = new char [PRN_BUF];
 cnt = 0;
};


_LowPrint::~_LowPrint(void){
 if(f==-1 || idx==-1)
   return;
 if(lines)
   EndPage();
 lseek(f,0,SEEK_SET);
 write(f,&pos,4);
 write(f,&pages,2);
 lseek(f,0,SEEK_END);
 long l=filelength(idx);

 int s;
 lseek(idx,0,SEEK_SET);
 if(!l){
   write(idx,"no idx",6);
 }else
   while(l){
     if(l>PRN_BUF) s=PRN_BUF; else s=l;
     read (idx,buffer,s);
     write(f  ,buffer,s);
     l-=s;
   };
 close(f);
 close(idx);
 //Debug("unlink %s\n",idxname);
 unlink(idxname);
 delete buffer;
};

void _LowPrint::EndPage(void){
 if(f==-1 || idx==-1)
   return;
  if(lines){
    PrintFooter();
    if(cnt){
      write(f,buffer,cnt);
      cnt=0;
    }
    pages++;
    pos=filelength(f);
    write(idx,&lines,2);
    write(idx,&pos,4);
    lines=0;
  }
};

void _LowPrint::PrintNL(char *p, int style){
 if(f==-1 || idx==-1)
   return;
  if(!lines && !heading) PrintHeader();
  int l=strlen(p);
  l=-l;
  if(cnt-l+4 < PRN_BUF){
    memcpy(buffer+cnt,&l,2);
    cnt+=2;
    memcpy(buffer+cnt,&style,2);
    cnt+=2;
    memcpy(buffer+cnt,p,-l);
    cnt-=l;
  }else{
    write(f,buffer,cnt);
    cnt=0;
    if(-l+4<PRN_BUF){
      memcpy(buffer+cnt,&l,2);
      cnt+=2;
      memcpy(buffer+cnt,&style,2);
      cnt+=2;
      memcpy(buffer+cnt,p,-l);
      cnt-=l;
    }else{
      write(f,&l,2);
      write(f,&style,2);
      write(f,p,strlen(p));
    }
  }
  lines++;
  if(lines+footerCnt==LinesPerPage ) EndPage();

};

void _LowPrint::Print(char *p, int style){
 if(f==-1 || idx==-1)
   return;
  if(!lines && !heading) PrintHeader();
  int l=strlen(p);
  if(cnt+l+4 < PRN_BUF){
    memcpy(buffer+cnt,&l,2);
    cnt+=2;
    memcpy(buffer+cnt,&style,2);
    cnt+=2;
    memcpy(buffer+cnt,p,l);
    cnt+=l;
  }else{
    write(f,buffer,cnt);
    cnt=0;
    if(l+4<PRN_BUF){
      memcpy(buffer+cnt,&l,2);
      cnt+=2;
      memcpy(buffer+cnt,&style,2);
      cnt+=2;
      memcpy(buffer+cnt,p,l);
      cnt+=l;
    }else{
      write(f,&l,2);
      write(f,&style,2);
      write(f,p,strlen(p));
    }
  }
};

void _LowPrint::PrintHeader(void){
 if(f==-1 || idx==-1)
   return;
 if(heading) return;
 heading =1;
 if(HPrint)
  HPrint(this,pages+1);
 heading=0;
}

void _LowPrint::PrintFooter(void){
 if(f==-1 || idx==-1)
   return;
 if(heading) return;
 heading=1;
 if(FPrint)
  FPrint(this,pages+1);
 heading=0;
}

void _LowPrint::EndPage(void (*procH)(_LowPrint * ,int), int cntF,  void (*procF)(_LowPrint * ,int)){
 if(f==-1 || idx==-1)
   return;
  EndPage();
  SetHeader(procH);
  if(cntF)
  SetFooter(cntF,procF);
};


static short first_page,last_page,ptype=0,cur_page=0;
static unsigned char formfeed =1;
static unsigned char condenced=1;
static short repeate;

static void Printing(_Event *E, void *parent);

static void PrintThePage(_Event *E, void *parent){
   // нет бумаги или
   int res=biosprint(2,0,LPT);
   if((res & PRN_TIMEOUT) || (res & PRN_NO_PAPER) || (res & PRN_ERROR) ){
     sound(440);
     delay(25);
     nosound();
     Debug("Printrer init result = %X\n",res);

#ifdef NO_TEST
     return;
#endif

   }

  _ShowPrint *sp =(_ShowPrint*) ((_Window *)parent)->parent;
  _Window *ww = (_Window *)parent;
  if(ww->child->array[2])
    ww->Del(ww->child->array[2]);
  ww->Add(new _Static(1,2,2,"Производится печать ..."));
  ww->Show();
  if(sp->Print(cur_page,cur_page)){

    MessageBox(
    "      Печать текущей страници завершилась неудачно !",
    "Возможно, кончилась бумага, или принтер не в режиме OnLine.",
    "Вы можете повторить печать, начиная с текущей страници.",
    "             Нажмите Esc для продолжения..."
    );
    if(ww->child->array[2])
      ww->Del(ww->child->array[2]);
    ww->Add(new _Static(1,2,2,"Вставте бумагу и нажмите ENTER"));
    ww->Show();

  }else{
    if(cur_page <last_page){
      ww->child->array[1]->Commit();
      cur_page++;
      ww->child->array[1]->Load();
      if(ww->child->array[2])
        ww->Del(ww->child->array[2]);
      ww->Add(new _Static(1,2,2,"Вставте бумагу и нажмите ENTER"));
      ww->Show();
    }else{
      E->msg.K = ESC;
      return;
    }
    if(cur_page <=last_page){

      // если печать идет на рулон симулируем нажатие ENTER
      if(!formfeed )
        GDt->Q.AddMessage(evKey,ENTER);
    }
  }
  E->ClearEvent(ww);
};

static void Printing(_Event *E, void *parent){

   if(!cur_page)
      biosprint(1,0,LPT);

   int res=biosprint(2,0,LPT);
   if(!(res & PRN_SELECT) || (res & PRN_TIMEOUT) || (res & PRN_ERROR) ){
      Debug("Printrer init result = %X\n",res);
      sound(440);
      delay(25);
      nosound();
#if NO_TEST
      return;
#endif
   }

  _ShowPrint *sp =(_ShowPrint*) ((_Window *)parent)->parent;
  ((_Window*)parent)->LostFocus();
  if(first_page >0 && last_page >0 && first_page <=last_page ){
    _Window *w= new _Window(1,1,35,4,ST_WINDOW,stFrame,"Printing");
    sp->Add(w);
    _Window *w1;

    if(!cur_page){
      cur_page=first_page;
      if(last_page>sp->pages)
        last_page=sp->pages;
    }

    w1=new _Static(1,1,ST_WINDOW,"Печать страници :");  w->Add(w1);
    w1=new _ShortCell(20,1,ST_WINDOW,4,&cur_page,0);
    w1->st |= stLocked;
    w->Add(w1);
    w1=new _Static(1,2,ST_WINDOW,"Вставте бумагу и нажмите ENTER");  w->Add(w1);
    w->AddCallBack(PrintThePage ,evKey,ENTER);
    w->GotFocus();
  }

};

static void PrintCard(_Event *E, void *parent){
  _ShowPrint *sp =(_ShowPrint*) parent;
  _Window *w= new _Window(1,1,35,5,ST_WINDOW,stFrame,"Print Card");
  sp->Add(w);
  _Window *w1;
  first_page=1;
  cur_page  =0;
  last_page=sp->pages;
  w1=new _Static(1,1,ST_WINDOW,"Печатать страници ОТ:");  w->Add(w1);
  w1=new _ShortCell(23,1,ST_WINDOW,4,&first_page,0);     w->Add(w1);
  w1=new _Static(1,2,ST_WINDOW,"                  ДО:");  w->Add(w1);
  w1=new _ShortCell(23,2,ST_WINDOW,4,&last_page,0);     w->Add(w1);
  w1=new _Static(1,3,ST_WINDOW,"Уплотненная печать  :");  w->Add(w1);
  w1=new _YNCell (23,3,ST_WINDOW,(unsigned char*)&condenced);     w->Add(w1);
  w1=new _Static(1,4,ST_WINDOW,"Листовая печать     :");  w->Add(w1);
  w1=new _YNCell (23,4,ST_WINDOW,(unsigned char*)&formfeed);     w->Add(w1);
  w->AddCallBack(Printing,evKey,ENTER);
  w->GotFocus();
  E->ClearEvent(sp);
};

static void DelFile(_Event *E, void *parent){
  _ShowPrint *sp =(_ShowPrint*) parent;
  int res = MessageBox(
              "Закончить работу с документом ?",
              "  Да - [Enter]   Нет - [Esc]   ");
  if(!res)
    E->ClearEvent(sp);
}

_ShowPrint::_ShowPrint(char *name, char * w_name):
  _Window(1,1,78,23,ST_SHOWPRINT,stBuffered | stFrame ,w_name)
{
  if(access(name,0)){
    hdl = -1;
    return;
  }
  min_w=10;
  min_h=8;
  strcpy(fname,name);
  hdl=open(name,O_RDONLY|O_BINARY);
  read(hdl,&pos,4);
  read(hdl,&pages,2);
  read(hdl,&LinesPerPage,2);
  cur=8;
  curpage=1;
  first=1;
  idx= new INDEX[pages+1];
  lseek(hdl,pos,SEEK_SET);
  read(hdl,&idx[1],pages*6);
  lseek(hdl,cur,SEEK_SET);
  total=0;
  idx[0].lines=0;
  idx[0].pos=8;
  for(int i=0;i <= pages;i++)
    total+=idx[i].lines;
  X=0;
  AddCallBack(PrintCard ,evKey,ENTER);
  AddCallBack(DelFile ,evKey,ESC);
};

_ShowPrint::~_ShowPrint(void){
  if(hdl !=-1){
    close(hdl);
    delete idx;
    if(fname[0])
      unlink(fname);
  }
};

int _ShowPrint::GetLine(char *buf){
  if(hdl ==-1) return 0;
  StrClear(buf,200);
  int l,style,l1;
  int cpos;

  if(cur<pos){
    cpos=0;
    do{
      if(read(hdl,&l,2)==-1)
        return 0;
      cur+=2;
      if(read(hdl,&style,2)==-1)
        return 0;
      cur+=2;
      if(l<0)
        l1=-l;
      else
        l1=l;
      if(cpos<200)
        if(cpos+l1<200){
            read(hdl,buf+cpos,l1);
            cpos+=l1;
        }else
          lseek(hdl,l1,SEEK_CUR);
      cur+=l1;
    }while(l>0  && cur<pos);
  }
  return cpos;
};

void _ShowPrint::SkipTo(int page){
  if(hdl ==-1) return;
  first=1;
  for(int i=0; i < page ;i++){
    first+=idx[i].lines;
  }
};

void _ShowPrint::Paint(void){
  if(hdl ==-1) return;
  char buf[201];
  _Window::Paint();
  short dx=0,dy=0;
  Coord(&dx,&dy);

  GDt->S.AWrite(dx+w-9,dy  ,'',Style2Color(style,1));
  GDt->S.AWrite(dx+w-3,dy  ,'',Style2Color(style,1));
  GDt->S.AWrite(dx+w-7,dy  ,27,Style2Color(style,1));
  GDt->S.AWrite(dx+w-5,dy  ,26,Style2Color(style,1));
  GDt->S.AWrite(dx+w-1,dy+2,'',Style2Color(style,1));
  GDt->S.AWrite(dx+w-1,dy+4,'',Style2Color(style,1));
  GDt->S.AWrite(dx+w-1,dy+6,'',Style2Color(style,1));
  GDt->S.AWrite(dx+w-1,dy+8,'',Style2Color(style,1));


  int cc;
  LineConvert(first, &curpage, &cc);
  if(lseek(hdl,idx[curpage-1].pos,SEEK_SET)==-1)
    return ;
  cur=idx[curpage-1].pos;
  for(int i=1;i<cc;i++)
    GetLine(buf);
  for(i=0;i<h-2;i++){
    GetLine(buf);
    GDt->S.AWrite(dx+1,dy+i+1,w-2,buf+X,Style2Color(style,1));
  };
};

void _ShowPrint::LineConvert(long curline, int *page, int *line){
  if(hdl ==-1) return;
  long s=curline;
  for(int i=0; i<=pages;i++){
    if(s<=idx[i].lines){
      *page=i;
      *line=s;
      return;
    }
    s-=idx[i].lines;
  }
};


static int WPrint(int ch){
  long cnt=0;
  int mode;
  do{
    cnt =0;
    while(!((mode=biosprint(2,0,LPT)) & PRN_BUSY) && cnt < 100000l) cnt++;
    if(cnt==100000l){
      Debug("wprint mode %x\n",mode);
      if(!MessageBox("Случилась какая-то беда с принтером",
                     "Попробуем еще       - [Enter]",
                     "или закончим печать - [Esc]"
                    )
        )
        return 1;
    }
  }while(cnt == 100000l);
  biosprint(0,ch,LPT);
  return 0;
};

int  _ShowPrint::Print(int page1, int page2){
  //Debug("page1=%d page2=%d (pages=%d)\n",page1,page2,pages);
  if(hdl ==-1) return 1;
  if(page1 > pages)
    return 1;
  int page_for_print,oldpage;
  int line_for_print;
  long old=first;
  char buf[200];
  int s,i;
  if(WPrint(27))  return 1;
  if(WPrint('@')) return 1;
  if(condenced){
    if(WPrint(15)) return 1;
    if(WPrint(27))  return 1;
    if(WPrint(15)) return 1;
  }
  SkipTo(page1);
  LineConvert(first, &page_for_print,&line_for_print);
  lseek(hdl,idx[page_for_print-1].pos,SEEK_SET);
  cur=idx[page_for_print-1].pos;
  oldpage=page1;
  do{
    s=GetLine(buf);
    for(i=0; i<s;i++)
      if(WPrint(buf[i])) return 1;
    if(WPrint(0x0a)) return 1;
    if(WPrint(0x0d)) return 1;
    first++;
    if(first > total) break;
    LineConvert(first,&page_for_print,&line_for_print);
    if(oldpage!=page_for_print){
      if(!formfeed){
        if(WPrint(0x0a)) return 1;
        if(WPrint(0x0d)) return 1;
        if(WPrint(0x0a)) return 1;
        if(WPrint(0x0d)) return 1;
      }else{
        if(WPrint(12)) return 1;
      }
      oldpage=page_for_print;
    }
  }while(page_for_print<=page2);
  first=old;
  return 0;
}

int _ShowPrint::Handle(_Event *E){
    TRACE("ShowPrint::Handle");
    if(hdl ==-1 ){
      if(E->Type==evKey|| E->msg.K !=ESC)
        E->msg.K=ESC;
      else
        E->ClearEvent(this);
    }

    int old=0,oldx,flag=0;
    old=first;
    oldx=X;
    if(E->Type==evKey)
      switch(E->msg.K){
        case SHIFT_DOWN:
          if(curpage<pages)
            SkipTo(curpage+1);
          flag =1;
          break;
       case SHIFT_UP:
          if(curpage>1)
            SkipTo(curpage-1);
          flag =1;
          break;
       case DOWN:
          if(first<total) first++;
          flag =1;
          break;
       case UP:
          if(first >1) first--;
          flag =1;
          break;
       case PGDN:
          if(first+h-3<total) first+=h-3;
          flag =1;
          break;
       case PGUP:
          if(first>h-3) first-=h-3; else first=1;
          flag =1;
          break;
       case CTRL_PGUP:
          first=1;
          flag =1;
          break;
       case CTRL_PGDN:
          first=total;
          flag =1;
          break;
       case LEFT:
          if(X>0) X--;
          flag =1;
          break;
       case RIGHT:
          if(X < 200-w) X++;
          flag =1;
          break;
       case SHIFT_LEFT:
          if(X>10) X-=10;
          else X=0;
          flag =1;
          break;
       case SHIFT_RIGHT:
          if(X<190-w) X+=10;
          else X=200-w;
          flag =1;
          break;
       case END:
          X=200-w;
          flag =1;
          break;
       case HOME:
          X=0;
          flag =1;
          break;
      };

    if(flag){
      E->ClearEvent(this);
      if(old!=first || oldx !=X){
        Show();
      }
    }
    return _Window::Handle(E);
};

int _ShowPrint::MHandle(_Event *E){
  if(hdl ==-1) return 0;
  if(E->Type==evMouse && st&stFocused){
    short dx=0,dy=0;
    Coord(&dx,&dy);

    if( E->msg.M.buttons & 1 && !FindFocused()){
      if(E->msg.M.y==dy)
        switch(E->msg.M.x-dx-w){
          case -7:
            GDt->Q.AddMessage(evKey,LEFT);
            E->ClearEvent(this);
            return 0;
          case -5:
            GDt->Q.AddMessage(evKey,RIGHT);
            E->ClearEvent(this);
            return 0;
          case -9:
            GDt->Q.AddMessage(evKey,SHIFT_LEFT);
            E->ClearEvent(this);
            return 0;
          case -3:
            GDt->Q.AddMessage(evKey,SHIFT_RIGHT);
            E->ClearEvent(this);
            return 0;

        };

      if(E->msg.M.x==dx+w-1)
        switch(E->msg.M.y-dy){
          case 2:
            GDt->Q.AddMessage(evKey,PGUP);
            E->ClearEvent(this);
            return 0;
          case 4:
            GDt->Q.AddMessage(evKey,UP);
            E->ClearEvent(this);
            return 0;
          case 6:
            GDt->Q.AddMessage(evKey,DOWN);
            E->ClearEvent(this);
            return 0;
          case 8:
            GDt->Q.AddMessage(evKey,PGDN);
            E->ClearEvent(this);
            return 0;
        }
    }
  }
  return _Window::MHandle(E);
};

void _ShowPrint::MoveOrGrawDone(short mode){
  switch(mode){
    case sysGrawBuf:
      if(h < 10)
        h=10;
      Show(-1);
      GDt->S.Paste();
      return ;
    case sysGraw:
      if(h < 10)
        h=10;
      ShowBase->Show(1);
      return ;
  }
  _Window::MoveOrGrawDone(mode);
};

