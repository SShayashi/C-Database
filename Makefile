CC = cc
CFLAGS = -g

# すべてのプログラムを作るルール
#all: microdb all-test

# すべてのテストプログラムを作るルール
#all-test: test-file

# すべてのテストプログラムを実行するルール
#do-test: test-file
#	./test-file

# 「microdb」を作成するためのルールは、今後追加される予定
# とりあえず、今のところは「何もしない」という設定にしておく。
main: main.o datadef.o file.o datamanip.o
	$(CC) -o main $(CFLAGS) -c main.o file.o datadef.o datamanip.o -lreadline -lcurses

datadef.o:datadef.c microdb.h
	$(CC) -o datadef.o $(CFLAGS) -c datadef.c

datamanip.o:datamanip.c microdb.h
	$(CC) -o datamanip.o $(CFLAGS) -c datamanip.c 

file.o: file.c microdb.h
	$(CC) -o file.o $(CFLAGS) -c file.c

main.o:main.c microdb.h
	$(CC) -o main.o $(CFLAGS) -c main.c

clean:
	rm -f *.o
