/****************************************************
 *          F20.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef F20_HEADER
#define F20_HEADER

#define E_STYLE20 ST_WINDOW
#define V_STYLE20 ST_FILEVIEW

extern file_20 * rf20;
extern Table *T20;

void Init20(void);
void Close20(void);
void AddF20( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
