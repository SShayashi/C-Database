/*
 * データ操作モジュールテストプログラム
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "microdb.h"

#define TABLE_NAME "student"

/*
 * test1 -- レコードの挿入
 */
Result test1()
{
    RecordData record;
    int i;

    /*
     * 以下のレコードを挿入
     * ('i04001', 'Mickey', 20, 'Urayasu')
     */
    i = 0;
    strcpy(record.fieldData[i].name, "id");
    record.fieldData[i].dataType = TYPE_STRING;
    strcpy(record.fieldData[i].stringValue, "i04001");
    i++;

    strcpy(record.fieldData[i].name, "name");
    record.fieldData[i].dataType = TYPE_STRING;
    strcpy(record.fieldData[i].stringValue, "Mickey");
    i++;

    strcpy(record.fieldData[i].name, "age");
    record.fieldData[i].dataType = TYPE_INTEGER;
    record.fieldData[i].intValue = 20;
    i++;

    strcpy(record.fieldData[i].name, "address");
    record.fieldData[i].dataType = TYPE_STRING;
    strcpy(record.fieldData[i].stringValue, "Urayasu");
    i++;

    record.numField = i;

    if (insertRecord(TABLE_NAME, &record) != OK) {
	fprintf(stderr, "Cannot insert record.\n");
	return NG;
    }

    /*
     * 以下のレコードを挿入
     * ('i04001', 'Mickey', 20, 'Urayasu')
     */
    i = 0;
    strcpy(record.fieldData[i].name, "id");
    record.fieldData[i].dataType = TYPE_STRING;
    strcpy(record.fieldData[i].stringValue, "i04001");
    i++;

    strcpy(record.fieldData[i].name, "name");
    record.fieldData[i].dataType = TYPE_STRING;
    strcpy(record.fieldData[i].stringValue, "Mickey");
    i++;

    strcpy(record.fieldData[i].name, "age");
    record.fieldData[i].dataType = TYPE_INTEGER;
    record.fieldData[i].intValue = 20;
    i++;

    strcpy(record.fieldData[i].name, "address");
    record.fieldData[i].dataType = TYPE_STRING;
    strcpy(record.fieldData[i].stringValue, "Urayasu");
    i++;

    record.numField = i;

    if (insertRecord(TABLE_NAME, &record) != OK) {
	fprintf(stderr, "Cannot insert record.\n");
	return NG;
    }

    /*
     * 以下のレコードを挿入
     * ('i04002', 'Minnie', 19, 'Urayasu')
     */
    i = 0;
    strcpy(record.fieldData[i].name, "id");
    record.fieldData[i].dataType = TYPE_STRING;
    strcpy(record.fieldData[i].stringValue, "i04002");
    i++;

    strcpy(record.fieldData[i].name, "name");
    record.fieldData[i].dataType = TYPE_STRING;
    strcpy(record.fieldData[i].stringValue, "Minnie");
    i++;

    strcpy(record.fieldData[i].name, "age");
    record.fieldData[i].dataType = TYPE_INTEGER;
    record.fieldData[i].intValue = 19;
    i++;

    strcpy(record.fieldData[i].name, "address");
    record.fieldData[i].dataType = TYPE_STRING;
    strcpy(record.fieldData[i].stringValue, "Urayasu");
    i++;

    record.numField = i;

    if (insertRecord(TABLE_NAME, &record) != OK) {
	fprintf(stderr, "Cannot insert record.\n");
	return NG;
    }

    /*
     * 以下のレコードを挿入
     * ('i04003', 'Donald', 17, 'Florida')
     */
    i = 0;
    strcpy(record.fieldData[i].name, "id");
    record.fieldData[i].dataType = TYPE_STRING;
    strcpy(record.fieldData[i].stringValue, "i04003");
    i++;

    strcpy(record.fieldData[i].name, "name");
    record.fieldData[i].dataType = TYPE_STRING;
    strcpy(record.fieldData[i].stringValue, "Donald");
    i++;

    strcpy(record.fieldData[i].name, "age");
    record.fieldData[i].dataType = TYPE_INTEGER;
    record.fieldData[i].intValue = 17;
    i++;

    strcpy(record.fieldData[i].name, "address");
    record.fieldData[i].dataType = TYPE_STRING;
    strcpy(record.fieldData[i].stringValue, "Florida");
    i++;

    record.numField = i;

    if (insertRecord(TABLE_NAME, &record) != OK) {
	fprintf(stderr, "Cannot insert record.\n");
	return NG;
    }

    /*
     * 以下のレコードを挿入
     * ('i04003', 'Donald', 17, 'Florida')
     */
    i = 0;
    strcpy(record.fieldData[i].name, "id");
    record.fieldData[i].dataType = TYPE_STRING;
    strcpy(record.fieldData[i].stringValue, "i04003");
    i++;

    strcpy(record.fieldData[i].name, "name");
    record.fieldData[i].dataType = TYPE_STRING;
    strcpy(record.fieldData[i].stringValue, "Donald");
    i++;

    strcpy(record.fieldData[i].name, "age");
    record.fieldData[i].dataType = TYPE_INTEGER;
    record.fieldData[i].intValue = 17;
    i++;

    strcpy(record.fieldData[i].name, "address");
    record.fieldData[i].dataType = TYPE_STRING;
    strcpy(record.fieldData[i].stringValue, "Florida");
    i++;

    record.numField = i;

    if (insertRecord(TABLE_NAME, &record) != OK) {
	fprintf(stderr, "Cannot insert record.\n");
	return NG;
    }

    /*
     * 以下のレコードを挿入
     * ('i04004', 'Daisy', 15, 'Florida')
     */
    i = 0;
    strcpy(record.fieldData[i].name, "id");
    record.fieldData[i].dataType = TYPE_STRING;
    strcpy(record.fieldData[i].stringValue, "i04004");
    i++;

    strcpy(record.fieldData[i].name, "name");
    record.fieldData[i].dataType = TYPE_STRING;
    strcpy(record.fieldData[i].stringValue, "Daisy");
    i++;

    strcpy(record.fieldData[i].name, "age");
    record.fieldData[i].dataType = TYPE_INTEGER;
    record.fieldData[i].intValue = 15;
    i++;

    strcpy(record.fieldData[i].name, "address");
    record.fieldData[i].dataType = TYPE_STRING;
    strcpy(record.fieldData[i].stringValue, "Florida");
    i++;

    record.numField = i;

    if (insertRecord(TABLE_NAME, &record) != OK) {
	fprintf(stderr, "Cannot insert record.\n");
	return NG;
    }

    /* データを表示する */
    printTableData(TABLE_NAME);

    return OK;
}

/*
 * test2 -- 検索
 */
Result test2()
{
    RecordSet *recordSet;
    Condition condition;

    /*
     * 以下の検索を実行
     * select * from TABLE_NAME where age > 17
     */
    strcpy(condition.name, "age");
    condition.dataType = TYPE_INTEGER;
    condition.operator = OPR_GREATER_THAN;
    condition.intValue = 17;
    condition.distinct = NOT_DISTINCT;

    if ((recordSet = selectRecord(TABLE_NAME, &condition)) == NULL) {
	fprintf(stderr, "Cannot select records.\n");
	return NG;
    }

    /* 結果を表示 */
    printf("age > 17, not distinct\n");
    printRecordSet(recordSet);

    /* 結果を解放 */
    freeRecordSet(recordSet);

    /*
     * 以下の検索を実行
     * select distinct * from TABLE_NAME where age > 17
     */
    strcpy(condition.name, "age");
    condition.dataType = TYPE_INTEGER;
    condition.operator = OPR_GREATER_THAN;
    condition.intValue = 17;
    condition.distinct = DISTINCT;

    if ((recordSet = selectRecord(TABLE_NAME, &condition)) == NULL) {
	fprintf(stderr, "Cannot select records.\n");
	return NG;
    }

    /* 結果を表示 */
    printf("age > 17, distinct\n");
    printRecordSet(recordSet);

    /* 結果を解放 */
    freeRecordSet(recordSet);

    /*
     * 以下の検索を実行
     * select * from TABLE_NAME where address != 'Florida'
     */
    strcpy(condition.name, "address");
    condition.dataType = TYPE_STRING;
    condition.operator = OPR_NOT_EQUAL;
    strcpy(condition.stringValue, "Florida");
    condition.distinct = NOT_DISTINCT;

    if ((recordSet = selectRecord(TABLE_NAME, &condition)) == NULL) {
	fprintf(stderr, "Cannot select records.\n");
	return NG;
    }

    /* 結果を表示 */
    printf("address != 'Florida', not distinct\n");
    printRecordSet(recordSet);

    /* 結果を解放 */
    freeRecordSet(recordSet);

    /*
     * 以下の検索を実行
     * select distinct * from TABLE_NAME where address != 'Florida'
     */
    strcpy(condition.name, "address");
    condition.dataType = TYPE_STRING;
    condition.operator = OPR_NOT_EQUAL;
    strcpy(condition.stringValue, "Florida");
    condition.distinct = DISTINCT;

    if ((recordSet = selectRecord(TABLE_NAME, &condition)) == NULL) {
	fprintf(stderr, "Cannot select records.\n");
	return NG;
    }

    /* 結果を表示 */
    printf("address != 'Florida', distinct\n");
    printRecordSet(recordSet);

    /* 結果を解放 */
    freeRecordSet(recordSet);

    return OK;
}

/*
 * test3 -- 削除
 */
Result test3()
{
    Condition condition;

    /*
     * 以下の検索を実行
     * delete from TABLE_NAME where name != 'Mickey'
     */
    strcpy(condition.name, "name");
    condition.dataType = TYPE_STRING;
    condition.operator = OPR_NOT_EQUAL;
    strcpy(condition.stringValue, "Mickey");

    if (deleteRecord(TABLE_NAME, &condition) != OK) {
	fprintf(stderr, "Cannot delete records.\n");
	return NG;
    }

    /* データを表示する */
    printTableData(TABLE_NAME);

    return OK;
}

/*
 * main -- データ操作モジュールのテスト
 */
int main(int argc, char **argv)
{
    char tableName[20];
    TableInfo tableInfo;
    int i;

    /* ファイルモジュールを初期化する */
    if (initializeFileModule() != OK) {
	fprintf(stderr, "Cannot initialize file module.\n");
	exit(1);
    }

    /* データ定義ジュールを初期化する */
    if (initializeDataDefModule() != OK) {
	fprintf(stderr, "Cannot initialize data definition module.\n");
	exit(1);
    }

    /* データ操作ジュールを初期化する */
    if (initializeDataManipModule() != OK) {
	fprintf(stderr, "Cannot initialize data manipulation module.\n");
	exit(1);
    }

    /*
     * このプログラムの前回の実行の時のデータ定義残っている可能性があるので、
     * とりあえず削除する
     */
    dropTable(TABLE_NAME);

    /*
     * 以下のテーブルを作成
     * create table student (
     *   id string,
     *   name string,
     *   age integer,
     *   address string
     * )
     */
    strcpy(tableName, TABLE_NAME);
    i = 0;

    strcpy(tableInfo.fieldInfo[i].name, "id");
    tableInfo.fieldInfo[i].dataType = TYPE_STRING;
    i++;

    strcpy(tableInfo.fieldInfo[i].name, "name");
    tableInfo.fieldInfo[i].dataType = TYPE_STRING;
    i++;

    strcpy(tableInfo.fieldInfo[i].name, "age");
    tableInfo.fieldInfo[i].dataType = TYPE_INTEGER;
    i++;

    strcpy(tableInfo.fieldInfo[i].name, "address");
    tableInfo.fieldInfo[i].dataType = TYPE_STRING;
    i++;

    tableInfo.numField = i;

    /* テーブルの作成 */
    if (createTable(tableName, &tableInfo) != OK) {
	/* テーブルの作成に失敗 */
	fprintf(stderr, "Cannot create table.\n");
	exit(1);
    }

    /* 挿入テスト */
    fprintf(stderr, "test1: Start\n\n");
    if (test1() == OK) {
	fprintf(stderr, "test1: OK\n\n");
    } else {
	fprintf(stderr, "test1: NG\n\n");
    }

    /* 検索テスト */
    fprintf(stderr, "test2: Start\n\n");
    if (test2() == OK) {
	fprintf(stderr, "test2: OK\n\n");
    } else {
	fprintf(stderr, "test2: NG\n\n");
    }

    /* 削除テスト */
    fprintf(stderr, "test3: Start\n\n");
    if (test3() == OK) {
	fprintf(stderr, "test3: OK\n\n");
    } else {
	fprintf(stderr, "test3: NG\n\n");
    }

    /* 後始末 */
    dropTable(TABLE_NAME);
    finalizeDataManipModule();
    finalizeDataDefModule();
    finalizeFileModule();
}
