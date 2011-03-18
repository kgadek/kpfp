-- *****************************************************************************
-- Wybieranie danych z pojedynczej tabeli
-- *****************************************************************************
--1
--select CompanyName, Address from Customers

--2
--select LastName, HomePhone from Employees

--3
--select ProductName, UnitPrice from Products

--4
--select CategoryName, Description from Categories

--5
--select CompanyName, HomePage from Suppliers

--6
--select CompanyName, Address from Customers
--where City = 'London'

--7
--select CompanyName, Address from Customers
--where Country in ('France', 'Spain')

--8
--select ProductName, UnitPrice from Products
--where UnitPrice between 20 and 30

--9
--select ProductName, UnitPrice from Products
--join Categories on Products.CategoryID = Categories.CategoryID
--where CategoryName like '[mM]eat%'

--10
--select ProductName, UnitsInStock from Products
--join Suppliers on Suppliers.SupplierID = Products.SupplierID
--where Suppliers.CompanyName = 'Tokyo Traders'

--11/12
--select ProductName, UnitsInStock from Products
--where UnitsInStock = 0 and QuantityPerUnit like '%bottle%'

--13
--select TitleOfCourtesy + ' ' + FirstName + ' ' + LastName, Title from Employees
--where SUBSTRING(LastName,1,1) between 'B' and 'L'

--14
--select TitleOfCourtesy + ' ' + FirstName + ' ' + LastName, Title from Employees
--where SUBSTRING(LastName,1,1) in ('B','L')

--15
--select CategoryName from Categories
--where Description like '%,%'

--16
--select CompanyName from Customers
--where CompanyName like '%[Ss]tore%'

--17
--select ProductName from Products
--where UnitPrice < 10 or UnitPrice > 20

--18
--select OrderID, OrderDate, CustomerID from Orders
--where ShippedDate is null and ShipCountry = 'Argentina'

--19
--select CompanyName, Country from Customers
--order by Country, CompanyName asc

--20
--select CategoryName, ProductName, UnitPrice from Products
--join Categories on Products.CategoryID = Categories.CategoryID
--order by CategoryName, UnitPrice desc

--21
--select CompanyName, Country from Customers
--where Country in ('Japan','Italy')
--order by Country, CompanyName

-- *****************************************************************************
-- Podzapytania
-- *****************************************************************************

--1a
--select Customers.CompanyName, Customers.Phone from Customers
--join Orders on Orders.CustomerID = Customers.CustomerID
--join Shippers on Shippers.ShipperID = Orders.ShipVia
--where Shippers.CompanyName = 'United Package'

--1b
--select Customers.CompanyName, Customers.Phone from Customers
--join Orders on Orders.CustomerID = Customers.CustomerID
--where ShipVia in (select ShipperID from Shippers
--                  where CompanyName = 'United Package')

--2
--select CompanyName, Phone from Customers
--join Orders on Customers.CustomerID = Orders.CustomerID
--join [Order Details] on [Order Details].OrderID = Orders.OrderID
--join Products on Products.ProductID = [Order Details].ProductID
--where CategoryID in (select CategoryID from Categories
--                    where CategoryName = 'Confections')
--group by CompanyName, Phone

--3
--select CompanyName, Phone from Customers 
--where CustomerID not in
--    (select Customers.CustomerID from Customers
--    join Orders on Customers.CustomerID = Orders.CustomerID
--    join [Order Details] on [Order Details].OrderID = Orders.OrderID
--    join Products on Products.ProductID = [Order Details].ProductID
--    where CategoryID in (select CategoryID from Categories
--                        where CategoryName = 'Confections')
--    group by Customers.CustomerID)

--4
--select CompanyName, Phone from Customers
--join Orders on Customers.CustomerID = Orders.CustomerID
--join [Order Details] on [Order Details].OrderID = Orders.OrderID
--join Products on Products.ProductID = [Order Details].ProductID
--where CategoryID in (select CategoryID from Categories
--                    where CategoryName = 'Confections')
--group by CompanyName, Phone
--having COUNT(*) > 3

--5
--select Products.ProductID, MAX([Order Details].Quantity) from Products
--join [Order Details] on [Order Details].ProductID = Products.ProductID
--group by Products.ProductID
--order by Products.ProductID

--5b bardzo głupie ;<
--select dupa.ProductID, (select MAX(Quantity) from [Order Details] where [Order Details].ProductID = dupa.ProductID group by [Order Details].ProductID)
--from Products as dupa
--join [Order Details] on [Order Details].ProductID = dupa.ProductID
--group by dupa.ProductID

--6
--select ProductName from Products
--where UnitPrice > (select AVG(UnitPrice) from Products)

--7
--select ProductName from Products as dupa
--where UnitPrice > (select AVG(UnitPrice) from Products where CategoryID = dupa.CategoryID group by CategoryID)

--8
--select ProductName, UnitPrice, (select AVG(UnitPrice) from Products), UnitPrice - (select AVG(UnitPrice) from Products) from Products

--9
--select dupa.ProductName, dupa.UnitPrice,
--      (select AVG(UnitPrice) from Products where dupa.CategoryID = Products.CategoryID group by Products.CategoryID)
--from Products as dupa

--10/11
--select OrderID, round(SUM((UnitPrice * Quantity) *
--                          (1 - Discount))
--                    + (select Freight from Orders where dupa.OrderID = Orders.OrderID), 2)
--from [Order Details] as dupa
--group by OrderID

--12
--select dupa.CustomerID, dupa.CompanyName from Customers as dupa
--where dupa.CustomerID not in (
--            select Customers.CustomerID from Customers
--            join Orders on Customers.CustomerID = Orders.CustomerID
--            where YEAR(OrderDate) = 1997)

--13 złe!!
--select Products.ProductID, Products.ProductName, COUNT(*) from Products
--join [Order Details] on [Order Details].ProductID = Products.ProductID
--join Orders on [Order Details].OrderID = Orders.OrderID
--group by Products.ProductID, Products.ProductName
--having COUNT(*) > 1


--select Products.ProductName from Products
--join [Order Details] on [Order Details].ProductID = Products.ProductID
--join Orders on [Order Details].OrderID = Orders.OrderID
--group by Products.ProductName, Orders.CustomerID
--order by Products.ProductName, Orders.CustomerID