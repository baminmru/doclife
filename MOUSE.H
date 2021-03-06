/****************************************************
 *        MOUSE.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
#ifndef MOUSE
#define MOUSE

struct MouseEventType
{
    unsigned char buttons;
    short x;
    short y;
};

class _Mouse
{

public:

    _Mouse();
    ~_Mouse();

    static void show();
    static void hide();
    static void huge MouseInt(void);
    static void setRange( unsigned short, unsigned short );
    static void MoveTo( unsigned short, unsigned short );
    static void Shift(  short,  short );
    static void getEvent( MouseEventType& );
    static int  getFromQ( MouseEventType& );
    static void registerHandler( unsigned, void (far *)() );
    static short present();

    static void suspend();
    static void resume();
    static void inhibit();

    static unsigned char buttonCount;
    static short  handlerInstalled;
    static short  noMouse;
    static short  Top;
    static short  Total;
    static short  Bottom;
    static MouseEventType *Q;

};

inline short _Mouse::present()
{
    return  buttonCount != 0 ;
}

inline void _Mouse::inhibit()
{
    noMouse = 1;
}

#endif
