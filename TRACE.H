/****************************************************
 *        TRACE.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef TRACE

extern int TraceFlag;
void  Trace (char *File, int line ,char *message);
void  Debug (char *format,...);
void  Error (char *format,...);
void  SetTrace(int x);

#ifdef NO_TRACE
#define TRACE(x)
#else
#define TRACE(x)  Trace(__FILE__,__LINE__,x)
#endif

#endif

