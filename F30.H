/****************************************************
 *          F30.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef F30_HEADER
#define F30_HEADER

#define E_STYLE30 ST_WINDOW		
#define V_STYLE30 ST_FILEVIEW

extern file_30 * rf30;
extern Table *T30;

void Init30(void);
void Close30(void);
void AddF30( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif
 
 
