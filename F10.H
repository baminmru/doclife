/****************************************************
 *          F10.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef F10_HEADER
#define F10_HEADER

#define E_STYLE10 ST_WINDOW
#define V_STYLE10 ST_FILEVIEW

extern file_10 * rf10;
extern Table *T10;

void Init10(void);
void Close10(void);
void AddF10( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
