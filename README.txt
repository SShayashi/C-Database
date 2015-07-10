データベースの実行まで

1.ターミナルで「make 」を叩いてコンパイルしてください
2.実行ファイル「main」を実行してください


データベースの機能
1. テーブルの作成
文法： create table “テーブル名” ( “フィールド名”　“intもしくはstring” , …)
sql文の例： create table student (name string , age int)
※現在レコードのデータ型は文字列と数値型しか実装されていません

2.レコードの挿入

文法：insert into “テーブル名” values(“フィールド値”, … )
sql文の例：insert into student (tanaka  29 , yamada 10)

3.データベースの参照
文法：select * from “テーブル名”　(where “フィールド名”　“条件式”　“値”)
sql文の例：select * from student 
	 select * from student where age > 10

※条件式は　( > , < , = , != )の４つにしか対応していません


4. レコードの削除
文法：delete from “テーブル名” where “条件式”
sql文の例：delete from student where age > 10

5.テーブルの削除
文法；drop table “テーブル名”
sql文の例：drop table student
 
5.データベースの終了
quit, exit　で終了します。
