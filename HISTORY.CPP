/****************************************************
 *    HISTORY.CPP                                   *
 *  Copyright (C) ��堨� �. ��࠭��                 *
 *  ����䨪��� ��� �।���⥫쭮�� ᮣ���� ����*
 *  �㤥� ��ᬠ�ਢ����� ��� ����襭�� ����᪮��  *
 *  � �����⢥����� �ࠢ�                          *
 *  ᮧ���: 21 ������� 1995, ��⢥�               *
 ****************************************************/
// �㭪樨 ࠡ��� � ���⮢�� ��⥬��
int CopyToHistory(long pos){
  long pp=pos;
  file_18 r;
  file_18_0 k;
  file_10 r10;
  T10->GDir(&r10,&pp);

  // �⮡� �� �ᯮ�짮���� �࠭���樨 ᭠砫� ����ᨬ �������� ���ଠ��
  // � ��⮬ �⬥砥� �� ������ ��ࠡ�⠭�

  if(rf10->Summa){

    // �����㥬 �� � ���⮢� 䠩�
    // ��室�� ����� ���⮢��� �����
    // ����� �ࢨ� ��� ��� �ᥣ�� ࠢ�� 1
    k.Code=2;
    k.Part=0;
    if(!T18->GLT(&r,&k,0)){
      if(k.Code==1){
        k.Part++;
      }else{
        k.Code=1;
        k.Part=1;
      }
    }else{
      k.Code=1;
      k.Part=1;
    }
    // ����㦠��
    r.Code=k.Code;
    r.Part=k.Part;
    r.Size=sizeof(r10);
    memcpy(r.Info,&r10,r.Size);
    if(T18->Insert(&r))
     goto c2h_error;
  }

  r10.flag|=COMPLETE_RECORD;
  if(T10->Update(&r10)){
    goto c2h_error;
  }

  T10->GDir(&r10,&pp);
  return 1;

 c2h_error:

  T10->GDir(&r10,&pp);
  return 0;
};

// image - �।��饥 ���ﭨ�
// rf10 -  ⥪�饥
void ChangeHistory(long pos){
  long pp=pos;
  file_15 r;

  //T10->St.TBegin();

  // ᨬ㫨�㥬 �⥭�� ����� �࠭���樨
  T10->GDir(&image,&pp);

  if(!memcmp(&image,rf10,sizeof(image))){
    //T10->St.TEnd();
    return;
  }

  // �����뢠�� ���������
  if(T10->Update(rf10)){
    //T10->St.TAbort();
    goto ch_error;
  }

  // ��୨�㥬 �।����� ������
  r.flag[0] = 0;

  if(image.Summa){
    memcpy(r.s1,image.cls,8);
    r.summa=-image.Summa;
    r.account=image.account;
    r.lnum=image.lnum;
    memcpy(r.mfo, image.MFO,16);
    memcpy(r.acc, image.Ras__shet,16);
    r.data1=image.date;
    r.data2=image.date_2;
    if(T15->Insert(&r)){
      //T10->St.TAbort();
      goto ch_error;
    }
  }

  // ����ᨬ �����
  memcpy(r.s1,rf10->cls,8);
  r.summa=rf10->Summa;
  r.account=rf10->account;
  r.lnum=rf10->lnum;
  memcpy(r.mfo, rf10->MFO,16);
  memcpy(r.acc, rf10->Ras__shet,16);
  r.data1=rf10->date;
  r.data2=rf10->date_2;
  if(T15->Insert(&r)){
    T10->St.TAbort();
    goto ch_error;
  }

  // �����뢠�� ���������
  if(T10->Update(rf10)){
    //T10->St.TAbort();
    goto ch_error;
  }

  //return 1;
  //T10->St.TEnd();

 ch_error:

  T10->GDir(rf10,&pp);

};


// rf10 - ⥪�饥 ���ﭨ�
void DeleteFromHistory(long pos){
  long pp=pos;
  file_15 r;

  T10->St.TBegin();

  // ᨬ㫨�㥬 �⥭�� ����� �࠭���樨
  T10->GDir(rf10,&pp);

  if(rf10->Summa){

    // ��୨�㥬 ������
    r.flag[0] = 0;
    memcpy(r.s1,rf10->cls,8);
    r.summa=-rf10->Summa;
    r.account=rf10->account;
    r.lnum=rf10->lnum;
    memcpy(r.mfo, rf10->MFO,16);
    memcpy(r.acc, rf10->Ras__shet,16);
    r.data1=rf10->date;
    r.data2=rf10->date_2;
    if(T15->Insert(&r)){
      T10->St.TAbort();
      goto dh_error;
    }
  }

  // 㤠�塞 ������
  if(T10->Delete()){
    T10->St.TAbort();
    goto dh_error;
  }

  if(T10->GNext(rf10))
     T10->GPrev(rf10);


  T10->St.TEnd();
  return;

 dh_error:

  T10->GDir(rf10,&pp);
};
