/****************************************************
 *       BBG1.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include <string.h>
#include "common.h"


/***************************************************************

										Работа с BTRV

****************************************************************/



// 0
int Table::Open  ( char * _file_name, char *_owner, int _access ){
  cur_rec_len=strlen(_owner)+1;
  status=BTRV( 0,pos_blk,_owner,&cur_rec_len,_file_name,_access);
	if(!status)
		opened=1;
	return status;
};

int Table::Open  ( char *_owner, int _access ){
  char _file_name[65];
  memset(_file_name,0,65);
  if(!St.GetFilePath(name,_file_name))
    return 2;
  cur_rec_len=strlen(_owner)+1;
  status=BTRV( 0,pos_blk,_owner,&cur_rec_len,_file_name,_access);
	if(!status)
		opened=1;
	return status;
};

//1
int Table::Close(void){
  status=BTRV( 1,pos_blk,0,0,0,0);
	if(!status)
		opened=0;
	return status;
};

//2
int Table::Insert( void * rec, void * _key, int new_key){
	set_key(new_key);
	cur_rec=(char*)rec;
	cur_rec_len=max_rec_len;
  status=BTRV( 2,pos_blk,cur_rec,&cur_rec_len,_key,cur_key_num);
  if(key!=_key)
    memcpy(key,_key,256);
	return status;
};

//2
int Table::Insert( void * rec,  int new_key){
	set_key(new_key);
	cur_rec=(char*)rec;
	cur_rec_len=max_rec_len;
  status=BTRV( 2,pos_blk,cur_rec,&cur_rec_len,key,cur_key_num);
	return status;
};

//3
int Table::Update( void * rec, int mode,int _new_key){
	set_key(_new_key);
	cur_rec=(char*)rec;
	cur_rec_len=max_rec_len;
  status=BTRV( 3+mode,pos_blk,cur_rec,&cur_rec_len,key,cur_key_num);
	return status;
};

//4
int Table::Delete( void ){
	cur_rec_len=max_rec_len;
  status=BTRV( 4,pos_blk,cur_rec,&cur_rec_len,key,cur_key_num);
	return status;
};

// получение записи с использованием ключа
//5
int Table::GEQ		( void * rec, int new_key, int mode){
	return GEQ(rec,key,new_key,mode);
};

//5
int Table::GEQ		( void * rec, void * _key, int new_key, int mode){
	set_key(new_key);
	cur_rec=(char*)rec;
	cur_rec_len=max_rec_len;
  status=BTRV( 5+mode,pos_blk,cur_rec,&cur_rec_len,_key,cur_key_num);
  if(_key!=key)
    memcpy(key,_key,256);
	return status;
};

//6
int Table::GNext ( void * rec, int mode){
	cur_rec=(char*)rec;
	cur_rec_len=max_rec_len;
  status=BTRV( 6+mode,pos_blk,cur_rec,&cur_rec_len,key,cur_key_num);
	return status;
};

//6
int Table::GNext ( void * rec, void *_key, int mode){
	cur_rec=(char*)rec;
	cur_rec_len=max_rec_len;
  status=BTRV( 6+mode,pos_blk,cur_rec,&cur_rec_len,_key,cur_key_num);
  if(_key!=key)
    memcpy(key,_key,256);
	return status;
};


//7
int Table::GPrev ( void * rec, int mode){
	cur_rec=(char*)rec;
	cur_rec_len=max_rec_len;
  status=BTRV( 7+mode,pos_blk,cur_rec,&cur_rec_len,key,cur_key_num);
	return status;
};

//7
int Table::GPrev ( void * rec, void *_key, int mode){
	cur_rec=(char*)rec;
	cur_rec_len=max_rec_len;
  status=BTRV( 7+mode,pos_blk,cur_rec,&cur_rec_len,_key,cur_key_num);
  if(_key!=key)
    memcpy(key,_key,256);
	return status;
};


//8
int Table::GGT		( void * rec, int new_key, int mode){
	return GGT(rec,key,new_key,mode);
};

//8
int Table::GGT		( void * rec, void * _key, int new_key, int mode){
	set_key(new_key);
	cur_rec=(char*)rec;
	cur_rec_len=max_rec_len;
  status=BTRV( 8+mode,pos_blk,cur_rec,&cur_rec_len,_key,cur_key_num);
  if(_key!=key)
    memcpy(key,_key,256);
	return status;
};

//9
int Table::GGE		( void * rec, int new_key, int mode){
	return GGE(rec,key,new_key,mode);
};

//9
int Table::GGE		( void * rec, void * _key, int new_key, int mode){
	set_key(new_key);
	cur_rec=(char*)rec;
	cur_rec_len=max_rec_len;
  status=BTRV( 9+mode,pos_blk,cur_rec,&cur_rec_len,_key,cur_key_num);
  if(_key!=key)
    memcpy(key,_key,256);
	return status;
};

//10
int Table::GLT		( void * rec, int new_key, int mode){
	return GLT(rec,key,new_key,mode);
};

//10
int Table::GLT		( void * rec, void * _key, int new_key, int mode){
	set_key(new_key);
	cur_rec=(char*)rec;
	cur_rec_len=max_rec_len;
  status=BTRV( 10+mode,pos_blk,cur_rec,&cur_rec_len,_key,cur_key_num);
  if(_key!=key)
    memcpy(key,_key,256);
	return status;
};

//11
int Table::GLE		( void * rec, int new_key, int mode){
	return GLE(rec,key,new_key,mode);
};

//11
int Table::GLE		( void * rec, void * _key, int new_key, int mode){
	set_key(new_key);
	cur_rec=(char*)rec;
	cur_rec_len=max_rec_len;
  status=BTRV( 11+mode,pos_blk,cur_rec,&cur_rec_len,_key,cur_key_num);
  if(_key!=key)
    memcpy(key,_key,256);
	return status;
};

//12
int Table::GFirst		( void * rec, int new_key, int mode){
	return GFirst(rec,key,new_key,mode);
};

//12
int Table::GFirst ( void * rec, void * _key, int new_key, int mode){
	set_key(new_key);
	cur_rec=(char*)rec;
	cur_rec_len=max_rec_len;
  status=BTRV( 12+mode,pos_blk,cur_rec,&cur_rec_len,_key,cur_key_num);
  if(_key!=key)
    memcpy(key,_key,256);
	return status;
};

//13
int Table::GLast		( void * rec, int new_key, int mode){
	return GLast(rec,key,new_key,mode);
};

//13
int Table::GLast  ( void * rec, void * _key, int new_key, int mode){
	set_key(new_key);
	cur_rec=(char*)rec;
	cur_rec_len=max_rec_len;
  status=BTRV( 13+mode,pos_blk,cur_rec,&cur_rec_len,_key,cur_key_num);
  if(_key!=key)
    memcpy(key,_key,256);
	return status;
};

//22
int Table::GPos  ( void * pos ){
		cur_rec_len=sizeof(cur_pos);
    status=BTRV( 22,pos_blk,&cur_pos,&cur_rec_len,key,0);
		if(pos)
			memcpy(pos,&cur_pos,sizeof(cur_pos));
		return status;
};

//23
int Table::GDir  ( void * rec, void * pos, int new_key, int mode){
    set_key(new_key);
		if(pos)
			memcpy(&cur_pos,pos,sizeof(cur_pos));
		cur_rec=(char*)rec;
		cur_rec_len=max_rec_len;
		memcpy(cur_rec,&cur_pos,sizeof(cur_pos));
    status=BTRV( 23+mode,pos_blk,cur_rec,&cur_rec_len,key,cur_key_num);
		return status;

};

// Получить текущую запись
int Table::GCur  ( void * rec, int mode){

	if(GPos())
		return status;
  return GDir(rec,0,-1,mode);
};



//24
int Table::SNext ( void * rec, int mode){
		cur_rec=(char*)rec;
		cur_rec_len=max_rec_len;
    status=BTRV( 24+mode,pos_blk,cur_rec,&cur_rec_len,key,0);
		return status;
};

//27 (0)
int Table::UnlockS(void){
		cur_rec_len=4;
    status=BTRV( 27,pos_blk,&cur_pos,&cur_rec_len,key,0);
		return status;
};

int Table::UnlockS(long pos){
		cur_rec_len=4;
    cur_pos=pos;
    status=BTRV( 27,pos_blk,&cur_pos,&cur_rec_len,key,0);
		return status;
};


//27 (-1)
int Table::UnlockSfromM(void *pos){
		if(pos)
			memcpy(&cur_pos,pos,sizeof(cur_pos));
		cur_rec_len=4;
    status=BTRV( 27,pos_blk,&cur_pos,&cur_rec_len,key,-1);
		return status;
};

//27 (-2)
int Table::UnlockM(void){
		cur_rec_len=4;
    status=BTRV( 27,pos_blk,&cur_pos,&cur_rec_len,key,-2);
		return status;
};

//28
int Table::SetOwner(char *owner){
    cur_rec_len=strlen(owner)+1;
    status=BTRV( 29,pos_blk,owner,&cur_rec_len,owner,0);
		return status;
};

//29
int Table::ClearOwner(char *owner){
    cur_rec_len=strlen(owner)+1;
    status=BTRV( 30,pos_blk,owner,&cur_rec_len,owner,0);
		return status;
};


//33
int Table::SFirst( void * rec, int mode){
		cur_rec=(char*)rec;
		cur_rec_len=max_rec_len;
    status=BTRV( 33+mode,pos_blk,cur_rec,&cur_rec_len,key,0);
		return status;
};

//34
int Table::SLast ( void * rec, int mode    ){
		cur_rec=(char*)rec;
		cur_rec_len=max_rec_len;
    status=BTRV( 34+mode,pos_blk,cur_rec,&cur_rec_len,key,0);
		return status;
};

//35
int Table::SPrev ( void * rec, int mode){
		cur_rec=(char*)rec;
		cur_rec_len=max_rec_len;
    status=BTRV( 35+mode,pos_blk,cur_rec,&cur_rec_len,key,0);
		return status;
};

int Table::CritBegin(void){
  if(crit)
    return 1;
  else
    crit=St.CritLock(name);

  if(crit)
    return 1;
  else
    return 0;
};

int Table::CritEnd  (void){
  if(crit){
    if(St.CritUnLock(crit)){
      crit=0;
      return 1;
    }else
      return 0;
  }else
    return 1;
};

