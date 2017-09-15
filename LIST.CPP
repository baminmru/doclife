/****************************************************
 *       LIST.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include <dos.h>
#include <dir.h>
#define NO_TRACE
#include "common.h"
#undef NO_TRACE


ListItem * ListItems::Last(void){
  ListItem *tmp =top;
  while(tmp && tmp->next){
    tmp=tmp->next;
  }
  return tmp;
};

void ListItems::SortByIndex(void){
  ListItem *tmp ;
  ListItem *cur = top;
  ListItem buf;
  while(cur){
    tmp=cur->next;
    while(tmp){
      if(cur->idx < tmp->idx){
        buf.text=tmp->text;
        buf.idx=tmp->idx;
        tmp->text=cur->text;
        tmp->idx=cur->idx;
        cur->text=buf.text;
        cur->idx=buf.idx;
      }
      tmp=tmp->next;
    }
    cur=cur->next;
  };
};
void ListItems::SortByText(void){
  ListItem *tmp = top;
  ListItem *cur = top;
  ListItem buf;
  while(cur){
    tmp=cur->next;
    while(tmp){
      if( strcmp(cur->text,tmp->text)>0){
        buf.text=tmp->text;
        buf.idx=tmp->idx;
        tmp->text=cur->text;
        tmp->idx=cur->idx;
        cur->text=buf.text;
        cur->idx=buf.idx;
      }
      tmp=tmp->next;
    }
    cur=cur->next;

  }while(cur);
};

void ListItems::AddItem(char *text, long idx){
  ListItem *tmp = Last();
  if(!tmp){
    top=new ListItem;
    top->text=new char [strlen(text)+1];
    strcpy(top->text,text);
    top->idx=idx;
    top->next=0;
    top->prev=0;
  }else{
    tmp->next=new ListItem;
    tmp->next->prev=tmp;
    tmp=tmp->next;
    tmp->text=new char [strlen(text)+1];
    strcpy(tmp->text,text);
    tmp->idx=idx;
    tmp->next=0;
  }
  switch(sort){
    case 1:
     SortByIndex();
     break;
    case 2:
     SortByText();
     break;
  }
};

void ListItems::DelItem(char *text){
    ListItem * tmp=cur;
    JumpTo(text);
    delete cur->text;
    cur->prev->next=cur->next;
    cur->next->prev=cur->prev;
    delete cur;
    cur=tmp;
};
void ListItems::DelItem(long idx){
    ListItem * tmp=cur;
    JumpTo(idx);
    delete cur->text;
    cur->prev->next=cur->next;
    cur->next->prev=cur->prev;
    delete cur;
    cur=tmp;
};

void ListItems::Next( short is_down){
  if(is_down){
    if(!cur->next)
      cur=top;
    else
      cur=cur->next;
  }else{
    if(!cur->prev)
      cur=Last();
    else
      cur=cur->prev;
  }
};
void ListItems::JumpTo(char * text){
  ListItem *tmp =top;
  while(tmp && !strcmp(tmp->text,text)){
    tmp=tmp->next;
  }
  if(tmp)
    cur=tmp;
  else
    cur=top;
};
void ListItems::JumpTo(long idx){
  ListItem *tmp =top;
  while(tmp && tmp->idx != idx){
    tmp=tmp->next;
  }
  if(tmp)
    cur=tmp;
  else
    cur=top;
};

ListItem * ListItems::GetItem(char *text){
  ListItem *tmp =top;
  while(tmp && !strcmp(tmp->text,text)){
    tmp=tmp->next;
  }
  return tmp;
};
ListItem * ListItems::GetItem(long idx){
  ListItem *tmp =top;
  while(tmp && tmp->idx!=idx){
    tmp=tmp->next;
  }
  return tmp;
};
ListItems::ListItems (ListItem * _top, short _sort){
  top=_top;
  sort=_sort;
};

ListItems::~ListItems(){
  ListItem *tmp=top;
  while(tmp){
    tmp=tmp->next;
    delete top->text;
    delete top;
    top=tmp;
  }
};

void _ListBox::Load(void){
  lbi.JumpTo(*info);
};

int _ListBox::Handle(_Event *E){
  TRACE("ListBox::Handle");
  if(!(st&stLocked)){
    // обработка событий клавиатуры
    if(E->Type==evKey){
      switch(E->msg.K){
        case ' ':
        case RIGHT:
          lbi.Next(1);
          Commit();
          E->ClearEvent(this);
          break;

        case LEFT:
          lbi.Next(0);
          Commit();
          E->ClearEvent(this);
          break;

        case END:
          lbi.cur=lbi.top;
          lbi.Next(0);
          Commit();
          E->ClearEvent(this);
          break;

        case HOME:
          lbi.cur=lbi.top;
          Commit();
          E->ClearEvent(this);
          break;

        case PGDN:
        case ALT_DOWN:
            {
              short dx=0,dy=0;
              Coord(&dx,&dy);

              _ListView *lv = new _ListView(dx,dy+1,w,h2-1,ST_LISTWINDOW,&lbi);
              lv->Exec();
              Commit();
              E->ClearEvent(this);
            }
          break;

        default:
          ;
      }
    }

  }
  return _Window::Handle(E);
};

void _ListBox::Commit(void){
    if(lbi.cur)
     *info=lbi.cur->idx;
    else
     *info=0;

    Show(1);
    _Window::Commit();
};

int _ListBox::MHandle(_Event *E){
  if(E->Type==evMouse){
    short dx=0,dy=0;
    Coord(&dx,&dy);
    if(E->msg.M.x==dx+w-1 && E->msg.M.y==dy+h-1){
      if(E->msg.M.buttons & 1 && st & stFocused){
        GDt->Q.AddMessage(evKey,ALT_DOWN);
        E->ClearEvent(this);
        return 0;
      }
    }
  }
  return _Window::MHandle(E);
};

void _ListBox::Paint(void){
  if(!lbi.cur || *info!=lbi.cur->idx)
    lbi.JumpTo(*info);
  TRACE("ListBox::Paint");
  short dx=0,dy=0;
  Coord(&dx,&dy);
  char * buf = new char[w];
  StrClear(buf,w-1);
  if(lbi.cur)
    strncpy(buf,lbi.cur->text,w-1);
  GDt->S.AWrite(dx,dy,w-1,1,buf,Style2Color(style,(st&stFocused)?2:1));
  delete buf;
  GDt->S.AWrite(dx+w-1,dy+h-1,1,1,"",Style2Color(style,1));
  *info=lbi.cur->idx;
};


_ListBox::_ListBox(short _x,short _y,short _w,short _h, short _style,ListItem *_lbi, long * _info):
  _Window(_x,_y,_w,1,_style,stNoMove|stNoESC,"ListBox"),
  info(_info),
  h2(_h)
  {
    lbi.top  =_lbi;
    lbi.sort = 1;
    lbi.cur = 0;
    TRACE("ListBox::ListBox");
   };

_ListBox::~_ListBox(){
  TRACE("ListBox::~ListBox");
};


void _ListView::Paint(void){
  _Window::Paint();
  ListItem *tmp = first;
  char * buf = new char[w+1];
  short dx=0,dy=0;
  Coord(&dx,&dy);
  short cnt=1;
  while(cnt < h-1){
    StrClear(buf,w);
    if(tmp) {
      strncpy(buf,tmp->text,w);
      if(tmp==lbi->cur) ActivLine=cnt;
      GDt->S.AWrite(dx+1,dy+cnt,w-2,1,buf,Style2Color(style,(tmp==lbi->cur)?2:1));
      tmp=tmp->next;
    }else
      GDt->S.AWrite(dx+1,dy+cnt,w-2,1,buf,Style2Color(style,1));

    cnt++;
  }
  delete buf;
  GDt->S.AWrite(dx+w-1,dy+1,1,1,"",Style2Color(style,1));
  GDt->S.AWrite(dx+w-1,dy+h-2,1,1,"",Style2Color(style,1));
};

int _ListView::Handle(_Event *E){
  if(E->Type==evKey){
    switch(E->msg.K){
      case ENTER:

        break;

      case ESC:
        lbi->cur=oldcur;
        break;

      case HOME:
        lbi->cur=lbi->top;
        first=lbi->top;
        E->ClearEvent(this);
        break;

      case END:
        lbi->cur=lbi->top;
        lbi->Next(0);
        MakeFirst();
        E->ClearEvent(this);
        break;

      case DOWN:
        //lbi->Next(1);
        if(lbi->cur->next)
          lbi->cur=lbi->cur->next;
        MakeFirst();
        E->ClearEvent(this);
        break;

      case UP:
        //lbi->Next(0);
        if(lbi->cur->prev)
          lbi->cur=lbi->cur->prev;
        MakeFirst();
        E->ClearEvent(this);
        break;

    }
    if(!E->Type)
      Show(1);
  }
  return _Window::Handle(E);
};

void _ListView::MakeFirst(void){
  ListItem *pf = first;

  // проверим что видна выделенная позиция
  while(first){
    ListItem * tmp = first;
    for(int i=1 ;i <h-1 ;i++){
      if(tmp==lbi->cur)
        return;
      tmp=tmp->next;
    }
    first=first->next;
  }

  first=pf;

  while(first && first!=lbi->cur){
    first=first->prev;

  }
  if(!first)
    first=lbi->top;
};

_ListView::_ListView(short _x,short _y,short _w,short _h, short _style,ListItems *items):
  _Window(_x,_y,(_w<3)?3:_w,(_h<4)?4:_h,_style,stBuffered,"ListView")
{
  lbi=items;
  oldcur=lbi->cur;
  first=lbi->top;
  MakeFirst();
};

_ListView::~_ListView(){
};

int _ListView::MHandle(_Event *E){
  if(E->Type==evMouse && st &stFocused){
    short dx=0,dy=0;
    Coord(&dx,&dy);
    if(E->msg.M.buttons & 1 ){
      if(E->msg.M.x==dx+w-1){
        if(E->msg.M.y==dy+1){
          GDt->Q.AddMessage(evKey,UP);
          E->ClearEvent(this);
          return 0;
        }
        if(E->msg.M.y==dy+h-2){
          GDt->Q.AddMessage(evKey,DOWN);
          E->ClearEvent(this);
          return 0;
        }
      }

      if(dx<E->msg.M.x && E->msg.M.x<dx+w-1 && dy<E->msg.M.y && E->msg.M.y<dy+h-1){
        if(E->msg.M.y-dy == ActivLine){
          GDt->Q.AddMessage(evKey,ENTER);
          E->ClearEvent(this);
          return 0;
        }else{
          if(E->msg.M.y-dy < h){
            ActivLine=E->msg.M.y-dy;
            ListItem *tmp=first;
            lbi->cur=first;
            for(int i=1; i<ActivLine;i++)
              if(lbi->cur->next)
                lbi->cur=lbi->cur->next;
            Show(1);
            E->ClearEvent(this);
            return 0;
          }
        }
      }

    }

  }
  return _Window::MHandle(E);
};


ListItems * MakeFileList(char * proto, char * dirname){
  ListItems *l = new ListItems(0,2);

  if(dirname)
    chdir(dirname);

  struct ffblk ffblk;
  int done;

  long idx=0;

  done = findfirst(proto,&ffblk,0);

  while (!done)
  {
    if(ffblk.ff_attrib & FA_DIREC)
      goto next_file;

    if(ffblk.ff_attrib & FA_LABEL )
      goto next_file;

    l->AddItem( ffblk.ff_name,++idx);

   next_file:

    done = findnext(&ffblk);
  }
  return l;
}

ListItems * MakeDirList(char * dirname){

  ListItems *l = new ListItems(0,2);

  if(dirname)
    chdir(dirname);

  struct ffblk ffblk;
  int done;

  long idx=0;

  done = findfirst("*.*",&ffblk,FA_DIREC);

  while (!done)
  {
    if(!(ffblk.ff_attrib & FA_DIREC))
      goto next_file;

    if(strcmp(ffblk.ff_name,"."))
      l->AddItem( ffblk.ff_name,++idx);

  next_file:

    done = findnext(&ffblk);
  }
  return l;
}


ListItems * MakeDriveList(void){
  ListItems *l = new ListItems(0,2);
  int curdrv=getdisk();
  char bb[2];
  long idx=0;
  bb[1]=0;
  for(int i=0;i<26;i++){
    setdisk(i);
    if(getdisk()==i){
      bb[0]='A'+i;
      l->AddItem(bb,++idx);
    }else
      break;
  }
  setdisk(curdrv);
  return l;
};

