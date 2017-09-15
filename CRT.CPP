/****************************************************
 *        CRT.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"
/*
// field_list
struct field_list{
  field_ddf f;
  field_list *next;
  field_list(field_ddf *_f){memcpy(&f,_f,sizeof(f)); next=0;};
  void add(field_ddf *_f,field_list **head);
  ~field_list(){
    if(next) delete next;
  }
};
void field_list::add(field_ddf *_f, field_list **head){
  field_list * tmp=*head;
  if(*head==0){
    *head=new field_list(_f);
  }else{
    while(tmp->next){
      tmp=tmp->next;
    }
    tmp->next= new field_list(_f);
  }
};

// index_list
struct index_list{
  index_ddf i;
  index_list *next;
  index_list(index_ddf *_i){memcpy(&i,_i,sizeof(i)); next=0;};
  void add(index_ddf *_i, index_list **head);
  ~index_list(){
    if(next) delete next;
  }
};
void index_list::add(index_ddf *_i, index_list **head){
  index_list * tmp=*head;
  if(*head==0){
    *head=new index_list(_i);
  }else{
    while(tmp->next){
      tmp=tmp->next;
    }
    tmp->next= new index_list(_i);
  }
};

int ValidType(int type){
  if(0<=type && type <=11) return 1;
  if(type==14 || type==15) return 1;
};

*/
int build(char *name){
  file_ddf  file;
  file_ddf_1 file_key;
  field_ddf field;
  field_ddf_1 field_key;
  index_ddf index;
  index_ddf_0 index_key;
  field_list * fl=0;
  index_list * il=0;
  FileDescriptor fd;
  int err;
  memset(&fd,0,sizeof(fd));
  Table T1("X$File",sizeof(file));
  Table T2("X$Field",sizeof(file));
  Table T3("X$Index",sizeof(file));

  if(T1.Open("")) return 0;
  if(T2.Open("")) return 0;
  if(T3.Open("")) return 0;

  memset(file_key.name,' ',20);
  if(strlen(name)>20) name[20]=0;
  memcpy(file_key.name,name,strlen(name));
  if(T1.GEQ(&file, &file_key,1))
    return 0;
  int off=0;
  // создаем список полей файла
  field_key.file_id=file.id;
  if(T2.GGE(&field,&field_key,1,100)) return 0;
  while(field_key.file_id==file.id){
    field.offset=off;
    off+=field.size;
    fl->add(&field,&fl);
    T2.Update(&field);
    if(T2.GNext(&field,&field_key,100)) break;
  }
  if(field_key.file_id!=file.id)
    T2.Update(&field);
  if(!fl) return 0;

  // создаем список индексов
  index_key.file_id=file.id;
  index_key.number=0;
  index_key.part=0;
  if(!T3.GGE(&index,&index_key,0)){
    while(index_key.file_id==file.id){
      il->add(&index,&il);
      if(T3.GNext(&index,&index_key)) break;
    }
  }

  // генерируем дескриптор файла
   field_list *fptr=fl;
   index_list *iptr=il;
   fd.FileFlags = file.flags;
   fd.UnusedPages = 0;

   fd.RecordLenth=0;
   int nomorefixed=0;
   while(fptr){
// длинна в пределах страници !!!
    if(nomorefixed || fd.RecordLenth+fptr->f.size>4080){
       nomorefixed=1;
       fd.FileFlags = fd.FileFlags | FFV_VARIABLE_LENTH;
    }else{
      fd.RecordLenth+=fptr->f.size;
    }
    fptr=fptr->next;
   }

   fd.IndexCount = 0;
   int previdx;
   if(iptr){
    previdx=iptr->i.number;
    fd.IndexCount = 1;
    while(iptr){
      if(iptr->i.number!=previdx){
        previdx=iptr->i.number;
        fd.IndexCount++;
      }
      iptr=iptr->next;
    }
   }
   fd.PageSize = 4096;

  //генерируем описания индексов
   int idx=0;
   iptr=il;

   if(iptr){
    previdx=iptr->i.number;

     while(iptr){
       if(iptr->i.number!=previdx){
         fd.KSeg[idx-1].KeyFlag = fd.KSeg[idx-1].KeyFlag  & ~KFV_SEGMENTED;
         previdx=iptr->i.number;
       }
       fd.KSeg[idx].KeyFlag = iptr->i.flags|KFV_SEGMENTED|KFV_EXTENDED_TYPE & (~KFV_BINARY);

       // ищем поле на которе ссылается индекс
       fptr=fl;
       fd.KSeg[idx].KeyPos = 0;
       fd.KSeg[idx].KeyLen = 0;
       fd.KSeg[idx].KeyType = 0;
       off=0;
       while(fptr){

          if(fptr->f.id==iptr->i.field_id){
             fd.KSeg[idx].KeyPos = off+1;
             fd.KSeg[idx].KeyLen = fptr->f.size;
             fd.KSeg[idx].KeyType = fptr->f.type;
             break;
          }else{
            off+=fptr->f.size;
          }
          fptr=fptr->next;
       }
       if(fd.KSeg[idx].KeyLen <=255 && ValidType(fd.KSeg[idx].KeyType))
         idx++;
       if(idx==24)
         break;
       iptr=iptr->next;
     }

     // снять признак сегмента у последнего
     fd.KSeg[idx-1].KeyFlag = fd.KSeg[idx-1].KeyFlag  & ~KFV_SEGMENTED;
   }


  //создаем файл
  int LBuf = sizeof( fd );
  char PosBlock[128];
  int RetCode;
  RetCode = BTRV( B_CREATE, PosBlock, &fd, &LBuf, file.path, 0 );
  return !RetCode;
};

void main(int argc, char * argv[]){
  if(argc==1){
    printf("Usage: crt file_alias\n");
    return ;
  }
  if(build(argv[1]))
    printf("Done !\n");
  else
    printf("Can't create file ! \n");
};



