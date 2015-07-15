/*
 * バッファリング機能テストプログラム
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "microdb.h"

/*
 * テスト名
 */
#define TEST_NAME "test-buffer"

/*
 * テスト用ファイルのファイル名
 */
#define TEST_FILE1 "testfile1"
#define TEST_FILE2 "testfile2"

/*
 * ファイルサイズ(ファイルに書き込むページ数)
 */
#define FILE_SIZE 6

/*
 * テスト回数
 */
#define TEST_SIZE 20

/*
 * initializeRandomGenerator -- 乱数発生器の初期化
 *
 * 引数:
 *	なし
 *
 * 返り値:
 *	なし
 */
void initializeRandomGenerator()
{
    /* 乱数用関数の初期化 */
    srand((unsigned int) time(NULL));

    return;
}

/*
 * getRandomInteger -- 乱数の発生
 */
int getRandomInteger(int min, int max)
{
    int r;

    /* min〜maxまでの(整数の)乱数を作る */
    r = min + (int) (max - min + 1) * (rand() / (RAND_MAX + 1.0));

    return r;
}

/*
 * test1 -- 読み出しのテスト
 */
void test1()
{
    File *file[2];
    char page[PAGE_SIZE];
    int i;
    int fileNum, pageNum;

    printf("---------- test1 start ----------");

    /* ファイルをープンする */
    if ((file[0] = openFile(TEST_FILE1)) == NULL) {
	fprintf(stderr, "Cannot open file.\n");
	exit(1);
    }

    if ((file[1] = openFile(TEST_FILE2)) == NULL) {
	fprintf(stderr, "Cannot open file.\n");
	exit(1);
    }

    /*
     * (TEST_SIZE)回に渡ってランダムにページにアクセスし、
     * 正しく稼働するかどうかを確認する
     */
    for (i = 0; i < TEST_SIZE; i++) {
	/* アクセスするファイルをランダムに決定 */
	fileNum = getRandomInteger(0, 1);

	/* アクセスするページをランダムに決定 */
	pageNum = getRandomInteger(0, FILE_SIZE - 1);

	/* ページにアクセス */
	printf("\nread file %d page %d\n", fileNum, pageNum);
	if (readPage(file[fileNum], pageNum, page) != OK) {
	    fprintf(stderr, "Cannot read page (fileNum = %d, pageNum = %d).\n", fileNum, pageNum);
	    exit(1);
	}

	/* バッファのリストの内容を出力 */
	printBufferList();
    }

    /* ファイルをクローズする */
    if (closeFile(file[0]) != OK) {
	fprintf(stderr, "Cannot close file.\n");
	exit(1);
    }

    if (closeFile(file[1]) != OK) {
	fprintf(stderr, "Cannot close file.\n");
	exit(1);
    }

    /*
     * 本格的にテストするためには、このあと何度かファイルをオープンし、
     * 同じようなテストを繰り返すのが良い。
     */

    printf("---------- test1 end ----------\n\n");
}

/*
 * test2 -- 読み書きのテスト
 */
void test2()
{
    File *file[2];
    char page[PAGE_SIZE];
    int i;
    int fileNum, pageNum;

    printf("---------- test2 start ----------\n");

    /* ファイルをープンする */
    if ((file[0] = openFile(TEST_FILE1)) == NULL) {
	fprintf(stderr, "Cannot open file.\n");
	exit(1);
    }

    if ((file[1] = openFile(TEST_FILE2)) == NULL) {
	fprintf(stderr, "Cannot open file.\n");
	exit(1);
    }
    printf("  -------- files opened -------  \n");

    /*
     * (TEST_SIZE)回に渡ってページの読み書きを交互に行い、
     * 正しく稼働するかどうかを確認する
     */
    for (i = 0; i < TEST_SIZE; i++) {
	/* アクセスするファイルをランダムに決定 */
	fileNum = getRandomInteger(0, 1);

	/* アクセスするページをランダムに決定 */
	pageNum = getRandomInteger(0, FILE_SIZE - 1);

	/* ページの読み出し */
	printf("\nread file %d page %d\n", fileNum, pageNum);
	if (readPage(file[fileNum], pageNum, page) != OK) {
	    fprintf(stderr, "Cannot read page (fileNum = %d, pageNum = %d).\n", fileNum, pageNum);
	    exit(1);
	}

	/* バッファのリストの内容を出力 */
	printBufferList();

	/* アクセスするファイルをランダムに決定 */
	fileNum = getRandomInteger(0, 1);

	/* アクセスするページをランダムに決定 */
	pageNum = getRandomInteger(0, FILE_SIZE - 1);

	/* ページに書き込む内容を作成 */
	sprintf(page, "%c%c%c", '0' + fileNum, 'A' + i, '0' + pageNum);

	/* ページの書き込み */
	printf("\nwrite file %d page %d -> %s\n", fileNum, pageNum, page);
	if (writePage(file[fileNum], pageNum, page) != OK) {
	    fprintf(stderr, "Cannot write page (fileNum = %d, pageNum = %d).\n", fileNum, pageNum);
	    exit(1);
	}

	/* バッファのリストの内容を出力 */
	printBufferList();
    }

    /* いったんファイルをクローズする */
    if (closeFile(file[0]) != OK) {
	fprintf(stderr, "Cannot close file.\n");
	exit(1);
    }

    if (closeFile(file[1]) != OK) {
	fprintf(stderr, "Cannot close file.\n");
	exit(1);
    }
    printf("  -------- files closed -------  \n");

    /* クローズした直後のバッファリストの状態を出力 */
    printBufferList();

    /* もう一度ファイルをオープンする */
    if ((file[0] = openFile(TEST_FILE1)) == NULL) {
	fprintf(stderr, "Cannot open file.\n");
	exit(1);
    }

    if ((file[1] = openFile(TEST_FILE2)) == NULL) {
	fprintf(stderr, "Cannot open file.\n");
	exit(1);
    }
    printf("  -------- files opened -------  \n");

    /*
     * もう一度、(TEST_SIZE)回に渡ってページの読み書きを交互に行い、
     * 正しく稼働するかどうかを確認する
     */
    for (i = 0; i < TEST_SIZE; i++) {
	/* アクセスするファイルをランダムに決定 */
	fileNum = getRandomInteger(0, 1);

	/* アクセスするページをランダムに決定 */
	pageNum = getRandomInteger(0, FILE_SIZE - 1);

	/* ページの読み出し */
	printf("\nread file %d page %d\n", fileNum, pageNum);
	if (readPage(file[fileNum], pageNum, page) != OK) {
	    fprintf(stderr, "Cannot read page (fileNum = %d, pageNum = %d).\n", fileNum, pageNum);
	    exit(1);
	}

	/* バッファのリストの内容を出力 */
	printBufferList();

	/* アクセスするファイルをランダムに決定 */
	fileNum = getRandomInteger(0, 1);

	/* アクセスするページをランダムに決定 */
	pageNum = getRandomInteger(0, FILE_SIZE - 1);

	/* ページに書き込む内容を作成 */
	sprintf(page, "%c%c%c", '0' + fileNum, 'A' + i, '0' + pageNum);

	/* ページの書き込み */
	printf("\nwrite file %d page %d -> %s\n", fileNum, pageNum, page);
	if (writePage(file[fileNum], pageNum, page) != OK) {
	    fprintf(stderr, "Cannot write page (fileNum = %d, pageNum = %d).\n", fileNum, pageNum);
	    exit(1);
	}

	/* バッファのリストの内容を出力 */
	printBufferList();
    }

    /* ファイルのクローズ */
    if (closeFile(file[0]) != OK) {
	fprintf(stderr, "Cannot close file.\n");
	exit(1);
    }

    if (closeFile(file[1]) != OK) {
	fprintf(stderr, "Cannot close file.\n");
	exit(1);
    }
    printf("  -------- files closed -------  \n");

    /* クローズした直後のバッファリストの状態を出力 */
    printBufferList();

    printf("---------- test2 end ----------\n\n");
}

/*
 * main -- バッファ管理モジュールのテスト
 */
int main(int argc, char **argv)
{
    File *file[2];
    char page1[PAGE_SIZE], page2[PAGE_SIZE];
    int i;

    /* 乱数用関数の初期化 */
    initializeRandomGenerator();

    /*
     * ファイルアクセスモジュールの初期化
     */
    if (initializeFileModule() != OK) {
	fprintf(stderr, "%s: initialization failed.\n", TEST_NAME);
    }

    /*
     * 前回このプログラムを実行したときのテストファイルが
     * 残っている可能性があるので、まず消去する
     */
    deleteFile(TEST_FILE1);
    deleteFile(TEST_FILE2);

    /* 空のファイルを2つ作り、オープンする */
    if (createFile(TEST_FILE1) != OK) {
	fprintf(stderr, "Cannot create file.\n");
	exit(1);
    }

    if (createFile(TEST_FILE2) != OK) {
	fprintf(stderr, "Cannot create file.\n");
	exit(1);
    }

    if ((file[0] = openFile(TEST_FILE1)) == NULL) {
	fprintf(stderr, "Cannot open file.\n");
	exit(1);
    }

    if ((file[1] = openFile(TEST_FILE2)) == NULL) {
	fprintf(stderr, "Cannot open file.\n");
	exit(1);
    }

    /*
     * ファイルの先頭から順に、(FILE_SIZE)ページ分のダミーデータを書く
     *
     * それぞれのページの先頭3バイトに、以下のように文字を書く
     * file1:
     *   0ページ目: 000
     *   1ページ目: 001
     *   2ページ目: 002
     *   (以下同様)
     * file2:
     *   0ページ目: 100
     *   1ページ目: 101
     *   2ページ目: 102
     *   (以下同様)
     */
    for (i = 0; i < FILE_SIZE; i++) {
	/* ページに書く内容を用意する */
	memset(page1, 0, PAGE_SIZE);
	memset(page2, 0, PAGE_SIZE);
	sprintf(page1, "00%c", '0' + i);
	sprintf(page2, "10%c", '0' + i);

	/* ファイルに書き込む */
	if (writePage(file[0], i, page1) != OK) {
	    fprintf(stderr, "Cannot write page.\n");
	    exit(1);
	}

	if (writePage(file[1], i, page2) != OK) {
	    fprintf(stderr, "Cannot write page.\n");
	    exit(1);
	}
    }

    /* いったんファイルをクローズする */
    closeFile(file[0]);
    closeFile(file[1]);

    /*
     * ここまでが下準備。ここからバッファリング機能をテストする
     */
    test1();
    test2();

    /*
     * ファイルアクセスモジュールの終了処理
     */
    if (finalizeFileModule() != OK) {
	fprintf(stderr, "%s: finalization failed.\n", TEST_NAME);
    }

    /* ファイルを削除する */
    deleteFile(TEST_FILE1);
    deleteFile(TEST_FILE2);
}
