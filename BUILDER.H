/****************************************************
 *      BUILDER.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
_Window * BuildCell(Table * T, int field, void * rec, short x,short* y ,short w, int style);
void BuildForm(_Window * w, Table *t, void * rec, short style, short *f_list);
void InitRecord(Table *t , void *rec);
