/****************************************************
 *       TEST.CPP                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include "date.h"


int main(void){
  _Date b;
  char cb[80];
  printf("������ ���� (dd.mm.yy) :");
  scanf("%s",cb);
  b.C2D(cb);
  int h= open("prevdate.dat",O_WRONLY|O_BINARY|O_CREAT|O_TRUNC,S_IWRITE);
  if(h==-1) return 0;
  write(h,&b,4);
  close(h);
  return 0;
}
