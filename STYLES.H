/****************************************************
 *       STYLES.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
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

// подложка
#define ST_DESKTOP    1

// инормационные окна
#define ST_MESWIN     2
#define ST_HELP       3

// окна для просмотра файлов
#define ST_CLSVIEW    4
#define ST_FILEVIEW   5
#define ST_RELVIEW    6

// принтерный
#define ST_SHOWPRINT  7

// простые окна
#define ST_GAUGE      8
#define ST_MENU       9
#define ST_VMENU      10
#define ST_WINDOW     11
#define ST_OPTION     12

#define ST_LISTWINDOW 13

// простые поля ввода для статических полей цвет сопадает
// с цветом родительского окна
#define ST_CELL_O     14   // цвет установочных редактируемых полей
#define ST_CELL       15   // цвет рабочих редактируемых полей
#define ST_CELL_2      16   // ???



#endif
