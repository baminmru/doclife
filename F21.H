/****************************************************
 *          F21.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef F21_HEADER
#define F21_HEADER

#define E_STYLE21 ST_WINDOW
#define V_STYLE21 ST_FILEVIEW

extern file_21 * rf21;
extern Table *T21;

void Init21(void);
void Close21(void);
void AddF21( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
