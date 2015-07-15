/*
 * datadef.c - データ定義モジュール
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "microdb.h"

/*
 * DEF_FILE_EXT -- データ定義ファイルの拡張子
 */
#define DEF_FILE_EXT ".def"

/*
 * DATA_FILE_EXT -- データファイルの拡張子
 */
#define DATA_FILE_EXT ".dat"


/*
 * initializeDataDefModule -- データ定義モジュールの初期化
 *
 * 引数:
 *	なし
 *
 * 返り値;
 *	成功ならOK、失敗ならNGを返す
 */
Result initializeDataDefModule()
{
    return OK;
}

/*
 * finalizeDataDefModule -- データ定義モジュールの終了処理
 *
 * 引数:
 *	なし
 *
 * 返り値;
 *	成功ならOK、失敗ならNGを返す
 */
Result finalizeDataDefModule()
{
    return OK;
}

/*
 * createTable -- 表(テーブル)の作成
 *
 * 引数:
 *	tableName: 作成する表の名前
 *	tableInfo: データ定義情報
 *
 * 返り値:
 *	成功ならOK、失敗ならNGを返す
 *
 * データ定義ファイルの構造(ファイル名: tableName.def)
 *   +-------------------+----------------------+-------------------+----
 *   |フィールド数       |フィールド名          |データ型           |
 *   |(sizeof(int)バイト)|(MAX_FIELD_NAMEバイト)|(sizeof(int)バイト)|
 *   +-------------------+----------------------+-------------------+----
 * 以降、フィールド名とデータ型が交互に続く。
 */
Result createTable(char *tableName, TableInfo *tableInfo)
{
	initializeDataDefModule();

    int i, len;
    char *filename;
    File *file;
    char page[PAGE_SIZE];
    char *p;

    /* [tableName].defという文字列を作る */   
    len = strlen(tableName) + strlen(DEF_FILE_EXT) + 1;
    if ((filename = malloc(len)) == NULL) {
        return NG;
    }
    snprintf(filename, len, "%s%s", tableName, DEF_FILE_EXT);

    /* [tableName].defというファイルを作る */
    if (createFile(filename) != OK) {
        return NG;
    }

    /* [tableName].defをオープンする */
    if ((file = openFile(filename)) == NULL) {
        return NG;
    }

    /* PAGE_SIZEバイト分の大きさを持つ配列pageを初期化する */
    memset(page, 0, PAGE_SIZE);
    p = page;

    /* 配列pageの先頭にフィールド数を記録する */
    memcpy(p, &(tableInfo->numField), sizeof(tableInfo->numField));
    p += sizeof(tableInfo->numField);

    /* それぞれのフィールドについて、フィールド名とデータ型をpageに記録する */
    for (i = 0; i < tableInfo->numField; i++) 
    {
        /* i番目のフィールドの名前の記録 */
        memcpy(p, tableInfo->fieldInfo[i].name, sizeof(tableInfo->fieldInfo[i].name) );
        p += sizeof(tableInfo->fieldInfo[i].name);

        /* i番目のフィールドのデータ型の記録 */
        memcpy(p, &(tableInfo->fieldInfo[i].dataType), sizeof(tableInfo->fieldInfo[i].dataType));
        p += sizeof(tableInfo->fieldInfo[i].dataType);
    }
    

    if(writePage( file, getNumPages(filename) , page) == NG){
        return NG;
    }
    
    if(closeFile(file) == NG){
        return NG;
    }

     /*datファイルの作成*/
    if( (createDataFile(tableName)) == NG){
        return NG;
    }

    finalizeDataDefModule();
    return OK;
}

/*
 * dropTable -- 表(テーブル)の削除
 *
 * 引数:
 *	tableName
 *
 * 返り値:
 *	Result
 */
Result dropTable(char *tableName)
{
	initializeDataDefModule();
    int len;
    char *filename;



     /* [tableName].defという文字列を作る */   
    len = strlen(tableName) + strlen(DEF_FILE_EXT) + 1;
    if ((filename = malloc(len)) == NULL) {
        return NG;
    }
    snprintf(filename, len, "%s%s", tableName, DEF_FILE_EXT);

    /*defファイルの削除*/
    if(deleteFile(filename) == NG){
        return NG;
    }

     /* [tableName].datという文字列を作る */   
    len = strlen(tableName) + strlen(DATA_FILE_EXT) + 1;
    if ((filename = malloc(len)) == NULL) {
        return NG;
    }

    snprintf(filename, len, "%s%s", tableName, DATA_FILE_EXT);
    /*datファイルの削除*/
    if(deleteFile(filename) == NG){
        return NG;
    }

    printf("テーブル%sを削除しました\n",tableName );
	finalizeDataDefModule();
	return OK;
}

/*
 * getTableInfo -- 表のデータ定義情報を取得する関数
 *
 * 引数:
 *	tableName: 情報を表示する表の名前
 *
 * 返り値:
 *	tableNameのデータ定義情報を返す
 *	エラーの場合には、NULLを返す
 *
 * ***注意***
 *	この関数が返すデータ定義情報を収めたメモリ領域は、不要になったら
 *	必ずfreeTableInfoで解放すること。
 */
TableInfo *getTableInfo(char *tableName)
{
	initializeDataDefModule();

    int i, len;
    char *filename;
    File *file;
    char page[PAGE_SIZE];
    char *p;

	TableInfo *tableInfo;
    tableInfo = (TableInfo *)malloc( sizeof(TableInfo));//sizeofで渡すデータ型はポインタで指す先のデータ型    
	
    /* [tableName].defという文字列を作る */   
    len = strlen(tableName) + strlen(DEF_FILE_EXT) + 1;
    if ((filename = malloc(len)) == NULL) {
        return NULL;
    }
    snprintf(filename, len, "%s%s", tableName, DEF_FILE_EXT);


    /* [tableName].defをオープンする */
    if ((file = openFile(filename)) == NULL) {
        return NULL;
    }

    /* PAGE_SIZEバイト分だけファイルから読み取ってページ数分だけループ */
    for (int i = 0 ; i < getNumPages(filename) ; i++ ){    

        readPage(file, i, page);
        p = page;

        /* 配列pageの先頭からフィールド数を読み取る */
        memcpy( &(tableInfo->numField), p , sizeof(tableInfo->numField));
        p += sizeof(tableInfo->numField);

        /* それぞれのフィールドについて、フィールド名とデータ型をpageに記録する */
        for (i = 0; i < tableInfo->numField ; i++) {
            /* i番目のフィールドの名前の記録 */
            memcpy(tableInfo->fieldInfo[i].name , p , sizeof(tableInfo->fieldInfo[i].name) );
            p += sizeof(tableInfo->fieldInfo[i].name);

            /* i番目のフィールドのデータ型の記録 */
            memcpy(&(tableInfo->fieldInfo[i].dataType), p , sizeof(tableInfo->fieldInfo[i].dataType));
            p += sizeof(tableInfo->fieldInfo[i].dataType);
        }

    }

    if(closeFile(file) == NG){
        return NULL;
    }
    finalizeDataDefModule();
    return tableInfo;
}

/*
 * freeTableInfo -- データ定義情報を収めたメモリ領域の解放
 *
 * 引数:
 *	tableInfo
 *
 * 返り値:
 *	なし
 *
 * ***注意***
 *	関数getTableInfoが返すデータ定義情報を収めたメモリ領域は、
 *	不要になったら必ずこの関数で解放すること。
 */
void freeTableInfo(TableInfo *tableInfo)
{
	initializeDataDefModule();
    free(tableInfo);
	finalizeDataDefModule();

}
/*
 * printTableInfo -- テーブルのデータ定義情報を表示する(動作確認用)
 *
 * 引数:
 *  tableName: 情報を表示するテーブルの名前
 *
 * 返り値:
 *  なし
 */
void printTableInfo(char *tableName)
{
    TableInfo *tableInfo;
    int i;

    /* テーブル名を出力 */
    printf("\nTable %s\n", tableName);

    /* テーブルの定義情報を取得する */
    if ((tableInfo = getTableInfo(tableName)) == NULL) {
    /* テーブル情報の取得に失敗したので、処理をやめて返る */
    return;
    }

    /* フィールド数を出力 */
    printf("number of fields = %d\n", tableInfo->numField);

    /* フィールド情報を読み取って出力 */
    for (i = 0; i < tableInfo->numField; i++) {
    /* フィールド名の出力 */
    printf("  field %d: name = %s, ", i + 1, tableInfo->fieldInfo[i].name);

    /* データ型の出力 */
    printf("data type = ");
    switch (tableInfo->fieldInfo[i].dataType) {
    case TYPE_INTEGER:
        printf("integer\n");
        break;
    case TYPE_STRING:
        printf("string\n");
        break;
    default:
        printf("unknown\n");
    }
    }

    /* データ定義情報を解放する */
    freeTableInfo(tableInfo);

    return;
}

