/****************************************************
 *          F28.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef F28_HEADER
#define F28_HEADER

#define E_STYLE28 ST_WINDOW		
#define V_STYLE28 ST_FILEVIEW

extern file_28 * rf28;
extern Table *T28;

void Init28(void);
void Close28(void);
void AddF28( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif
 
 
