/****************************************************
 *      QUEUE.CPP                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#define NO_TRACE
#include "common.h"
#undef NO_TRACE

_SystemQueue::_SystemQueue(void){
  Top=Total=Bottom=0;
};
_SystemQueue::~_SystemQueue(void){
};
int _SystemQueue::AddMessage(long msg){
    TRACE("_SystemQueue::AddMessage");
    if(Total<SQ_MAX){
      Total++;
      Top++;
      Top=Top%SQ_MAX;
      SQ[Top].msg.L=msg;
      SQ[Top].Type=evSystem;
      return 1;
    }else
      return 0;
};
int _SystemQueue::AddMessage(short type, long msg){
    TRACE("_SystemQueue::AddMessage");
    if(Total<SQ_MAX){
      Total++;
      Top++;
      Top=Top%SQ_MAX;
      SQ[Top].msg.L=msg;
      SQ[Top].Type=type;
      return 1;
    }else
      return 0;
};

_Event* _SystemQueue::GetMessage(void){
  TRACE("_SystemQueue::GetMessage");
  if(Total){
    Total--;
    Bottom++;
    Bottom=Bottom%SQ_MAX;
    return &(SQ[Bottom]);
  }else
    return 0;
}
