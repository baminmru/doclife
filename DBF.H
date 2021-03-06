/****************************************************
 *          DBF.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
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
  Byte version;       // �����
  Byte yy;            // ��᫥���� ���४�஢��
  Byte mm;
  Byte dd;
  long records;
  short size;         // ࠧ��� ���������
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
  short firstinbuf;         // ����� ��ࢮ� �����
  short bufsize;            // ������⢮ ����ᥩ
  short bufered;
  short fast;
  short fields;
  short error;              // ��� �訡��
  long curent;              // ⥪��� ������
  Byte *record;             // ᮤ�ন��� � ��⮬ �ਧ���� 㤠�����
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
