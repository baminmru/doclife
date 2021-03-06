/****************************************************
 *       WINDOW.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef WINDOW
#define WINDOW

/*
  ���� ����� 䨪�஢���� ࠧ���
  ��� ⠪�� �� ����� ���� ��� ��࠭���� �।��饣� ���ﭨ�
*/

#define stBuffered  1
#define stHide      2
#define stCursor    4
#define stLocked    8
#define stNoMove    16
#define stNoESC     32
#define stInsert    64
#define stFrame     128
#define stFocused   256

class _Desktop;
class _Window;


struct CallBack
{
  void (*func)(_Event *,void *);      // �㭪�� ��ࠡ�⪨
  int type;                   // ⨯ ᮡ���
  long code;                  // ��� ᮡ���
  CallBack *next;
  CallBack(void (*_func)(_Event*,void *), short _type, long _code):
    func(_func), code(_code), type(_type) ,next(0) {};
  ~CallBack();
};


class _WinList{
  public:
  short index;
  short total;
  _Window **array;
  _Window *FindWin  ( char *name);
  int Find( _Window *it);
  int Del ( _Window *it);
  void Add( _Window *it);
  _WinList(void);
  ~_WinList(void);
};

class _Window{
  public:
  static _Window *Zord[512];
  static short MaxZ;
  short Result;
  char *name;
  short min_w,min_h;
  short x,y,w,h; // ���न����
  short style;       // �⨫� ���᪨

  _Window *parent;   //த�⥫�᪮� ����
  _WinList *child;    // ���
  unsigned short st; //���ﭨ� ����
  char NotOnScreen;
  char *WBuffer;
  char *ExecBuf;
  CallBack *F;
  int HelpID;


  virtual int CanClose(void);      // �����頥� 1 �᫨ ����� ������� ����
  virtual int CanSwitch(void);     // �����頥� 1 �᫨ ����� ��४����� �����
  virtual int  CanLostFocus(void); // �����頥� 1 �᫨ ����� ��।��� 䮪��
  virtual void LostFocus(int l=1);    // ����� 䮪��
  virtual void GotFocus(int l=1);     // ����祭�� 䮪��
  virtual void Commit(void);       // �८�ࠧ����� ᮤ�ন��� ���� � ��室���
                                   // ���祭��
  virtual void Load(void);         // ����⢨� ���⭮� Commit
  virtual void MoveOrGrawDone(short mode);

  void SwitchFocus(short Down=1, int l=1);  // ��४����� 䮪�� �� ᫥���饣�/�।��饣� �뭠
  void SwitchFocus(_Window *Focus, int l=1);// ��४����� 䮪�� �� �����⭮� ����
  _Window *FindFocused(void);      // ���� ���� ���஥ � 䮪��

  virtual int Handle(_Event * E);  // ��ࠡ�⪠ ᮡ�⨩
  virtual int MHandle(_Event *E);


  void Show(short a=1);            // �������� ����
  virtual void Paint(void);        // ����ᮢ��� ᥡ�

  void Hide(short a=1);            // ���� ���� � ��࠭�
  void UnHide(short a=1);          // ����⠭����� ���� �� ��࠭�
  void Move(short dx, short dy);   // ��������� ����
  virtual void Graw(short dx, short dy);   // ��������� ����

  void Add(_Window* NewWindow);
  void Del(_Window *OldWindow);
  _Window*  Find( char * name);      // ���� ���� �� �����

  void Coord(short *x, short *y);  // �८�ࠧ����� ���न����
                                   // �� �⭮�⥫���  ��᮫���

  _Window(short _x,short _y,short _w,short _h, short _style, short _Status=0,char *_name="Window");
  virtual ~_Window(void);
  void dbg(short start=1);
  void AddCallBack(void (*func)(_Event*, void *), short type, long code);
  void DelCallBack(short type, long code);
  CallBack* FindCallBack(short type, long code);
  int Exec(void);
  void FreeAllBuffers(void);

  void Center(void);
  void CenterScr(void);
  void BottomRight(void);
  void CheckSize();
  void (*Changed)(void*);
  void (*Befor)(void*);
  void (*After)(void*);
  short ZFind  (_Window*);
  void  ZInsert(_Window *);
  void  ZDelete (_Window*);
  int SaveSize(void);
  int LoadSize(void);
};
void ScrollWrite(_Window *Win, char *f,...);
extern _Window *ShowBase;

#endif
