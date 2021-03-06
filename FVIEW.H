/****************************************************
 *        FVIEW.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef FVIEV
#define FVIEW

#define MAX_STRING_WIDTH 256
#define MAX_LINES 25

class _FileView : public _Window
{
 public:
 Table *T;
 char *CurRec;                        // ���� ⥪�饩 �����
 char LineBuffer[MAX_STRING_WIDTH+1]; // ���� ��࠭�
 unsigned long RecPos[MAX_LINES];     // ���ᨢ ���ᮢ ����ᥩ
 char *Header[5];
 int KeyNum;
 char KeyFilter[256];                 // ���祭�� 䨫��� ����
 int KeyFilterSize;                   // ����� 䨫���
 int ActivLine;                       // ⥪騩 ������
 int MaxLine;                         // ���ᨬ��쭮� ������⢮ ��ப
 int TotalLine;                       // ���ᨬ��쭮� ������⢮ ��ப ᥩ��
 int FirstY;
 int PageChanged;
 int LineX;
 int LockMode;
 int Next(void);                      // ᫥����� ������
 int Previous(void);                  // �।���� ������
 void PageUp(void);                    // ��࠭�� �����
 void PageDn(void);                    // ��࠭�� ����
 void ToTop(void);                     // � ��砫�
 void ToBottom(void);                  // � �����
 void (*WriteRec)(void * Rec ,char * Buf); // �������� ������ � ����
 void ReShowLine(int Line);           // ����ᮢ��� �����
 void ReShowAll(void);                // ����ᮢ���
 int  RecordOK(void * Rec);            // ������ ����᪠���� 䨫��஬
 void Repaint(int lock = 1);
 void SetHeader(short idx, char *HeaderLine);


 _FileView(  Table * _T
           ,int     _KeyNum
           ,void *  _KeyFilter
           ,int     _KeyFilterSize
           ,void (* _WriteRec)(void * ,char * )
           ,int _x
           ,int _y
           ,int _w
           ,int _h
           ,int _top
           ,short _style
           ,char *_name="FileView"
 );

 ~_FileView();

 void Paint(void);
 int MHandle(_Event *E);
 int Handle(_Event *E);               // 横� ��ᬮ�� (�����. ��� ������)
 unsigned long CurRecPos(void);       // ������� ������ ⥪�饩 ����� � 䠩��
 Table *GetTable(void) {return T;}
 void JumpToRec(void * Rec);
 void JumpToKey(void * Key);
 void JumpToPos(long newpos);
 int Insert(void *R);
 int Delete(void);
 int Update(void *R, int unlock=0);
 int Lock(void *R, int mode=200);
 void Unlock(void);
 void ChangeFilter(int _KeyNum, void *_KeyFilter, int _KeySize);
 int Find(short FieldID);
 void MoveOrGrawDone(short mode);
};

#endif
