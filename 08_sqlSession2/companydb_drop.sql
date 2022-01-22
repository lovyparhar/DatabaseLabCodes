alter table employee
drop FOREIGN KEY fk_employee1;

alter table employee
drop FOREIGN KEY fk_employee2;



alter table department
drop FOREIGN KEY fk_department;



alter table dept_locations
drop FOREIGN KEY fk_dept_locations;




alter table works_on
drop FOREIGN KEY fk_works_on1;

alter table works_on
drop FOREIGN KEY fk_works_on2;




alter table project
drop FOREIGN KEY fk_project;




alter table dependent
drop FOREIGN KEY fk_dependent;





drop table employee;
drop table department;
drop table dept_locations;
drop table works_on;
drop table project;
drop table dependent;


drop database companydb;






