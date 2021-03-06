/****************************************************
 *        BBG.CPP                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include "common.h"

#define min(x,y) ((x<y)?x:y)
#define max(x,y) ((x>y)?x:y)

int ValidType(int type){
  if(0<=type && type <=11) return 1;
  if(type==14 || type==15) return 1;
  return 0;
};


Station Table::St;

Table::Table(char  *  _name  ,    int _max_rec_len , char* owner){
  memset(this,0,sizeof(Table));
  strncpy(name,_name,20);
  name[20]=0;
	max_rec_len=_max_rec_len;
  St.AddTable(this);
  Open(owner);
};

Table::Table(char  *  _name ,char *owner){
  memset(this,0,sizeof(Table));
  strncpy(name,_name,20);
  name[20]=0;
  max_rec_len=0;
  ReadDescription();
  for(int i=Fields() ;i>0 ;i--){
    max_rec_len+=FieldSize(i);
  }
  St.AddTable(this);
  Open(owner);
};

Table::Table(  int _max_rec_len
){
  memset(this,0,sizeof(Table));
	max_rec_len=_max_rec_len;
  St.AddTable(this);
};


Table::~Table(){
	if(opened)
		Close();
  if(fl) delete fl;
  if(il) delete il;
  St.DelTable(this);
};


struct err_list{
  char *msg;
  int code;
};


// ������� ᮮ�饭�� �� �訡�� �� ������ �訡��
char * Table::Error(int code){
    err_list em[]={
    {"������ �ᯥ譮 �����襭�", 0}
    ,{"������ ��� ����樨", 1}
    ,{"�訡�� �����-��뢮��", 2}
    ,{"���� �� �����", 3}
    ,{"������ � ������� ���祬 �� �����㦥��", 4}
    ,{"�� ࠧ�襭� �������騥�� ����", 5}
    ,{"������ ����� ����", 6}
    ,{"��ᮮ⢥��⢨� ����� ����", 7}
    ,{"�訡�� ����樮��஢����", 8}
    ,{"���⨣��� ����� 䠩��", 9}
    ,{"����⪠ �८�ࠧ����� �� �������㥬� ����", 10}
    ,{"����୮� ��� 䠩��", 11}
    ,{"���� �� ������", 12}
    ,{"�訡�� ���७��� 䠩��", 13}
    ,{"�訡�� ������ PRE-IMAGE 䠩��", 14}
    ,{"�訡�� ࠡ��� � PRE-IMAGE 䠩���",15}
    ,{"�訡�� ��᪠ �� 㢥��祭�� ࠧ��� 䠫�", 16}
    ,{"�訡�� ������� 䠩��", 17}
    ,{"��� �����", 18 }
    ,{"�����⠭������ �訡�� (�ᯮ���� butil -recover)", 19}
    ,{"BTRIEV �� ����㦥� !!!",20}
    ,{"���� ���� ᫨誮� ���", 21}
    ,{"���� ����� ᫨誮� ���", 22}
    ,{"����樮��� ���� ᫨誮� ���",23}
    ,{"����ୠ� ����� ��࠭��", 24}
    ,{"�訡�� �����-�뢮�� �� ᮧ�����", 25}
    ,{"����୮� �᫮ ᥣ���⮢ ����", 26}
    ,{"����୮� ���ᠭ�� ᥣ���� ����", 27}
    ,{"�� �ࠢ��쭮 ������ ����� �����", 28}
    ,{"����ୠ� ����� ����", 29}
    ,{"���� �� ���� ����� ������", 30}
    ,{"���� 㦥 ��।���� ��� ���७��", 31}
    ,{"�訡�� ���७�� 䠩��", 32}
    ,{"???", 33}
    ,{"����୮� ���७�� ���", 34}
    ,{"����୮ ����� ��४�਩", 35}
    ,{"�訡�� �࠭���樨", 36}
    ,{"���� �࠭����� � ��㣮� �⠭樨", 37}
    ,{"�訡�� �� ࠡ�� � �࠭���樮��� 䠩���", 38}
    ,{"�࠭����� �� ��稭�����", 39}
    ,{"���誮� ����� 䠩��� �ᯮ�짮���� � �࠭���樨", 40}
    ,{"������ �� ����㯭�", 41}
    ,{"�������襭�� �᪮७�� �����", 42}
    ,{"������ ���� �����", 43}
    ,{"���⮩ ����", 44}
    ,{"��ᮢ���⨬� 䫠�� ����", 45}
    ,{"����� � 䠩�� ����饭", 46}
    ,{"���ᨬ��쭮� ������⢮ ������� 䠩���", 47}
    ,{"����୮ ��।����� ����ୠ⨢��� ��᫥����⥫쭮���", 48}
    ,{"�訡�� ⨯� ����", 49}
    ,{"���鶴 㦥 ��⠭�����", 50}
    ,{"����୮� ��� 宧鶴�", 51}
    ,{"�訡�� ����� ����", 52}
    ,{"���ॢ訩 ����䥩�", 53}
    ,{"�訡�� �⥭�� ����� ��६����� �����", 54}
    ,{"���ࠢ��쭮� �ᯮ�짮����� ��⮨��६��⭮�� ����", 55}
    ,{"�������襭�� ������", 56}
    ,{"�訡�� ࠡ��� � ���७��� �������", 57}
    ,{"���� ��� 㯠����� ����ᥩ ᫨誮� ���", 58}
    ,{"���� 㦥 �������", 59}
    ,{"���稪 ����ᥩ ��९����� ???", 60}
    ,{"����祥 ����࠭�⢮ ��� ���७�� ����権 ᫨誮� ����", 61}
    ,{"����୮� ���ᠭ�� ���७��� �����", 62}
    ,{"�� �ࠢ��쭮 ��।���� ���� ��� ���७���� INSERT", 63}
    ,{"���⨣��� �।�� 䨫���樨", 64}
    ,{"���ࠢ��쭮� ᬥ饭�� ����", 65}
    ,{"�࠭����� ��⮬���᪨ �����襭�", 76}
    ,{"����筠� �����஢�� �ணࠬ�", 78}
    ,{"���䫨��", 80}
    ,{"�訡�� �����஢��", 81}
    ,{"����ﭠ ������", 82}
    ,{"�⥭�� ��� �࠭���樨", 83}
    ,{"������ �������஢���", 84}
    ,{"���� �������஢��", 85}
    ,{"������ 䠩��� �����", 86}
    ,{"������ 㪠��⥫�� 䠩��� �����", 87}
    ,{"��ᮢ���⨬� ०�� ������ 䠩��", 88}
    ,{"�訡�� �ࢥ�", 91}
    ,{"������ �࠭���権 �����", 92}
    ,{"��ᮢ���⨭� ⨯ �����஢��", 93}
    ,{"����⪠ ����� � ��ࠧ�襭�� ��४�਩", 94}
    ,{"�த������� ᥠ�� ����������", 95}
    ,{"�訡�� ��", 96}
    ,{"����饭�� ᫨誮� ����", 97}
    ,{"����७��� �訡�� �࠭���樨", 98}
    ,{"�訡�� �� ��������",-1}
  };
 int i=0;
 while(em[i].code!=-1){
  if(em[i].code==code)
    return em[i].msg;
  i++;
 }
 return em[i].msg;
};


void Table::ReadDescription(void){
  if(!fl){
    // ᮧ���� ᯨ᮪ ����� 䠩��
    int id=St.GetFileID(name);
    field_ddf_1 field_key;
    field_ddf field;
    field_key.file_id=id;
    if(St.field_file->GGE(&field,&field_key,1)) return ;
    while(field_key.file_id==id){
      fl->add(&field,&fl);
      if(St.field_file->GNext(&field,&field_key)) break;
    }

    // ᮧ���� ᯨ᮪ �����ᮢ
    index_ddf index;
    index_ddf_0 index_key;
    index_key.file_id=id;
    index_key.number=0;
    index_key.part=0;
    if(!St.index_file->GGE(&index,&index_key,0)){
      while(index_key.file_id==id){
        il->add(&index,&il);
        if(St.index_file->GNext(&index,&index_key)) break;
      }
    }
  }
};
void Table::BuildIndex(void *r ,void *_key, int key){
  index_list *iptr =il;
  field_list *fptr =fl;
  int previdx;
  int off=0,keyoff=0;
  int idx=0;
  if(iptr){
    previdx=iptr->i.number;

    while(iptr){
      if(iptr->i.number!=previdx){
        previdx=iptr->i.number;
        idx++;
      }
      if(idx>key)
        break;
      if(idx==key){
         // �饬 ���� �� ���� ��뫠���� ������
         fptr=fl;
         off=0;
         while(fptr){

            if(fptr->f.id==iptr->i.field_id){
               if(keyoff+fptr->f.size<256 && ValidType(fptr->f.type)){
                 memcpy((char*)_key+keyoff,(char*)r+off,fptr->f.size);
                 keyoff+=fptr->f.size;
               }
               break;
            }else{
              off+=fptr->f.size;
            }
            fptr=fptr->next;
         }
       }
       iptr=iptr->next;
     }

   }

};

void Table::FillIndex(void *_key, int key , int minimum){
  index_list *iptr =il;
  field_list *fptr =fl;
  int previdx;
  int off=0,keyoff=0;
  int idx=0;
  if(iptr){
    previdx=iptr->i.number;

    while(iptr){
      if(iptr->i.number!=previdx){
        previdx=iptr->i.number;
        idx++;
      }
      if(idx>key)
        break;
      if(idx==key){
         // �饬 ���� �� ���� ��뫠���� ������
         fptr=fl;
         off=0;
         while(fptr){

            if(fptr->f.id==iptr->i.field_id){
               if(keyoff+fptr->f.size<256 && ValidType(fptr->f.type)){
                 switch(fptr->f.type){
                  default:
                     if(minimum)
                       memset((char*)_key+keyoff,0,fptr->f.size);
                     else
                       memset((char*)_key+keyoff,0xff,fptr->f.size);
                     break;
                  case 1:
                  case 2:
                  case 15:
                     if(minimum)
                       memset((char*)_key+keyoff,0xff,fptr->f.size);
                     else
                       memset((char*)_key+keyoff,0x7f,fptr->f.size);
                     break;
                 }
                 keyoff+=fptr->f.size;
                 break;
               }
               break;
            }
            fptr=fptr->next;
         }
       }
       iptr=iptr->next;
     }

   }

};

unsigned short Table::FieldSize  (int n){
  if(fl){
    field_list *tmp=fl;
    int cnt=n;
    do{
      cnt--;
      if(!cnt)
        return tmp->f.size;
      tmp=tmp->next;
    }while(tmp);
    return 0xffff;
  }
  return 0xffff;
};
unsigned short Table::FieldOffset(int n){
  if(fl){
    field_list *tmp=fl;
    int cnt=n;
    do{
      cnt--;
      if(!cnt)
        return tmp->f.offset;
      tmp=tmp->next;
    }while(tmp);
    return 0xffff;
  }
  return 0xffff;

};
unsigned short Table::FieldType(int n){
  if(fl){
    field_list *tmp=fl;
    int cnt=n;
    do{
      cnt--;
      if(!cnt)
        return tmp->f.type;
      tmp=tmp->next;
    }while(tmp);
    return 0xffff;
  }
  return 0xffff;
};

char* Table::FieldName(int n, char * buf){
  if(fl){
    field_list *tmp=fl;
    int cnt=n;
    do{
      cnt--;
      if(!cnt){
        memcpy(buf,tmp->f.name,20);
        buf[20]=0;
        return buf;
      }
      tmp=tmp->next;
    }while(tmp);
    return 0;
  }
  return 0;
};

int Table::Fields(void){
  if(fl){
    field_list *tmp=fl;
    int cnt=0;
    do{
      tmp=tmp->next;
      cnt++;
    }while(tmp);
    return cnt;
  }
  return 0;
};
int Table::SizeForPrint(int n){
  unsigned short type =FieldType(n);
  unsigned short size =FieldSize(n);
  switch(type){
    case 0:
      return size;
    case 1:
    case 15:
      if(size==2)
        return 6;
      else
        return 11;
    case 2:
      if(size==4)
        return 15;
      else
        return 18;
    case 3:
      return 10;
    case 4:
      return 11;
    case 7:
      return 3;
    case 11:
      return size;
    case 14:
      if(size==2)
        return 5;
      else
        return 10;
    case 16:
        return 23;
    default:
      return strlen("_not supported_");

  }
};
char *Table::PrintField(int n, void *R ,char *buf){
  unsigned short type =FieldType(n);
  unsigned short size =FieldSize(n);
  unsigned int off=FieldOffset(n);
  switch(type){
    case 0:
      memcpy(buf,(char*)R+off,size);
      break ;
    case 1:
    case 15:
      switch(size){
      case 1:
        {
          int t=*((unsigned char*)R+off);
          sprintf(buf,"%6d",t);
        }
        break;
      case 2:
        sprintf(buf,"%6d",*(int*)((char*)R+off));
        break;
      case 4:
        sprintf(buf,"%11ld",*(long*)((char*)R+off));
        break;
      }
      break;
    case 2:
      if(size==4)
        sprintf(buf,"%15.2f",*(float*)((char*)R+off));
      else
        sprintf(buf,"%18.2lf",*(double*)((char*)R+off));
      break;
    case 3:
      ((_Date*)((char*)R+off))->D2C(buf);
      break;
    case 4:
      ((_Time*)((char*)R+off))->T2C(buf);
      break;
    case 7:
      if(*((char*)R+off))
        strcpy(buf,"�� ");
      else
        strcpy(buf,"���");
      break;
    case 11:
      strcpy(buf,(char*)R+off);
      break;
    case 14:
      if(size==1){
          int t=*((unsigned char*)R+off);
          sprintf(buf,"%6u",t);
          break;
      }

      if(size==2)
        sprintf(buf,"%6u",*(int*)((char*)R+off));
      else
        sprintf(buf,"%11lu",*(long*)((char*)R+off));
      break;
    case 16:
        sprintf(buf,"%23.2Lf",*(long double*)((char*)R+off));
      break;

    default:
      strcpy(buf,"_not supported_");

  }
  return buf;
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

void Report(Table *Tab,int KeyNum ,char *name)
{
  _LowPrint *lp = new _LowPrint(20,name);
  char *rec=new char [Tab->max_rec_len];
  char key[256];
  Tab->ReadDescription();
  int flds=Tab->Fields();
  Tab->GFirst(rec,key,KeyNum);
  char *buf;
  int sz;
  int hc;
  do{
    for(int i=1;i<= flds;i++){
      buf = new char[(sz=Tab->SizeForPrint(i))+1];
      memset(buf,0,sz+1);
      hc=heapcheck();
      lp->Print("�");
      hc=heapcheck();
      Tab->PrintField(i, rec ,buf);
      hc=heapcheck();
      if(sz>40) buf[40]=0;
      hc=heapcheck();
      lp->Print(buf);
      hc=heapcheck();
      delete buf;
    }
    lp->PrintNL("�");
  }while(!Tab->GNext(rec));
  delete rec;
  delete lp;
}

