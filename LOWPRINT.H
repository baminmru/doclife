/****************************************************
 *     LOWPRINT.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef LOWPRINT
#define LOWPRINT

class _LowPrint
{
	int heading;
	int idx;           // index handle;
	int f;             // file handle;
	unsigned long pos; // curent position in file;
	int pages;         //number of pages
	void (*HPrint)(_LowPrint * , int page);
	void (*FPrint)(_LowPrint * , int page);
	int footerCnt;
	int lines;        // lines in last page
	int LinesPerPage;
	char idxname[14];

	void PrintHeader(void);
	void PrintFooter(void);
  _Time t;
	_Date d;

 public:
  char *buffer;
  short cnt;
	_LowPrint(int _LinesPerPage,char *name=0);
	~_LowPrint(void);
	_Date GetDate(void){return d;};
  _Time GetTime(void){return t;};
	void SetHeader(void (*proc)(_LowPrint * ,int)){HPrint=proc;};
	void SetFooter(int cnt, void (*proc)(_LowPrint * ,int)){footerCnt=cnt; FPrint=proc;};
	void EndPage(void (*procH)(_LowPrint * ,int), int cntF=0,  void (*procF)(_LowPrint * ,int)=0);
	void EndPage(void);
	void PrintNL(char *text, int style=0);
  void Print  (char *text, int style=0);
};

#define MAXSTYLE 10
struct INDEX {
	int lines;
	long pos;
};

class _ShowPrint : public _Window
{
  int hdl;
  long pos;
  int LinesPerPage;
  long cur;
  int curpage;
  int first;
  INDEX * idx;
  long total;
  int X;
  void Paint(void);
  int  GetLine(char *buf);
  void SkipTo(int page);
  void LineConvert(long totalline, int *page, int *line);

 public:
  char fname[15];
  int  pages;
  int Print(int page1, int page2);
  _ShowPrint(char * filename, char *windowname);
  ~_ShowPrint();
  int Handle(_Event *E);
  int MHandle(_Event *E);
 void MoveOrGrawDone(short mode);
};
#endif
