/****************************************************
 *          F19.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef F19_HEADER
#define F19_HEADER

#define E_STYLE19 ST_WINDOW
#define V_STYLE19 ST_FILEVIEW

extern file_19 * rf19;
extern Table *T19;

void Init19(void);
void Close19(void);
void AddF19( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
