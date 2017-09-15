/****************************************************
 *          DDF.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
// FILE.DDF
struct file_ddf{
  short id;
  char name[20];
  char path[64];
  char flags;
};

struct file_ddf_0
{
  short id;           //no d no m
};

struct file_ddf_1
{
  char name[20];      //no d m
};

// FIELD.DDF
struct field_ddf{
  short int id;
  short int file_id;
  char name[20];
  char type;
  short int offset;
  short int size;
  char dec;         // ??
  char flags[2];    // ??
};

struct field_ddf_0{
  short int id;              //no d no m
};

struct field_ddf_1{
  short int file_id;         //d no m
};

struct field_ddf_2{
  char name[20];             //d m
};

struct field_ddf_3{
  short int file_id;         //no d m
  char name[20];             //no d m
};

struct index_ddf
{
  short file_id;
  short field_id;
  short number;
  short part;
  short flags;
};


struct index_ddf_0
{
  short file_id;
  short number;
  short part;
};

extern char *KeyTypeName[];
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

