/****************************************************
 *          F26.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef F26_HEADER
#define F26_HEADER

#define E_STYLE26 ST_WINDOW
#define V_STYLE26 ST_FILEVIEW

extern file_26 * rf26;
extern Table *T26;

void Init26(void);
void Close26(void);
void AddF26( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
