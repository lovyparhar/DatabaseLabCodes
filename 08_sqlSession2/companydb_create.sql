create database companydb;
use companydb;

create table employee(
	fname varchar(30) NOT NULL,
	minit char(1),
	lname varchar(30),
	ssn char(9),
	bdate date,
	address varchar(50),
	sex char(1),
	salary int,
	super_ssn char(9),
	dno int,
	constraint pk_employee PRIMARY KEY (ssn)
);


create table department(
	dname varchar(30) NOT NULL,
	dnumber int,
	mgr_ssn char(9),
	mgr_start_date date,
	constraint pk_department PRIMARY KEY (dnumber)
);


create table dept_locations(
	dnumber int,
	dlocation varchar(30),
	constraint pk_dept_locations PRIMARY KEY (dnumber, dlocation)
);


create table works_on(
	essn char(9),
	pno int,
	hours decimal(5, 2), 
	constraint pk_works_on PRIMARY KEY (essn, pno)
);

create table project(
	pname varchar(20),
	pnumber int,
	plocation varchar(20),
	dnum int,
	constraint pk_project PRIMARY KEY (pnumber)
); 

create table dependent(
	essn char(9),
	dependent_name varchar(20),
	sex char(1),
	bdate date,
	relationship varchar(10),
	constraint pk_dependent PRIMARY KEY (essn, dependent_name)
);









