/****************************************************
 *       BBG_NB.H                                   *
 *  Copyright (C) Михаил М. Баранов                 *
 *  Модификация без предварительного согласия автора*
 *  будет рассматривается как нарушение авторского  *
 *  и имущественного права                          *
 *  создан: 21 Сентября 1995, Четверг               *
 ****************************************************/
int BTRV( int, void *, void *, int *, void *, int );
#define offset(rec,field) (int)((unsigned long)&(rec.field) - (unsigned long)&rec)
#define boffset(rec,field) (int)((unsigned long)&(rec.field) - (unsigned long)&rec+1)


// Extended key types
  const int EKT_STRING = 0;
  const int EKT_INTEGER = 1;
  const int EKT_FLOAT = 2;
  const int EKT_DATE = 3;
  const int EKT_TIME = 4;
  const int EKT_DECIMAL = 5;
  const int EKT_MONEY = 6;
  const int EKT_LOGICAL = 7;
  const int EKT_NUMERIC = 8;
  const int EKT_BFLOAT = 9;
  const int EKT_LSTRING = 10;
  const int EKT_ZSTRING = 11;
  const int EKT_UNSIGNED_BINARY = 14;
  const int EKT_AUTOINCREMENT = 15;

// File flags values
  const int FFV_VARIABLE_LENTH = 1;
  const int FFV_BLANK_TRUNCATION = 2;
  const int FFV_PREALLOCATION = 4;
  const int FFV_DATA_COMPRESSION = 8;
  const int FFV_KEY_ONLY = 16;
  const int FFV_10_FREE_SPACE = 64;
  const int FFV_20_FREE_SPACE = 128;
  const int FFV_30_FREE_SPACE = 192;

// Key flags values
  const int KFV_DUPLICATE = 1;
  const int KFV_MODIFIABLE = 2;
  const int KFV_BINARY = 4;
  const int KFV_NULL = 8;
  const int KFV_SEGMENTED = 16;
  const int KFV_ALT_COL_SEQ = 32;
  const int KFV_DESCENDING = 64;
  const int KFV_SUPPLEMENTAL = 128;
  const int KFV_EXTENDED_TYPE = 256;
  const int KFV_MANUAL = 512;

// Open file modes
  const int OFM_NORMAL = 0;
  const int OFM_ACCELERATED = -1;
  const int OFM_READ_ONLY = -2;
  const int OFM_VERIFY = -3;
  const int OFM_EXCLUSIVE = -4;


// Btrieve operations
  const int B_OPEN = 0;
  const int B_CLOSE = 1;
  const int B_INSERT = 2;
  const int B_UPDATE = 3;
  const int B_DELETE = 4;
  const int B_GET_EQUAL = 5;
  const int B_GET_NEXT = 6;
  const int B_GET_PREVIOUS = 7;
  const int B_GET_GREATER = 8;
  const int B_GET_GREATER_OR_EQUAL = 9;
  const int B_GET_LESS_THAN = 10;
  const int B_GET_LESS_THAN_OR_EQUAL = 11;
  const int B_GET_FIRST = 12;
  const int B_GET_LAST = 13;
  const int B_CREATE = 14;
  const int B_STAT = 15;
  const int B_EXTEND = 16;
  const int B_SET_DIRECTORY = 17;
  const int B_GET_DIRECTORY = 18;
  const int B_BEGIN_TRANSACTION = 19;
  const int B_END_TRANSACTION = 20;
  const int B_ABORT_TRANSACTION = 21;
  const int B_GET_POSITION = 22;
  const int B_GET_DIRECT = 23;
  const int B_STEP_NEXT = 24;
  const int B_STOP = 25;
  const int B_VERSION = 26;
  const int B_UNLOCK = 27;
  const int B_RESET = 28;
  const int B_SET_OWNER = 29;
  const int B_CLEAR_OWNER = 30;
  const int B_CREATE_SUPPLEMENTAL_INDEX = 31;
  const int B_DROP_SUPPLEMENTAL_INDEX = 32;
  const int B_STEP_FIRST = 33;
  const int B_STEP_LAST = 34;
  const int B_STEP_PREVIOUS = 35;
  const int B_GET_NEXT_EXTENDED = 36;
  const int B_GET_PREVIOUS_EXTENDED = 37;
  const int B_STEP_NEXT_EXTENDED = 38;
  const int B_STEP_PREVIOUS_EXTENDED = 39;
  const int B_INSERT_EXTENDED = 40;

// mode definition
  const int B_GET_KEY = 50;
  const int B_SINGLE_REC_WAIT_LOCK = 100;
  const int B_SINGLE_REC_NOWAIT_LOCK = 200;
  const int B_NOWAIT_FILE_LOCK = 200;
  const int B_MULTIPLE_REC_WAIT_LOCK = 300;
  const int B_MULTIPLE_REC_NOWAIT_LOCK = 400;

// fast definitions
#define KDMS   KFV_DUPLICATE+KFV_EXTENDED_TYPE+KFV_SEGMENTED+KFV_MODIFIABLE
#define KDM    KFV_DUPLICATE+KFV_EXTENDED_TYPE+KFV_MODIFIABLE
#define KMS    KFV_EXTENDED_TYPE+KFV_SEGMENTED+KFV_MODIFIABLE
#define KM     KFV_EXTENDED_TYPE+KFV_MODIFIABLE
#define KDS    KFV_DUPLICATE+KFV_EXTENDED_TYPE+KFV_SEGMENTED
#define KD     KFV_DUPLICATE+KFV_EXTENDED_TYPE
#define K_S    KFV_EXTENDED_TYPE+KFV_SEGMENTED
#define K_     KFV_EXTENDED_TYPE
#define FD     EKT_DATE
#define FS     EKT_STRING
#define FB     EKT_UNSIGNED_BINARY

class  Table;
struct field_list;
struct index_list;

struct KeySegment {
   int KeyPos;
   int KeyLen;
   int KeyFlag;
   int NofKeysLow;
   int NofKeysHigh;
   char KeyType;
   char Reserve1[5];
};

struct FileDescriptor {
   int RecordLenth;
   int PageSize;
   int IndexCount;
   int NofRecLow;
   int NofRecHigh;
   int FileFlags;
   char Reserve2[2];
   int UnusedPages;
   struct KeySegment KSeg[24];
   //unsigned char AltColSeq[265];//[n]
};



class  Station{

 public:
  static Table  *main_file;
  static Table  *field_file;
  static Table  *index_file;
  //static Table  *relation_file;
  static Table  *winsize_file;
  static Table  *list;
  static FileDescriptor FD;
  int Create  (char * path,int mode=0); // создать файл
  int Stat    (Table * _tab); // получить характеристики файла
  long Records(Table * _tab); // получить количество записей в таблице
  int SetDir (char * _path); // установить директорий
  int GetDir (int drive, char * _path); // получить директорий
  int TBegin (int mode=0); // начать транзакцию
  int TEnd   (void); // закончить транзакцию
  int TAbort (void); // прервать транзакцию
  int Stop   (void); // выгрузить BTRIEV из памяти
  int Version(int * ver); // получить номер версии BTRIEV -а
  int ResetMe(void); // освободить ресурсы занятые текущей станцией
  int ResetIt(int *station); // освободить ресурсы станции с заданным адресом
  int GetAddress(unsigned char *addres); //6-и байтный адрес
  int GetConnect(void); // получить номер соединения
  Station();
  ~Station();
  long CritLock  (char *file);
  int CritUnLock (long pos  );
  int GetFilePath(char *file, char *path);
  int GetFileID(char *file);
  void AddTable(Table *t);
  void DelTable(Table *t);
  Table *FindTable(int Id);
  Table *FindTable(char *name);
};



class   Table{
 public:
  Table * next;
  char name[21]      ;   //  Название таблици

  int opened         ;   //  Флаг: таблица была открыта

  long cur_pos       ;   //  Блок для записи позиции
  int cur_key_num    ;   //  Текущий ключ
  int cur_rec_len    ;   //  Текущая длина записи
  long crit          ;   //  позиция для блокировки критических
                         //  точек системы


  static Station St  ;
  char pos_blk[128]  ;   //  ...
  int max_rec_len    ;   //  Максимальная длина записи
  char * cur_rec     ;   //  Адрес текущей записи
  char key [256]     ;   //  Текущий ключ
  int status         ;   //  Результат последней операции
  index_list * il    ;   // список индексов
  field_list * fl    ;   // список полей

  Table(char  *  _name  ,  int _max_rec_len , char *owner="" );
  Table(char  *  _name  ,  char *owner ="" );
  Table(int _max_rec_len );


	~Table();

	void set_key(int new_key){
    if(0 <= new_key )
			cur_key_num=new_key;
	};

  int Open  ( char * _file_name, char *_owner, int _access=0);
  int Open  (char *_owner , int _access=0);  // открытие файла через file.ddf
	int Close (void);
	int Insert( void * rec, void * key, int new_key=-1);
  int Insert( void * rec, int new_key=-1);
	int Update( void * rec, int mode=0, int new_key=-1);
	int Delete(void);

	// получение записи с использованием ключа
	int GEQ		( void * rec, int new_key=-1, int mode=0);
	int GGE		( void * rec, int new_key=-1, int mode=0);
	int GGT		( void * rec, int new_key=-1, int mode=0);
	int GLE		( void * rec, int new_key=-1, int mode=0);
	int GLT		( void * rec, int new_key=-1, int mode=0);
	int GFirst( void * rec, int new_key=-1, int mode=0);
	int GLast ( void * rec, int new_key=-1, int mode=0);

	int GEQ		( void * rec, void * key, int new_key=-1, int mode=0);
	int GGE		( void * rec, void * key, int new_key=-1, int mode=0);
	int GGT		( void * rec, void * key, int new_key=-1, int mode=0);
	int GLE		( void * rec, void * key, int new_key=-1, int mode=0);
	int GLT		( void * rec, void * key, int new_key=-1, int mode=0);
	int GFirst( void * rec, void * key, int new_key=-1, int mode=0);
	int GLast ( void * rec, void * key, int new_key=-1, int mode=0);

	int GNext ( void * rec, int mode=0);
	int GPrev ( void * rec, int mode=0);

  int GNext ( void * rec, void * key, int mode=0);
  int GPrev ( void * rec, void * key, int mode=0);

  int GDir  ( void * rec, void * pos=0, int new_key=-1, int mode=0);

	// получение записи без использования ключа
	int SFirst( void * rec, int mode=0);
	int SLast ( void * rec, int mode=0);
	int SNext ( void * rec, int mode=0);
	int SPrev ( void * rec, int mode=0);


	int GPos  ( void * pos=0 );
	int GCur  ( void * rec, int mode=0);

	// разблокирование записей
  int UnlockS(void);
  int UnlockS(long pos);
  int UnlockSfromM(void *pos);
  int UnlockM(void);
  int CritBegin(void);
  int CritEnd  (void);
  long Crit(void) {return crit; };


	// получить сообщение об ошибке по номеру ошибки
	char * Error(int code);

  // ddf функции
  void ReadDescription(void);                // прочитать описание таблици из файла
  void BuildIndex(void *record ,void *_key, int keynum);   // построить ключ из записи
  void FillIndex(void *_key, int keynum, int minimum=1);
  unsigned short FieldSize  (int);
  unsigned short FieldOffset(int);
  unsigned short FieldType  (int);
  char *FieldName(int ,char *);
  int Fields(void);
  int SizeForPrint(int n);
  char *PrintField(int n, void *R ,char *buf);
};

// общие полезные функции
int ValidType(int type);
void Report(Table *Tab ,int KeyNum=0,char *name=0);


