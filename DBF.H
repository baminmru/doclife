/****************************************************
 *          DBF.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef DBF_FILE
#define DBF_FILE
typedef unsigned char Byte;
struct FieldDesc{
  Byte name[11];
  Byte type;
  long address;
  Byte size;
  Byte points;
  Byte reserved1[2];
  Byte workspace;
  Byte reserved2[2];
  Byte setfield;
  Byte reserved3[8];
};

struct DBFHeader{
  Byte version;       // версия
  Byte yy;            // последняя корректировка
  Byte mm;
  Byte dd;
  long records;
  short size;         // размер заголовка
  short recordlen;
  Byte reserved[20];
};

struct LockStruct{
  long pos;
  long size;
  LockStruct * nxt;
  LockStruct(long _pos,long _size){
    pos=_pos;
    size=_size;
    nxt=0;
  };
};

class DBFFile{
  int h;
  Byte OK;
  char name[81];
  DBFHeader hdr;
  FieldDesc *flds;
  Byte *buffer;
  short firstinbuf;         // номер первой записи
  short bufsize;            // количество записей
  short bufered;
  short fast;
  short fields;
  short error;              // код ошибки
  long curent;              // текущая запись
  Byte *record;             // содержимое с учетом признака удаления
  LockStruct *list;
  void ReloadHeader();
  short InsertLock(long offset,long size);
  short DeleteLock(long offset,long size);
  short LockTimes;

 public:
  _Date today;
  DBFFile(char *name);
  ~DBFFile();
  short Error(){return error;};
  long Records();
  short RecSize();
  short Insert(void *rec);
  short Update(void *rec);
  short Delete();
  short UnDelete();
  short Pack();
  short LockRecord();
  short UnLockRecord();
  Byte GetRecordState();
  long GetRecID(){return curent;};
  short Goto(long ID);
  short Goto(long ID,void *rec);
  short Top(void *rec,short all=0);
  short Bottom(void *rec,short all=0);
  short Next( void * rec,short all=0);
  short Prev( void * rec,short all=0);
  void FastRead(short OnOff){if(buffer) fast=OnOff?1:0; else fast=0;}
  short GetLast(void *rec);
  short Fields(){return fields;};
  FieldDesc GetFieldDesc(short id){
      if(id>=0 && id<fields)
        return flds[id];
      else
        return flds[0];
  };
  Byte *GetField(void *rec ,short ID);
  Byte *GetField(void *rec ,char *Name);

  void SetField(void *rec, char *fld, short ID);
  void SetField(void *rec, char *fld, char *Name);

  void Blank(void *r);

  ////////////////////////////////////////
  void  SetLockingStyle(short style);
  short GetLockingStyle();
  short Lock(long offset, long size);
  short UnLock(long offset,long size);
  short UnLockAll();
  short LockHeader();
  short UnLockHeader();
};

#endif
