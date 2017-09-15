/****************************************************
 *      PROTECT.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
typedef short BOOL;
#define FALSE 0
#define TRUE  1

BOOL ProtectNotOK(char *Name);
BOOL ProtectOK(char *Name);
short InstallEXE(char *Name);
void SavePasswd(unsigned char *pwd);
int CmpPasswd(unsigned char *pwd);
