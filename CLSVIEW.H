/****************************************************
 *      CLSVIEW.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef CLSVIEV
#define CLSVIEW


struct clsctrs {
  short   id       ;
  short   item     ;
  char    name[31] ;
};

struct clsctrs_0 {
  short   id       ;
  short   item     ;
};


class _ClsView : public _Window
{
 long * result;
 static Table T;                      //("X$Classificator");
 clsctrs CurRec;                      // ���� ⥪�饩 �����
 char LineBuffer[81];                 // ���� ��࠭�
 unsigned long RecPos[25];            // ���ᨢ ���ᮢ ����ᥩ (2 ��࠭��)
 short KeyFilter[2];
 int KeyFilterSize;                   // ����� 䨫���
 int ActivLine;                       // ⥪騩 ������
 int MaxLine;                         // ���ᨬ��쭮� ������⢮ ��ப
 int TotalLine;                       // ���ᨬ��쭮� ������⢮ ��ப ᥩ��
 int FirstY;
 int PageChanged;


 int Next(void);                      // ᫥����� ������
 int Previous(void);                  // �।���� ������
 void PageUp(void);                    // ��࠭�� �����
 void PageDn(void);                    // ��࠭�� ����
 void ToTop(void);                     // � ��砫�
 void ToBottom(void);                  // � �����
 void (*WriteRec)(void * Rec ,char * Buf); // �������� ������ � ����
 void ReShowLine(int Line);           // ����ᮢ��� �����
 void ReShowAll(void);                // ����ᮢ���
 void Repaint(void);
 int RecordOK(void *Rec);
 void Paint(void);

 public:

 _ClsView( long * _result
           ,short ID
           ,int _x
           ,int _y
           ,int _w
           ,int _h
           ,int _top
           ,char *_name="ClsView"
 );

 ~_ClsView();

 int MHandle(_Event *E);               // 横� ��ᬮ�� (�����. ��� ������)
 int Handle(_Event *E);               // 横� ��ᬮ�� (�����. ��� ������)
};

#endif
