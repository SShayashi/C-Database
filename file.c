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
 * NUM_BUFFER -- ファイルアクセスモジュールが管理するバッファの大きさ(ページ数)
 */
#define NUM_BUFFER 4

/*
 * modifyFlag -- 変更フラグ
 */
typedef enum { UNMODIFIED = 0, MODIFIED = 1 } modifyFlag;

/*
 * Buffer -- 1ページ分のバッファを記憶する構造体
 */
typedef struct Buffer Buffer;
struct Buffer {
    File *file;					/* バッファの内容が格納されたファイル */
								/* file == NULLならこのバッファは未使用 */
    int pageNum;				/* ページ番号 */
    char page[PAGE_SIZE];		/* ページの内容を格納する配列 */
    struct Buffer *prev;		/* 一つ前のバッファへのポインタ */
    struct Buffer *next;		/* 一つ後ろのバッファへのポインタ */
    modifyFlag modified;		/* ページの内容が更新されたかどうかを示すフラグ */
};

/*
 * bufferListHead -- LRUリストの先頭へのポインタ
 */
static Buffer *bufferListHead = NULL;

/*
 * bufferListTail -- LRUリストの最後へのポインタ
 */
static Buffer *bufferListTail = NULL;

/*
 * initializeBufferList -- バッファリストの初期化
 *
 * **注意**
 *	この関数は、ファイルアクセスモジュールを使用する前に必ず一度だけ呼び出すこと。
 *	(initializeFileModule()から呼び出すこと。)
 *
 * 引数:
 *	なし
 *
 * 返り値:
 *	初期化に成功すればOK、失敗すればNGを返す。
 */
static Result initializeBufferList()
{
    Buffer *oldBuf = NULL;
    Buffer *buf;
    int i;

    /*
     * NUM_BUFFER個分のバッファを用意し、
     * ポインタをつないで両方向リストにする
     */
    for (i = 0; i < NUM_BUFFER; i++) {	
		/* 1個分のバッファ(Buffer構造体)のメモリ領域の確保 */
		if ((buf = (Buffer *) malloc(sizeof(Buffer))) == NULL) {
		    /* メモリ不足なのでエラーを返す */
		    return NG;
		}

		/* Buffer構造体の初期化 */
		buf->file = NULL;
		buf->pageNum = -1;
		buf->modified = UNMODIFIED;
		memset(buf->page, 0, PAGE_SIZE);
		buf->prev = NULL;
		buf->next = NULL;

		/* ポインタをつないで両方向リストにする */
		if (oldBuf != NULL) {
		    oldBuf->next = buf;
		}
		buf->prev = oldBuf;

		/* リストの一番最初の要素へのポインタを保存 */
		if (buf->prev == NULL) {
		    bufferListHead = buf;
		}

		/* リストの一番最後の要素へのポインタを保存 */
		if (i == NUM_BUFFER - 1) {
		    bufferListTail = buf;
		}

		/* 次のループのために保存 */
		oldBuf = buf;
    }

    return OK;
}


/*
 * moveBufferToListHead -- バッファをリストの先頭へ移動
 *
 * 引数:
 *	buf: リストの先頭に移動させるバッファへのポインタ
 *
 * 返り値:
 *	なしです
 */
static void moveBufferToListHead(Buffer *buf)
{
	/*　先頭のポインタを見つけるようのポインタ*/
	Buffer *p = NULL;
    int i;

    /* bufferが先頭の場合何もしない */
    if( buf == bufferListHead){
    	return ;
    }

    /* 現在のリストを遡って先頭のポインタを見つける */
	for( p = buf ; p -> prev != NULL ; p = p -> prev ){
	}

	/* bufferが最後尾の場合*/
	if( buf == bufferListTail){

		/*最後尾を更新*/
		bufferListTail =  buf -> prev -> next ;

		/*リストからの削除処理*/
		buf -> prev -> next = NULL;
		buf -> prev = NULL;

		/*先頭への挿入処理*/
		buf -> next = p;
		p -> prev = buf;
		buf -> prev = NULL;
		bufferListTail = buf;

		return ;
	}

	/*リストからの削除処理*/
	buf -> prev -> next = NULL;
	buf -> prev = NULL;

	/*先頭への挿入処理*/
	buf -> next = p;
	p -> prev = buf;
	buf -> prev = NULL;
	bufferListTail = buf;
}


/*
 * finalizeBufferList -- バッファリストの終了処理
 *
 * **注意**
 *	この関数は、ファイルアクセスモジュールの使用後に必ず一度だけ呼び出すこと。
 *	(finalizeFileModule()から呼び出すこと。)
 *
 * 引数:
 *	なし
 *
 * 返り値:
 *	終了処理に成功すればOK、失敗すればNGを返す。
 */
static Result finalizeBufferList()
{
    /* TODO */
    return OK;
}
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
	if(initializeBufferList() != OK){
		return NG;
	}
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
	if( creat(filename, S_IRUSR | S_IWUSR) == -1 )
		return NG;

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
	
	if(unlink(filename) == -1) {
		return NG;
	}
	
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
	
	if( (file -> desc = open(filename , O_RDWR)) == -1 ){
		return NULL;
	}
	strcpy(file -> name , filename);
	 

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
	Buffer *buf,*emptyBuf;

	/*同じファイルから読み込まれているページが複数ある可能性もある*/
	for (buf = bufferListHead; buf != NULL; buf = buf->next)
	{
	/* 引数のファイルが保存されているバッファを見つける */
	if (buf->file == file ) {
		/*変更フラグが立っていたら書き戻す*/
	 	if( buf -> modified == MODIFIED){
	 		if (lseek(buf -> file -> desc,PAGE_SIZE*buf -> pageNum, SEEK_SET) == -1) {
    	    	return NG;
   			}
	    	if (write(buf -> file -> desc, buf -> page, PAGE_SIZE ) == -1) {
	    		return NG;   
	    	}
	    	/* 変更フラグを0に戻す */
	    	buf -> modified = UNMODIFIED;
	 	}
	 	/* 書き戻したのでバッファの中身を空にする */
	 	buf -> file = NULL; 
	}
	}
	if( close (file -> desc) == -1 ){
		return NG;
	}	   
 	free(file); 	
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
	
	Buffer *buf,*emptyBuf;
	/*
     * 読み出しを要求されたページがバッファに保存されているかどうか、
     * リストの先頭から順に探す
     */
    for (buf = bufferListHead; buf != NULL; buf = buf->next) {
	/* 要求されたページがリストの中にあるかどうかチェックする */
	if (buf->file == file && buf->pageNum == pageNum) {
	    /* 要求されたページがバッファにあったので、その内容を引数のpageにコピーする */
	    memcpy(page, buf -> page , PAGE_SIZE );

	    /* アクセスされたバッファを、リストの先頭に移動させる */
	    moveBufferToListHead(buf);

	    return OK;
	}

	/* バッファの空きがあれば,emptyBufとする */
	if( buf -> file == NULL){
		// emptyBuf = buf;
		// emptyBuf -> file = file;
		// emptyBuf -> pageNum = pageNum;
		// memcpy(emptyBuf -> page , page , PAGE_SIZE);
		emptyBuf = buf;
		break;
	}
    }
	
	/*空きが見つからなかったら、バッファリストの一番最後のバッファを空ける。*/
    if( emptyBuf == NULL){
    	emptyBuf = bufferListTail;
    }
    
	/*
	 *このとき、最後のバッファに変更フラグが立っていたら、
	 *バッファの内容が変更されているので、その内容をlseekとwriteでファイルに書き戻す。
	 */
	 if( emptyBuf -> modified == MODIFIED){
	 	if (lseek(emptyBuf -> file -> desc,PAGE_SIZE*pageNum, SEEK_SET) == -1) {
    	    return NG;
   		}
	    if (write(emptyBuf -> file -> desc, page, PAGE_SIZE ) == -1) {
	    	return NG;   
	    }
	    /* 変更フラグを0に戻す */
	    emptyBuf -> modified = UNMODIFIED;
	 }

	/*
     * lseekとreadシステムコールで空きバッファにファイルの内容を読み込み、
     * Buffer構造体に保存する
     */

    /* 読み出し位置の設定 */
    if (lseek(file->desc, pageNum * PAGE_SIZE, SEEK_SET) == -1) {
	return NG;
    }

    /* 1ページ分のデータをemptyBufへ読み出す */
    if (read(file->desc, emptyBuf->page, PAGE_SIZE) < PAGE_SIZE) {
	return NG;
    }

    /* バッファの内容を引数のpageにコピー */
    memcpy(page, emptyBuf -> page , PAGE_SIZE );

    /* Buffer構造体(emptyBuf)への各種情報の設定 */
	emptyBuf -> file = file;
	emptyBuf -> pageNum = pageNum;
	memcpy(emptyBuf -> page , page , PAGE_SIZE);

    /* アクセスされたバッファ(emptyBuf)を、リストの先頭に移動させる */
    moveBufferToListHead(emptyBuf);

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
	
	Buffer *buf,*emptyBuf;

	/*
     * 書き出しを要求されたページがバッファに保存されているかどうか、
     * リストの先頭から順に探す
     */
    for (buf = bufferListHead; buf != NULL; buf = buf->next) {
	/* 要求されたページがリストの中にあるかどうかチェックする */
	if (buf->file == file && buf->pageNum == pageNum) {
	    /* 要求されたページがバッファにあったので、その引数のpageの内容をバッファにコピーする */
	    memcpy(buf -> page, page, PAGE_SIZE );
	    /*データの更新を行ったのでMODIFIEDにする*/
	    buf -> modified = MODIFIED;
	    /* アクセスされたバッファを、リストの先頭に移動させる */
	    moveBufferToListHead(buf);

	    return OK;
	}

	/* バッファの空きがあれば、emptyBufとする */
	if( buf -> file == NULL){
		emptyBuf = buf;
		break;
	}
    }
	
	/*空きが見つからなかったら、バッファリストの一番最後のバッファを空ける。*/
    if( emptyBuf == NULL){
    	emptyBuf = bufferListTail;
    }
    
	/*
	 *このとき、最後のバッファに変更フラグが立っていたら、
	 *バッファの内容が変更されているので、その内容をlseekとwriteでファイルに書き戻してから書き直す
	 */
	 if( emptyBuf -> modified == MODIFIED){
	 	if (lseek(emptyBuf -> file -> desc,PAGE_SIZE*pageNum, SEEK_SET) == -1) {
    	    return NG;
   		}
	    if (write(emptyBuf -> file -> desc, page, PAGE_SIZE ) == -1) {
	    	return NG;   
	    }
	    /* 変更フラグを0に戻す */
	    emptyBuf -> modified = UNMODIFIED;
	 }
    
    /* Buffer構造体(emptyBuf)への各種情報の設定 */
	emptyBuf -> file = file;
	emptyBuf -> pageNum = pageNum;
	/* emptyBufに引数のpageを書き込む*/
	memcpy(emptyBuf -> page , page , PAGE_SIZE);

    /* アクセスされたバッファ(emptyBuf)を、リストの先頭に移動させる */
    moveBufferToListHead(emptyBuf);

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
	
	if(stat (filename , &stbuf) == -1){
		return -1;
	}
	
	
	return (stbuf.st_size/PAGE_SIZE);
}








