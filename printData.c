/*
 * printTableData -- すべてのデータの表示(テスト用)
 *
 * 引数:
 *	tableName: データを表示するテーブルの名前
 */
void printTableData(char *tableName)
{
    TableInfo *tableInfo;
    File *file;
    int len;
    int i, j, k;
    int recordSize;
    int numPage;
    char *filename;
    char page[PAGE_SIZE];

    /* テーブルのデータ定義情報を取得する */
    if ((tableInfo = getTableInfo(tableName)) == NULL) {
	return;
    }

    /* 1レコード分のデータをファイルに収めるのに必要なバイト数を計算する */
    recordSize = getRecordSize(tableInfo);

    /* データファイルのファイル名を保存するメモリ領域の確保 */
    len = strlen(tableName) + strlen(DATA_FILE_EXT) + 1;
    if ((filename = malloc(len)) == NULL) {
	freeTableInfo(tableInfo);
	return;
    }

    /* ファイル名の作成 */
    snprintf(filename, len, "%s%s", tableName, DATA_FILE_EXT);

    /* データファイルのページ数を求める */
    numPage = getNumPages(filename);

    /* データファイルをオープンする */
    if ((file = openFile(filename)) == NULL) {
	free(filename);
	freeTableInfo(tableInfo);
	return;
    }

    free(filename);

    /* レコードを1つずつ取りだし、表示する */
    for (i = 0; i < numPage; i++) {
        /* 1ページ分のデータを読み込む */
        readPage(file, i, page);

        /* pageの先頭からrecord_sizeバイトずつ切り取って処理する */
        for (j = 0; j < (PAGE_SIZE / recordSize); j++) {
            /* 先頭の「使用中」のフラグが0だったら読み飛ばす */
	    char *p = &page[recordSize * j];
	    if (*p == 0) {
		continue;
	    }

	    /* フラグの分だけポインタを進める */
	    p++;

            /* 1レコード分のデータを出力する */
	    for (k = 0; k < tableInfo->numField; k++) {
		int intValue;
		char stringValue[MAX_STRING];

		printf("Field %s = ", tableInfo->fieldInfo[k].name);

		switch (tableInfo->fieldInfo[k].dataType) {
		case TYPE_INTEGER:
		    memcpy(&intValue, p, sizeof(int));
		    p += sizeof(int);
		    printf("%d\n", intValue);
		    break;
		case TYPE_STRING:
		    memcpy(stringValue, p, MAX_STRING);
		    p += MAX_STRING;
		    printf("%s\n", stringValue);
		    break;
		default:
		    /* ここに来ることはないはず */
		    return;
		}
	    }

	    printf("\n");
	}
    }
}

/*
 * printRecordSet -- レコード集合の表示
 *
 * 引数:
 *	recordSet: 表示するレコード集合
 */
void printRecordSet(RecordSet *recordSet)
{
    RecordData *record;
    int i, j, k;

    /* レコード数の表示 */
    printf("Number of Records: %d\n", recordSet->numRecord);

    /* レコードを1つずつ取りだし、表示する */
    for (record = recordSet->recordData; record != NULL; record = record->next) {
        /* すべてのフィールドのフィールド名とフィールド値を表示する */
	for (i = 0; i < record->numField; i++) {
	    printf("Field %s = ", record->fieldData[i].name);

	    switch (record->fieldData[i].dataType) {
	    case TYPE_INTEGER:
		printf("%d\n", record->fieldData[i].intValue);
		break;
	    case TYPE_STRING:
		printf("%s\n", record->fieldData[i].stringValue);
		break;
	    default:
		/* ここに来ることはないはず */
		return;
	    }
	}

	printf("\n");
    }
}
