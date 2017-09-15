/****************************************************
 *    BUILDER.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"

_Window * BuildCell(Table * T, int field, void * rec, short x,short *y ,short w, int style){
  int size=T->FieldSize(field);
  int type=T->FieldType(field);
  char * off =(char *) rec;
  off=off+T->FieldOffset(field);
  int cur_w,cur_h;
  cur_w=size-1;
  cur_h=1;
  if(cur_w>w-2){
     cur_w=(size-1)/2;
     cur_h++;
     if(cur_w>w-2){
      cur_w=(size-1)/3;
      cur_h++;
     }
  };
  _Window *_w;

  switch(type){
    case 0:
    case 11:
       _w=new _Cell(x,*y,cur_w,cur_h,style,off);
       *y+=cur_h;
       return _w;
    case 1:
    case 15:
      if(size==1)
        return new _ByteCell(x,(*y)++,style,4,off,1);
      if(size==2)
        return new _ShortCell(x,(*y)++,style,6,(short*)off,1);
      if(size==4)
        return new _DigCell(x,(*y)++,style,11,(long*)off,1);
    break;
    case 2:
      if(size==4)
        return new _FltCell(x,(*y)++,style,8,(float*)off);
      if(size==8)
        return new _DblCell(x,(*y)++,style,15,(double*)off);
      break;
    case 3:
        return new _DateCell(x,(*y)++,style,(_Date*)off); ;
    case 4:
        return  new _TimeCell(x,(*y)++,style,(_Time*)off) ;
    case 7:
        return new _YNCell(x,(*y)++,style,(unsigned char *)off);
    case 14:
      if(size==1)
        return new _ByteCell(x,(*y)++,style,4,off);
      if(size==2)
        return new _ShortCell(x,(*y)++,style,6,(short*)off);
      if(size==4)
        return new _DigCell(x,(*y)++,style,11,(long*)off);
    break;
   case 16:
     return new _LDblCell(x,(*y)++,style,15,(long double*)off);
  };
  return new _Static(x,(*y)++,style, "Not Supported...");
};

void BuildForm(_Window * w, Table *t, void * rec, short style, short *fields){
    int flds=t->Fields();
    w->h=1;
    char n[21];
    short i=0;
    while(fields[i]){
      w->Add(new _Static(1,(w->h),style,t->FieldName(fields[i],n)));
      w->Add(BuildCell(t,fields[i],rec,1,&w->h,w->w,style ));
      i++;
    }
    //w->h++;
};

void InitRecord(Table *t , void *rec){
    memset(rec ,0 , t->max_rec_len);
    int flds=t->Fields();
    char n[21];
    int size;
    int type;
    int off;
    for(short i=1;i<= flds;i++){
       size=t->FieldSize(i);
       type=t->FieldType(i);
       off =t->FieldOffset(i);
       switch(type){
        case 0:
        case 11:
         StrClear((char*)rec+off,size-1);
       };
    }
};


