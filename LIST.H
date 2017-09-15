/****************************************************
 *         LIST.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef LIST
#define LIST


struct ListItem{
  char *text;
  long idx;
  ListItem *next;
  ListItem *prev;
};

class ListItems {
  void SortByIndex(void);
  void SortByText(void);
 public:
  ListItem *top;
  ListItem *cur;
  short sort;
  void AddItem(char *text, long idx=0);
  void DelItem(char *text);
  void DelItem(long idx);
  void Next( short is_down);
  void JumpTo(char * text);
  void JumpTo(long idx);
  ListItem * Last(void);
  ListItem * GetItem(char *text);
  ListItem * GetItem(long idx);
  ListItems (ListItem * _top=0, short _sort=2);
  ~ListItems();
};

class _ListBox:public _Window{
 protected:

  int Handle(_Event *E);
  int MHandle(_Event *E);
  short h2;
 public:
  ListItems lbi;
  long Curent(void);
  void Paint(void);
  void Commit(void);
  void Load(void);
  long *info;
  _ListBox(short _x,short _y,short _w,short _h, short _style,ListItem *items, long *info);
  ~_ListBox();
};


class _ListView : public _Window {
  short ActivLine;
  ListItems *lbi;
  void Paint(void);
  int Handle(_Event *E);
  int MHandle(_Event *E);
  void MakeFirst(void);
 public:
  ListItem *oldcur;
  ListItem *first;
  _ListView(short _x,short _y,short _w,short _h, short _style,ListItems *items);
  ~_ListView();
};

ListItems * MakeFileList(char * proto, char * dirname);
ListItems * MakeDirList(char * dirname);
ListItems * MakeDriveList(void);

#endif
