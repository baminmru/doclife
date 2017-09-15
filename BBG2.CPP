/****************************************************
 *       BBG2.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"

// Функции класса Station

Table  *Station::main_file=0;
Table  *Station::field_file=0;
Table  *Station::index_file=0;
//Table  *Station::relation_file=0;
Table  *Station::winsize_file=0;
Table  *Station::list=0;
FileDescriptor Station::FD;

int Station::Create (char * _path,int mode){
      int sss;
      int lll=sizeof(FileDescriptor);
      char pbl[128];
      sss=BTRV(14,pbl,&FD,&lll,_path,mode);
      return sss;


};

int Station::Stat   (Table * _tab){
      int sss;
      int lll=sizeof(FileDescriptor);
      char key[256];
      sss=BTRV(15,_tab->pos_blk,&FD,&lll,key,0);
      return sss;
};

long Station::Records   (Table * _tab){
      int sss;
      int lll=sizeof(FileDescriptor);
      char key[256];
      sss=BTRV(15,_tab->pos_blk,&FD,&lll,key,0);
      if(sss)
        return -1;
      else
        return  (((long)FD.NofRecHigh) << 16)+ (long)FD.NofRecLow;

};


int Station::SetDir (char * _path){
  int l;
  return BTRV(18,0,&l,0,_path,0);
};

int Station::GetDir (int _drive , char * _path){
  int l;
  return BTRV(18,0,&l,0,_path,_drive);
};

int Station::TBegin (int mode){
  int l;
  return BTRV(19+mode,0,&l,0,0,0);
};

int Station::TEnd   (){
  int l;
  return BTRV(20,0,&l,0,0,0);
};
int Station::TAbort (){
  int l;
  return BTRV(21,0,&l,0,0,0);
};
int Station::Stop   (void){
  int l;
  return BTRV(25,0,&l,0,0,0);
};

int Station::Version(int * ver){
  int lll=4;
  return BTRV(26,0,ver,&lll,0,0);
};
int Station::ResetMe(void){
  int l;
  return BTRV(28,0,&l,0,0,0);

};
int Station::ResetIt(int *station){

  return BTRV(28,0,0,0,station,-1);
};


int Station::GetAddress(unsigned char *addres){ //6-и байтный адрес
 union REGS regs;
 regs.h.ah=0xee;
 intdos(&regs,&regs);
 if(regs.x.cflag!=0)
  return -1;
 else{
  addres[0]=regs.h.ch;
  addres[1]=regs.h.cl;
  addres[2]=regs.h.bh;
  addres[3]=regs.h.bl;
  addres[4]=regs.h.ah;
  addres[5]=regs.h.al;
  return 0;
 }

};


int Station::GetConnect(void){
 union REGS regs;
 regs.h.ah=0xdc;
 intdos(&regs,&regs);
 if(regs.x.cflag!=0)
  return 0;
 else
  return regs.h.al;
};

Station::Station(void){
  list=0;
  if(!main_file){
    main_file =(Table*)-1L;
    main_file = new Table (sizeof(file_ddf));
    if(main_file){
      strcpy(main_file->name,"X$File");
      main_file->Open("file.ddf","BMMDDF",0);

      if(!main_file->opened){
        printf("\nError : MAIN FILE NOT OPEN (%s)\n",main_file->Error(main_file->status));
        exit(0);
      }

    }else{
      printf("\nError: MAIN FILE Table not created\n");
      exit(0);
    }
    field_file= new Table("X$Field",sizeof(field_ddf),"BMMDDF");
    if(!field_file->opened){
      printf("\nError : FIELD FILE NOT OPEN (%s)\n",field_file->Error(field_file->status));
      exit(0);
    }
    index_file= new Table("X$Index",sizeof(index_ddf),"BMMDDF");
    if(!index_file->opened){
      printf("\nError : INDEX FILE NOT OPEN (%s)\n",index_file->Error(index_file->status));
      exit(0);
    }
    winsize_file= new Table("X$Winsize","BMMDDF");
    if(!winsize_file->opened){
      printf("\nError : WINSIZE FILE NOT OPEN (%s)\n",winsize_file->Error(winsize_file->status));
      exit(0);
    }
  }
};

Station :: ~Station(){
 if(main_file){
  ResetMe();
  delete main_file;
  main_file=0;
  delete index_file;
  delete field_file;
  delete winsize_file;
 }
}

long Station ::CritLock  (char *file){
  file_ddf buf;
  char key[20];
  memset(key,' ',20);
  if(strlen(file)>20) file[20]=0;
  memcpy(key,file,strlen(file));
  int err;
  if(err = main_file->GEQ(&buf,key,1,B_MULTIPLE_REC_NOWAIT_LOCK))
    return 0;
  long pos;
  err=main_file->GPos(&pos);
  if(err)
    return 0;
  return pos;
};

int Station ::CritUnLock(long pos){
  int err=main_file->UnlockSfromM(&pos);
  return !err;
};

int Station::GetFilePath(char * file, char * path){
  file_ddf buf;
  char key[20];
  memset(key,' ',20);
  if(strlen(file)>20) file[20]=0;
  memcpy(key,file,strlen(file));
  int err;
  if(err = main_file->GEQ(&buf,key,1))
    return 0;
  strncpy(path, buf.path,64);
  return 1;

};

int Station::GetFileID(char * file){
  file_ddf buf;
  char key[20];
  memset(key,' ',20);
  if(strlen(file)>20) file[20]=0;
  memcpy(key,file,strlen(file));
  int err;
  if(err = main_file->GEQ(&buf,key,1))
    return 0;
  return buf.id;
};

void Station::AddTable(Table *t){
  Table *tmp=list;
  if(!list)
    list=t;
  else{
    while(tmp->next){
      tmp=tmp->next;
    }
    tmp->next=t;
  }
  t->next=0;
};

void Station::DelTable(Table *t){
  Table *tmp=list;
  if(list==t){
    list=tmp->next;
    return;
  }
  while(tmp->next && tmp->next!=t){
      tmp=tmp->next;
  }
  if(!tmp->next) return;
  tmp->next=t->next;
};

Table* Station::FindTable(int id){
  Table *tmp=list;
  while(tmp){
      if(GetFileID(tmp->name)==id)
        break;
      tmp=tmp->next;
  }
  return tmp;
};

Table* Station::FindTable(char *name){
  //Debug("FindTable (%s)\n",name);
  Table *tmp=list;
  while(tmp){
      //Debug("TabName=%s\n",tmp->name);
      if(!strcmp(name,tmp->name))
        break;
      tmp=tmp->next;
  }
  return tmp;
};

char *KeyTypeName[]={
  "deleted",
  "string",
  "integer",
  "float",
  "date",
  "time",
  "decimal",
  "money",
  "logical",
  "numeric",
  "bfloat",
  "lstring",
  "zstring",
  "note",
  "-------",
  "unsigned",
  "autoinc"
};

