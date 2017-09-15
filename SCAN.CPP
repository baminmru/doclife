/****************************************************
 *       SCAN.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/

#include <stdio.h>
#include <dir.h>
#include <dos.h>
#include <string.h>

void read_my_file(char *);

class filelist{
     public:
       static filelist *FileHead;
       char n[50];
       filelist *next;
       ~filelist(void){
          if(next)
           delete next;
       };
       filelist(char *name);
};

filelist *filelist::FileHead=0;

filelist::filelist( char *name){
      filelist *tmp=FileHead;
      if(!FileHead)
        FileHead=this;
      else{
        while(tmp->next){
          tmp=tmp->next;
        };
        tmp->next=this;
      }
      next=0;
      strcpy(n,name);

};

static int check(char *name){
  if(name[8]!='.')
    return 0;
  int ok=1;
  for(int i=0; i<11 ;i++){
    if(i!=8)
      if(name[i]<'0' || name[i]>'9')
        return 0;
  }
  return 1;
};

void onedir(void)
{
   struct ffblk ffblk;
   int done;

   if(filelist::FileHead){
      delete filelist::FileHead;
      filelist::FileHead=0;
   }

   int idx=0;
   // сканируем для файлов типа 1
   done = findfirst("????????.??a",&ffblk,0);
   while (!done)
   {
      if(check(ffblk.ff_name))
        new filelist( ffblk.ff_name);

      done = findnext(&ffblk);
   }

   done = findfirst("????????.??b",&ffblk,0);
   while (!done)
   {
      if(check(ffblk.ff_name))
        new filelist(ffblk.ff_name);

      done = findnext(&ffblk);
   }

   filelist *tmp;
   filelist *ttt=filelist::FileHead;
   char buf[50];
   while(ttt){
     tmp=ttt->next;
     while(tmp){

       if(strcmp(ttt->n,tmp->n)>0){
          strcpy(buf,ttt->n);
          strcpy(ttt->n,tmp->n);
          strcpy(tmp->n,buf);
       }
       tmp=tmp->next;
     }
     ttt=ttt->next;
   }
   ttt=filelist::FileHead;
   while(ttt){
     read_my_file(ttt->n);
     ttt=ttt->next;
   }

   if(filelist::FileHead) delete filelist::FileHead;

   filelist::FileHead=0;
};

void scan(char* dirname)
{
   if(dirname)
    chdir(dirname);

   struct ffblk ffb;
   int done;

   int idx=0;
   onedir();

   // сканируем для файлов типа 1
   done = findfirst("*.*",&ffb,FA_DIREC);
   while (!done)
   {
      if(ffb.ff_attrib!=FA_DIREC)
       goto next_step;

      if(!strcmp(ffb.ff_name,".") ||!strcmp(ffb.ff_name,"..")  )
        goto next_step;
      printf("Dir:%s\n",ffb.ff_name);
      scan(ffb.ff_name);

     next_step:
      done = findnext(&ffb);
   }
   if(dirname)
    chdir("..");

};

