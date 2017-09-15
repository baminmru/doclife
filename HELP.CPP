/****************************************************
 *       HELP.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>

extern short HelpLock;
static char *idx_file="HELP_IDX.$$$";
static char *idx_proto="###%d\n";
char *code =
"HelpCodeString";
int _Help::h=-1;
_Help::_Help(char *file){
  h=-1;
  h=open(file,O_RDONLY|O_BINARY);
  if(h==-1){
    Error("Help file not found\n");
  }
  char hlp[5];
  read(h,hlp,4);
  if(strncmp(hlp,idx_file,4)){
    close(h);
    h=-1;
  }
  //Create("nbhlp.txt","nbhlp.hlp");
};

_Help::~_Help(){
  if(h!=-1)
    close(h);
};


/*
  структура файла
  -сигнатура
  -адрес индекса
  -количество индексов

  //// информация
  [длина строки , строка]
  ...
  //// индекс
  [Id, к-во строк, позиция]
  ...
*/
struct help_idx{
  short ID;
  short lines;
  long  pos;
};

void  _Help::Build(int ID){
  if(h==-1){
    HelpLock=0;
    return;
  }

  if(!ID){
    HelpLock=0;
    return;
  }
  // тупой поиск
  long idx_pos;
  short cnt;
  help_idx idx;
  char* buf[20];

  lseek(h,4,SEEK_SET);
  read(h,&idx_pos,4);
  read(h,&cnt,2);
  lseek(h,idx_pos,SEEK_SET);
  for(int i=0;i<cnt;i++){
    read(h,&idx,sizeof(help_idx));
    if(idx.ID==ID)
     break;
  }
  if(idx.ID==ID){
    lseek(h,idx.pos,SEEK_SET);
    memset(buf,0,sizeof(buf));
    for(i=0;i<idx.lines;i++){
       read(h,&cnt,2);
       buf[i]= new char[cnt];
       read(h,buf[i],cnt);
       for(int j =0; j<cnt-1;j++)
          buf[i][j]=buf[i][j]^code[13-j%14];
    }

    short result;
    _MesWin * MW = new _MesWin(0,0,ST_HELP,&result,buf[0 ], buf[1 ], buf[2 ], buf[3 ], buf[4 ], buf[5 ], buf[6 ], buf[7 ], buf[8 ], buf[9 ],
                                                     buf[10], buf[11], buf[12], buf[13], buf[14], buf[15], buf[16], buf[17], buf[18], buf[19]);
    MW->CenterScr();
    for(i=0;i<idx.lines;i++)
      delete buf[i];

    MW->Exec();
    delete MW;
  }
  HelpLock=0;
  return;

};


void _Help::Unpack(char *dest_file){
  if(h==-1){
    return;
  }
  FILE *out =fopen(dest_file,"w");
  if(!out)
    return;
  // тупой поиск
  long idx_pos;
  short cnt,cnt2;
  help_idx idx;
  static char buf[81];

  lseek(h,4,SEEK_SET);
  read(h,&idx_pos,4);
  read(h,&cnt,2);

  for(int k=0;k<cnt;k++){
    lseek(h,idx_pos,SEEK_SET);
    read(h,&idx,sizeof(help_idx));
    idx_pos+=sizeof(help_idx);
    fprintf(out,idx_proto,idx.ID);
      lseek(h,idx.pos,SEEK_SET);
      memset(buf,0,sizeof(buf));
      for(int i=0;i<idx.lines;i++){
         read(h,&cnt2,2);
         read(h,buf,cnt2);
         for(int j =0; j<cnt2-1;j++)
            buf[j]=buf[j]^code[13-j%14];
         fprintf(out,"%s\n",buf);
      }
  }
  fclose(out);
};

void _Help::Create(char *text_file, char *dest_file){
  FILE * in=fopen(text_file,"r");
  if(!in)
    return;
  int out = open(dest_file,O_WRONLY|O_BINARY|O_CREAT|O_TRUNC,S_IWRITE);
  if(out==-1){
    fclose(in);
    return;
  }
  int idx=open(idx_file,O_WRONLY|O_BINARY|O_CREAT|O_TRUNC,S_IWRITE);
  if(idx==-1){
    fclose(in);
    close(out);
    unlink(dest_file);
    return;
  }
  static char buf[83];
  short slen=0,cnt=0;

  write(out,idx_file,10);
  help_idx id;
  long pos=10;
  while(fgets(buf,82,in)){
    if(!strncmp(buf,idx_proto,3)){
      if(pos>10)
        write(idx,&id,sizeof(id));
      id.pos=pos;
      id.lines=0;
      sscanf(buf,idx_proto,&id.ID);
      cnt++;
    }else{
      slen=strlen(buf);
      buf[slen-1]=0;           // 12\n  len=3 [2]=0 --> 12\0
      write(out,&slen,2);
      pos+=2;
      for(int j =0; j<slen-1;j++)
        buf[j]=buf[j]^code[13-j%14];
      write(out,buf,slen);
      pos+=slen;
      id.lines++;
    }
  }
  if(id.lines)
    write(idx,&id,sizeof(id));
  else
    cnt--;

  close(idx);
  fclose(in);

  // записываем заголовок файла
  lseek(out,4,SEEK_SET);
  write(out,&pos,4);
  write(out,&cnt,2);

  // добавляем в конец индекс
  idx=open(idx_file,O_RDONLY|O_BINARY);
  long len=filelength(idx);
  long cur;
  lseek(out,0,SEEK_END);
  while(len){
    if(len>80)
      cur=80;
    else
      cur=len;
    read(idx,buf,cur);
    write(out,buf,cur);
    len-=cur;
  }

  close(out);
  close(idx);
  unlink("HELP_IDX.$$$");
};

