/****************************************************
 *      FVIEW.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include <string.h>
#define NO_TRACE
#include "common.h"
#undef NO_TRACE



_FileView::_FileView(
 Table * _T
  ,int     _KeyNum
  ,void *  _KeyFilter
  ,int     _KeyFilterSize
  ,void (* _WriteRec)(void * ,char * )
  ,int _X
  ,int _Y
  ,int _W
  ,int _H
  ,int _FirstY
  ,short _style
  ,char *name
  ):
  _Window(_X,_Y,_W,_H,_style,stBuffered |  stFrame ,name){
      memset(Header,0,sizeof(Header));
      T            = _T;
      KeyNum       = _KeyNum;
      KeyFilterSize= _KeyFilterSize;
      WriteRec     = _WriteRec;
      FirstY       = _FirstY  ;
      LineX        = 0;
      LockMode     = 0;
      min_h=8;
      memset(KeyFilter,0,256);
      if( _KeyFilterSize > 0 && _KeyFilterSize <= 256 && _KeyFilter)
        memcpy(KeyFilter,_KeyFilter,_KeyFilterSize);

      CurRec = new char[T->max_rec_len];
      memset(RecPos,0,sizeof(RecPos));
      ActivLine=-1;
      //MaxLine=_H-2;
      TotalLine=0;
      T->ReadDescription();
};


_FileView::~_FileView(){
  delete CurRec;
  for(int i=0;i<5;i++)
    if(Header[i])
      delete Header[i];
};

void _FileView::Paint(void){
  _Window::Paint();
  short dx=0,dy=0;
  Coord(&dx,&dy);
  GDt->S.AWrite(dx+w-5,dy  ,'',Style2Color(style,1));
  GDt->S.AWrite(dx+w-3,dy  ,'',Style2Color(style,1));
  GDt->S.AWrite(dx+w-1,dy+1,'',Style2Color(style,1));
  GDt->S.AWrite(dx+w-1,dy+2,'',Style2Color(style,1));
  GDt->S.AWrite(dx+w-1,dy+3,'+',Style2Color(style,1));
  GDt->S.AWrite(dx+w-1,dy+4,'?',Style2Color(style,1));
  GDt->S.AWrite(dx+w-1,dy+5,'-',Style2Color(style,1));
  GDt->S.AWrite(dx+w-1,dy+6,'',Style2Color(style,1));
  GDt->S.AWrite(dx+w-1,dy+7,'',Style2Color(style,1));
  {
    for(int i=1;i<FirstY && i<6;i++){
      if(Header[i-1]){
        StrClear(LineBuffer,sizeof(LineBuffer)-1);
        strncpy(LineBuffer,Header[i-1],255);
        LineBuffer[strlen(LineBuffer)]=' ';
        short dx,dy;
        dx=1;
        dy=i;
        Coord(&dx,&dy);
        GDt->S.AWrite(dx,dy,w-2,LineBuffer+LineX,Style2Color(style,1));
      }
    }
  }
  if(!TotalLine){
    ToTop();
  }
  ReShowAll();
};

void _FileView::SetHeader(short idx, char *HeaderLine){
  if(idx<5){
    if(Header[idx])
      delete Header[idx];
    Header[idx]=new char[strlen(HeaderLine)+1];
    strcpy(Header[idx],HeaderLine);
  }
};


void _FileView::ReShowAll(void){
  if(TotalLine>(h-FirstY-1))
    TotalLine=h-FirstY-1;
  for(int i=0; i<(h-FirstY-1); i++)
		ReShowLine(i);
};

void _FileView::ReShowLine(int Line){
  short color=1;
  StrClear(LineBuffer,sizeof(LineBuffer)-1);
	if(ActivLine<TotalLine){
    if(!T->GDir(CurRec,&RecPos[Line],KeyNum)){
			WriteRec(CurRec,LineBuffer);
			if(Line!=ActivLine)
        color=1;
			else
        color=2;
		}else{
      color=1;
      StrClear(LineBuffer,sizeof(LineBuffer)-1);
		}
	}
	LineBuffer[strlen(LineBuffer)]=' ';
  //LineBuffer[w-2]=0;
  short dx,dy;
  dx=1;
  dy=Line+FirstY;
  Coord(&dx,&dy);
  //color+=(2-2*(st&stFocused));
  GDt->S.AWrite(dx,dy,w-2,LineBuffer+LineX,Style2Color(style,color));

};


unsigned long _FileView::CurRecPos(void){
  return RecPos[ActivLine];
};


int _FileView::MHandle(_Event *E){
  if(E->Type==evMouse && st & stFocused){
    short dx=0,dy=0;
    Coord(&dx,&dy);

    if( E->msg.M.buttons & 1 && !FindFocused() ){

      if(E->msg.M.y==dy)
        switch(E->msg.M.x-dx-w){
          case -5:
            GDt->Q.AddMessage(evKey,LEFT);
            E->ClearEvent(this);
            return 0;
          case -3:
            GDt->Q.AddMessage(evKey,RIGHT);
            E->ClearEvent(this);
            return 0;
        };

      if(E->msg.M.x==dx+w-1)
        switch(E->msg.M.y-dy){
          case 1:
            GDt->Q.AddMessage(evKey,PGUP);
            E->ClearEvent(this);
            return 0;
          case 2:
            GDt->Q.AddMessage(evKey,UP);
            E->ClearEvent(this);
            return 0;
          case 3:
            GDt->Q.AddMessage(evKey,INSERT);
            E->ClearEvent(this);
            return 0;

          case 4:
            GDt->Q.AddMessage(evKey,F5);
            E->ClearEvent(this);
            return 0;

          case 5:
            GDt->Q.AddMessage(evKey,DELETE);
            E->ClearEvent(this);
            return 0;

          case 6:
            GDt->Q.AddMessage(evKey,DOWN);
            E->ClearEvent(this);
            return 0;
          case 7:
            GDt->Q.AddMessage(evKey,PGDN);
            E->ClearEvent(this);
            return 0;
        }

      if(dx<E->msg.M.x && E->msg.M.x<dx+w-1 && dy+((FirstY)?FirstY-1:0) < E->msg.M.y && E->msg.M.y<dy+h-1){
        if(E->msg.M.y-dy-FirstY == ActivLine){
          GDt->Q.AddMessage(evKey,ENTER);
          E->ClearEvent(this);
          return 0;
        }else{
          if(E->msg.M.y-dy-FirstY < TotalLine){
            ActivLine=E->msg.M.y-dy-FirstY;
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

int _FileView::Handle(_Event *E){
  int OldActivLine;
  int ch;
  short done=0;
  PageChanged=0;
  if(E->Type==evKey){
  switch(E->msg.K){
    case DOWN:
      OldActivLine=ActivLine;
      Next();
      if(PageChanged){
        Show(1);
        done=1;
        break;
      }
      if(ActivLine!=OldActivLine){
        GDt->S.Lock();
        ReShowLine(OldActivLine);
        ReShowLine(ActivLine);
        GDt->S.Paste();

      }
      done=1;
      break;

    case UP:
      OldActivLine=ActivLine;
      Previous();
      if(PageChanged){
        Show(1);
        done=1;
        break;
      }
      if(ActivLine!=OldActivLine){
        GDt->S.Lock();
        ReShowLine(OldActivLine);
        ReShowLine(ActivLine);
        GDt->S.Paste();
      }
      done=1;
      break;
    case LEFT:
      if(LineX>10)
        LineX-=10;
      else
        LineX=0;
      done=1;
      Show(1);
      break;

    case RIGHT:
      {
              short mmm=0;
              for(int i=0;i<5;i++)
                if(Header[i])
                  if(mmm<strlen(Header[i]))
                        mmm=strlen(Header[i]);
              if(mmm==0)
                mmm=MAX_STRING_WIDTH;
              if(mmm<=w-2){
                LineX=0;
              }else{
                if(mmm-LineX-10>=w-2)
                  LineX+=10;
                else
                  LineX=mmm-w+2;
              }
              done=1;
              Show(1);
              break;
      }
    case PGUP:
      PageUp();
      Show(1);
      done=1;
      break;

    case PGDN:
      PageDn();
      Show(1);
      done=1;
      break;

    case HOME:
      ToTop();
      Show(1);
      done=1;
      break;

    case END:
      ToBottom();
      Show(1);
      done=1;
      break;

    case F5:
      if(Find(0))
        Show(1);
      done =1;
      break;

    //case ESC:
    // return parent->Handle(E);;
    default:
      break;

    }
  }


  if(done) E->ClearEvent(this);

  // отправили на стандартную обработку
  return _Window::Handle(E);
};


int _FileView::RecordOK(void *Rec){
  char _Key[256];
  if( !KeyFilterSize)
    return 1;

  else{
    //long tmppos;
    //T->GPos(&tmppos);
    T->BuildIndex(Rec,_Key,KeyNum);
    //T->GDir(Rec,&tmppos,KeyNum);
    if(memcmp(_Key,KeyFilter,KeyFilterSize))
      return 0;
    else
      return 1;
  }
};

void _FileView::JumpToPos(long newpos){
  memset(RecPos,0,sizeof(RecPos));
  ActivLine=0;
  TotalLine=0;
  PageChanged=1;
  if(!T->GDir(CurRec,&newpos,KeyNum)){
    if(RecordOK(CurRec)){
      memset(RecPos,0,sizeof(RecPos));
      ActivLine=0;
      T->GPos(&RecPos[ActivLine]);
      TotalLine=1;
      for(int i=0;i<(h-FirstY-1)-1;i++)
        if(!Next()) break;
      ActivLine=0;
    }
  };
};

void _FileView::JumpToRec(void *Rec){
    char _Key[256];
    memset(_Key,0,256);
    T->BuildIndex(Rec,_Key,KeyNum);
    JumpToKey(_Key);
};

void _FileView::JumpToKey(void *Key){
  memset(RecPos,0,sizeof(RecPos));
  ActivLine=0;
  TotalLine=0;
  PageChanged=1;
  if(!T->GGE(CurRec,Key,KeyNum)){
    if(RecordOK(CurRec)){
      memset(RecPos,0,sizeof(RecPos));
      ActivLine=0;
      T->GPos(&RecPos[ActivLine]);
      TotalLine=1;
      for(int i=0;i<(h-FirstY-1)-1;i++)
        if(!Next()) break;
      ActivLine=0;
    }
  };
};

void _FileView::Repaint(int lock){
    unsigned long pos;
    memset(RecPos,0,sizeof(RecPos));
    ActivLine=0;
    TotalLine=0;
    T->GPos(&pos);
    RecPos[ActivLine]=pos;
    TotalLine=0;
    for(int i=0;i<(h-FirstY-1);i++)
      if(!Next()){
         ToBottom();
         break;
      }
    for(i=0;i<(h-FirstY-1);i++)
      if(RecPos[i]==pos){
        ActivLine=i;
        break;
      }
    PageChanged=1;
    Show(lock);
};


int _FileView::Next(void){
    if(TotalLine>(h-FirstY-1))
      TotalLine=h-FirstY-1;
    if(ActivLine<TotalLine-1)
      // движение внутри страници
      ActivLine++;
    else{
      T->GDir(CurRec,&RecPos[ActivLine],KeyNum);
      if(!T->GNext(CurRec)){
        if(RecordOK(CurRec)){

          // появилась новая запись
          if(TotalLine<(h-FirstY-1)){
            T->GPos(&RecPos[++ActivLine]);
            TotalLine++;
          }else{

            // прокрутка страници
            T->GPos(&RecPos[(h-FirstY-1)]);
            for(int i=0; i<(h-FirstY-1); i++){
              RecPos[i]=RecPos[i+1];
            }
            PageChanged=1;

          }
        }else
          return 0;
      } else
        return 0;

    }
    return 1;
};

int _FileView::Previous(void){
    if(TotalLine>(h-FirstY-1))
      TotalLine=h-FirstY-1;

    if(ActivLine>0)
      // движение внутри страници
      ActivLine--;
    else{
      T->GDir(CurRec,&RecPos[ActivLine],KeyNum);
      if(!T->GPrev(CurRec)){
        if(RecordOK(CurRec)){

          // прокрутка страници
          for(int i=(h-FirstY-1); i>0; i--){
            RecPos[i]=RecPos[i-1];
            if(TotalLine<(h-FirstY-1))
              TotalLine++;
          }
          T->GPos(&RecPos[0]);

          PageChanged=1;


        } else
          return 0;
      }else
        return 0;
    }
    return 1;
};


void _FileView::PageUp(void){
  for(int i=0;i<(h-FirstY-1)-1;i++)
    if(!Previous()) break;
};

void _FileView::PageDn(void){
  for(int i=0;i<(h-FirstY-1)-1;i++)
    if(!Next()) break;
};

void _FileView::ToTop(void){
  char _Key[256];
  memset(RecPos,0,sizeof(RecPos));
  TotalLine=0;
  ActivLine=0;
  PageChanged=1;
  T->FillIndex(_Key,KeyNum);
  memcpy(_Key,KeyFilter,KeyFilterSize);
  if(!T->GGE(CurRec,_Key,KeyNum)){
    if(RecordOK(CurRec)){
      memset(RecPos,0,sizeof(RecPos));
      ActivLine=0;
      T->GPos(&RecPos[ActivLine]);
      TotalLine=1;
      for(int i=0;i<(h-FirstY-1)-1;i++)
        if(!Next()) break;
      ActivLine=0;
    }
  };
};

void _FileView::ToBottom(void){
  char _Key[256];
  TotalLine=0;
  ActivLine=0;

  T->FillIndex(_Key,KeyNum,0);
  memcpy(_Key,KeyFilter,KeyFilterSize);
  memset(RecPos,0,sizeof(RecPos));

  if(!T->GLE(CurRec,_Key,KeyNum)){
    for(int i=0;i<(h-FirstY-1);){
      if(T->GPrev(CurRec))
        break;
      if(RecordOK(CurRec)){
        T->GPos(&RecPos[ActivLine]);
        i++;
      }
    }

    if(RecPos[0])
      TotalLine=1;
    else
      TotalLine=0;
    ActivLine=0;
    for(;i>=0;i--)
      if(!Next()) break;
    ActivLine=TotalLine-1;
    PageChanged=1;
  };
};
int _FileView::Update(void *R, int unlock){
  int i=1;
  if(RecordOK(R)){
    if(T->GDir(CurRec,&RecPos[ActivLine],KeyNum,LockMode))
      T->GDir(CurRec,&RecPos[ActivLine],KeyNum);
    i=T->Update(R);
    if(unlock){
      long pos ;
      T->GPos(&pos);
      T->UnlockSfromM(&pos);
    }
    Repaint();
  }
  return !i;
};

int _FileView::Insert(void *R){
  int i=1;
  long pos=0;
  if(RecordOK(R)){
    // блокировка по созданию новой записи
    if(!T->Crit()){
      while(!T->CritBegin()) ;
      i=T->Insert(R,T->key);
      if(!i)
        T->GPos(&pos);
      while(!T->CritEnd());
    } else{
      i=T->Insert(R,T->key);
      if(!i)
        T->GPos(&pos);
    }
    if(pos)
      JumpToPos(pos);
  }
  return !i;
};

int _FileView::Delete(void){
  if(!TotalLine)
    return 0;
  int i=1;
  int res=MessageBox("Вы действительно хотите удалить запись ?",
                     "        Да - [Enter]  Нет - [Esc]");
  if(res){
    if(!Lock(CurRec)) return 0;
    i=T->Delete();
    if(!i){
      if(T->GNext(CurRec))
        T->GPrev(CurRec);
      Repaint();
    }
    return !i;
  }else
    return 0;
};

int _FileView::Lock(void *R , int mode){
  if(!RecPos[ActivLine]) return 0;
  int i=1;
  long  prev;
  int cnt=0;
  LockMode = mode;
  _Gauge *g=0;
  if(mode)
    prev=clock();

  do{
    i=T->GDir(R,&RecPos[ActivLine],KeyNum,mode);
    if(i && LockMode){
      if(!g){
        g=new _Gauge(0,0,w-3 ,ST_GAUGE,200);
        Add( g);
        SwitchFocus(g);
      }

      if(prev<clock()){
        prev=clock();

        if(g)
          g->Update(++cnt);

        if(cnt==200){
          // if(g) Del(g);
          // g=0;
          cnt=0;
          if(!MessageBox(
              "Запись занята другим пользователем !",
              "Ждать - [Enter]     Не ждать - [Esc]"
              )
          )
            break;
        }

      }
    }
  }while(i);
  if(g)  Del(g);

  return !i;
};

void _FileView::Unlock(void){
  if(LockMode==200 || LockMode==100)
    T->UnlockS(CurRecPos());
  if(LockMode==400 || LockMode==300){
    long pos=CurRecPos();
    T->UnlockSfromM(&pos);
  }
};

void _FileView::ChangeFilter(int _KeyNum, void *_KeyFilter , int _KeyFilterSize){
      KeyNum=_KeyNum;
      memset(KeyFilter,0,256);
      if( _KeyFilterSize > 0 && _KeyFilterSize <= 256 && _KeyFilter){
        KeyFilterSize=_KeyFilterSize;
        memcpy(KeyFilter,_KeyFilter,KeyFilterSize);
      }else{
        KeyFilterSize=0;
      }

      ToTop();
      Show();
} ;

static short QrySayOK;
static void QryOK(_Event *E, void *p){
    QrySayOK=1;

}

int _FileView::Find(short FieldID){
  short ID;
  if(!FieldID){
    short allocated=0;
    short type;
    short *flds;
    char **names;
    flds = new short[T->Fields()];
    names = new char *[T->Fields()+1];
    for(int i = 1; i<=T->Fields(); i++){
      type =T->FieldType(i);
      if(type ==0 || type == 11){
        names[allocated]= new char [21];
        T->FieldName(i,names[allocated]);
        flds[allocated++]=i;
      }
    }
    if(!allocated){
      delete names;
      delete flds;
      MessageBox("Нет текстовых полей !","Поиск не возможен...");
      return 0;
    }else{
      names[allocated]=0;
    }

    if(allocated>1){
      _Window *ttt=new _VMenu(30,3,names,&ID);
      ttt->Add( new _Static(1,0,ST_VMENU,"Определите поле"));
      ttt->Exec();
      delete ttt;
      ID = flds[ID-1];
    }else
      ID = flds[0];

    for(i=0; i<allocated;i++)
      delete (names[i]);
    delete names;
    delete flds;
  }else{
    ID=FieldID;
  }
  short off = T->FieldOffset(ID);
  short sz =  T->FieldSize(ID);
  short type =T->FieldType(ID);
  if(type == 0 || type == 11){
    char Input[31];
    StrClear(Input,30);
    _Window *Qry = new _Window(5,5,35,4,ST_OPTION,stBuffered | stFrame,"Поиск по полю");
    Qry->Add(new _Static(1,1,ST_OPTION,"Поиск по вхождению:"));
    Qry->Add(new _Cell(1,2,30,1,ST_OPTION,Input));
    Qry->AddCallBack(QryOK,evKey, ENTER);
    QrySayOK=0;
    Qry->Exec();
    delete Qry;
    if(!QrySayOK)
     return 0;

    StrTrim(Input);
    if(Input[0]==0)
      return 0;
    StrUpper(Input);
    char * buf = new char [sz+1];
    char *_Key = new char [256];
    long g_cnt = 0;
    _Gauge *g=new _Gauge(0,0,w-3,ST_GAUGE,T->St.Records(T));
    Add( g);
    SwitchFocus(g);

    T->FillIndex(_Key,KeyNum);
    memcpy(_Key,KeyFilter,KeyFilterSize);
    if(!T->GGE(CurRec,_Key,KeyNum)){
      do{
        g->Update(++g_cnt);
        if(RecordOK(CurRec)){
          memset(buf,0,sz+1);
          memcpy(buf,CurRec+off,sz);
          StrTrim(buf);
          StrUpper(buf);
          if(strstr(buf,Input)){
            T->BuildIndex(CurRec,_Key,KeyNum);
            _Window *Qry = new _Window(6,6,45,4,ST_OPTION,stBuffered | stFrame,"Проверка поиска");
            Qry->Add(new _Static(1,1,ST_OPTION,"Подходит ли найденный вариант (Enter/Esc) ?"));
            int sz2=40;
            if(strlen(buf)<sz2)
              sz2=strlen(buf);
            _Cell * ttt = new _Cell(1,2,sz2,1,ST_OPTION,buf);
            ttt->st|=stLocked;
            Qry->Add(ttt);
            Qry->Add(new _Static(sz2+1,2,ST_OPTION,"..."));
            Qry->AddCallBack(QryOK,evKey, ENTER);
            QrySayOK=0;
            Qry->Exec();
            delete Qry;
            long newpos;
            T->GPos(&newpos);
            if(QrySayOK){
              JumpToPos(newpos);
              delete _Key;
              delete buf;
              Del(g);
              return 1;
            }
            T->GDir(CurRec,&newpos,KeyNum);
          }
        }else
         break;

      }while (!T->GNext(CurRec));
    }
    delete _Key;
    delete buf;
    MessageBox("Подходящий вариант не обнаружен");
    Del(g);
    return 0;
  }
  return 0;
};

void _FileView::MoveOrGrawDone(short mode){
  switch(mode){
      case sysGrawBuf:
        if(h<9) h=9;
        LineX=0;
        JumpToPos(CurRecPos());
        Show(-1);
        return ;

      case sysGraw:
        if(h<9) h=9;
        LineX=0;
        JumpToPos(CurRecPos());
        ShowBase->Show(1);
        return;
  }
  _Window::MoveOrGrawDone(mode);
};
