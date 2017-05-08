#C-MicroDatabase

## What is this?
This is a DatabaseSystem which have few features ,So we call it  <em>Micro</em>Database.

##Usage

###build
	cd ./src
	make
	./main

###Create table
	create table TABLE_NAME (COLUMN TYPE , ... COLUMN TYPE)

###insert tuple
	insert into TABLE_NAME values(VALUE, … VALUE)

###select tuple

	select * from TABLE_NAME
	select * from TABLE_NAME where COLUMN (<,>,=,!=) VALUE

###delete tuple

	delete from TABLE_NAME where COLUMN (<,>,=,!=) VALUE

###drop table
	drop table TABLE_NAME
	
### Exit process
	exit
	

## For example
	
	create table students (id int , name string ,age int)
	insert into students values(1,"Tom",20)
	insert into students values(2,"May",30)
	select * from students where age > 10
	delete from students where id = 1
	drop table students
	exit
	
## TODO

* Optimization
* Cache data
* The program is allowed to type `select column1,column2,..., TABLE_NAME`.
* Add some value types.For example `text,char,bool,time...`
