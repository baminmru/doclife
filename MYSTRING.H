/****************************************************
 *     MYSTRING.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef MYSTRING
#define MYSTRING

void  StrClear(char *s, int l, char f=' ');
char* StrTrim (char *s, char *buf=0);
char* StrUpper(char *s, char *buf=0);
/*
  mode =  0 - Cut
  mode =  1 - Wrap
  mode = 'C' - Fill with 'C'
*/
char* StrL(char *s,char *buf, int l, int mode=0);
char* StrR(char *s,char *buf, int l, int mode=0);
char* StrC(char *s,char *buf, int l, int mode=0);
char* StrJ(char *s,char *buf, int l, int mode=0);

int   StrEmpty(char *s);
int   StrWrap (char *s, int l);
int   StrRead (char *s ,char *f, ...);
int   StrForm (char *s, char *buf[], int l,int array);

char *MakeTemp(char *ext="TMP");

#endif
