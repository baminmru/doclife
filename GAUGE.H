/****************************************************
 *        GAUGE.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef GAUGE
#define GAUGE

class _Gauge : public _Window {
  public:
   long info;
   long Range;
   void SetRange(long NewRange);
   void Paint(void);
   _Gauge(short _x, short _y, short _w, short _style, long _Range , char *name="Gauge");
   ~_Gauge();
  int Handle(_Event *E);
  void Update( long info);
  void Halt( void );
};
#endif
