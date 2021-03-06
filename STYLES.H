/****************************************************
 *       STYLES.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef STYLE
#define STYLE

#define    BLACK           0
#define    BLUE            1
#define    GREEN           2
#define    CYAN            3
#define    RED             4
#define    MAGENTA         5
#define    BROWN           6
#define    LIGHTGRAY       7
#define    DARKGRAY        8
#define    LIGHTBLUE       9
#define    LIGHTGREEN      10
#define    LIGHTCYAN       11
#define    LIGHTRED        12
#define    LIGHTMAGENTA    13
#define    YELLOW          14
#define    WHITE           15

unsigned char Style2Color(short style, short status);
void SaveColor(char *name);
short LoadColor(char *name);

// ��������
#define ST_DESKTOP    1

// ���ଠ樮��� ����
#define ST_MESWIN     2
#define ST_HELP       3

// ���� ��� ��ᬮ�� 䠩���
#define ST_CLSVIEW    4
#define ST_FILEVIEW   5
#define ST_RELVIEW    6

// �ਭ���
#define ST_SHOWPRINT  7

// ����� ����
#define ST_GAUGE      8
#define ST_MENU       9
#define ST_VMENU      10
#define ST_WINDOW     11
#define ST_OPTION     12

#define ST_LISTWINDOW 13

// ����� ���� ����� ��� ����᪨� ����� 梥� ᮯ�����
// � 梥⮬ த�⥫�᪮�� ����
#define ST_CELL_O     14   // 梥� ��⠭������ ।����㥬�� �����
#define ST_CELL       15   // 梥� ࠡ��� ।����㥬�� �����
#define ST_CELL_2      16   // ???



#endif
