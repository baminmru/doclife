/****************************************************
 *       CHOOSE.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/

enum{ csValue=1,csBorders, csSubstr, csClass, csVocabulary};

struct _ChooseType {
  short field;
  short type;
              // 1 - �ᯮ������ ���祭��
              // 2 - �ᯮ������ �஬���⮪ ���祭��
              // 3 - �ᯮ������ �宦�����
              // 4 - �ᯮ������ ���祭�� �� �����䨪����
              // 5 - �ᯮ������ ���祭�� �� �ࠢ�筨��

  short relation; // - ����� 䠩�� �ࠢ�筨�� / �����䨪���
  short relkey;   // - ���� ��� �裡 � �ࠢ�筨���
  short vfield;   // - ���� ��� ������ � �ࠢ�筨��
  void (*Writer)(void * ,char*); // ��楤�� ������  ��ப� �����䨪���
  unsigned char yn;     // - ���� �ᯮ������ ��� ���᪠
};
// �� ��᪥ ��ந��� �᫮��� ��� �롮� ����� l,r ⮩ �� �������� �� � ������
// � ���� ������
void ChooseTheDocs(Table *t, _ChooseType *mask, char *l, char *r);

// ��� ����� rec �஢������ �᫮��� (mask+l+r)
short CheckRecord (Table *t, _ChooseType *mask, void *rec, void *l , void *r);
void ChooseTheDocs(_FileView *fv);
