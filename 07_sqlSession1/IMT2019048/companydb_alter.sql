alter table employee
add constraint fk_employee1
FOREIGN KEY (super_ssn)
REFERENCES employee(ssn);

alter table employee
add constraint fk_employee2
FOREIGN KEY (dno)
REFERENCES department(dnumber);



alter table department
add constraint fk_department
FOREIGN KEY (mgr_ssn)
REFERENCES employee(ssn);



alter table dept_locations
add constraint fk_dept_locations
FOREIGN KEY (dnumber)
REFERENCES department(dnumber);



alter table works_on
add constraint fk_works_on1
FOREIGN KEY (essn)
REFERENCES employee(ssn);

alter table works_on
add constraint fk_works_on2
FOREIGN KEY (pno)
REFERENCES project(pnumber);




alter table project
add constraint fk_project
FOREIGN KEY (dnum)
REFERENCES department(dnumber);




alter table dependent
add constraint fk_dependent
FOREIGN KEY (essn)
REFERENCES employee(ssn);