/****************************************************
 *         HELP.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef HELP
#define HELP


class _Help{
 public:
  static int h;
  _Help(char *file_name);
  ~_Help();
  static void Build(int ID);
  static void Unpack(char *dest_file);
  static void Create(char *text_file, char *dest_file);
};

#endif


