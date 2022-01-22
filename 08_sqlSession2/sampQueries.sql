select fname as 'First Name', lname as 'Last Name' from employee;

select fname as 'First Name', lname as 'Last Name', 12*salary as 'Annual Salary' from employee;

select fname as 'First Name', lname as 'Last Name', 12*salary as 'Annual Salary', dno as 'Department Number' from employee where dno=1;

select fname as 'First Name', lname as 'Last Name', 12*salary as 'Annual Salary', dno as 'Department Number' from employee where dno=4 or salary > 35000;

select fname, dno, salary from employee where dno = (select dnumber from department where dname = 'Administration');

select * from employee where salary > (select salary from employee where fname='Franklin');

select * from employee where dno in (select dnumber from dept_locations where dlocation = 'Houston');

select * from employee where super_ssn is NULL;

select * from employee where sex = 'F' and ssn in (select essn from works_on where pno = 1);

select * from employee where sex = 'M' and ssn in (select essn from works_on where pno = 2) and salary > 20000;

select * from employee where ssn not in (select essn from works_on);

select count(*) from employee;

select count(super_ssn) from employee;

select distinct essn from works_on;

select distinct essn, pno from works_on;

select avg(salary), min(salary), max(salary), sum(salary) from employee;

select * from employee where salary > (select avg(salary) from employee);

select * from employee where salary between 25000 and 38000;

select * from employee where fname like 'J%';

select * from employee where year(bdate) > 1965;

select avg(salary), dno from employee group by dno;

select avg(salary), dno, sex from employee group by dno, sex;

select avg(salary), dno, sex from employee group by dno, sex having avg(salary) > 30000;

select avg(salary), dno from employee where year(bdate) > 1965 group by dno;

select * from employee order by salary;

select * from employee order by salary desc;





Quiz questions

1.
select sum(hours), pno 
from works_on 
where pno in (select pnumber from project where dnum = (select dnumber from department where dname='Research')) group by pno order by pno asc;

2.
select date_format( from_days( datediff(now(),bdate) ), '%Y')+0 as Age from employee order by Age;

3.
select essn from works_on group by essn having avg(hours) >= 10 order by essn;

4.
select fname 
from employee as e 
where salary > (select avg(salary) from employee group by dno having dno = e.dno) order by fname;

5.
select * from employee where fname like '%s%' order by fname;

10. 
select * from employee where sex = 'F' and salary between 30000 and 50000 order by dno;

11.
select * from employee where ssn not in (select essn from dependent) order by fname;

12.
select min(salary) from employee where dno = (select dnumber from department where dname = 'Research');

13.
select fname as First from employee order  by fname;

14.
select sex, sum(salary) from employee group by sex order by sex;

15.
select count(*) from employee where dno=5;

16. 
select essn, max(hours) from works_on group by essn order by essn;

17.
select min(salary) from employee where dno = (select dnumber from department where dname = 'Administration');

18. select avg(hours) from works_on where pno = (select pnumber from project where pname='Computerization');


19. select dno, count(ssn) from employee where salary > 30000 group by dno order by dno;

20. select essn from dependent group by essn having count(*) > 1 order by essn;

21. 














1. select lname from employee where ssn in (select essn from dependent where sex = 'F') order by lname;

2. select sum(hours), pno from works_on where pno in (select pnumber from project where dnum = (select dnumber from department where dname = 'Research')) group by pno order by pno;

3. select (year(current_date()) - year(bdate)) as age from employee order by age;

4. select essn, sum(hours) from works_on group by essn having avg(hours) >= 10 order by essn;

5. select fname from employee as eo where eo.salary > (select avg(salary) from employee where dno = eo.dno) order by fname;

6. select fname from employee where (fname like '%s%') or (fname like '%S%') order by fname;

7. select fname, lname, salary from employee where salary >= 25000 order by lname;

8. select distinct plocation from project order by plocation;

9. select * from employee where year(bdate) > 1950 and sex = 'M' order by salary;

10. select fname, lname, bdate, salary from employee where salary < 30000 or super_ssn is NULL order by bdate desc;

11. 




