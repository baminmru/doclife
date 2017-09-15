/****************************************************
 *   COLORKIT.CPP                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#include "common.h"

extern unsigned char ColorArray[16][2];
static _ColorCell *act, *pas;
static _Window *win;
static long style=1;
static long fcolor=0;
static long fback=0;
static long pcolor=0;
static long pback=0;
static _ListBox *l1,*l2,*l3,*l4;

void ChangedP(void *p){
  ColorArray[style-1][0]=pback*16+pcolor;
  pas->color=ColorArray[style-1][0];
  pas->Show(1);
};

void ChangedF(void *p){
  ColorArray[style-1][1]=fback*16+fcolor;
  act->color=ColorArray[style-1][1];
  act->Show(1);
};

void ChangedStyle(void *p){
  fcolor=ColorArray[style-1][1];
  fback=fcolor;
  fcolor&=0x0f;
  fback>>=4;
  fback&=7;
  pcolor=ColorArray[style-1][0];
  pback=pcolor;
  pcolor&=0x0f;
  pback>>=4;
  pback&=7;
  l1->Load();
  l2->Load();
  l3->Load();
  l4->Load();
  ChangedP(p);
  ChangedF(p);
  win->Show(1);
};


short ColorKit(void){
  _Window * w= new _Window(0,0,76,8,ST_OPTION,stFrame,"Выбор цветов");
  win=w;
  w->Add(new _Static(1,1,ST_OPTION,"Тип цвета:"));
  _ListBox *l=new _ListBox(15,1,30,8,ST_OPTION,0,&style);
  l->lbi.sort=0;
  l->lbi.AddItem("Главное окно", ST_DESKTOP    );
  l->lbi.AddItem("Сообщения", ST_MESWIN     );
  l->lbi.AddItem("Помощь", ST_HELP       );
  l->lbi.AddItem("Встроенные классификаторы", ST_CLSVIEW    );
  l->lbi.AddItem("Просмотр файла", ST_FILEVIEW   );
  l->lbi.AddItem("Справочные базы данных", ST_RELVIEW    );
  l->lbi.AddItem("Печать документа", ST_SHOWPRINT  );
  l->lbi.AddItem("Прогресс индикатор", ST_GAUGE      );
  l->lbi.AddItem("Меню", ST_MENU );
  l->lbi.AddItem("Вертикальное меню", ST_VMENU );
  l->lbi.AddItem("Окно редактирования", ST_WINDOW );
  l->lbi.AddItem("Окно установок", ST_OPTION     );
  l->lbi.AddItem("Список вариантов", ST_LISTWINDOW );
  //l->lbi.AddItem("Поле редактирования установок", ST_CELL_O     );
  //l->lbi.AddItem("Поле редактирования ", ST_CELL     );
  //l->lbi.AddItem("Резерв", ST_CELL_2    );
  l->Changed=ChangedStyle;
  w->Add(l);
  w->Add(new _Static(1,2,ST_OPTION,"Активный элемент:"));
  w->Add(new _Static(1,3,ST_OPTION,"Цвет фона:"));
  l=new _ListBox(15,3,20,8,ST_OPTION,0,&fback);
  l->lbi.sort=0;
  l->lbi.AddItem("Черный",BLACK );
  l->lbi.AddItem("Синий",BLUE  );
  l->lbi.AddItem("Зеленый",GREEN );
  l->lbi.AddItem("Голубой",CYAN  );
  l->lbi.AddItem("Красный",RED   );
  l->lbi.AddItem("Малиновый",MAGENTA);
  l->lbi.AddItem("Коричневый",BROWN  );
  l->lbi.AddItem("Светло серый",LIGHTGRAY);

  l->Changed=ChangedF;
  w->Add(l);
  l1=l;
  w->Add(new _Static(1,4,ST_OPTION,"Цвет символа:"));
  l=new _ListBox(15,4,20,8,ST_OPTION,0,&fcolor);
  l->lbi.sort=0;
  l->lbi.AddItem("Черный",BLACK );
  l->lbi.AddItem("Синий",BLUE  );
  l->lbi.AddItem("Зеленый",GREEN );
  l->lbi.AddItem("Голубой",CYAN  );
  l->lbi.AddItem("Красный",RED   );
  l->lbi.AddItem("Малиновый",MAGENTA);
  l->lbi.AddItem("Коричневый",BROWN  );
  l->lbi.AddItem("Светло серый",LIGHTGRAY);

  l->lbi.AddItem("Темно серый",DARKGRAY );
  l->lbi.AddItem("Свело синий",LIGHTBLUE);
  l->lbi.AddItem("Светло зеленый",LIGHTGREEN);
  l->lbi.AddItem("Светло голубой",LIGHTCYAN );
  l->lbi.AddItem("Светло красный",LIGHTRED  );
  l->lbi.AddItem("Светло малиновый",LIGHTMAGENTA);
  l->lbi.AddItem("Желтый",YELLOW      );
  l->lbi.AddItem("Белый",WHITE       );
  l->Changed=ChangedF;
  w->Add(l);
  l2=l;
  w->Add(act=new _ColorCell(1,6,(fback*16+fcolor)&0xff,"Пример активного элемента",1));

  w->Add(new _Static(37,2,ST_OPTION,"Пассивный элемент:"));
  w->Add(new _Static(37,3,ST_OPTION,"Цвет фона:"));
  l=new _ListBox(52,3,20,8,ST_OPTION,0,&pback);
  l->lbi.sort=0;
  l->lbi.AddItem("Черный",BLACK );
  l->lbi.AddItem("Синий",BLUE  );
  l->lbi.AddItem("Зеленый",GREEN );
  l->lbi.AddItem("Голубой",CYAN  );
  l->lbi.AddItem("Красный",RED   );
  l->lbi.AddItem("Малиновый",MAGENTA);
  l->lbi.AddItem("Коричневый",BROWN  );
  l->lbi.AddItem("Светло серый",LIGHTGRAY);

  l->Changed=ChangedP;
  l3=l;
  w->Add(l);
  w->Add(new _Static(37,4,ST_OPTION,"Цвет символа:"));
  l=new _ListBox(52,4,20,8,ST_OPTION,0,&pcolor);
  l->lbi.sort=0;
  l->lbi.AddItem("Черный",BLACK );
  l->lbi.AddItem("Синий",BLUE  );
  l->lbi.AddItem("Зеленый",GREEN );
  l->lbi.AddItem("Голубой",CYAN  );
  l->lbi.AddItem("Красный",RED   );
  l->lbi.AddItem("Малиновый",MAGENTA);
  l->lbi.AddItem("Коричневый",BROWN  );
  l->lbi.AddItem("Светло серый",LIGHTGRAY);

  l->lbi.AddItem("Темно серый",DARKGRAY );
  l->lbi.AddItem("Свело синий",LIGHTBLUE);
  l->lbi.AddItem("Светло зеленый",LIGHTGREEN);
  l->lbi.AddItem("Светло голубой",LIGHTCYAN );
  l->lbi.AddItem("Светло красный",LIGHTRED  );
  l->lbi.AddItem("Светло малиновый",LIGHTMAGENTA);
  l->lbi.AddItem("Желтый",YELLOW      );
  l->lbi.AddItem("Белый",WHITE       );
  l->Changed=ChangedP;
  l4=l;
  w->Add(l);
  w->Add(pas=new _ColorCell(37,6,(pback*16+pcolor)&0xff,"Пример пассивного элемента",1));
  w->CenterScr();
  ChangedStyle(w);
  short res=w->Exec();
  delete w;
  ShowBase->Show(1);
  return res;
};
