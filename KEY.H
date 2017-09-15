/****************************************************
 *          KEY.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
#ifndef KEY
#define KEY

// коды клавиш
const unsigned int ASTERISK   = 0x2a;
const unsigned int PLUS       = 0x2b;
const unsigned int MINUS      = 0x2d;
const unsigned int SLASH      = 0x2f;


const unsigned int SPACE      = 0x20;
const unsigned int ALT_SPACE = 0x3908;
const unsigned int CTRL_SPACE = 0x3904;

const unsigned int BACKSPACE  = 0x8;
const unsigned int CTRL_BACKSPACE  = 0x0e04;
const unsigned int ALT_BACKSPACE  = 0x0e08;

const unsigned int ENTER      = 0xd;
const unsigned int CTRL_ENTER  = 0x1C04;
const unsigned int ALT_ENTER  = 0x1C08;

const unsigned int ESC        = 0x1b;
const unsigned int CTRL_ESC  = 0x0104;
const unsigned int ALT_ESC  = 0x0108;

const unsigned int TAB        = 0x9;
const unsigned int SHIFT_TAB    = 0x0f03;
const unsigned int ALT_TAB    = 0xa508;
const unsigned int CTRL_TAB    = 0x9404;


const unsigned int F1 = 0x3b00;
const unsigned int F2 = 0x3c00;
const unsigned int F3 = 0x3d00;
const unsigned int F4 = 0x3e00;
const unsigned int F5 = 0x3f00;
const unsigned int F6 = 0x4000;
const unsigned int F7 = 0x4100;
const unsigned int F8 = 0x4200;
const unsigned int F9 = 0x4300;
const unsigned int F10 = 0x4400;
const unsigned int F11 = 0x8500;
const unsigned int F12 = 0x8600;

const unsigned int SHIFT_F1 = 0x5403;
const unsigned int SHIFT_F2 = 0x5503;
const unsigned int SHIFT_F3 = 0x5603;
const unsigned int SHIFT_F4 = 0x5703;
const unsigned int SHIFT_F5 = 0x5803;
const unsigned int SHIFT_F6 = 0x5903;
const unsigned int SHIFT_F7 = 0x5a03;
const unsigned int SHIFT_F8 = 0x5b03;
const unsigned int SHIFT_F9 = 0x5c03;
const unsigned int SHIFT_F10 = 0x5d03;
const unsigned int SHIFT_F11 = 0x8703;
const unsigned int SHIFT_F12 = 0x8803;

const unsigned int ALT_F1 = 0x6808;
const unsigned int ALT_F2 = 0x6908;
const unsigned int ALT_F3 = 0x6a08;
const unsigned int ALT_F4 = 0x6b08;
const unsigned int ALT_F5 = 0x6c08;
const unsigned int ALT_F6 = 0x6d08;
const unsigned int ALT_F7 = 0x6e08;
const unsigned int ALT_F8 = 0x6f08;
const unsigned int ALT_F9 = 0x7008;
const unsigned int ALT_F10 = 0x7108;
const unsigned int ALT_F11 = 0x8b08;
const unsigned int ALT_F12 = 0x8c08;

const unsigned int CTRL_F1 = 0x5e04;
const unsigned int CTRL_F2 = 0x5f04;
const unsigned int CTRL_F3 = 0x6004;
const unsigned int CTRL_F4 = 0x6104;
const unsigned int CTRL_F5 = 0x6204;
const unsigned int CTRL_F6 = 0x6304;
const unsigned int CTRL_F7 = 0x6404;
const unsigned int CTRL_F8 = 0x6504;
const unsigned int CTRL_F9 = 0x6604;
const unsigned int CTRL_F10 = 0x6704;
const unsigned int CTRL_F11 = 0x8904;
const unsigned int CTRL_F12 = 0x8a04;

const unsigned int ALT_1 = 0x7808;
const unsigned int ALT_2 = 0x7908;
const unsigned int ALT_3 = 0x7a08;
const unsigned int ALT_4 = 0x7b08;
const unsigned int ALT_5 = 0x7c08;
const unsigned int ALT_6 = 0x7d08;
const unsigned int ALT_7 = 0x7e08;
const unsigned int ALT_8 = 0x7f08;
const unsigned int ALT_9 = 0x8008;
const unsigned int ALT_0 = 0x8108;
const unsigned int ALT_MINUS = 0x8208;
const unsigned int ALT_EQL = 0x8308;
const unsigned int ALT_BSLASH = 0x2b08;



const unsigned int ALT_Q = 0x1008;
const unsigned int ALT_W = 0x1108;
const unsigned int ALT_E = 0x1208;
const unsigned int ALT_R = 0x1308;
const unsigned int ALT_T = 0x1408;
const unsigned int ALT_Y = 0x1508;
const unsigned int ALT_U = 0x1608;
const unsigned int ALT_I = 0x1708;
const unsigned int ALT_O = 0x1808;
const unsigned int ALT_P = 0x1908;
const unsigned int ALT_LBR = 0x1a08;
const unsigned int ALT_RBR = 0x1b08;

const unsigned int ALT_A = 0x1E08;
const unsigned int ALT_S = 0x1F08;
const unsigned int ALT_D = 0x2008;
const unsigned int ALT_F = 0x2108;
const unsigned int ALT_G = 0x2208;
const unsigned int ALT_H = 0x2308;
const unsigned int ALT_J = 0x2408;
const unsigned int ALT_K = 0x2508;
const unsigned int ALT_L = 0x2608;
const unsigned int ALT_DDOT = 0x2708;
const unsigned int ALT_APP = 0x2808;

const unsigned int ALT_Z = 0x2C08;
const unsigned int ALT_X = 0x2D08;
const unsigned int ALT_C = 0x2E08;
const unsigned int ALT_V = 0x2F08;
const unsigned int ALT_B = 0x3008;
const unsigned int ALT_N = 0x3108;
const unsigned int ALT_M = 0x3208;
const unsigned int ALT_LESS = 0x3308;
const unsigned int ALT_GRTR = 0x3408;
const unsigned int ALT_WHAT = 0x3508;


const unsigned int CTRL_Q = 0x1004;
const unsigned int CTRL_W = 0x1104;
const unsigned int CTRL_E = 0x1204;
const unsigned int CTRL_R = 0x1304;
const unsigned int CTRL_T = 0x1404;
const unsigned int CTRL_Y = 0x1504;
const unsigned int CTRL_U = 0x1604;
const unsigned int CTRL_I = 0x1704;
const unsigned int CTRL_O = 0x1804;
const unsigned int CTRL_P = 0x1904;
const unsigned int CTRL_LBR = 0x1a04;
const unsigned int CTRL_RBR = 0x1b04;

const unsigned int CTRL_A = 0x1E04;
const unsigned int CTRL_S = 0x1F04;
const unsigned int CTRL_D = 0x2004;
const unsigned int CTRL_F = 0x2104;
const unsigned int CTRL_G = 0x2204;
const unsigned int CTRL_H = 0x2304;
const unsigned int CTRL_J = 0x2404;
const unsigned int CTRL_K = 0x2504;
const unsigned int CTRL_L = 0x2604;

const unsigned int CTRL_Z = 0x2C04;
const unsigned int CTRL_X = 0x2D04;
const unsigned int CTRL_C = 0x2E04;
const unsigned int CTRL_V = 0x2F04;
const unsigned int CTRL_B = 0x3004;
const unsigned int CTRL_N = 0x3104;
const unsigned int CTRL_M = 0x3204;

const unsigned int INSERT = 0x5200;
const unsigned int HOME   = 0x4700;
const unsigned int PGUP   = 0x4900;
const unsigned int DELETE = 0x5300;
const unsigned int END    = 0x4f00;
const unsigned int PGDN   = 0x5100;
const unsigned int UP     = 0x4800;
const unsigned int LEFT   = 0x4b00;
const unsigned int DOWN   = 0x5000;
const unsigned int RIGHT  = 0x4d00;

const unsigned int SHIFT_INSERT = 0x5203;
const unsigned int SHIFT_HOME   = 0x4703;
const unsigned int SHIFT_PGUP   = 0x4903;
const unsigned int SHIFT_DELETE = 0x5303;
const unsigned int SHIFT_END    = 0x4f03;
const unsigned int SHIFT_PGDN   = 0x5103;
const unsigned int SHIFT_UP     = 0x4803;
const unsigned int SHIFT_LEFT   = 0x4b03;
const unsigned int SHIFT_DOWN   = 0x5003;
const unsigned int SHIFT_RIGHT  = 0x4d03;

const unsigned int CTRL_INSERT = 0x9204;
const unsigned int CTRL_HOME   = 0x7704;
const unsigned int CTRL_PGUP   = 0x8404;
const unsigned int CTRL_DELETE = 0x9304;
const unsigned int CTRL_END    = 0x7504;
const unsigned int CTRL_PGDN   = 0x7604;
const unsigned int CTRL_UP     = 0x8d04;
const unsigned int CTRL_LEFT   = 0x7304;
const unsigned int CTRL_DOWN   = 0x9104;
const unsigned int CTRL_RIGHT  = 0x7404;

const unsigned int ALT_INSERT = 0xa208;
const unsigned int ALT_HOME   = 0x9708;
const unsigned int ALT_PGUP   = 0x9908;
const unsigned int ALT_DELETE = 0xa308;
const unsigned int ALT_END    = 0x9f08;
const unsigned int ALT_PGDN   = 0xa108;
const unsigned int ALT_UP     = 0x9808;
const unsigned int ALT_LEFT   = 0x9b08;
const unsigned int ALT_DOWN   = 0xa008;
const unsigned int ALT_RIGHT  = 0x9d08;

const unsigned int CTRL_GSLASH   = 0x9504;
const unsigned int CTRL_GASTERISK= 0x9604;
const unsigned int CTRL_GMINUS   = 0x8e04;
const unsigned int CTRL_GPLUS    = 0x9004;
const unsigned int CTRL_GENTER   = 0xe004;

const unsigned int ALT_GSLASH   = 0xa408;
const unsigned int ALT_GASTERISK= 0x3708;
const unsigned int ALT_GMINUS   = 0x4a08;
const unsigned int ALT_GPLUS    = 0x4e08;
const unsigned int ALT_GENTER   = 0xa608;




unsigned int GetKey( int no_wait= 0 );
unsigned CheckKey(unsigned key,unsigned *list,int listlen);
unsigned GetKeyArray( unsigned s, ... );
unsigned GKArr( unsigned s, ... );


#endif
