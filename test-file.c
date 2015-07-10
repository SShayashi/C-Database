/*
 * ファイルI/Oモジュールテストプログラム
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "microdb.h"

/*
 * テスト名
 */
#define TEST_NAME "test-file"

/*
 * テスト用ファイルのファイル名
 */
#define TEST_FILE1 "testfile1"
#define TEST_FILE2 "testfile2"

/*
 * ファイルサイズ(ファイルに書き込むページ数)
 */
#define FILE_SIZE 10

/*
 * ファイルに書くパターン
 */
char pagePattern[FILE_SIZE][PAGE_SIZE];

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
 * test1 -- ファイルの作成とデータの書き出し
 */
Result test1()
{
    int i;
    File *file1, *file2;

    /* 空のファイルを作る */
    if (createFile(TEST_FILE1) != OK) {
	fprintf(stderr, "Cannot create file.\n");
	return NG;
    }

    if (createFile(TEST_FILE2) != OK) {
	fprintf(stderr, "Cannot create file.\n");
	return NG;
    }

    /* ファイルをオープンする */
    if ((file1 = openFile(TEST_FILE1)) == NULL) {
	fprintf(stderr, "Cannot open file.\n");
	return NG;
    }

    if ((file2 = openFile(TEST_FILE2)) == NULL) {
	fprintf(stderr, "Cannot open file.\n");
	return NG;
    }

    /* ファイルの先頭からFILE_SIZE分のページを書き込む */
    for (i = 0; i < FILE_SIZE; i++) {
	/* テストパターンをファイルに書き込む */
	if (writePage(file1, i, pagePattern[i]) != OK) {
	    fprintf(stderr, "Cannot write page.\n");
	    return NG;
	}

	if (writePage(file2, i, pagePattern[i]) != OK) {
	    fprintf(stderr, "Cannot write page.\n");
	    return NG;
	}
    }

    /* ファイルをクローズする */
    if (closeFile(file1) == NG) {
	fprintf(stderr, "Cannot close file.\n");
	return NG;
    }

    if (closeFile(file2) == NG) {
	fprintf(stderr, "Cannot close file.\n");
	return NG;
    }

    return OK;
}

/*
 * test2 -- ファイルの大きさの確認
 */
Result test2()
{
    /* ファイルの正しいページ数が得られるかどうかをチェック */
    if (getNumPages(TEST_FILE1) != FILE_SIZE) {
	fprintf(stderr, "Number of pages is wrong.\n");
	return NG;
    }

    if (getNumPages(TEST_FILE2) != FILE_SIZE) {
	fprintf(stderr, "Number of pages is wrong.\n");
	return NG;
    }

    return OK;
}

/*
 * test3 -- ファイルからのデータの読み出し
 */
Result test3()
{
    int i;
    int fileNum, pageNum;
    File *file[2];
    char page[PAGE_SIZE];	/* ファイルからの読み出しに使う配列 */

    /* ファイルをオープンする */
    if ((file[0] = openFile(TEST_FILE1)) == NULL) {
	fprintf(stderr, "Cannot open file.\n");
	return NG;
    }

    if ((file[1] = openFile(TEST_FILE2)) == NULL) {
	fprintf(stderr, "Cannot open file.\n");
	return NG;
    }

    /* ファイルからランダムにページを読み出し、正しく読めるかチェックする */
    for (i = 0; i < (3 * FILE_SIZE); i++) {
	/* 0〜1の範囲の乱数を発生させる */
	fileNum = getRandomInteger(0, 1);

	/* 0〜FILE_SIZE-1の範囲の乱数を発生させる */
	pageNum = getRandomInteger(0, FILE_SIZE - 1);

	/* ファイルからページ番号pageNumのページを読み出す */
	if (readPage(file[fileNum], pageNum, page) != OK) {
	    fprintf(stderr, "Cannot read page.\n");
	    exit(1);
	}

	/* 書き込んだものと同じものが読み出せたかどうかチェックする */
	if (memcmp(pagePattern[pageNum], page, PAGE_SIZE) == 0) {
	    printf("  File %2d, Page %2d: OK\n", fileNum, pageNum);
	} else {
	    printf("  File %2d, Page %2d: NG\n", fileNum, pageNum);
	}
    }

    /* ファイルをクローズする */
    if (closeFile(file[0]) == NG) {
	fprintf(stderr, "Cannot close file.\n");
	return NG;
    }

    if (closeFile(file[1]) == NG) {
	fprintf(stderr, "Cannot close file.\n");
	return NG;
    }

    return OK;
}

/*
 * test4 -- ファイルの削除
 */
Result test4()
{
    if (deleteFile(TEST_FILE1) == NG) {
	fprintf(stderr, "Cannot delete file.\n");
	return NG;
    }

    if (deleteFile(TEST_FILE2) == NG) {
	fprintf(stderr, "Cannot delete file.\n");
	return NG;
    }

    return OK;
}

/*
 * main -- エントリポイント
 */
int main(int argc, char **argv)
{
    int i, j;

    /* 乱数の初期化 */
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

    /* FILE_SIZE分のページの内容を乱数で作成 */
    for (i = 0; i < FILE_SIZE; i++) {
	for (j = 0; j < PAGE_SIZE; j++) {
	    /* 0〜9の範囲の乱数を発生しパターンを作る */
	    int x;
	    x = getRandomInteger(0, 9);

	    /* ページに'0'〜'9'の文字を入れる */
	    pagePattern[i][j] = '0' + x;
	}
    }

    /* テストの実行 */
    fprintf(stderr, "%s: test 1: Start\n", TEST_NAME);
    if (test1() == OK) {
	fprintf(stderr, "%s: test 1: OK\n\n", TEST_NAME);
    } else {
	fprintf(stderr, "%s: test 1: NG\n\n", TEST_NAME);
    }

    fprintf(stderr, "%s: test 2: Start\n", TEST_NAME);
    if (test2() == OK) {
	fprintf(stderr, "%s: test 2: OK\n\n", TEST_NAME);
    } else {
	fprintf(stderr, "%s: test 2: NG\n\n", TEST_NAME);
    }

    fprintf(stderr, "%s: test 3: Start\n", TEST_NAME);
    if (test3() == OK) {
	fprintf(stderr, "%s: test 3: OK\n\n", TEST_NAME);
    } else {
	fprintf(stderr, "%s: test 3: NG\n\n", TEST_NAME);
    }

    fprintf(stderr, "%s: test 4: Start\n", TEST_NAME);
    if (test4() == OK) {
	fprintf(stderr, "%s: test 4: OK\n\n", TEST_NAME);
    } else {
	fprintf(stderr, "%s: test 4: NG\n\n", TEST_NAME);
    }

    /*
     * ファイルアクセスモジュールの終了処理
     */
    if (finalizeFileModule() != OK) {
	fprintf(stderr, "%s: finalization failed.\n", TEST_NAME);
    }

    exit(0);
}
