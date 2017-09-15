/****************************************************
 *          DOC.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/

//-- DDF file generator (c) 1994 --

//Статьи_расходов (f10.dat)
struct file_10 { 
  char                Nomer_statji        [6]  ; //Номер_статьи        
  char                Nazvanie_statji     [71] ; //Название статьи     
};

struct file_10_0{
  char                Nomer_statji        [6]  ; //Номер_статьи        
};

//-- DDF file generator (c) 1994 --

//Смета_из_Москвы (f11.dat)
struct file_11 { 
  char                Nomer_statji        [6]  ; //Номер_статьи        
  double              Summa_na_1_kv            ; //Сумма_на_1_кв       
  double              Summa_na_2_kv            ; //Сумма_на_2_кв       
  double              Summa_na_3_kv            ; //Сумма_на_3_кв       
  double              Summa_na_4_kv            ; //Сумма_на_4_кв       
  char                Zamena                   ; //Замена              
  _Date               Data_utohneniy           ; //Дата уточнения      
  short               God                      ; //Год                 
};

struct file_11_0{
  short               God                      ; //Год                 
  char                Nomer_statji        [6]  ; //Номер_статьи        
  _Date               Data_utohneniy           ; //Дата уточнения      
  char                Zamena                   ; //Замена              
};

//-- DDF file generator (c) 1994 --

//Номера_счетов_КЗР (f12.dat)
struct file_12 { 
  char                Nomer_sheta         [16] ; //Номер_счета         
  char                Naimenovanie_sheta  [76] ; //Наименование_счета  
  char                Direktoriy_aksporta [81] ; //Директорий экспорта 
  char                Psevdonim           [16] ; //Псевдоним           
};

struct file_12_0{
  char                Nomer_sheta         [16] ; //Номер_счета         
};

//-- DDF file generator (c) 1994 --

//Движение_средств (f13.dat)
struct file_13 { 
  char                Nomer_sheta         [16] ; //Номер счета         
  _Date               data                     ; //дата                
  double              Summa                    ; //Сумма               
  char                Kod_dogovora        [13] ; //Код_договора        
  char                Rashod                   ; //Расход              
};

struct file_13_0{
  char                Nomer_sheta         [16] ; //Номер счета         
  _Date               data                     ; //дата                
};

struct file_13_1{
  _Date               data                     ; //дата                
  char                Nomer_sheta         [16] ; //Номер счета         
};

//-- DDF file generator (c) 1994 --

//Справочник_районов (f14.dat)
struct file_14 { 
  short               Nomer_rayona             ; //Номер_района        
  char                Naimenovanie_rayona [76] ; //Наименование_района 
};

struct file_14_0{
  short               Nomer_rayona             ; //Номер_района        
};

struct file_14_1{
  char                Naimenovanie_rayona [76] ; //Наименование_района 
};

//-- DDF file generator (c) 1994 --

//Типы_договоров (f15.dat)
struct file_15 { 
  short               Kod_tipa                 ; //Код_типа            
  char                Nazvanie            [76] ; //Название            
};

struct file_15_0{
  short               Kod_tipa                 ; //Код_типа            
};

struct file_15_1{
  char                Nazvanie            [76] ; //Название            
};

//-- DDF file generator (c) 1994 --

//Матрица_приоритетов (f16.dat)
struct file_16 { 
  short               Pokazatelj               ; //Показатель          
  short               Znahenie_pokazately      ; //Значение показателя 
  long                Kolihestvo_ballov        ; //Количество баллов   
};

struct file_16_0{
  short               Pokazatelj               ; //Показатель          
  short               Znahenie_pokazately      ; //Значение показателя 
};

//-- DDF file generator (c) 1994 --

//Договор (f17.dat)
struct file_17 { 
  char                Nomer_dogovora      [13] ; //Номер_договора      
  short               Tip_dogovora             ; //Тип_договора        
  _Date               Data_zakluheniy          ; //Дата_заключения     
  short               Kod_partnera             ; //Код_партнера        
  _Date               Data_nahala              ; //Дата_начала         
  _Date               Data_okonhaniy           ; //Дата_окончания      
  short               Kod_poluhately           ; //Код_получателя      
  char                Soderwanie          [211]; //Содержание          
  short               Sostoynie_dogovora       ; //Состояние_договора  
  char                Primehanie          [211]; //Примечание          
  char                Kratkoe_opisanie    [72] ; //Краткое_описание    
  short               Priznak_zakritiy         ; //Признак закрытия    
  short               Priznak_sheta            ; //Признак счета       
  short               Rayon                    ; //Район               
  short               Tip_finansirovaniy       ; //Тип финансирования  
};

struct file_17_0{
  char                Nomer_dogovora      [13] ; //Номер_договора      
};

struct file_17_1{
  short               Tip_dogovora             ; //Тип_договора        
  char                Nomer_dogovora      [13] ; //Номер_договора      
};

struct file_17_2{
  short               Priznak_zakritiy         ; //Признак закрытия    
  short               Tip_dogovora             ; //Тип_договора        
  char                Nomer_dogovora      [13] ; //Номер_договора      
};

//-- DDF file generator (c) 1994 --

//Оплаты_по_договору (f18.dat)
struct file_18 { 
  char                Nomer_dogovora      [13] ; //Номер_договора      
  _Date               Data_oplati              ; //Дата_оплаты         
  double              Summa                    ; //Сумма               
  char                Nomer_sheta         [16] ; //Номер_счета         
  char                Nomer_platewki      [13] ; //Номер платежки      
};

struct file_18_0{
  char                Nomer_dogovora      [13] ; //Номер_договора      
};

struct file_18_1{
  _Date               Data_oplati              ; //Дата_оплаты         
  char                Nomer_dogovora      [13] ; //Номер_договора      
};

struct file_18_2{
  _Date               Data_oplati              ; //Дата_оплаты         
  char                Nomer_platewki      [13] ; //Номер платежки      
  char                Nomer_dogovora      [13] ; //Номер_договора      
};

//-- DDF file generator (c) 1994 --

//Предложение_к_оплате (f19.dat)
struct file_19 { 
  char                Nomer_dogovora      [13] ; //Номер_договора      
  double              Summa                    ; //Сумма               
  char                Nomer_sheta         [16] ; //Номер_счета         
  _Date               Data                     ; //Дата                
  char                Razoviy_shet             ; //Разовый счет        
};

struct file_19_0{
  char                Razoviy_shet             ; //Разовый счет        
  char                Nomer_dogovora      [13] ; //Номер_договора      
};

struct file_19_1{
  char                Nomer_sheta         [16] ; //Номер_счета         
  char                Razoviy_shet             ; //Разовый счет        
  char                Nomer_dogovora      [13] ; //Номер_договора      
};

//-- DDF file generator (c) 1994 --

//Справочник_партнеров (f20.dat)
struct file_20 { 
  short               Kod_partnera             ; //Код партнера        
  char                Naimenovanie        [211]; //Наименование        
  char                Telefoni            [101]; //Телефоны            
  char                Faks                [31] ; //Факс                
};

struct file_20_0{
  short               Kod_partnera             ; //Код партнера        
};

struct file_20_1{
  char                Naimenovanie        [211]; //Наименование        
};

//-- DDF file generator (c) 1994 --

//Справочник_работ (f21.dat)
struct file_21 { 
  short               Kod_stroki               ; //Код_строки          
  char                Naimenovanie        [211]; //Наименование        
  char                Nomer_statji        [6]  ; //Номер статьи        
};

struct file_21_0{
  short               Kod_stroki               ; //Код_строки          
};

struct file_21_1{
  char                Naimenovanie        [211]; //Наименование        
};

//-- DDF file generator (c) 1994 --

//Статья_договора (f22.dat)
struct file_22 { 
  char                Dogovor             [13] ; //Договор             
  long                Stroka_dogovora          ; //Строка договора     
  short               Kod_raboti               ; //Код_работы          
  char                Ne_vkluhatj_v_predl_     ; //Не включать в предл.
  short               Periodihnostj            ; //Периодичность       
  char                Avansirovanie            ; //Авансирование       
};

struct file_22_0{
  char                Dogovor             [13] ; //Договор             
};

struct file_22_1{
  long                Stroka_dogovora          ; //Строка договора     
};

//-- DDF file generator (c) 1994 --

//История_статьи (f23.dat)
struct file_23 { 
  char                Dogovor             [13] ; //Договор             
  long                Statjy                   ; //Статья              
  _Date               Data_nahala_deystviy     ; //Дата начала действия
  short               Operaciy                 ; //Операция            
  double              Izmenyem_na              ; //Изменяем на         
  char                Primehanie          [211]; //Примечание          
  char                Avansoviy_platew         ; //Авансовый платеж    
  short               Hto_izmenyetsy           ; //Что изменяется      
};

struct file_23_0{
  char                Dogovor             [13] ; //Договор             
  long                Statjy                   ; //Статья              
  _Date               Data_nahala_deystviy     ; //Дата начала действия
};

//-- DDF file generator (c) 1994 --

//Тип_финансирования (f24.dat)
struct file_24 { 
  short               Kod_tipa                 ; //Код типа            
  short               Prioritet                ; //Приоритет           
  char                Nomer_sheta         [16] ; //Номер счета         
};

struct file_24_0{
  short               Kod_tipa                 ; //Код типа            
};

struct file_24_1{
  short               Kod_tipa                 ; //Код типа            
  short               Prioritet                ; //Приоритет           
};

//-- DDF file generator (c) 1994 --

//Код_финансирования (f26.dat)
struct file_26 { 
  short               Kod                      ; //Код                 
  char                Nazvanie            [71] ; //Название            
};

struct file_26_0{
  short               Kod                      ; //Код                 
};

struct file_26_1{
  char                Nazvanie            [71] ; //Название            
};

//-- DDF file generator (c) 1994 --

//Промежуточный_итог (f25.dat)
struct file_25 { 
  char                Nomer_sheta         [16] ; //Номер счета         
  char                Kod_dogovora        [13] ; //Код договора        
  short               Statjy                   ; //Статья              
  double              Summa                    ; //Сумма               
  short               Ball                     ; //Балл                
  char                Obrabotan                ; //Обработан           
};

struct file_25_0{
  short               Ball                     ; //Балл                
  char                Nomer_sheta         [16] ; //Номер счета         
};

struct file_25_1{
  char                Kod_dogovora        [13] ; //Код договора        
  short               Statjy                   ; //Статья              
};

//-- DDF file generator (c) 1994 --

//Карточка_договора (f27.dat)
struct file_27 { 
  char                Kod_dogovora        [13] ; //Код договора        
  _Date               Data                     ; //Дата                
  char                Nazvanie            [71] ; //Название            
  double              Znahenie1                ; //Значение1           
  double              Znahenie2                ; //Значение2           
  double              Znahenie3                ; //Значение3           
  double              Znahenie4                ; //Значение4           
  _Date               Data_2                   ; //Дата 2              
};

struct file_27_0{
  char                Kod_dogovora        [13] ; //Код договора        
};

struct file_27_1{
  char                Kod_dogovora        [13] ; //Код договора        
  _Date               Data                     ; //Дата                
};

//-- DDF file generator (c) 1994 --

//Временные_счета (f28.dat)
struct file_28 { 
  char                Nomer_sheta         [13] ; //Номер счета         
  char                Priznak_oplati           ; //Признак_оплаты      
  _Date               Data_postupleniy         ; //Дата поступления    
  _Date               Data_oplati              ; //Дата оплаты         
  short               Kod_partnera             ; //Код партнера        
  double              Summa                    ; //Сумма               
  char                Statjy              [6]  ; //Статья              
  char                Primehanie          [76] ; //Примечание          
  char                Shet_dly_oplati     [16] ; //Счет для оплаты     
  char                Ne_vkluhatj_v_predl_     ; //Не включать в предл.
};

struct file_28_0{
  char                Nomer_sheta         [13] ; //Номер счета         
};

struct file_28_1{
  char                Priznak_oplati           ; //Признак_оплаты      
  char                Nomer_sheta         [13] ; //Номер счета         
};

struct file_28_2{
  char                Priznak_oplati           ; //Признак_оплаты      
  _Date               Data_postupleniy         ; //Дата поступления    
  char                Nomer_sheta         [13] ; //Номер счета         
};

//-- DDF file generator (c) 1994 --

//Уровни_доступа (f29.dat)
struct file_29 { 
  char                Kod_dostupa         [11] ; //Код доступа         
  char                Parolj              [31] ; //Пароль              
  char                Privelegii          [51] ; //Привелегии          
  char                Primehanie          [51] ; //Примечание          
};

struct file_29_0{
  char                Kod_dostupa         [11] ; //Код доступа         
  char                Parolj              [31] ; //Пароль              
};

//-- DDF file generator (c) 1994 --

//Счета_партнеров (f30.dat)
struct file_30 { 
  short               Kod_partnera             ; //Код_партнера        
  char                Nomer_sheta         [16] ; //Номер счета         
  char                Psevdonim           [16] ; //Псевдоним           
};

struct file_30_0{
  short               Kod_partnera             ; //Код_партнера        
  char                Nomer_sheta         [16] ; //Номер счета         
};

struct file_30_1{
  char                Nomer_sheta         [16] ; //Номер счета         
  char                Psevdonim           [16] ; //Псевдоним           
};
#include "clsctrs.h"
#include "f10.h"
#include "f11.h"
#include "f12.h"
#include "f13.h"
#include "f14.h"
#include "f15.h"
#include "f16.h"
#include "f17.h"
#include "f18.h"
#include "f19.h"
#include "f20.h"
#include "f21.h"
#include "f22.h"
#include "f23.h"
#include "f24.h"
#include "f25.h"
#include "f26.h"
#include "f27.h"
#include "f28.h"
#include "f29.h"
#include "f30.h"
extern char* OWNER;
extern char Dostup[];
#define D_NOACCESS 0
#define D_READ     1
#define D_WRITE    2
