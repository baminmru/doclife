/****************************************************
 *      PROTECT.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
typedef short BOOL;
#define FALSE 0
#define TRUE  1

BOOL ProtectNotOK(char *Name);
BOOL ProtectOK(char *Name);
short InstallEXE(char *Name);
void SavePasswd(unsigned char *pwd);
int CmpPasswd(unsigned char *pwd);
