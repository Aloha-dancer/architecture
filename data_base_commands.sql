USE serv_store;

CREATE DATABASE IF NOT EXISTS SERV_STORE;
create user myyuser@localhost identified by '123';
grant all privileges on *.* to myyuser@localhost IDENTIfied by '123';

CREATE TABLE IF NOT EXISTS SERV_STORE.USERS(
	ID BIGINT AUTO_INCREMENT,
	NAME VARCHAR(50),
	LAST_NAME VARCHAR(50),
	LOGIN VARCHAR(50) UNIQUE NOT NULL,
	EMAIL VARCHAR(100) UNIQUE NOT NULL,
	PWD VARCHAR(30) NOT NULL,
	CONSTRAINT PK_USERS PRIMARY KEY (ID)
)

CREATE TABLE IF NOT EXISTS SERV_STORE.S_SERVICE(
	ID BIGINT AUTO_INCREMENT,
	CAPTION VARCHAR(100) UNIQUE NOT NULL,
	CATEGORY VARCHAR(100) NOT NULL,
	DESCRIPTION TEXT NOT NULL,
	PRICE FLOAT NOT NULL,
	CONSTRAINT PK_SERVICE PRIMARY KEY (ID)
)

CREATE TABLE IF NOT EXISTS SERV_STORE.U_ORDER(
	ID BIGINT AUTO_INCREMENT,
	ID_USER BIGINT NOT NULL,
	ID_SERVICE BIGINT NOT NULL,
	ORDER_DATE DATE NOT NULL,
	STATUS VARCHAR(50) NOT NULL,
	CONSTRAINT PK_ORDER PRIMARY KEY (ID),
	CONSTRAINT FK1_ORDER_USER FOREIGN KEY (ID_USER) REFERENCES SERV_STORE.USERS(ID),
	CONSTRAINT FK2_ORDER_SERVICE FOREIGN KEY (ID_SERVICE) REFERENCES SERV_STORE.S_SERVICE(ID)
)
use serv_store;
alter table `user` modify id bigint(20) auto_increment;
alter table `order` drop foreign key `fk1_o_u`;
alter table `order` add constraint `fk1_o_u` foreign key (id_user) references `user` (id);
alter table `order` add constraint `fk1_o_s` foreign key (id_service) references `service` (id);


INSERT INTO `user`
VALUES(
0,'VALERY', 'POPOV', 'FLAMMENWERFER', 'LEAVEMEALONE@GMAIL.COM', 'SOMEBODYTOLDME'
),
(0,'TESTY', '', 'NOMERCY', 'ADORABLEGUY@YANDEX.RU', 'TST')
;

INSERT INTO service  
VALUES
(0, 'MONITOR REPAIR', 'MONITORS', 'REPAIRING MONITORS ACTIVITY', 4000),
(0, 'PK DIAGNOSTICS', 'GENERAL', 'GENERAL COMPUTER DIAGNOSTICS ACTIVITY', 1500);

INSERT INTO `order`
VALUES
(0, 1, 2, '2010-11-20', 'DONE');


ALTER TABLE SERV_STORE.USERS RENAME SERV_STORE.USER;
ALTER TABLE SERV_STORE.U_ORDER RENAME SERV_STORE.ORDER; 
ALTER TABLE `ORDER` ADD KEY `K1_DATE`(`ORDER_DATE`) ;
ALTER TABLE `USER` ADD KEY `K1_NAME` (`NAME`), ADD KEY `K2_LAST_NAME` (`LAST_NAME`), ADD KEY `K3_LOGIN` (`LOGIN`);
alter table `order` modify id bigint not null;
alter table `order` drop Primary key;
alter table `order` add primary key (id, id_user, id_service);


select * from `order` o ;
select * from `user` u ;
select * from `service` s ;
Select id, caption, category, description, price from service;
Select id, caption, category, description, price from service;
SELECT id, name, last_name, email, login, pwd FROM user where id=1;
select id, id_user, id_service, order_date, status  from `order`;

SELECT id, first_name, last_name, email, login, password FROM user;

select case when first_name is null then 1 else 0 end from user where id=3;
