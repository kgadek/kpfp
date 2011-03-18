drop table Kraje;
drop table Klienci;
create table Kraje (
  Nazwa_kraju text not null,
  ID_kraju int identity(1,1) primary key clustered
);
create table Wycieczki (
  [ID] int identity(1,1) primary key clustered,
  ID_kraju int not null,
  Nazwa_pensjonatu text null,
  Adres_pensjonatu text null,
  Cena money not null,
  Liczba_miejsc int not null,
  Data_wyjazdu date not null,
  Data_przyjazdu date not null,
  foreign key (ID_kraju) references Kraje(ID_kraju),
  constraint Data_przyjazdu check(Data_wyjazdu<Data_przyjazdu)
);
create table Klienci (
  ID_klienta int identity(1,1) primary key clustered,
  Nazwa text not null,
  Rodzaj_klienta char not null,
  Adres text null,
  Miasto text null,
  Kod_pocztowy text null,
  Telefon_kontaktowy text null,
  NIP text null
);

INSERT INTO Klienci (Nazwa, Rodzaj_klienta, Adres, Miasto, Kod_pocztowy, Telefon_kontaktowy, NIP)
  select 'Malwina Adamus', 'O', 'al. Pokoju 94', 'Gniezno', '42-714', '+48038082033', NULL
UNION ALL
  select 'Adela Jurkowski', 'O', 'ul. Marii Konopnickiej 23', 'Dąbrowa Górnicza', '36-873', '+48095222121', NULL
UNION ALL
  select 'Maciej Fuk', 'O', 'ul. Pawia 92B', 'Włocławek', '14-990', '454775008', NULL
UNION ALL
  select 'Monika Cetnarowicz', 'O', 'ul. Pawia 18', 'Legnica', '22-548', '520053192', '2091139749'
UNION ALL
  select 'Alicja Lewandowski', 'O', 'ul. Sobieskiego 71', 'Gdynia', '19-095', '+48-149-470-855', NULL
UNION ALL
  select 'Mateusz Madziar', 'O', 'ul. Pawia 77', 'Wrocław', '50-729', '+48-534-458-151', NULL
UNION ALL
  select 'Mirosław Valenta', 'O', 'ul. Toruńska 39B', 'Szczecin', '83-752', '844-548-920', NULL
UNION ALL
  select 'Maksymilian Drabek', 'F', 'al. Marszałka Ferdinanda Focha 60', 'Szczecin', '35-315', '382428912', '9529168752'
UNION ALL
  select 'Magda Strojniak', 'O', 'ul. Monte Cassino 84', 'Rzeszów', '44-178', '180-860-786', NULL
UNION ALL
  select 'Jakub Anielski', 'O', 'ul. Opolska 55', 'Gniezno', '60-011', '211485032', NULL
select * from Klienci