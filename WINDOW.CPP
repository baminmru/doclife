/****************************************************
 *     WINDOW.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include <stdarg.h>
#include <malloc.h>
#include <bios.h>
#define NO_TRACE
#include "common.h"
#undef NO_TRACE

extern _Window *ShowBase;
extern short HelpLock;
short ShutDown=0;
_Window * _Window::Zord [512];
short _Window::MaxZ=0;

CallBack::~CallBack(){
  TRACE("CallBack::~CallBack");
};


_WinList::_WinList(void){
  TRACE("WinList::WinList");
  index=total=0;
  array=0;

};

_WinList::~_WinList(void){
  TRACE("WinList::~WinList");
  if(array)
    delete array;
};


int _WinList::Del( _Window *it){
  if(!array || !total)
    return 0;
  _Window ** newarr=0;
  int cnt=0;
  for(int i=0;i<total;i++){
    if(array [i]==it){
      delete array[i];
      if(total>1){
        newarr = new _Window*[total-1];
        cnt=0;
        for(int j=0;j<total;j++)
          if(i!=j)
            newarr[cnt++]=array[j];
        delete array;
        array=newarr;
        total--;
        index=index%total;
      }else{
        delete array;
        array=0;
        index=total=0;
      }
    }
  }
  return 0;
}


int  _WinList::Find( _Window *it){
  TRACE("WinList::Find");
  for(int i=0;i<total;i++){
    if(array[i]==it){
      return i+1;
    }
  };
  return 0;
};

_Window * _WinList::FindWin(char *name){
  TRACE("WinList::FindWin");
  for(int i=0;i<total;i++){
    if(!strcmp(array[i]->name,name)){
      return array[i];
    }
  };
  return 0;
};

void _WinList::Add( _Window *it){
  TRACE("WinList::Add");
  if(!total){
    total=1;
    index=0;
    array = new _Window* [1];
    array[0]=it;
  }else{
    _Window** newarr=new _Window* [total+1];
    memcpy(newarr,array,sizeof(_Window**)*total);
    newarr[total++]=it;
    delete array;
    array=newarr;
  }
};


void _Window::Add(_Window* NewWindow){
  TRACE("Window::Add");
  child->Add(NewWindow);
  NewWindow->parent=this;
  CheckSize();
};

void _Window::Del(_Window *OldWindow){
  TRACE("Window::Del");
  child->Del(OldWindow);
};

_Window * _Window::Find(char *name){
  TRACE("Window::Find");
  return child->FindWin(name);
};

_Window::_Window(short _x,short _y,short _w,short _h, short _style, short _Status, char *_name){
    TRACE("Window::Window");
    x=_x;
    y=_y;
    w=_w;
    h=_h;
    style=_style;
    parent=0;
    st=_Status;
    child=new _WinList();
    name=_name;
    if(st&stFrame)
      LoadSize();
    F=0;
    ExecBuf=0;
    HelpID=0;
    WBuffer = 0;
    NotOnScreen = 1;
    min_w=0;
    min_h=0;
    Changed=Befor=After=0;
};


static char tree[20];
void _Window::dbg(short start){
  if(start){
    tree[0]=0;
    Debug("Coreleft =%ld\n",coreleft());
  }
  _Window *tmp;
  Debug("%sWinList----------------------------\n",tree);
  for(int i=0;i<child->total;i++){
    Debug("%sW=%p ",tree,child->array[i]);
    tmp=child->array[i];
    if(tmp){
      Debug(" name =%s ", tmp->name);
      Debug ("X=%d Y =%d W=%d H =%d \n",tmp->x,tmp->y,tmp->w,tmp->h);
      Debug("%sst=",tree);
      if(tmp->st &stFocused)   Debug("Focused ");
      if(tmp->st &stBuffered)  Debug("Buffered (%p) ",tmp->WBuffer);
      if(tmp->st &stHide)      Debug("Hide ");
      if(tmp->st &stCursor)    Debug("Cursor ");
      if(tmp->st &stLocked)    Debug("Locked ");
      if(tmp->st &stNoMove)    Debug("NoMove ");
      if(tmp->st &stNoESC)     Debug("NoESC ");
      if(tmp->st &stFrame)     Debug("Frame ");
      Debug("\n");

      strcat(tree,"\t");
      tmp->dbg(0);
      tree[strlen(tree)-1]=0;
    }else
      Debug("\n");
  }
  Debug("%s-----------------------------------\n",tree);
};


int _Window::CanLostFocus(void){
  TRACE("Window::CanLostFocus");
  return 1;
};

int _Window::CanClose(void){
  TRACE("Window::CanClose");
  short s=1;
  for(int i=0;i<child->total;i++){
      s*=child->array[i]->CanClose();
  }
  return s;
};

int _Window::CanSwitch(void){
  TRACE("Window::CanSwitch");
  short s=1;
  for(int i=0;i<child->total;i++){
      s*=child->array[i]->CanSwitch();
  }
  return s;
};

void _Window::LostFocus(int Lock){
  TRACE("Window::LostFocus");
  Commit();
  if(After) After(this);
  st = st & (~stFocused);

  for(int i=0;i<child->total;i++){
    child->array[i]->LostFocus(0);
  }

  if(st & stBuffered && WBuffer){
    delete WBuffer;
    WBuffer =0;
  }
  if(Lock)
        Show(Lock);
};

void _Window::GotFocus(int Lock){
  TRACE("Window::GotFocus");
  ZInsert(this);
  Load();
  if(Befor) Befor(this);
  st = st | stFocused;
  if(Lock)
    Show(Lock);
};

void _Window::SwitchFocus(short Down,int Lock){
  TRACE("Window::SwitchFocus");

  _Window *tmp=0;
  int i;
  int delta;
  if(Down)
    delta=1;
  else
    delta=-1;

  if(child->total){
    tmp=child->array[(child->index)];

    // если возможна потеря фокуса
    if(tmp->st & stFocused){
      if( tmp->CanLostFocus())
      {
        tmp->LostFocus(Lock);
      }else
        return;
    }else{
      if( !(tmp->st & stLocked) &&
          !(tmp->st & stHide)){
          tmp->GotFocus(Lock);
          return;
      }
    }
    i=1;
    do{
      tmp=child->array[(child->index+i*delta+child->total)%child->total];
      if( !(tmp->st & stLocked) && !(tmp->st & stHide)){
          tmp->GotFocus(Lock);
          child->index=(child->index+i*delta+child->total)%child->total;
          break;
      }
      i++;
    }while(i<=child->total);
  }
};

void _Window::SwitchFocus(_Window * New, int Lock){
  TRACE("Window::SwitchFocus (W)");

  _Window *tmp=0;
  if(child->total && child->Find(New)){

    tmp=child->array[(child->index)];

    // если возможна потеря фокуса
    if(tmp->st & stFocused){
      if(  tmp->CanLostFocus())
      {
        tmp->LostFocus(Lock);
      }else
        return;
    }else{
      if( !(tmp->st & stLocked) && !(tmp->st & stHide)){
          tmp->GotFocus(Lock);
          return;
       }
    }

    tmp = child->array[child->Find(New)-1];
    //Debug("Got\n");
    tmp->GotFocus(Lock);
    child->index=child->Find(New)-1;
  }
};

_Window* _Window::FindFocused(void){
  TRACE("Window::FindFocused");

   if(!child->total) return 0;
  _Window *tmp;
  _Window *tmp2;
   // найти того кто в фокусе
  int i=0;
  do{
    tmp=child->array[(child->index+i)%child->total];
    if(tmp->st&stFocused && !(tmp->st&stLocked) && !(tmp->st&stHide) ){
      child->index=(child->index+i)%child->total;
      tmp2=tmp->FindFocused();
      if(tmp2)
        return tmp2;
      else
        return tmp;
    }
    i++;
  }while(i<child->total);
  return 0;
};



void _Window::MoveOrGrawDone(short mode){
  switch(mode){
    case sysGraw:
      {
        ShowBase->Show(1);
      }
      break;
    case sysMove:
      {
        ShowBase->Show(1);
      }
      break;

    case sysGrawBuf:
      {
        Show(-1);
        GDt->S.Paste();
      }
      break;

    case sysMoveBuf:
      {
        Show(-1);
        GDt->S.Paste();
      }
      break;
  }
};

int _Window::Handle(_Event * E){
  TRACE("Window::Handle");
  CallBack * tmp=FindCallBack(E->Type, E->msg.L);
  if(tmp && tmp->func){
    tmp->func(E,this);
  }

  // обработать свою часть сообщения и отдать папе
  switch(E->Type){
    case evSystem:
      switch(E->msg.L){
        case sysStop:
          E->ClearEvent(this);
          return 1;

        case sysHelp:
          if(HelpID){
            _Help::Build(HelpID);
            E->ClearEvent(this);
          }else
            HelpLock = 0;
          break;
        default:
          break;
      };
      break;
    case evMouse:
      {
        short res=0;
        for(short ii=MaxZ-1;ii>=0 && !res && E->Type==evMouse;ii--)
          if(Zord[ii]) {
           // проверяем есть ли это окно в текущем дереве процессов
           _Window *tmpw=Zord[ii];
           while(tmpw){
            if(tmpw==ShowBase)
              break;
            tmpw=tmpw->parent;
           }
           if(!tmpw)
             res=0;
           else
             res=Zord[ii]->MHandle(E);
          }
        E->ClearEvent(this);
        return res;
      }
    case evKey:
      switch(E->msg.K){
        case TAB:
        case RIGHT:
        case DOWN:
          if(this!=GDt && child->total){
            SwitchFocus();
            E->ClearEvent(this);
          }
          break;
        case SHIFT_TAB:
        case UP:
        case LEFT:
          if(this!=GDt && child->total){
            SwitchFocus((short)0);
            E->ClearEvent(this);
          }
          break;
        case ALT_S:
          if(st&stFrame)
            SaveSize();
          break;

        case CTRL_LEFT:
            if(st & stNoMove) break;
            if(!(st & stFrame)) break;
            Move(-1,0);
            E->ClearEvent(this);
          break;
        case CTRL_RIGHT:
            if(st & stNoMove) break;
            if(!(st & stFrame)) break;
            Move(1,0);
            E->ClearEvent(this);
          break;

        case CTRL_DOWN:
            if(st & stNoMove) break;
            if(!(st & stFrame)) break;
            Move(0,1);
            E->ClearEvent(this);
          break;
        case CTRL_UP:
            if(!(st & stFrame)) break;
            if(st & stNoMove) break;
            Move(0,-1);
            E->ClearEvent(this);
          break;
        case CTRL_GSLASH:
            if(!(st & stFrame)) break;
            if(st & stNoMove) break;
            Graw(-1,0);
            E->ClearEvent(this);
          break;
        case CTRL_GASTERISK:
            if(!(st & stFrame)) break;
            if(st & stNoMove) break;
            Graw(+1,0);
            E->ClearEvent(this);
          break;

        case CTRL_GPLUS:
            if(!(st & stFrame)) break;
            if(st & stNoMove) break;
            Graw(0,1);
            E->ClearEvent(this);
          break;

        case CTRL_GMINUS:
            if(!(st & stFrame)) break;
            if(st & stNoMove) break;
            Graw(0,-1);
            E->ClearEvent(this);
          break;


        case ESC:
          if(st& stNoESC)
            break;
          Result=0;
          if(parent){
            //if(CanClose())
            {
              _Window *p =parent;
              parent->child->Del( this);
              E->Type=evSystem;
              E->msg.L=sysChildClosed;
              return p->Handle(E);
            //}else{
            //  E->ClearEvent(this);
            }
          }else{
            if(this !=GDt)
              return 1;
          }
          break;

        case ENTER:
          if(st& stNoESC)
            break;
          Result=1;
          if(parent){
            if(CanClose()){
              _Window *p =parent;
              parent->child->Del( this);
              E->Type=evSystem;
              E->msg.L=sysChildClosed;
              return p->Handle(E);
            }else{
              E->ClearEvent(this);
            }
          }else{
            if(this !=GDt)
              return 1;
          }
          break;

        case ALT_GENTER:
          ShowBase->dbg();
          E->ClearEvent(this);
          return 0;

        default:
          break;
      }
      break;
  }
  if(parent)
    return parent->Handle(E);
  return 0;
};

int _Window::MHandle(_Event *E){
  if(E->Type!=evMouse)
    return 0;


  //  обрабатываем если это нам
  short dx=0,dy=0;
  Coord(&dx,&dy);

  // в нас ткнули
  if(!(st&stLocked) &&
     dx<=E->msg.M.x && E->msg.M.x<=dx+w-1 &&
     dy<=E->msg.M.y && E->msg.M.y<=dy+h-1){

    // Control + LeftBtn
    if((E->msg.M.buttons & Ctrl_1) == Ctrl_1){
      if(!HelpLock){
        HelpLock= GDt->Q.AddMessage(sysHelp);
        E->ClearEvent(this);
        return 0;
      }
    }

    if((E->msg.M.buttons & Alt_1)==Alt_1){
      GDt->Q.AddMessage(evKey,ENTER);
      E->ClearEvent(this);
      return 0;
    }

    if((E->msg.M.buttons & Shift_1)==Shift_1){
      GDt->Q.AddMessage(evKey,ALT_UP);
      E->ClearEvent(this);
      return 0;

    }


    if((E->msg.M.buttons & Ctrl_2) == Ctrl_2){
      GDt->Q.AddMessage(evKey,F2);
      E->ClearEvent(this);
      return 0;
    }

    if((E->msg.M.buttons & Alt_2)==Alt_2){
      GDt->Q.AddMessage(evKey,INSERT);
      E->ClearEvent(this);
      return 0;
    }


    if((E->msg.M.buttons & Shift_2)==Shift_2){
      GDt->Q.AddMessage(evKey,ALT_DOWN);
      E->ClearEvent(this);
      return 0;
    }

    if(E->msg.M.buttons & 2){
      // нажали правую кнопку
      GDt->Q.AddMessage(evKey,ESC);
      E->ClearEvent(this);
      return 0;
    }

    if(E->msg.M.buttons & 1){

      _Window *p1,*p2;
      p1=parent;
      if(p1){
        p2=p1->parent;
        while(p1 && p2 ){
          if(p2 && !(p1->st&stFocused))
            p2->SwitchFocus(p1);
          p1=p2;
          p2=p1->parent;
        }
      }

      // нажали левую кнопку
      if(!(st & stFocused) ){
        if(parent )
            parent->SwitchFocus(this);
      }else{
        if(st&stFrame){
          if(E->msg.M.x==dx && E->msg.M.y==dy && !(st & stNoMove)){
              Graw(-1,-1);
              E->ClearEvent(this);
              return 0;
          }
          if(E->msg.M.x==dx+1 &&E->msg.M.y==dy && !(st & stNoMove)){
              Graw(1,0);
              E->ClearEvent(this);
              return 0;
          }
          if(E->msg.M.x==dx &&E->msg.M.y==dy+1 && !(st & stNoMove)){
              Graw(0,1);
              E->ClearEvent(this);
              return 0;
          }

          if(E->msg.M.x == dx+w-1 && E->msg.M.y==dy){
              GDt->Q.AddMessage(evKey,ENTER);
              E->ClearEvent(this);
              return 0;
          }

          // Graw mode
          if(E->msg.M.x==dx+w-1 && E->msg.M.y==dy+h-1 && !(st & stNoMove)){
             _Event *ne=0;
             short mw=w,mh=h;
             char *DragBuf = new char [4000];
             GDt->S.Get(0,0,80,25,DragBuf);
             do{
               if(ne) delete ne;
               while(!(ne=MouseHandler()));

               // правую кнопку игнорируем
               if(ne->msg.M.buttons & 3) continue;

               if(ne->msg.M.buttons & 4){
                  if(dx < ne->msg.M.x && dy + 1 < ne->msg.M.y){
                    mh = ne->msg.M.y-dy+1; //+1
                    mw = ne->msg.M.x-dx+1; //+1
                    GDt->S.Put(0,0,80,25,DragBuf);
                    GDt->S.Bar(dx,dy,mw,mh,-2,0x17);
                    GDt->S.Paste();
                  }
               }

             }while(ne->msg.M.buttons & 4) ;
             GDt->S.Put(0,0,80,25,DragBuf);
             GDt->S.Paste();
             Graw( mw-w, mh-h);
             delete ne;
             delete DragBuf;
             E->ClearEvent(this);
             return 0;
          }

          // drag mode
          if(E->msg.M.y==dy && !(st & stNoMove)){
             _Event *ne=0;
             short ox,oy,mx,my;
             mx=E->msg.M.x;
             my=E->msg.M.y;
             ox=mx-dx; // >=0
             char *DragBuf = new char [4000];
             GDt->S.Get(0,0,80,25,DragBuf);
             do{
               if(ne) delete ne;
               while(!(ne=MouseHandler()));

               // правую кнопку игнорируем
               if(ne->msg.M.buttons & 3) continue;

               if(ne->msg.M.buttons & 4){
                  if(mx!=ne->msg.M.x || my !=ne->msg.M.y){
                    mx = ne->msg.M.x;
                    if(mx < ox) mx = ox;
                    my = ne->msg.M.y;
                    GDt->S.Put(0,0,80,25,DragBuf);
                    GDt->S.Bar(mx-ox,my,w,h,-2,0x17);
                    GDt->S.Paste();
                  }
               }

             }while(ne->msg.M.buttons & 4) ;
             GDt->S.Put(0,0,80,25,DragBuf);
             GDt->S.Paste();
             Move( mx-ox-dx, my-dy);
             delete ne;
             delete DragBuf;
             E->ClearEvent(this);
             return 0;
          }
        }
      }
    }

    E->ClearEvent(this);
    return 0;
  }
  return 0;
};


void _Window::Coord(short *absx, short *absy){
  short dx,dy;
  dx=*absx;dy=*absy;
  _Window *tmp=this;
  while(tmp){
    dx+=tmp->x;
    dy+=tmp->y;
    tmp=tmp->parent;
  }

  if((st&stFrame) && dx<0)
    {x-=dx; dx=0;}
  if((st&stFrame) && dy<0)
    {y-=dy; dy=0;}
  *absx=dx;
  *absy=dy;

}

void _Window::Paint(void){
  TRACE("Window::Paint");
  short border=st&stFocused?1:2;
  short dx=0,dy=0;
  Coord(&dx,&dy);
  {
    if(dx>=80) dx=79;
    if(dy>=25) dy=24;

    GDt->S.Bar(dx,dy,w,h,border,Style2Color(style,1));
    if(st&stFrame){
      GDt->S.AWrite(dx,dy,        '┌',Style2Color(style,1));
      GDt->S.AWrite(dx+1,dy,      26 ,Style2Color(style,1));
      GDt->S.AWrite(dx,dy+1,      25 ,Style2Color(style,1));
      GDt->S.AWrite(dx+w-1,dy+h-1,'┘',Style2Color(style,1));

      GDt->S.AWrite(dx+w-1,dy,'',Style2Color(style,1));
    }
  }
};

// a = 0 - отрисовать только себя
// a = 1 - захват экрана отрисовка себя и сыновей
// a = -1 - тоже что 1 но без захвата экрана
void _Window::Show(short a){
  TRACE("Window::Show");

  if( a ){
    if(a>0) GDt->S.Lock();
    if(ExecBuf)
      GDt->S.Write(0,0,80,25,ExecBuf);
  }

  if(NotOnScreen){
    if(!ExecBuf && st & stFocused && st & stBuffered && !WBuffer){
      WBuffer = new char [4000];
      GDt->S.Get(0,0,80,25,WBuffer);
    }
  }
  NotOnScreen = 0;

  if(a && ZFind(this)==-1)
    // вставляем себя и всех своих детей
    ZInsert(this);

  // отобразить себя на экране
  if(!(st&stHide) ){
     if(!a) Paint();
     if(st&stFocused)
        if(st & stCursor && !(st&stLocked)){
          if(st & stInsert)
            GDt->S.SetCursor(2);
          else
            GDt->S.SetCursor(1);
        }else
          GDt->S.SetCursor(0);
  }

  if(st&stFocused && !FindFocused())
          SwitchFocus(1,0);


  if(a)
    for(int i=ZFind(this);i<MaxZ ;i++)
      if(Zord[i])
         Zord[i]->Show(0);

  if(a)
    GDt->S.Paste();
};




void _Window::Hide(short a){
  TRACE("Window::Hide");
  ZDelete(this);
  st=st | stHide;

  // Убрать всех своих детей
  for(int i=0;i<child->total;i++)
      child->array[i]->Hide(0);

  GDt->S.SetCursor(0);

  short ax=0,ay=0;
  Coord(&ax,&ay);

  // перерисовать все что ниже этого окна
  NotOnScreen = 1;
  if(a){
    if(st & stBuffered && WBuffer) {
      GDt->S.Lock();
      GDt->S.Write(0,0,80,25,WBuffer);
      GDt->S.Paste();
      delete WBuffer;
      WBuffer = 0;
      if(parent && st&stFocused)
        parent->SwitchFocus(1,1);
      return;
    }
    ShowBase->Show(1);
  }

};

void _Window::UnHide(short a){
  TRACE("Window::Hide");

  st=st & (~stHide);

  for(int i=0;i<child->total;i++)
    child->array[i]->UnHide(0);

  short ax=0,ay=0;
  Coord(&ax,&ay);
  // перерисовать себя
  if(a){
    if(st & stBuffered && !WBuffer){
      WBuffer =  new char [4000];
      GDt->S.Get(0,0,80,25,WBuffer);

    }
    Show(1);
  }
};

void _Window::Move(short dx, short dy){
 if(this!=GDt){
   short ox=0,oy=0,yes=0;

   short minx,miny,maxx,maxy;
   Coord(&ox,&oy);
   if(ox+dx<80 && ox+dx >=0){
     x+=dx;
     yes = 1;
     if(dx>0){
       minx = x;
       maxx = x+w-1+dx;
     }else{
       minx =x+dx;
       maxx =x+w-1;
     }
   }else{
    minx=x;
    maxx=x+w-1;
   }
   if(oy+dy<25 && oy+dy >=0){
     y+=dy;
     yes=1;
     if(dy>0){
       miny = y;
       maxy = y+h-1+dy;
     }else{
       miny =y+dy;
       maxy =y+h-1;
     }
   }else{
    miny=y;
    maxy=y+h-1;
   }
   if(!yes)
     return;
   for(int i=0; i < this->child->total;i++)
     if((child->array[i]->st&stBuffered) && child->array[i]->WBuffer){
       delete child->array[i]->WBuffer;
       child->array[i]->WBuffer=0;
     }

  NotOnScreen = 1;
  if(st &stBuffered && WBuffer) {
    GDt->S.Lock();
    GDt->S.Write(0,0,80,25,WBuffer);
    delete WBuffer;
    WBuffer = 0;
    MoveOrGrawDone(sysMoveBuf);
    return;
  }
  MoveOrGrawDone(sysMove);
 }
}

void _Window::Graw(short dx, short dy){
 if(this!=GDt){
   short yes=0;
   short ax=0,ay=0;
   Coord(&ax,&ay);
   short oh = h;
   short ow = w;
   short mw,mh;
   if(w+dx<=80 && w+dx >0){
     if(dx>0){
       mw = w+dx;
     }else{
       mw = w;
     }
     w+=dx;
     yes = 1;
   }else{
    mw=w;
   }

   if(h+dy<=25 && h+dy >0){
     if(dy>0){
       mh = h+dy;
     }else{
       mh =h;
     }
     h+=dy;
     yes=1;

   }else{
    mh=h;
   }

   if(!yes)
     return;
   CheckSize();
   for(int i=0; i < this->child->total;i++)
     if((child->array[i]->st&stBuffered) && child->array[i]->WBuffer){
       delete child->array[i]->WBuffer;
       child->array[i]->WBuffer=0;
     }

   NotOnScreen = 1;
   if(st &stBuffered && WBuffer) {
    short ax=0,ay=0;
    Coord(&ax,&ay);
    GDt->S.Write(0,0,80,25,WBuffer);
    delete WBuffer;
    WBuffer = 0;
    MoveOrGrawDone(sysGrawBuf);
    return;
  }

  MoveOrGrawDone(sysGraw);
 }
}

_Window::~_Window(void){
  TRACE("Window::~Window");


  if(parent){
   if(!(parent->st&stHide)){
     Hide(1);
   }else
     Hide(0);
  }

  if(st&stBuffered && WBuffer){
    delete WBuffer;
    WBuffer = 0;
  }


  // удалить всех своих детей
  for(int i=0;i<child->total;i++){
    delete child->array[i];
  }
  delete child;

  // отключение CallBack функций
  {
    CallBack *tmp=F ,* prev;
    while(tmp){
      prev=tmp;
      tmp=tmp->next;
      delete prev;
    }
  }
  F=0;
  parent = 0;
  ZDelete(this);
};

CallBack* _Window::FindCallBack(short type, long code){
    TRACE("FindCallBack");
    CallBack *tmp=F;
    while(tmp){
       if(tmp->type==type && tmp->code==code)
         return tmp;
       tmp=tmp->next;
    }
    return 0;
};

void _Window::AddCallBack(void (*func)(_Event*,void *), short type, long code){
   TRACE("AddCallBack");
   if(!F){
     F= new CallBack(func,type,code);
   }else{
     CallBack *tmp;
     if(tmp=FindCallBack(type,code)){
      tmp->func=func;
      return;
     }
     tmp=F;
     while(tmp->next){
        tmp=tmp->next;
     }
     tmp->next = new CallBack(func,type,code);
   }
};

void _Window::DelCallBack(short type, long code){
  TRACE("DelCallBack");
  CallBack *tmp=F, *prev =F;
  if(F && F->type==type && F->code==code){
    F=F->next;
    delete tmp;
    return;
  }

  if(tmp){
    tmp=tmp->next;
    while(tmp){
        if(tmp->type==type && tmp->code==code){
           prev->next=tmp->next;
           delete tmp;
           return;
        }
        tmp=tmp->next;
        prev=prev->next;
    }
  }
};


int _Window::Exec(void){
  short Quit=0;
  _Window * prevSB = ShowBase;

  // отводим буфер для предыдущего состояния экрана
  ExecBuf = new char [4000];
  GDt->S.Get(0,0,80,25,ExecBuf);

  ShowBase = this;

  GotFocus(1);
  _Event *E=0;
  while(!Quit){

   // опрос системных сообщений
   while((E=GDt->Q.GetMessage())) {
      // передать сообщение активному процессу
      _Window *tmp;
      tmp=FindFocused();
      if(tmp)
        Quit=tmp->Handle(E);
      else
        Quit=Handle(E);

      // если никто не признался сбрасываем сообщение
      if(E->Type) E->ClearEvent(this);
     if(Quit){
      E=0;
      break;
     }
   }
   if(!Quit)
     for(int i=0; i<MAX_INPUT_CHANEL ;i++){
       if(GDt->proc[i]!=0 &&
          (E=(GDt->proc[i])())
         ) {
        // передать сообщение активному процессу
        _Window *tmp;
        tmp=FindFocused();
        if(tmp)
          Quit=tmp->Handle(E);
        else
          Quit=Handle(E);

        // если никто не признался сбрасываем сообщение
        if(E->Type) E->ClearEvent(this);
        delete E;

         if(Quit)
          break;
       }
     }

  }
  LostFocus(0);
  ShowBase =prevSB;

  GDt->S.Put(0,0,80,25,ExecBuf);
  GDt->S.Paste();

  st|=stHide;
  delete ExecBuf;

  return this->Result;
}

void _Window::FreeAllBuffers(void){
  if(st & stBuffered && WBuffer){
    delete WBuffer;
    WBuffer = 0;
  }
  for(int i=0;i<child->total;i++){
    child->array[i]->FreeAllBuffers();
  }

};

void _Window::Center(void){
  if(NotOnScreen){
    x-=w/2;
    if(x<0)
      x=0;
    y-=h/2;
    if(y<0)
      y=0;
  }
};
void _Window::CenterScr(void){
  if(NotOnScreen){
    x=40-w/2;
    if(x<0)
      x=0;
    y=12-h/2;
    if(y<0)
      y=0;
  }
};

void _Window::BottomRight(void){
  if(NotOnScreen){
    x-=w;
    if(x<0)
      x=0;
    y-=h;
    if(y<0)
      y=0;
  }
};

void _Window::CheckSize(void){
  _Window *tmp;
  short max_w=min_w;
  short max_h=min_h;
  if(st&stNoMove)
    return;
  for(int i=0;i<child->total;i++){
    tmp=child->array[i];
    if(tmp->st&stNoMove
       ){
     if(max_w<tmp->x+((tmp->w>tmp->min_w)?tmp->w:tmp->min_w))
        max_w=tmp->x+((tmp->w>tmp->min_w)?tmp->w:tmp->min_w);
     if(max_h<tmp->y+((tmp->h>tmp->min_h)?tmp->h:tmp->min_h))
        max_h=tmp->y+((tmp->h>tmp->min_h)?tmp->h:tmp->min_h);
    }
  }
  max_w++;
  max_h++;
  if(w<max_w) w=max_w;
  if(h<max_h) h=max_h;
};

void _Window::Commit(void){
  if(Changed)
    Changed(this);
};

void _Window::Load(void){
};




short _Window:: ZFind  (_Window* it){
    for(int i=MaxZ;i>=0;i--)
        if(Zord[i]==it)
           return i;
    return -1;
};

void _Window::ZInsert(_Window * it){
  ZDelete(it);
  if(MaxZ==511)
   abort();
  Zord[MaxZ]=it;
  MaxZ++;
  if(it->child->total){
    for(int i=0; i < it->child->total;i++)
      if(!(it->child->array[i]->st&stHide) && !(it->child->array[i]->st&stFocused))
        ZInsert(it->child->array[i]);
    for(i=0; i < it->child->total;i++)
      if(!(it->child->array[i]->st&stHide)&& (it->child->array[i]->st&stFocused))
        ZInsert(it->child->array[i]);
  }
};

void _Window::ZDelete (_Window* it){
  int pos=ZFind(it);
  if(pos==-1)
    return;
  if(pos<MaxZ)
    memmove(&Zord[pos], &Zord[pos+1],sizeof(_Window*)*(MaxZ-pos));
  MaxZ--;
};


struct file_9 {
  char                WinName             [41] ; //WinName
  short               x                        ; //x
  short               y                        ; //y
  short               w                        ; //w
  short               h                        ; //h
};

struct file_9_0{
  char                WinName             [41] ; //WinName
};


int _Window::SaveSize(void){
  if(Table::St.main_file){
    file_9  rec,rec1;
    file_9_0 key;
    strcpy(rec.WinName,name);
    strcpy(key.WinName,name);
    rec.x=x;
    rec.y=y;
    rec.w=w;
    rec.h=h;
    if(Table::St.winsize_file->GEQ(&rec1,&key,0))
      Table::St.winsize_file->Insert(&rec,&key,0);
    else
      Table::St.winsize_file->Update(&rec);
    return 1;
  }
  return 0;
};

int _Window::LoadSize(void){
  if(Table::St.main_file){
    file_9  rec;
    file_9_0 key;
    strcpy(rec.WinName,name);
    strcpy(key.WinName,name);
    if(!Table::St.winsize_file->GEQ(&rec,&key,0)){
      x=rec.x;
      y=rec.y;
      w=rec.w;
      h=rec.h;
      return 1;
    }

  }
  return 0;

};

void ScrollWrite(_Window *Win, char *f,...){
  if(!Win || Win->st & stHide ) return;

  char h[81];
  StrClear(h,80);
  va_list vl; va_start(vl,f); vsprintf(h,f,vl); va_end(vl);
  GDt->S.Lock();
  short ax=1,ay=1;
  Win->Coord(&ax,&ay);
  GDt->S.ScrollUp(ax,ay,Win->w-2,Win->h-2,1,Style2Color(Win->style,1));
  GDt->S.AWrite(ax,ay+Win->h-3,Win->w-2,h,Style2Color(Win->style,1));
  GDt->S.Paste();
};

