/****************************************************
 *      MYNEW.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include <malloc.h>

#define NO_TRACE
#include "common.h"
#undef NO_TRACE

static int First=0;
static short Attantion =0;
long MyNewCnt=0;
long MaxHeap;

void * operator new(unsigned size){
  short tmp;
  if(!First){
    First=1;
    MaxHeap=coreleft();
  }

  if(heapcheck()==-1){
    Error("Heap CORRAPTED !!!\n");
    exit(0);
  }

  TRACE("MYNEW");
  if(!size){
    Error("Error: ZERO block !!!\n");
    return 0;
  }else{
    tmp=(size+4)/16;
    if((size+4)%16) tmp++;
    MyNewCnt+=tmp;
    //Debug("Allocator: P=%5d (%5ld) Size=%d\n",tmp,MyNewCnt,size);
    void * ptr;
    int res;
    ptr=0;
    ptr=malloc(size);
    if(!ptr){
      Error("Error: Can't allocate %d bytes of memory !!!\n" ,size);
      exit(0);
    }
    return ptr;
  }
};


void operator delete(void * ptr){
  short tmp;
  TRACE("MYDELETE");

  if(ptr){
    tmp=((unsigned short*)ptr)[-2];
    MyNewCnt-=tmp;
    free(ptr);
    ptr=0;
  } else
    Error("Error: Delete for zero pointer !!!\n");
  if(!MyNewCnt){
    Error("\nAll new calls balansed!\n");
    if(MaxHeap!=coreleft())
      Error("MaxHeap =%ld, but Coreleft = %ld\n",MaxHeap,coreleft());
  }else
    if(MyNewCnt<0)
      Error("Error: disbalansed delete !!!\n");
  if(heapcheck()==-1){
    Error("Heap CORRAPTED !!!\n");
    abort();
  }


  //Debug("Allocator:-P=%5d (%5ld)\n",tmp,MyNewCnt);

};


