/****************************************************
 *          F22.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef F22_HEADER
#define F22_HEADER

#define E_STYLE22 ST_WINDOW
#define V_STYLE22 ST_FILEVIEW

extern file_22 * rf22;
extern Table *T22;

void Init22(void);
void Close22(void);
void AddF22( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
