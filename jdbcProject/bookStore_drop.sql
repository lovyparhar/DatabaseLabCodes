alter table lend_info
drop FOREIGN KEY fk_lend_info_book;

alter table lend_info
drop FOREIGN KEY fk_lend_info_cust;


drop table customer;
drop table book;
drop table lend_info;

drop database bookstoredb;






