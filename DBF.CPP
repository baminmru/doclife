/****************************************************
 *        DBF.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include <io.h>
#include <sys\stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <share.h>
#include "common.h"
#include "dbf.h"





DBFFile::DBFFile(char *_name){
  OK=curent=firstinbuf=bufsize=fast=bufered=0;
  buffer=0;
  record=0;
  flds=0;
  list=0;
  LockTimes=100;
  h=sopen(_name,O_RDWR|O_BINARY,SH_DENYNO);
  if(h==-1){
    error=1;
    return;
  }
  read(h,&hdr,sizeof(hdr));
  record = new Byte[hdr.recordlen];

  /*
  if(64000U/hdr.recordlen>5){
    buffer=new Byte[64000U/hdr.recordlen*hdr.recordlen];
    bufsize=64000u/hdr.recordlen;
    fast=1;
  }
  */

  fields=(hdr.size-33)/32;
  flds=new FieldDesc [fields];
  read(h,flds,fields*32);
  strcpy(name,_name);
  read(h,&OK,1);
  if(OK!=0x0D)
    OK=0;
  //Debug("DBF open = %x\n",OK);
};

DBFFile::~DBFFile(){
  if(OK)
    close(h);

  if(record)
    delete record;

  if(flds)
    delete flds;

  if(buffer)
    delete buffer;

 UnLockAll();
};

long DBFFile::Records(){
  return hdr.records;
};

short DBFFile::RecSize(){
  return hdr.recordlen-1;
};

short DBFFile::Insert(void *rec){
  ReloadHeader();
  firstinbuf=0;
  long befor=filelength(h);
  Byte eof=0x1a;
  Lock(befor-1,1);
  lseek(h,hdr.size+hdr.recordlen*hdr.records,SEEK_SET);
  memcpy(record+1,rec,hdr.recordlen-1);
  *record=' ';
  write(h,record,hdr.recordlen);
  write(h,&eof,1);
  lseek(h,1,SEEK_SET);
  hdr.records++;
  today.Today();
  hdr.yy=today.y%100;
  hdr.mm=today.m;
  hdr.dd=today.d;
  write(h,&hdr.yy,7);
  UnLockHeader();
  UnLock(befor-1,1);
  curent=hdr.records;
  return error=0;
};

// 1 - open file error
// 2 - Нет текущей записи
// 3 - lock error
// 4 - unlock error
// 5 - function not supported

short DBFFile::LockRecord(){
  if(!curent)
    return error=2;
  error=Lock(hdr.size+(curent-1)*hdr.recordlen,hdr.recordlen);
  return error=0;
};

short DBFFile::UnLockRecord(){
  if(!curent)
    return error=2;
  error=UnLock(hdr.size+(curent-1)*hdr.recordlen,hdr.recordlen);
  return error=0;
};

short  DBFFile::LockHeader(){
  return Lock(0,32);
};

short  DBFFile::UnLockHeader(){
  return UnLock(0,32);
};

short DBFFile::Update(void *rec){
  error=0;
  if(!curent)
    return error=2;
  firstinbuf=0;
  LockHeader();
  lseek(h,hdr.size+(curent-1)*hdr.recordlen,SEEK_SET);
  memcpy(record+1,rec,hdr.recordlen-1);
  *record=' ';
  write(h,record,hdr.recordlen);
  lseek(h,1,SEEK_SET);
  today.Today();
  hdr.yy=today.y%100;
  hdr.mm=today.m;
  hdr.dd=today.d;
  write(h,&hdr.yy,3);
  UnLockHeader();
  return error=0;
};

short DBFFile::Delete(){
  Byte del='*';
  if(!curent)
    return error=2;
  firstinbuf=0;
  LockHeader();
  Lock(hdr.size+(curent-1)*hdr.recordlen,hdr.recordlen);

  lseek(h,hdr.size+(curent-1)*hdr.recordlen,SEEK_SET);
  write(h,&del,1);
  lseek(h,1,SEEK_SET);
  today.Today();
  hdr.yy=today.y%100;
  hdr.mm=today.m;
  hdr.dd=today.d;
  write(h,&hdr.yy,3);

  UnLock(hdr.size+(curent-1)*hdr.recordlen,hdr.recordlen);
  UnLockHeader();

  return error=0;
};

short DBFFile::UnDelete(){
  Byte del=' ';
  if(!curent)
    return error=2;
  firstinbuf=0;
  LockHeader();
  Lock(hdr.size+(curent-1)*hdr.recordlen,hdr.recordlen);

  lseek(h,hdr.size+(curent-1)*hdr.recordlen,SEEK_SET);
  write(h,&del,1);
  lseek(h,1,SEEK_SET);
  today.Today();
  hdr.yy=today.y%100;
  hdr.mm=today.m;
  hdr.dd=today.d;
  write(h,&hdr.yy,3);

  UnLock(hdr.size+(curent-1)*hdr.recordlen,hdr.recordlen);
  UnLockHeader();

  return error=0;
};

short DBFFile::Pack(){
  return error=5;
};

// 6 - invalid ID
short DBFFile::Goto(long ID){
  if(!fast || !firstinbuf)
    ReloadHeader();
  if(ID>0 && ID <= hdr.records)
    curent=ID;
  else{
    curent=0;
    return error=6;
  }

  if(!fast){
    lseek(h,hdr.size+(curent-1)*hdr.recordlen,SEEK_SET);
    read(h,record,hdr.recordlen);
  }else{
    if(firstinbuf<=curent && firstinbuf+bufered > curent){
      //printf("+");
      memcpy(record,buffer+hdr.recordlen*(curent-firstinbuf),hdr.recordlen);
    }else{
      //printf("-");
      lseek(h,hdr.size+(curent-1)*hdr.recordlen,SEEK_SET);
      read(h,buffer,hdr.recordlen*bufsize);
      firstinbuf=curent;
      if(bufsize+firstinbuf<hdr.records)
        bufered=bufsize;
      if(hdr.records-firstinbuf<bufsize)
        bufered=hdr.records-firstinbuf+1;

      memcpy(record,buffer+hdr.recordlen*(curent-firstinbuf),hdr.recordlen);
    }
  }
  return error=0;
};

short DBFFile::Goto(long ID,void *rec){
  Goto(ID);
  if(!error)
    GetLast(rec);
   return error;
};

short DBFFile::Top(void *rec, short all){
  long ID=1;
  do{
    Goto(ID++);
    if( !error && all || *record==' ')
      break;
  }while(!error );
  if(!error)
   GetLast(rec);
  return error;
};

short DBFFile::Bottom(void *rec,short all){
  long ID=hdr.records;
  do{
    Goto(ID--);
    if( !error && all || *record==' ')
      break;
  }while(ID>=0 && !error );

  if(!ID<0)
    return error=2;

  if(!error)
    GetLast(rec);

  return error;
};

// 7 - last record

short DBFFile::Next( void * rec,short all){
  long id;
  if(!curent)
    return error=2;
  id=curent;
  while(!error && id<hdr.records){
     Goto(++id);
     if(!error && all || *record==' ' ){
       GetLast(rec);
       return error=0;
     }
  }
  return error=7;
};

short DBFFile::Prev( void * rec,short all){
  long id;
  if(!curent)
    return error=2;
  id=curent;
  while(!error && id>1){
    Goto(--id);
    if(!error && all || *record==' '){
      GetLast(rec);
      return error=0;
    }
  }
  return error=7;
};

short DBFFile::GetLast(void *rec){
   if(curent){
     memcpy(rec,record+1,hdr.recordlen-1);
     return error=0;
   }else
    return error=2;
};

void DBFFile::ReloadHeader(){
  lseek(h,0,SEEK_SET);
  read(h,&hdr,sizeof(hdr));
};

Byte DBFFile::GetRecordState(void){
   if(curent){
     return *record;
   }else
    return 0;
};
Byte *DBFFile::GetField(void *rec,short id){
  if(id>=fields || id <0)
    return 0;
  Byte *ptr=(Byte*)rec;
  short s=0;
  while(id){
    ptr+=flds[s].size;
    s++;id--;
  }
  return ptr;
};

Byte *DBFFile::GetField(void *rec,char *Name){
  for(int id=0;id<fields;id++){
    if(!strnicmp(flds[id].name,Name,strlen(Name)))
      break;
  }
  if(id>=fields || id <0)
    return 0;
  Byte *ptr=(Byte*)rec;
  short s=0;
  while(id){
    ptr+=flds[s].size;
    s++;id--;
  }
  return ptr;
};

void DBFFile::SetField(void *rec, char *fld, short id){
  Byte *ptr=GetField(rec,id);
  if(ptr){
    memset(ptr,' ',flds[id].size);
    if(strlen(fld)<flds[id].size)
      memcpy(ptr,fld,strlen(fld));
    else
      memcpy(ptr,fld,flds[id].size);
  }
};
void DBFFile::SetField(void *rec, char *fld, char *Name){
  for(int id=0;id<fields;id++){
    if(!strnicmp(flds[id].name,Name,strlen(Name)))
      break;
  }
  Byte *ptr=GetField(rec,id);
  if(ptr){
    memset(ptr,' ',flds[id].size);
    if(strlen(fld)<flds[id].size)
      memcpy(ptr,fld,strlen(fld));
    else
      memcpy(ptr,fld,flds[id].size);
  }
};
void DBFFile::Blank(void *r){
  memset(r,' ',hdr.recordlen);
};

short DBFFile::InsertLock(long offset,long size){
  LockStruct *tmp=list;
  list=new LockStruct(offset,size);
  list->nxt=tmp;
  return error=0;
};

short DBFFile::DeleteLock(long offset,long size){
  LockStruct *tmp=list,*tmp2;
  if(list->pos==offset && list->size==size){
    list=list->nxt;
    delete tmp;
    return error=0;
  }
  tmp2=tmp;
  tmp=tmp->nxt;
  while(tmp){
    if(tmp->pos==offset && tmp->size==size){
      tmp2->nxt=tmp->nxt;
      delete tmp;
      return error=0;
    }
    tmp=tmp->nxt;
    tmp2=tmp2->nxt;
  }
  return error=0;
};

short DBFFile::Lock(long offset, long size){
  if(LockTimes){
    if(LockTimes>0){
      do{
      for(int i=0;i<LockTimes;i++){
        if(!lock(h,offset,size)){
          InsertLock(offset,size);
          return error=0;
        }
      }
      }while(MessageBox("      Не возможно блокировать запись",
                        "Пробуем еще - Enter или прекратить - Esc"));
      return error=3;
    }else{
      while(lock(h,offset,size));
      return error=0;
    }
  }
  return error=0;
};
short DBFFile::UnLock(long offset,long size){
  DeleteLock(offset,size);
  if(LockTimes){
    if(LockTimes>0){
      do{
      for(int i=0;i<LockTimes;i++){
        if(!unlock(h,offset,size)){
          return error=0;
        }
      }
      }while(MessageBox("    Не возможно разблокировать запись",
                        "Пробуем еще - Enter или прекратить - Esc"));
      return error=4;
    }else{
      while(unlock(h,offset,size));
      return error=0;
    }
  }
  return error=0;
};
short DBFFile::UnLockAll(){
  error=0;
  while(list && !error){
    UnLock(list->pos,list->size);
  }
  return error;
};

void  DBFFile::SetLockingStyle(short style){
  if(style!=LockTimes)
    UnLockAll();
  LockTimes=style;
};

short  DBFFile::GetLockingStyle(){
  return LockTimes;
};

