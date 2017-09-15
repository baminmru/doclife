/****************************************************
 *         HELP.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
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


