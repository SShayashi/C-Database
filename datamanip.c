/*
 * datamanip.c -- データ操作モジュール
 */

#include "microdb.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * DATA_FILE_EXT -- データファイルの拡張子
 */
#define DATA_FILE_EXT ".dat"

/*
 * addFlag -- データ追加フラグ
 * このフラグが立っているレコードデータのみ追加する    
 */
typedef enum { NOT_ADD = 0, ADD = 1 } AddFlag;

/*
 * initializeDataManipModule -- データ操作モジュールの初期化
 *
 * 引数:
 *	なし
 *
 * 返り値;
 *	成功ならOK、失敗ならNGを返す
 */
Result initializeDataManipModule()
{
    return OK;
}

/*
 * finalizeDataManipModule -- データ操作モジュールの終了処理
 *
 * 引数:
 *	なし
 *
 * 返り値;
 *	成功ならOK、失敗ならNGを返す
 */
Result finalizeDataManipModule()
{
    return OK;
}

/*
 * compareFieldData --渡されたフィールドが等しいかどうかを返す  
 *   
 * 引数；
 *  x：FieldData型のデータ
 *  y：FieldData型のデータ
 *  返り値：
 *  Result 
 */
static Result compareFieldData(FieldData *x , FieldData *y){
    if( strcmp(x -> name ,y -> name) != 0 ){
        return NG;
    }

    if( x -> dataType != y -> dataType){
        return NG;
    }

    if( x -> intValue != y -> intValue){
        return NG;
    }

    if( strcmp(x -> stringValue , y -> stringValue) != 0) {
        return NG;
    }
    return OK;
}


/*
 * compareRecordData --渡されたレコードが等しいかどうかを返す  
 *   
 * 引数；
 *  x：RecordData型のデータ
 *  y：RecordData型のデータ
 *  返り値：
 *   Result
 */
static Result compareRecordData(RecordData *x , RecordData *y){
    
    if( x -> numField != y -> numField){
        return NG;
    }

    if( !compareFieldData(x -> fieldData , y -> fieldData)){
        return NG;
    }
    
    return OK;
}




/*
 * getRecordSize -- 1レコード分の保存に必要なバイト数の計算
 *
 * 引数:
 *	tableInfo: データ定義情報を収めた構造体
 *
 * 返り値:
 *	tableInfoのテーブルに収められた1つのレコードを保存するのに
 *	必要なバイト数
 * 教官からのコメント
 * データレコードが"まとめて"ページにはいるかどうかを確認しているプログラムのため、
 * フィールドとフィールドの間に空きができることはない！！！
 * また、小分けしてフィールドの情報を格納する場合は、ポインタを動かす度に、逐一4096を超えているか
 * 確認する必要がある！！           
 */
static int getRecordSize(TableInfo *tableInfo)
{
    int total = 0;
    int i;
    for ( i = 0 ; i < tableInfo -> numField ; i ++) {//フィールド数分だけループする
        /* i番目のフィールドがINT型かSTRING型か調べる */

        /* INT型ならtotalにsizeof(int)を加算 */
        if( tableInfo -> fieldInfo[i].dataType == TYPE_INTEGER){
            total += sizeof(int);
        }
        /* STRING型ならtotalにMAX_STRINGを加算 */
        if(tableInfo -> fieldInfo[i].dataType == TYPE_STRING){
            total += MAX_STRING;
        }
    }

    /* フラグの分の1を足す */
    total++;

    return total;
}

/*
 * insertRecord -- レコードの挿入
 *
 * 引数:
 *	tableName: レコードを挿入するテーブルの名前
 *	recordData: 挿入するレコードのデータ
 *
 * 返り値:
 *	挿入に成功したらOK、失敗したらNGを返す
 */
Result insertRecord(char *tableName, RecordData *recordData)
{
    TableInfo *tableInfo;
    int recordSize;
    int numPage;
    char *record;
    char *p;
    char page[PAGE_SIZE];
    char *filename;
    long len;
    File *file;
    int i,j;


    /* テーブルの情報を取得する */
    if ( (tableInfo = getTableInfo(tableName)) == NULL){
        /* エラー処理 */
        return NG;
    }

    /* 1レコード分のデータをファイルに収めるのに必要なバイト数を計算する */
    recordSize = getRecordSize(tableInfo);

    /* 必要なバイト数分のメモリを確保する */
    if ((record = (char *)malloc(recordSize)) == NULL) {
        /* エラー処理 */
        return NG;
    }
   
    p = record;

    /* 先頭に、「使用中」を意味するフラグを立てる */
    memset(p, 1, 1);
    p += 1;

    /* 確保したメモリ領域に、フィールド数分だけ、順次データを埋め込む */
    for (i = 0; i < tableInfo->numField; i++) {
    	switch (tableInfo->fieldInfo[i].dataType) {
    	case TYPE_INTEGER:
            memcpy(p, &(recordData-> fieldData[i].intValue) , sizeof(int));
    	    p += sizeof(int);
    	    break;
    	case TYPE_STRING:
    	    memcpy(p, &(recordData-> fieldData[i].stringValue) , MAX_STRING);
    	    p += MAX_STRING;
    	    break;
    	default:
    	    /* ここにくることはないはず */
                freeTableInfo(tableInfo);
                free(record);
    	    return NG;
    	}
    }


    /* 使用済みのtableInfoデータのメモリを解放する */
    freeTableInfo(tableInfo);

    /*
     * ここまでで、挿入するレコードの情報を埋め込んだバイト列recordができあがる
     */
    
     /* [tableName].datという文字列を作る */   
    len = strlen(tableName) + strlen(DATA_FILE_EXT) + 1;
    if ((filename = malloc(len)) == NULL) {
        return NG;
    }
    snprintf(filename, len, "%s%s", tableName, DATA_FILE_EXT);

    /* データファイルをオープンする */
    if((file = openFile(filename)) == NULL){
        return NG;
    }

    /* データファイルのページ数を調べる */
    numPage = getNumPages(filename);


    /* PAGE_SIZEバイト分の大きさを持つ配列pageを初期化する */
    memset(page, 0, PAGE_SIZE);
    

    /* レコードを挿入できる場所を探す */
    for ( i = 0; i < numPage; i++) {
        /* 1ページ分のデータを読み込む */
        if (readPage(file, i, page) != OK) {
            free(record);
	    return NG;
	   }
        /* pageの先頭からrecordSizeバイトずつ飛びながら、先頭のフラグが「0」(未使用)の場所を探す */
        for ( j = 0; j < (PAGE_SIZE / recordSize); j++) {
    	    char *q;
            q = page; 
    	    q = q + recordSize*j;
    	    if (*q == 0) {
        		/* 見つけた空き領域に上で用意したバイト列recordを埋め込む */
        		memcpy(q, record , recordSize);

        		/* ファイルに書き戻す */
        		if (writePage(file, i, page) != OK) {
                            free(record);
        		    return NG;
        		}
        		closeFile(file);
                        free(record);
        		return OK;
	       }
	    }
    }

    /*
     * ファイルの最後まで探しても未使用の場所が見つからなかったら
     * ファイルの最後に新しく空のページを用意し、そこに書き込む
     */
    /*からページの作成*/
    char Epage[PAGE_SIZE];
    memset(Epage , 0 , PAGE_SIZE);
    /*空ページの書き込み*/
    if(writePage(file , numPage, Epage) != OK){
        return NG;
    }
    /*numPageが0のとき（初めてデータを書き込むとき）のみの処理(要改善)*/
    if(numPage == 0){
        char *q;
        q = page;
        /*レコードを書き込む*/
        memcpy(q, record , recordSize);
    }

    /* ファイルに書き戻す */
    if (writePage(file, numPage , page) != OK) {
                free(record);
        return NG;
    }
    closeFile(file);
    free(record);
    return OK;
}



/*
 * checkCondition -- レコードが条件を満足するかどうかのチェック
 *
 * 引数:
 *	recordData: チェックするレコード
 *	condition: チェックする条件
 *
 * 返り値:
 *	レコードrecordが条件conditionを満足すればOK、満足しなければNGを返す
 */
static Result checkCondition(RecordData *recordData, Condition *condition)
{
  int i;
    /*条件式が存在せず、全てのレコード表示の場合*/
    if(condition -> allmach == 1 ){
        return OK;
    }

     /* 条件conditionに指定されているフィールド名をrecordから探す */
    for ( i = 0; i < recordData->numField ; i++) {
        /* フィールド名が同じかどうかチェック */
        if (strcmp(recordData -> fieldData[i].name, condition->name) == 0) {

            /* 比較演算子を満足するかどうかのチェック */ 
            if( recordData -> fieldData[i].dataType == TYPE_STRING)
            {
                switch( condition -> operator){
                case OPR_EQUAL : 
                        if( strcmp(recordData -> fieldData[i].stringValue, condition -> stringValue) == 0  ){

                        }else{
                            return NG;
                        }
                    break;
                case OPR_NOT_EQUAL : 
                    if( strcmp(recordData -> fieldData[i].stringValue, condition -> stringValue) != 0){

                    }else{
                        return NG;
                    }
                    break;  /* != */
                case OPR_GREATER_THAN : /* > */
                    return NG;
                case OPR_LESS_THAN :/* < */     
                    return NG;
                default  : 
                    return NG;
                }
            }

            if( recordData -> fieldData[i].dataType == TYPE_INTEGER)
            {
                switch( condition -> operator){
                case OPR_EQUAL : 

                 if( recordData -> fieldData[i].intValue == condition -> intValue){

                    }else{
                        return NG;
                    }
                    break;       /* = */                   
                case OPR_NOT_EQUAL : 
                 if( recordData -> fieldData[i].intValue != condition -> intValue){

                    }else{
                        return NG;
                    }

                    break;  /* != */
                case OPR_GREATER_THAN :
                   
                 if( recordData -> fieldData[i].intValue > condition -> intValue){
                    
                    }else{
                        return NG;
                    }
                    break;       /* > */
                case OPR_LESS_THAN   :
                 if( recordData -> fieldData[i].intValue < condition -> intValue){

                    }else{
                        return NG;
                    }
                    break;  /* < */     
                    default  : 
                    return NG;
                }
            }
            if(recordData -> fieldData[i].dataType == TYPE_UNKNOWN){
                return NG;
            }
        }
    }
    return OK;
}

/*
 * selectRecord -- レコードの検索
 *
 * 引数:
 *	tableName: レコードを検索するテーブルの名前
 *	condition: 検索するレコードの条件
 *
 * 返り値:
 *	検索に成功したら検索されたレコード(の集合)へのポインタを返し、
 *	検索に失敗したらNULLを返す。
 *	検索した結果、該当するレコードが1つもなかった場合も、レコードの
 *	集合へのポインタを返す。
 *
 * ***注意***
 *	この関数が返すレコードの集合を収めたメモリ領域は、不要になったら
 *	必ずfreeRecordSetで解放すること。
 */
RecordSet *selectRecord(char *tableName, Condition *condition)
{
     RecordSet *recordSet; 
     RecordData *recordData; 
     File *file;
     TableInfo *tableInfo;
     long len;
     char *filename;
     int numPage;
     char page[PAGE_SIZE];
     int recordSize;
     int i,j,k;
     /*レコードセットの初期化 */
     if ((recordSet = (RecordSet *) malloc(sizeof(RecordSet))) == NULL) 
     {
                    /* エラー処理 */
        return NULL;
     }
     recordSet -> numRecord = 0;
     recordSet -> tail = NULL;
     recordSet -> recordData = NULL;

    /* [tableName].datという文字列を作る */   
    len = strlen(tableName) + strlen(DATA_FILE_EXT) + 1;
    if ((filename = malloc(len)) == NULL) {
        return NULL;
    }
    snprintf(filename, len, "%s%s", tableName, DATA_FILE_EXT);

    /*ファイルのオープン*/
    if( (file = openFile(filename))== NULL){
        return NULL;
    }



    /*ページ数の取得*/
    numPage = getNumPages(filename);


    /*テーブル情報の取得*/
    tableInfo = getTableInfo(tableName);

    /*レコードサイズの取得*/
    recordSize = getRecordSize(tableInfo);

    
    /* データが挿入されていない時はそのままrecordSetを返す */
    if ( numPage == 0){

        freeTableInfo(tableInfo);
        if((closeFile(file)) != OK){
            return NULL;
        }
    return recordSet;

    }

    /*ページ数の数だけループする*/
    for( i = 0 ; i  < numPage ; i ++ ){

        
        /*1ページ分読み込む*/
        if (readPage(file, i, page) != OK) {
            /* エラー処理 */
            return NULL;
        }

        /*pageの戦闘からrecord_sizeバイトずつ切り取って処理する*/
        
        for (  j = 0 ; j < (PAGE_SIZE/recordSize) ; j++ ){
            char *p;
            p = page + recordSize * j; 
            
            /*先頭のポインタが１だったら（使用中だったら）レコードを読みこむ */
            if (*p == 1){
                /*メモリが使用中かどうかのフラグの数だけポインタを進める*/
                p++;
                /* RecordData構造体のためのメモリを確保する */
                if ((recordData = (RecordData *) malloc(sizeof(RecordData))) == NULL) {
                    /* エラー処理 */
                }
                
                /*１レコード分のデータを、RecordData構造体に入れる*/
                recordData -> numField = tableInfo -> numField;
                recordData -> next = NULL;
                
                /* フィールド数分だけループして、データ型ごとに読み込んで構造体を作る*/
                for( k = 0 ; k  < (tableInfo -> numField) ; k++){
                    /*フィールド情報*/
                    strcpy(recordData -> fieldData[k].name ,tableInfo -> fieldInfo[k].name);
                    recordData -> fieldData[k].dataType = tableInfo -> fieldInfo[k].dataType;
                    switch (tableInfo->fieldInfo[k].dataType) {
                    case TYPE_INTEGER:
                        memcpy(&(recordData -> fieldData[k].intValue) , p , sizeof(int));
                        p += sizeof(int);
                        break;
                    case TYPE_STRING:
                        memcpy(&(recordData -> fieldData[k].stringValue) ,p , MAX_STRING);
                        p += MAX_STRING;
                        break;
                    default:
                        /* ここにくることはないはず */
                            freeTableInfo(tableInfo);
                            free(recordData);
                        return NULL;
                    }
                }
                
                /*条件に合ったレコードを挿入する*/
                if( checkCondition(recordData , condition) == OK){
		  RecordData *r;
		  r = recordSet -> recordData;
		  /* 基本は追加用のフラッグを立てておく */
		  AddFlag  addFlag = ADD;
		  
		  /*レコードセットにまだ一つもレコードがない場合*/
		  if( r == NULL)
          {
		      recordSet -> recordData = recordData;
		      recordSet -> tail = recordData;                        
		      /* レコードの数を追加 */
		      recordSet -> numRecord++;
		      continue;
            }
            /* 重複削除宣言がされていた場合 */
            if(condition->distinct == DISTINCT)
            {
              /*レコードセットの中を一つずつ見ていく*/
              while( r -> next != NULL)
              {
                /* RecordDataとまったく同じレコードがすでに RecordSetの線形リストにあったら、そのRecordDataは追加しない */
                if (compareRecordData(r , recordData) == OK)
                {
                  /* 追加しないようフラッグを立てる */
                  addFlag = NOT_ADD;
                  break;
                }
                r = r -> next;
              }  
            }
    		  		  
		  /* レコードを追加する必要がないため、処理を抜ける */
		  if(addFlag == NOT_ADD){
		    continue;
		  }
		  if(addFlag == ADD){
		    /* 末尾にデータを追加 */ 
		    recordSet -> tail -> next = recordData;
		    /* 追加したデータを末尾に登録 */
		    recordSet -> tail = recordData;       
		    /* レコードの数を増やす */
            recordSet -> numRecord++;
		  }
                }
            }
        }
    }
    
    freeTableInfo(tableInfo);
    if((closeFile(file)) != OK){
        return NULL;
    }
    return recordSet;
}




/*
 * freeRecordSet -- レコード集合の情報を収めたメモリ領域の解放
 *
 * 引数:
 *	recordSet: 解放するメモリ領域
 *
 * 返り値:
 *	なし
 *
 * ***注意***
 *	関数selectRecordが返すレコードの集合を収めたメモリ領域は、
 *	不要になったら必ずこの関数で解放すること。
 */
void freeRecordSet(RecordSet *recordSet)
{   
    RecordData *p;
    p = recordSet -> recordData;
    
    while(p != NULL){

        p = p -> next;
        free(recordSet -> recordData);
        recordSet -> recordData = p;

    }
    free(recordSet);
}

/*
 * deleteRecord -- レコードの削除
 *
 * 引数:
 *	tableName: レコードを削除するテーブルの名前
 *	condition: 削除するレコードの条件
 *
 * 返り値:
 *	削除に成功したらOK、失敗したらNGを返す
 */
Result deleteRecord(char *tableName, Condition *condition)
{

    File *file;
    TableInfo *tableInfo;
    int numPage;
    char *filename;
    char page[PAGE_SIZE];
    int recordSize;
    int len;
    int i,j,k;

    /* [tableName].datという文字列を作る */   
    len = strlen(tableName) + strlen(DATA_FILE_EXT) + 1;
    if ((filename = malloc(len)) == NULL) {
        return NG;
    }
    snprintf(filename, len, "%s%s", tableName, DATA_FILE_EXT);

    /*ファイルのオープン*/
    if( (file = openFile(filename))== NULL){
        return NG;
    }

    /*ページ数の取得*/
    numPage = getNumPages(filename);

    /*テーブル情報の取得*/
    tableInfo = getTableInfo(tableName);

    /*レコードサイズの取得*/
    recordSize = getRecordSize(tableInfo);


    /* レコードを1つずつ取りだし、条件を満足するかどうかチェックする */
    for ( i = 0; i < numPage; i++) {
        /* 1ページ分のデータを読み込む */
        if (readPage(file, i, page) != OK) {
            /* エラー処理 */
        }
        /* pageの先頭からrecord_sizeバイトずつ切り取って処理する */
        for ( j = 0; j < (PAGE_SIZE / recordSize); j++) {
            RecordData *recordData;
                char *p;

                /* 先頭の「使用中」のフラグが0だったら読み飛ばす */
            p = &page[recordSize * j];
            if (*p == 0) {
                continue;
            }

            /* RecordData構造体のためのメモリを確保する */
            if ((recordData = (RecordData *) malloc(sizeof(RecordData))) == NULL) {
                /* エラー処理 */
            }

            /* フラグの分だけポインタを進める */
            p++;

            
             /*１レコード分のデータを、RecordData構造体に入れる*/
                recordData -> numField = tableInfo -> numField;
                recordData -> next = NULL;
            for ( k = 0; k < tableInfo->numField; k++) {
                /* TableInfo構造体からRecordData構造体にフィールド名とデータ型の情報をコピーする */
                /*フィールド情報*/
                    strcpy(recordData -> fieldData[k].name ,tableInfo -> fieldInfo[k].name);
                    recordData -> fieldData[k].dataType = tableInfo -> fieldInfo[k].dataType;
                /* フィールド値の読み取り */
                switch (recordData->fieldData[k].dataType) {
                case TYPE_INTEGER:
                    memcpy(&(recordData -> fieldData[k].intValue ), p , sizeof(int));
                    p += sizeof(int);
                    break;
                case TYPE_STRING:
                    memcpy(&(recordData -> fieldData[k].stringValue) ,p , MAX_STRING);
                    p += MAX_STRING;
                    break;
                default:
                    /* ここに来ることはないはず */
                    freeTableInfo(tableInfo);
                    free(recordData);
                    return NG;
                }
            }
            /* 条件を満足するかどうか調べる */
            if (checkCondition(recordData, condition) == OK) {
            /* 条件を満足したので、そのレコードを削除する(使用フラグを0に書き換えていく) */
                page[recordSize * j] = 0;
            }
                /* 削除処理が終ったら不要なRecordData構造体のメモリを解放する */
                free(recordData);
        }

        /* ページの内容をファイルに書き戻す */
        if (writePage(file, i, page) != OK) {
            /* エラー処理 */
            return NG;
        }
    }
    printf("テーブルを削除しました");
    return OK;
}

/*
 * createDataFile -- データファイルの作成
 *
 * 引数:
 *	tableName: 作成するテーブルの名前
 *
 * 返り値:
 *	作成に成功したらOK、失敗したらNGを返す
 */
Result createDataFile(char *tableName)
{
    int len;
    char *filename;
    /* [tableName].datという文字列を作る */   
    len = (int)strlen(tableName) + strlen(DATA_FILE_EXT) + 1;
    if ((filename = malloc(len)) == NULL) {
        return NG;
    }
    snprintf(filename, len, "%s%s", tableName, DATA_FILE_EXT);
    
    if( (createFile(filename)) == NG){
        return NG;
    }

    return OK;

}

/*
 * deleteDataFile -- データファイルの削除
 *
 * 引数:
 *	tableName: 削除するテーブルの名前
 *
 * 返り値:
 *	削除に成功したらOK、失敗したらNGを返す
 */
Result deleteDataFile(char *tableName)
{
    int len;
    char *filename;
    /* [tableName].datという文字列を作る */   
    len = (int)strlen(tableName) + strlen(DATA_FILE_EXT) + 1;
    if ((filename = malloc(len)) == NULL) {
        return NG;
    }
    snprintf(filename, len, "%s%s", tableName, DATA_FILE_EXT);
    
    if( (deleteFile(filename)) == NG){
        return NG;
    }
    return OK;
}

/*
 * printRecordSet -- レコード集合の表示
 *
 * 引数:
 *	recordSet: 表示するレコード集合
 *
 * 返り値:
 *	なし
 */
void printRecordSet(RecordSet *recordSet)
{
    /*レコードデータが一つも挿入されてないとき*/
    if(recordSet -> numRecord == 0){
        printf("データがありません");
        return ;
    }

    RecordData *record;
    int i,sub;
    char fieldStr[MAX_STRING];// |Field  |Field  となっている部分
    char IntStr[MAX_STRING];//数値を文字列に変換した
    record = recordSet->recordData;
    int NumField = record -> numField;

    /* Fieldの上部分を表示*/
    printTableFence(NumField);

    /*
     * | Field名(20byte) | (20byte) |
     * を表示していく
     */
    for( i = 0 ; i < record -> numField ; i ++){
        /*空白を初期化*/
        memset(fieldStr,0,MAX_STRING);

        printf("|");
        printf("%s",record->fieldData[i].name);
        
        /*フィールドを代入した後の残りの空白の数を取得    */
        sub = MAX_STRING - strlen(record->fieldData[i].name);
        memset(fieldStr,' ',sub);
        printf("%s",fieldStr);
    }
    /*最後の"+"*/
    printf("|\n"); 

    /*Field部分の下部分を表示*/
    printTableFence(NumField);

    /* レコードを1つずつ取りだし、表示する */
    for (record = recordSet->recordData; record != NULL; record = record->next) {
            /* すべてのフィールドのフィールド名とフィールド値を表示する */
        for (i = 0; i < record->numField; i++) {
        /*空白を初期化*/
        memset(fieldStr,0,MAX_STRING);
        printf("|");

        switch (record->fieldData[i].dataType) {
        case TYPE_INTEGER:

        memset(IntStr,0,MAX_STRING);
        sprintf(IntStr,"%d",record->fieldData[i].intValue );        
        sub = MAX_STRING - strlen(IntStr);
        memset(fieldStr,' ',sub);

        printf("%s",fieldStr );
        printf("%d", record->fieldData[i].intValue);

        break;
        case TYPE_STRING:

         /*フィールドを代入した後の残りの空白の数を取得    */
        sub = MAX_STRING - strlen(record->fieldData[i].stringValue);
        memset(fieldStr,' ',sub);
        printf("%s", record->fieldData[i].stringValue);
        printf("%s",fieldStr);
       

        break;
        default:
        /* ここに来ることはないはず */
        return;
        }

    }
    printf("|");
    printf("\n");
    }
    printTableFence(NumField);
}
/*
 * printTableFence -- +--------+の部分を表示する
 *
 * 引数:
 *  NumField: フィールド数
 */
void printTableFence(int NumField){

char tableStr[MAX_STRING]; // +-------+------　の部分
int i;
 /* tableStrに"-"を詰め込んでいく*/
memset(tableStr,'-',MAX_STRING);

/*描画開始*/
for(i = 0; i <  NumField ; i ++){
        printf("+");
        printf("%s",tableStr);
    }
    /*最後の"+"*/
    printf("+\n");

}


/*
 * printTableData -- すべてのデータの表示(テスト用)
 *
 * 引数:
 *  tableName: データを表示するテーブルの名前
 */
void printTableData(char *tableName)
{
    TableInfo *tableInfo;
    File *file;
    long len;
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

