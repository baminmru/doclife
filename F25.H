/****************************************************
 *          F25.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef F25_HEADER
#define F25_HEADER

#define E_STYLE25 ST_WINDOW
#define V_STYLE25 ST_FILEVIEW

extern file_25 * rf25;
extern Table *T25;

void Init25(void);
void Close25(void);
void AddF25( void (*CallBackFunc)(_Event *,void *), _Window * parent=0);

#endif

 
