/****************************************************
 *          DOC.H                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/

//-- DDF file generator (c) 1994 --

//����_��室�� (f10.dat)
struct file_10 { 
  char                Nomer_statji        [6]  ; //�����_����        
  char                Nazvanie_statji     [71] ; //�������� ����     
};

struct file_10_0{
  char                Nomer_statji        [6]  ; //�����_����        
};

//-- DDF file generator (c) 1994 --

//����_��_��᪢� (f11.dat)
struct file_11 { 
  char                Nomer_statji        [6]  ; //�����_����        
  double              Summa_na_1_kv            ; //�㬬�_��_1_��       
  double              Summa_na_2_kv            ; //�㬬�_��_2_��       
  double              Summa_na_3_kv            ; //�㬬�_��_3_��       
  double              Summa_na_4_kv            ; //�㬬�_��_4_��       
  char                Zamena                   ; //������              
  _Date               Data_utohneniy           ; //��� ��筥���      
  short               God                      ; //���                 
};

struct file_11_0{
  short               God                      ; //���                 
  char                Nomer_statji        [6]  ; //�����_����        
  _Date               Data_utohneniy           ; //��� ��筥���      
  char                Zamena                   ; //������              
};

//-- DDF file generator (c) 1994 --

//�����_��⮢_��� (f12.dat)
struct file_12 { 
  char                Nomer_sheta         [16] ; //�����_���         
  char                Naimenovanie_sheta  [76] ; //������������_���  
  char                Direktoriy_aksporta [81] ; //��४�਩ ��ᯮ�� 
  char                Psevdonim           [16] ; //�ᥢ�����           
};

struct file_12_0{
  char                Nomer_sheta         [16] ; //�����_���         
};

//-- DDF file generator (c) 1994 --

//��������_�।�� (f13.dat)
struct file_13 { 
  char                Nomer_sheta         [16] ; //����� ���         
  _Date               data                     ; //���                
  double              Summa                    ; //�㬬�               
  char                Kod_dogovora        [13] ; //���_�������        
  char                Rashod                   ; //���室              
};

struct file_13_0{
  char                Nomer_sheta         [16] ; //����� ���         
  _Date               data                     ; //���                
};

struct file_13_1{
  _Date               data                     ; //���                
  char                Nomer_sheta         [16] ; //����� ���         
};

//-- DDF file generator (c) 1994 --

//��ࠢ�筨�_ࠩ���� (f14.dat)
struct file_14 { 
  short               Nomer_rayona             ; //�����_ࠩ���        
  char                Naimenovanie_rayona [76] ; //������������_ࠩ��� 
};

struct file_14_0{
  short               Nomer_rayona             ; //�����_ࠩ���        
};

struct file_14_1{
  char                Naimenovanie_rayona [76] ; //������������_ࠩ��� 
};

//-- DDF file generator (c) 1994 --

//����_������஢ (f15.dat)
struct file_15 { 
  short               Kod_tipa                 ; //���_⨯�            
  char                Nazvanie            [76] ; //��������            
};

struct file_15_0{
  short               Kod_tipa                 ; //���_⨯�            
};

struct file_15_1{
  char                Nazvanie            [76] ; //��������            
};

//-- DDF file generator (c) 1994 --

//�����_�ਮ��⮢ (f16.dat)
struct file_16 { 
  short               Pokazatelj               ; //������⥫�          
  short               Znahenie_pokazately      ; //���祭�� ������⥫� 
  long                Kolihestvo_ballov        ; //������⢮ ������   
};

struct file_16_0{
  short               Pokazatelj               ; //������⥫�          
  short               Znahenie_pokazately      ; //���祭�� ������⥫� 
};

//-- DDF file generator (c) 1994 --

//������� (f17.dat)
struct file_17 { 
  char                Nomer_dogovora      [13] ; //�����_�������      
  short               Tip_dogovora             ; //���_�������        
  _Date               Data_zakluheniy          ; //���_�����祭��     
  short               Kod_partnera             ; //���_���⭥�        
  _Date               Data_nahala              ; //���_��砫�         
  _Date               Data_okonhaniy           ; //���_����砭��      
  short               Kod_poluhately           ; //���_�����⥫�      
  char                Soderwanie          [211]; //����ঠ���          
  short               Sostoynie_dogovora       ; //����ﭨ�_�������  
  char                Primehanie          [211]; //�ਬ�砭��          
  char                Kratkoe_opisanie    [72] ; //��⪮�_���ᠭ��    
  short               Priznak_zakritiy         ; //�ਧ��� �������    
  short               Priznak_sheta            ; //�ਧ��� ���       
  short               Rayon                    ; //�����               
  short               Tip_finansirovaniy       ; //��� 䨭���஢����  
};

struct file_17_0{
  char                Nomer_dogovora      [13] ; //�����_�������      
};

struct file_17_1{
  short               Tip_dogovora             ; //���_�������        
  char                Nomer_dogovora      [13] ; //�����_�������      
};

struct file_17_2{
  short               Priznak_zakritiy         ; //�ਧ��� �������    
  short               Tip_dogovora             ; //���_�������        
  char                Nomer_dogovora      [13] ; //�����_�������      
};

//-- DDF file generator (c) 1994 --

//������_��_�������� (f18.dat)
struct file_18 { 
  char                Nomer_dogovora      [13] ; //�����_�������      
  _Date               Data_oplati              ; //���_������         
  double              Summa                    ; //�㬬�               
  char                Nomer_sheta         [16] ; //�����_���         
  char                Nomer_platewki      [13] ; //����� ���⥦��      
};

struct file_18_0{
  char                Nomer_dogovora      [13] ; //�����_�������      
};

struct file_18_1{
  _Date               Data_oplati              ; //���_������         
  char                Nomer_dogovora      [13] ; //�����_�������      
};

struct file_18_2{
  _Date               Data_oplati              ; //���_������         
  char                Nomer_platewki      [13] ; //����� ���⥦��      
  char                Nomer_dogovora      [13] ; //�����_�������      
};

//-- DDF file generator (c) 1994 --

//�।�������_�_����� (f19.dat)
struct file_19 { 
  char                Nomer_dogovora      [13] ; //�����_�������      
  double              Summa                    ; //�㬬�               
  char                Nomer_sheta         [16] ; //�����_���         
  _Date               Data                     ; //���                
  char                Razoviy_shet             ; //������ ���        
};

struct file_19_0{
  char                Razoviy_shet             ; //������ ���        
  char                Nomer_dogovora      [13] ; //�����_�������      
};

struct file_19_1{
  char                Nomer_sheta         [16] ; //�����_���         
  char                Razoviy_shet             ; //������ ���        
  char                Nomer_dogovora      [13] ; //�����_�������      
};

//-- DDF file generator (c) 1994 --

//��ࠢ�筨�_���⭥஢ (f20.dat)
struct file_20 { 
  short               Kod_partnera             ; //��� ���⭥�        
  char                Naimenovanie        [211]; //������������        
  char                Telefoni            [101]; //����䮭�            
  char                Faks                [31] ; //����                
};

struct file_20_0{
  short               Kod_partnera             ; //��� ���⭥�        
};

struct file_20_1{
  char                Naimenovanie        [211]; //������������        
};

//-- DDF file generator (c) 1994 --

//��ࠢ�筨�_ࠡ�� (f21.dat)
struct file_21 { 
  short               Kod_stroki               ; //���_��ப�          
  char                Naimenovanie        [211]; //������������        
  char                Nomer_statji        [6]  ; //����� ����        
};

struct file_21_0{
  short               Kod_stroki               ; //���_��ப�          
};

struct file_21_1{
  char                Naimenovanie        [211]; //������������        
};

//-- DDF file generator (c) 1994 --

//�����_������� (f22.dat)
struct file_22 { 
  char                Dogovor             [13] ; //�������             
  long                Stroka_dogovora          ; //��ப� �������     
  short               Kod_raboti               ; //���_ࠡ���          
  char                Ne_vkluhatj_v_predl_     ; //�� ������� � �।�.
  short               Periodihnostj            ; //��ਮ��筮���       
  char                Avansirovanie            ; //�����஢����       
};

struct file_22_0{
  char                Dogovor             [13] ; //�������             
};

struct file_22_1{
  long                Stroka_dogovora          ; //��ப� �������     
};

//-- DDF file generator (c) 1994 --

//�����_���� (f23.dat)
struct file_23 { 
  char                Dogovor             [13] ; //�������             
  long                Statjy                   ; //�����              
  _Date               Data_nahala_deystviy     ; //��� ��砫� ����⢨�
  short               Operaciy                 ; //������            
  double              Izmenyem_na              ; //�����塞 ��         
  char                Primehanie          [211]; //�ਬ�砭��          
  char                Avansoviy_platew         ; //����ᮢ� ���⥦    
  short               Hto_izmenyetsy           ; //�� ���������      
};

struct file_23_0{
  char                Dogovor             [13] ; //�������             
  long                Statjy                   ; //�����              
  _Date               Data_nahala_deystviy     ; //��� ��砫� ����⢨�
};

//-- DDF file generator (c) 1994 --

//���_䨭���஢���� (f24.dat)
struct file_24 { 
  short               Kod_tipa                 ; //��� ⨯�            
  short               Prioritet                ; //�ਮ���           
  char                Nomer_sheta         [16] ; //����� ���         
};

struct file_24_0{
  short               Kod_tipa                 ; //��� ⨯�            
};

struct file_24_1{
  short               Kod_tipa                 ; //��� ⨯�            
  short               Prioritet                ; //�ਮ���           
};

//-- DDF file generator (c) 1994 --

//���_䨭���஢���� (f26.dat)
struct file_26 { 
  short               Kod                      ; //���                 
  char                Nazvanie            [71] ; //��������            
};

struct file_26_0{
  short               Kod                      ; //���                 
};

struct file_26_1{
  char                Nazvanie            [71] ; //��������            
};

//-- DDF file generator (c) 1994 --

//�஬������_�⮣ (f25.dat)
struct file_25 { 
  char                Nomer_sheta         [16] ; //����� ���         
  char                Kod_dogovora        [13] ; //��� �������        
  short               Statjy                   ; //�����              
  double              Summa                    ; //�㬬�               
  short               Ball                     ; //����                
  char                Obrabotan                ; //��ࠡ�⠭           
};

struct file_25_0{
  short               Ball                     ; //����                
  char                Nomer_sheta         [16] ; //����� ���         
};

struct file_25_1{
  char                Kod_dogovora        [13] ; //��� �������        
  short               Statjy                   ; //�����              
};

//-- DDF file generator (c) 1994 --

//����窠_������� (f27.dat)
struct file_27 { 
  char                Kod_dogovora        [13] ; //��� �������        
  _Date               Data                     ; //���                
  char                Nazvanie            [71] ; //��������            
  double              Znahenie1                ; //���祭��1           
  double              Znahenie2                ; //���祭��2           
  double              Znahenie3                ; //���祭��3           
  double              Znahenie4                ; //���祭��4           
  _Date               Data_2                   ; //��� 2              
};

struct file_27_0{
  char                Kod_dogovora        [13] ; //��� �������        
};

struct file_27_1{
  char                Kod_dogovora        [13] ; //��� �������        
  _Date               Data                     ; //���                
};

//-- DDF file generator (c) 1994 --

//�६����_��� (f28.dat)
struct file_28 { 
  char                Nomer_sheta         [13] ; //����� ���         
  char                Priznak_oplati           ; //�ਧ���_������      
  _Date               Data_postupleniy         ; //��� ����㯫����    
  _Date               Data_oplati              ; //��� ������         
  short               Kod_partnera             ; //��� ���⭥�        
  double              Summa                    ; //�㬬�               
  char                Statjy              [6]  ; //�����              
  char                Primehanie          [76] ; //�ਬ�砭��          
  char                Shet_dly_oplati     [16] ; //��� ��� ������     
  char                Ne_vkluhatj_v_predl_     ; //�� ������� � �।�.
};

struct file_28_0{
  char                Nomer_sheta         [13] ; //����� ���         
};

struct file_28_1{
  char                Priznak_oplati           ; //�ਧ���_������      
  char                Nomer_sheta         [13] ; //����� ���         
};

struct file_28_2{
  char                Priznak_oplati           ; //�ਧ���_������      
  _Date               Data_postupleniy         ; //��� ����㯫����    
  char                Nomer_sheta         [13] ; //����� ���         
};

//-- DDF file generator (c) 1994 --

//�஢��_����㯠 (f29.dat)
struct file_29 { 
  char                Kod_dostupa         [11] ; //��� ����㯠         
  char                Parolj              [31] ; //��஫�              
  char                Privelegii          [51] ; //�ਢ������          
  char                Primehanie          [51] ; //�ਬ�砭��          
};

struct file_29_0{
  char                Kod_dostupa         [11] ; //��� ����㯠         
  char                Parolj              [31] ; //��஫�              
};

//-- DDF file generator (c) 1994 --

//���_���⭥஢ (f30.dat)
struct file_30 { 
  short               Kod_partnera             ; //���_���⭥�        
  char                Nomer_sheta         [16] ; //����� ���         
  char                Psevdonim           [16] ; //�ᥢ�����           
};

struct file_30_0{
  short               Kod_partnera             ; //���_���⭥�        
  char                Nomer_sheta         [16] ; //����� ���         
};

struct file_30_1{
  char                Nomer_sheta         [16] ; //����� ���         
  char                Psevdonim           [16] ; //�ᥢ�����           
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