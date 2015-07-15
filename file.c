/*
 * file.c -- ファイルアクセスモジュール 
 */

#include "microdb.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * initializeFileModule -- ファイルアクセスモジュールの初期化処理
 *
 * 引数:
 *	なし
 *
 * 返り値:
 *	成功の場合OK、失敗の場合NG
 */
Result initializeFileModule()
{
    return OK;
}

/*
 * finalizeFileModule -- ファイルアクセスモジュールの終了処理
 *
 * 引数:
 *	なし
 *
 * 返り値:
 *	成功の場合OK、失敗の場合NG
 */
Result finalizeFileModule()
{
    return OK;
}

/*
 * createFile -- ファイルの作成
 *
 * 引数:
 *	*filename: 作成するファイルのファイル名
 *
 * 返り値:
 *	成功の場合OK、失敗の場合NG
 */
Result createFile(char *filename)
{
	initializeFileModule();
	if( creat(filename, S_IRUSR | S_IWUSR) == -1 )
		return NG;
	finalizeFileModule();
	return OK;
}

/*
 * deleteFile -- ファイルの削除
 *
 * 引数:
 *	*filename
 *
 * 返り値:
 *	Result
 */
Result deleteFile(char *filename)
{
	initializeFileModule();
	if(unlink(filename) == -1) {
		return NG;
	}
	finalizeFileModule();
	return OK;
}

/*
 * openFile -- ファイルのオープン
 *
 * 引数:
 *	*fileame
 *
 * 返り値:
 *	オープンしたファイルのFile構造体
 *	オープンに失敗した場合にはNULLを返す
 */
File *openFile(char *filename)
{
	File *file;
	file = malloc(sizeof(File));
	if ( file == NULL){
		return NULL;
	}
	initializeFileModule();
	if( (file -> desc = open(filename , O_RDWR)) == -1 ){
		return NULL;
	}
	strcpy(file -> name , filename);
	finalizeFileModule();

	return file;
}

/*
 * closeFile -- ファイルのクローズ
 *
 * 引数:
 *	クローズするファイルのFile構造体
 *
 * 返り値:
 *	成功の場合OK、失敗の場合NG
 */
Result closeFile(File *file)
{

	initializeFileModule();
	if( close (file -> desc) == -1 ){
		return NG;
	}	
	free(file);
	finalizeFileModule();
	return OK;
}

/*
 * readPage -- 1ページ分のデータのファイルからの読み出し
 *
 * 引数:
 *	file: アクセスするファイルのFile構造体
 *	pageNum: 読み出すページの番号
 *	page: 読み出した内容を格納するPAGE_SIZEバイトの領域
 *
 * 返り値:
 *	成功の場合OK、失敗の場合NG
 */
Result readPage(File *file, int pageNum, char *page)
{
	initializeFileModule();

	if (lseek(file -> desc,PAGE_SIZE*pageNum, SEEK_SET) == -1) {
        return NG;
    }
    if (read(file -> desc, page, PAGE_SIZE ) == -1) {
    	return NG;   
    }
	finalizeFileModule();
	return OK;
}

/*
 * writePage -- 1ページ分のデータのファイルへの書き出し
 *
 * 引数:
 *	file: アクセスするファイルのFile構造体
 *	pageNum: 書き出すページの番号
 *	page: 書き出す内容を格納するPAGE_SIZEバイトの領域
 *
 * 返り値:
 *	成功の場合OK、失敗の場合NG
 */
Result writePage(File *file, int pageNum, char *page)
{
	initializeFileModule();
	if (lseek(file -> desc,PAGE_SIZE*pageNum, SEEK_SET) == -1) {
        return NG;
    }
    if (write(file -> desc, page, PAGE_SIZE ) == -1) {
    	return NG;   
    }
	finalizeFileModule();
	return OK;
}

/*
 * getNumPage -- ファイルのページ数の取得
 *
 * 引数:
 *	filename: ファイル名
 *
 * 返り値:
 *	引数で指定されたファイルの大きさ(ページ数)
 *	エラーの場合には-1を返す
 */
int getNumPages(char *filename)
{
	struct stat stbuf;
	initializeFileModule();
	if(stat (filename , &stbuf) == -1){
		return -1;
	}
	finalizeFileModule();
	return (stbuf.st_size/PAGE_SIZE);
}
