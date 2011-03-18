/*select M.member_no, firstname, lastname, isbn, log_date from member as M
join reservation as R on M.member_no=R.member_no
where R.member_no=250 or R.member_no=1675 or R.member_no=342

select M.firstname, M.lastname, M.member_no, A.state, COUNT(J.member_no) from member as M
join adult as A On A.member_no=M.member_no
join juvenile as J On J.adult_member_no=A.member_no
where A.state='AZ'
group by M.firstname, M.lastname, M.member_no, A.state
having COUNT(J.member_no)>2*/


/*select ProductName from Products 
where (select COUNT(CustomerID) from [Order Details] as Ord
join Orders as O on O.OrderID=Ord.OrderID
where Products.ProductID=Ord.ProductID)>1*/
/*
select firstname, lastname, street, city, state,ISNULL((select COUNT(*) from loan where member.member_no=loan.member_no),0)as ilosc,'Dorosly' from member
join adult as A On A.member_no=member.member_no

union

select firstname, lastname, street, city, state, ISNULL((select COUNT(*) from loan where member.member_no=loan.member_no),0) as ilosc,'Dziecko' from member
join juvenile as J On J.member_no=member.member_no
join Adult as A On J.adult_member_no=A.member_no
order by ilosc*//*
select firstname, lastname, street, city, state,(select COUNT(*) from loan where member.member_no=loan.member_no AND YEAR(out_date)=2002 AND MONTH(out_date) between 10 and 12)as ilosc,'Dorosly' from member
join adult as A On A.member_no=member.member_no

union

select firstname, lastname, street, city, state, (select COUNT(*) from loan where member.member_no=loan.member_no AND YEAR(out_date)=2002 AND MONTH(out_date) between 10 and 12)as ilosc,'Dziecko' from member
join juvenile as J On J.member_no=member.member_no
join Adult as A On J.adult_member_no=A.member_no
order by ilosc
*/

/*select distinct firstname, lastname, A.street, A.city from member
join adult as A On A.member_no=member.member_no
join juvenile as J On J.adult_member_no=A.member_no
where member.member_no not in(select member_no from loan) AND J.birth_date<'1/1/96'*/

/*select distinct  CompanyName from Customers
left join Orders as O on Customers.CustomerID=O.CustomerID and YEAR(OrderDate)=1996
where OrderID is null

select CompanyName from Customers
where not exists(select Orderid from Orders where YEAR(OrderDate)=1996 AND Orders.CustomerID=Customers.CustomerID)*/

/*select FirstName, LastName, COUNT(O.EmployeeID), YEAR(OrderDate), MONTH(OrderDate), DatePart(Quarter, OrderDate) from Employees
join Orders as O on O.EmployeeID=Employees.EmployeeID
group by FirstName, LastName, YEAR(OrderDate), MONTH(OrderDate), DatePart(Quarter, OrderDate)
with rollup*/

/*select top 1 FirstName, LastName, SUM(UnitPrice*(1-Discount)*Quantity+Freight)as ilosc from Employees as E
join Orders as O on O.EmployeeID=E.EmployeeID
join [Order Details] as Ord on Ord.OrderID=O.OrderID
where YEAR(OrderDate)=1997
group by FirstName, LastName
order by ilosc desc*/
/*select companyname from Customers 
where CustomerID not in(select Cust.customerid from Customers as Cust
join orders on Orders.CustomerID=Cust.CustomerID
join [Order Details] as Ord on Ord.OrderID=Orders.OrderID
join Products as P on P.ProductID=Ord.ProductID
join Categories as C on C.CategoryID=P.CategoryID
where C.CategoryName like 'Confections' and YEAR(OrderDate)=1996)*/

