/****************************************************
 *       MESWIN.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef MESWIN
#define MESWIN

class _MesWin:public _Window {
  short *result;
  char *l[20];
  public:
  _MesWin(
    short _x,    short _y,     short _style, short *result,
    char * l0,    char * l1=0,  char * l2=0,  char * l3=0,  char * l4=0,
    char * l5=0,  char * l6=0,  char * l7=0,  char * l8=0,  char * l9=0,
    char * l10=0, char * l11=0, char * l12=0, char * l13=0, char * l14=0,
    char * l15=0, char * l16=0, char * l17=0, char * l18=0, char * l19=0
  );
  ~_MesWin(void);
  int Handle(_Event* E);

};
short MessageBox(  char *l0,    char *l1=0,  char *l2=0,  char *l3=0,  char *l4=0,
                   char *l5=0,  char *l6=0,  char *l7=0,  char *l8=0,  char *l9=0,
                   char *l10=0, char *l11=0, char *l12=0, char *l13=0, char *l14=0,
                   char *l15=0, char *l16=0, char *l17=0, char *l18=0, char *l19=0
                 );


#endif

