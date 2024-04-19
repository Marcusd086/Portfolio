/* 
   Marcus DeKnatel
   10/22/20
   SQL Homework #4
   Pages 340-358:
   57, 60, 64, 69, 70, 71, 73, 75, 80, 85, 87, 
   89, 98, 99, 100, 101, 104, 106, 107, 109
*/

/* Problem 57 */
select PAT_FNAME, PAT_LNAME
from PATRON
order by UPPER(PAT_LNAME), UPPER(PAT_FNAME);

/* Problem 60 */
select DISTINCT(BOOK_YEAR)
from BOOK
order by BOOK_YEAR;

/* Problem 64 */
select BOOK_TITLE, BOOK_YEAR, BOOK_SUBJECT
from BOOK
order by BOOK_SUBJECT asc, BOOK_YEAR desc, BOOK_TITLE asc;

/* Problem 69 */
select BOOK_NUM, BOOK_TITLE, BOOK_SUBJECT, BOOK_COST
from BOOK 
where BOOK_SUBJECT like '%Cloud%' and BOOK_COST > 70 or BOOK_SUBJECT like '%Middleware%' and BOOK_COST > 70
order by BOOK_NUM;

/* Problem 70 */
select AU_ID, AU_FNAME, AU_LNAME, AU_BIRTHYEAR
from AUTHOR
where AU_BIRTHYEAR >= 1980 AND AU_BIRTHYEAR < 1990
order by AU_ID;

/* Problem 71 */
select BOOK_NUM, BOOK_TITLE, BOOK_SUBJECT
from BOOK
where UPPER(BOOK_TITLE) like "%DATABASE%" 
order by BOOK_NUM;

/* Problem 73 */
select PAT_ID, PAT_FNAME, PAT_LNAME, PAT_TYPE
from PATRON 
where UPPER(PAT_LNAME) like "C%"
order by PAT_ID;

/* Problem 75 */
select AU_ID, AU_FNAME, AU_LNAME
from AUTHOR
where AU_BIRTHYEAR is not null;    

/* Problem 80 */
select count(BOOK_NUM) as 'Available Books'
from BOOK 
where PAT_ID is null;

/* Problem 85 */
select AU_ID, count(BOOK_NUM) as 'Books Written'
from WRITES
group by AU_ID
order by count(BOOK_NUM) desc, AU_ID asc;

/* Problem 87 */
select PAT_ID as PATRON, BOOK_NUM as BOOK, datediff(CHECK_IN_DATE, CHECK_OUT_DATE) as DAYSKEPT
from CHECKOUT 
order by DAYSKEPT desc, PAT_ID, BOOK_NUM;

/* Problem 89 */
select BOOK_NUM, concat(BOOK_TITLE, ' (', BOOK_YEAR, ')') as BOOK, BOOK_SUBJECT
from BOOK
order by BOOK_NUM;

/* Problem 98 */
select BOOK.BOOK_NUM, BOOK_TITLE, count(CHECK_NUM) as 'Times Checked Out'
from BOOK left join CHECKOUT on CHECKOUT.BOOK_NUM = BOOK.BOOK_NUM
group by BOOK_NUM
order by count(CHECK_NUM) desc, BOOK_TITLE;

/* Problem 99 */
select BOOK.BOOK_NUM, BOOK_TITLE, count(CHECK_NUM) as 'Times Checked Out'
from BOOK join CHECKOUT on CHECKOUT.BOOK_NUM = BOOK.BOOK_NUM
group by BOOK_NUM
having count(CHECK_NUM) > 5
order by count(CHECK_NUM) desc, BOOK_TITLE;

/* Problem 100 */
select AUTHOR.AU_ID, AU_LNAME, BOOK_TITLE, CHECK_OUT_DATE, PAT_LNAME
from AUTHOR join WRITES on AUTHOR.AU_ID = WRITES.AU_ID join CHECKOUT on WRITES.BOOK_NUM = CHECKOUT.BOOK_NUM 
   join BOOK on WRITES.BOOK_NUM = BOOK.BOOK_NUM join PATRON on PATRON.PAT_ID = CHECKOUT.PAT_ID
where UPPER(AU_LNAME) like "BRUER" AND UPPER(PAT_LNAME) like "MILES"
order by CHECK_OUT_DATE;

/* Problem 101 */
select PAT_ID, PAT_FNAME, PAT_LNAME
from PATRON 
where PAT_ID not in (select CHECKOUT.PAT_ID from CHECKOUT)
order by PAT_LNAME, PAT_FNAME;

/* Problem 104 */
select PAT_ID, avg(datediff(CHECK_IN_DATE,CHECK_OUT_DATE)) as DAYSKEPT
from CHECKOUT
group by PAT_ID
having count(CHECK_NUM) > 2
order by DAYSKEPT desc;

/* Problem 106 */
select AU_ID, AU_FNAME, AU_LNAME
from AUTHOR
where AU_ID not in (
   select AU_ID from WRITES inner join BOOK on WRITES.BOOK_NUM = BOOK.BOOK_NUM where BOOK_SUBJECT like "%Programming%"
   )
order by AU_LNAME;

/* Problem 107 */
select BOOK_NUM, BOOK_TITLE, BOOK.BOOK_SUBJECT, AVGCOST, (BOOK_COST-AVGCOST) as DIFFERENCE
from BOOK inner join (select AVG(BOOK_COST) as AVGCOST, BOOK_SUBJECT from BOOK group by BOOK_SUBJECT) as AVGTABLE on 
   AVGTABLE.BOOK_SUBJECT = BOOK.BOOK_SUBJECT
order by BOOK_NUM, BOOK_TITLE;

/* Problem 109 */
select min(AVGCOST) as 'Lowest Average Cost', max(AVGCOST) as 'Highest Average Cost'
from (select BOOK_SUBJECT, avg(BOOK_COST) as AVGCOST from BOOK group by BOOK_SUBJECT) as AVGTABLE;
