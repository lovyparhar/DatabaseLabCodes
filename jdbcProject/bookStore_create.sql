create database bookstoredb;
use bookstoredb;

create table customer(
	cname varchar(60) NOT NULL,
	aadhar_num char(12),
	address varchar(50),
	sex char(1),
	constraint pk_customer PRIMARY KEY (aadhar_num)
);


create table book(
	book_id int AUTO_INCREMENT,
	bname varchar(50) NOT NULL,
	author varchar(50),
	genre varchar(50),
	constraint pk_book PRIMARY KEY (book_id)
);


create table lend_info(
	book_id int,
	customer_id char(12) NOT NULL,
	constraint pk_lend_info PRIMARY KEY (book_id)
);

alter table lend_info
add constraint fk_lend_info_book
FOREIGN KEY (book_id)
REFERENCES book(book_id)
ON DELETE CASCADE
ON UPDATE CASCADE;

alter table lend_info
add constraint fk_lend_info_cust
FOREIGN KEY (customer_id)
REFERENCES customer(aadhar_num)
ON DELETE CASCADE
ON UPDATE CASCADE;

