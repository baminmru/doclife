/****************************************************
 *          F27.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef F27_HEADER
#define F27_HEADER

#define E_STYLE27 ST_WINDOW
#define V_STYLE27 ST_FILEVIEW

extern file_27 * rf27;
extern Table *T27;

void Init27(void);
void Close27(void);
void AddF27( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
