/*
 * microdb.h - 共通定義ファイル
 */

/*
 * Result -- 成功/失敗を返す返り値
 */
typedef enum { OK, NG } Result;

/*
 * PAGE_SIZE -- ファイルアクセスの単位(バイト数)
 */
#define PAGE_SIZE 4096

/*
 * MAX_FILENAME -- オープンするファイルの名前の長さの上限
 */
#define MAX_FILENAME 256

/*
 * File - オープンしたファイルの情報を保持する構造体
 */
typedef struct File File;
struct File {
    int desc;                           /* ファイルディスクリプタ */
    char name[MAX_FILENAME];            /* ファイル名 */
};

/*
 * file.cに定義されている関数群
 */
extern Result initializeFileModule();
extern Result finalizeFileModule();
extern Result createFile(char *);
extern Result deleteFile(char *);
extern File *openFile(char *);
extern Result closeFile(File *);
extern Result readPage(File *, int, char *);
extern Result writePage(File *, int, char *);
extern int getNumPages(char *);


/*
 * MAX_FIELD -- 1レコードに含まれるフィールド数の上限
 */
#define MAX_FIELD 40

 /*
 * MAX_FIELD_NAME -- フィールド名の長さの上限(バイト数)
 */
#define MAX_FIELD_NAME 20

/*
 * dataType -- データベースに保存するデータの型
 */
typedef enum DataType DataType;
enum DataType {
    TYPE_UNKNOWN = 0,			/* データ型不明 */
    TYPE_INTEGER = 1,			/* 整数型 */
    TYPE_STRING  = 2			/* 文字列型 */
};

/*
 * FieldInfo -- フィールドの情報を表現する構造体
 */
typedef struct FieldInfo FieldInfo;
struct FieldInfo {
    char name[MAX_FIELD_NAME];		/* フィールド名 */
    DataType dataType;			/* フィールドのデータ型 */
};

/*
 * TableInfo -- テーブルの情報を表現する構造体
 */
typedef struct TableInfo TableInfo;
struct TableInfo {
    int numField;				/* フィールド数 */
    FieldInfo fieldInfo[MAX_FIELD];		/* フィールド情報の配列 */
};


/*
 * datadef.cに定義されている関数群
 */
extern Result initializeDataDefModule();
extern Result finalizeDataDefModule();
extern Result createTable(char *, TableInfo *);
extern Result dropTable(char *);
extern TableInfo *getTableInfo(char *);
extern void freeTableInfo(TableInfo *);


/*
 * MAX_STRING -- 文字列型データの長さの上限
 */
#define MAX_STRING 20

/*
 * FieldData -- 1つのフィールドのデータを表現する構造体
 */
typedef struct FieldData FieldData;
struct FieldData {
    char name[MAX_FIELD_NAME];		/* フィールド名 */
    DataType dataType;			/* フィールドのデータ型 */
    int intValue;			/* integer型の場合の値 */
    char stringValue[MAX_STRING];	/* string型の場合の値 */
};

/*
 * RecordData -- 1つのレコードのデータを表現する構造体
 */
typedef struct RecordData RecordData;
struct RecordData {
    int numField;			/* フィールド数 */
    FieldData fieldData[MAX_FIELD];	/* フィールド情報 */
    RecordData *next;
};

/*
 * RecordSet -- レコードの集合を表現する構造体
 */
typedef struct RecordSet RecordSet;
struct RecordSet {
    int numRecord;			/* レコード数 */
    RecordData *tail;       /*最後のレコードリストへのポインタ  */
    RecordData *recordData;	/* レコードのリストへのポインタ */
};


/*
 * OpratorType -- 比較演算子を表す列挙型
 */
typedef enum OperatorType OperatorType;
enum OperatorType {
    OPR_EQUAL,              /* = */
    OPR_NOT_EQUAL,          /* != */
    OPR_GREATER_THAN,           /* > */
    OPR_LESS_THAN           /* < */
};

/*
 * Condition -- 検索や削除の条件式を表現する構造体
 */
typedef struct Condition Condition;
struct Condition {
    char name[MAX_FIELD_NAME];      /* フィールド名 */
    DataType dataType;          /* フィールドのデータ型 */
    OperatorType operator;      /* 比較演算子 */
    int intValue;           /* integer型の場合の値 */
    char stringValue[MAX_STRING];   /* string型の場合の値 */
    int allmach; /* 条件文がない時(*で全表示されるとき)に１が入力される*/
};

/*
 * datamanip.cに定義されている関数群
 */
extern Result initializeDataManipModule();
extern Result finalizeDataManipModule();
extern Result insertRecord(char *, RecordData *);
extern RecordSet *selectRecord(char *,Condition *);
extern void freeRecordSet(RecordSet *);
extern Result deleteRecord(char *, Condition *);
extern Result createDataFile(char *);
extern Result deleteDataFile(char *);
extern void printRecordSet(RecordSet *);
extern void printTableData(char *);
/* データを表にして表示する関数 */
extern void printTableFence(int);

/* データのString判定をする関数　（バグありTODO）*/
extern Result checkTokenString(char *);

/* バッファリングテスト関数 */
extern void printBufferList();







