/****************************************************
 *        BBG.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
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


// получить сообщение об ошибке по номеру ошибки
char * Table::Error(int code){
    err_list em[]={
    {"Операция успешно завершена", 0}
    ,{"Неверный код операции", 1}
    ,{"Ошибка ввода-ввывода", 2}
    ,{"Файл не открыт", 3}
    ,{"Запись с заданным ключем не обнаружена", 4}
    ,{"Не разрешены повторяющиеся ключи", 5}
    ,{"Неверный номер ключа", 6}
    ,{"Несоответствие номера ключа", 7}
    ,{"Ошибка позиционирования", 8}
    ,{"Достигнут конец файла", 9}
    ,{"Попытка преобразовать не модифицируемый ключ", 10}
    ,{"Неверное имя файла", 11}
    ,{"Файл не найден", 12}
    ,{"Ошибка расширеного файла", 13}
    ,{"Ошибка открытия PRE-IMAGE файла", 14}
    ,{"Ошибка работы с PRE-IMAGE файлом",15}
    ,{"Ошибка диска при увеличении размера фала", 16}
    ,{"Ошибка закрытия файла", 17}
    ,{"Диск полон", 18 }
    ,{"Невостановимая ошибка (используйте butil -recover)", 19}
    ,{"BTRIEV не загружен !!!",20}
    ,{"Буфер ключа слишком мал", 21}
    ,{"Буфер записи слишком мал", 22}
    ,{"Позиционный блок слишком мал",23}
    ,{"Неверная длина страници", 24}
    ,{"Ошибка ввода-вывода при создании", 25}
    ,{"Неверное число сегментов ключа", 26}
    ,{"Неверное описание сегмента ключа", 27}
    ,{"Не правильно задана длина записи", 28}
    ,{"Неверная длина ключа", 29}
    ,{"Файл не является базой данных", 30}
    ,{"Файл уже определен как расширенный", 31}
    ,{"Ошибка расширения файла", 32}
    ,{"???", 33}
    ,{"Неверное расширеное имя", 34}
    ,{"Неверно задан директорий", 35}
    ,{"Ошибка транзакции", 36}
    ,{"Начата транзакция с другой станции", 37}
    ,{"Ошибка при работе с транзакционным файлом", 38}
    ,{"Транзакция не начиналась", 39}
    ,{"Слишком много файлов использовано в транзакции", 40}
    ,{"Операция не доступна", 41}
    ,{"Незавершенный ускоренный доступ", 42}
    ,{"Неверный адрес записи", 43}
    ,{"Пустой ключ", 44}
    ,{"Несовместимые флаги ключа", 45}
    ,{"Доступ к файлу запрещен", 46}
    ,{"Максимальное количество открытых файлов", 47}
    ,{"Неверно определена альтернативная последовательность", 48}
    ,{"Ошибка типа ключа", 49}
    ,{"Хозяин уже установлен", 50}
    ,{"Неверное имя хозяина", 51}
    ,{"Ошибка записи буфера", 52}
    ,{"Устаревший интерфейс", 53}
    ,{"Ошибка чтения записи переменной длины", 54}
    ,{"Неправильное использование автоинкрементного ключа", 55}
    ,{"Незавершенный индекс", 56}
    ,{"Ошибка работы с расширенной памятью", 57}
    ,{"Буфер для упаковки записей слишком мал", 58}
    ,{"Файл уже существует", 59}
    ,{"Счетчик записей переполнен ???", 60}
    ,{"Рабочее пространство для расширеных операций слишком мало", 61}
    ,{"Неверное описание расширеного запроса", 62}
    ,{"Не правильно определен буфер для расширенного INSERT", 63}
    ,{"Достигнут предел фильтрации", 64}
    ,{"Неправильное смещение поля", 65}
    ,{"Транзакция автоматически завершена", 76}
    ,{"Встречная блокировка программ", 78}
    ,{"Конфликт", 80}
    ,{"Ошибка блокировки", 81}
    ,{"Потеряна позиция", 82}
    ,{"Чтение вне транзакции", 83}
    ,{"Запись заблокирована", 84}
    ,{"Файл заблокирован", 85}
    ,{"Таблица файлов полна", 86}
    ,{"Таблица указателей файлов полна", 87}
    ,{"Несовместимый режим открытия файла", 88}
    ,{"Ошибка сервера", 91}
    ,{"Таблица транзакций полна", 92}
    ,{"Несовместиный тип блокировки", 93}
    ,{"Попытка записи в неразрешенный директорий", 94}
    ,{"Продолжение сеанса невозможно", 95}
    ,{"Ошибка сети", 96}
    ,{"Сообщение слишком мало", 97}
    ,{"Внутренняя ошибка транзакции", 98}
    ,{"Ошибка не опознана",-1}
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
    // создаем список полей файла
    int id=St.GetFileID(name);
    field_ddf_1 field_key;
    field_ddf field;
    field_key.file_id=id;
    if(St.field_file->GGE(&field,&field_key,1)) return ;
    while(field_key.file_id==id){
      fl->add(&field,&fl);
      if(St.field_file->GNext(&field,&field_key)) break;
    }

    // создаем список индексов
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
         // ищем поле на которе ссылается индекс
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
         // ищем поле на которе ссылается индекс
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
        strcpy(buf,"Да ");
      else
        strcpy(buf,"Нет");
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
      lp->Print("│");
      hc=heapcheck();
      Tab->PrintField(i, rec ,buf);
      hc=heapcheck();
      if(sz>40) buf[40]=0;
      hc=heapcheck();
      lp->Print(buf);
      hc=heapcheck();
      delete buf;
    }
    lp->PrintNL("│");
  }while(!Tab->GNext(rec));
  delete rec;
  delete lp;
}

