/****************************************************
 *          F14.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef F14_HEADER
#define F14_HEADER

#define E_STYLE14 ST_WINDOW
#define V_STYLE14 ST_FILEVIEW

extern file_14 * rf14;
extern Table *T14;

void Init14(void);
void Close14(void);
void AddF14( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
