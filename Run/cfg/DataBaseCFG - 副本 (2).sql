PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE LoginInfo(id integer primary key, name text, pwd text, pwr integer, phone text, mail text);
INSERT INTO "LoginInfo" VALUES(1,'admin','spunky',1,'13983089826','imf_no.1@hotmail.com');
CREATE TABLE ComCfgInfo(port_num integer, baudrate integer, parity text, data_bits integer, stop_bits integer, timeout integer, scan_time integer, m_serve_ip integer, m_serve_port integer, s_serve_ip integer, s_serve_port integer, gross_time integer, bell_time integer);
INSERT INTO "ComCfgInfo" VALUES(2,19200,'N',8,1,100,100,-1062731775,65003,-1062731774,65004, 15, 15000);
CREATE TABLE TerminalCfgInfo(id integer primary key, dev_id integer, dev_cmd integer, dev_chn integer, dev_addr integer, dev_type integer);
INSERT INTO "TerminalCfgInfo" VALUES(1,15,4,10,30001,0);
INSERT INTO "TerminalCfgInfo" VALUES(2,16,4,10,30001,0);
INSERT INTO "TerminalCfgInfo" VALUES(3,17,4,10,30001,0);
INSERT INTO "TerminalCfgInfo" VALUES(4,18,4,10,30001,0);
INSERT INTO "TerminalCfgInfo" VALUES(5,2,3,2,40212,0);
INSERT INTO "TerminalCfgInfo" VALUES(6,2,6,2,40312,1);
INSERT INTO "TerminalCfgInfo" VALUES(7,2,3,2,40001,0);
INSERT INTO "TerminalCfgInfo" VALUES(8,2,6,2,40001,1);
CREATE TABLE KilnInfo(kiln_id integer, kiln_type integer, kiln_total integer, kiln_temps integer, kiln_tempas integer, kiln_press integer, kiln_humis integer, kiln_motor integer, kiln_gross integer, kiln_ctrl integer, kiln_std integer);
INSERT INTO "KilnInfo" VALUES(1,3,150,21,10,0,0,2,1,1,1);
CREATE TABLE AutoCtrlInfo(id integer primary key, out_up double, out_dn double, front_pts integer, rear_pts integer, ctrl_time integer, inc_coeff double, gross_waittime integer, gross_proctime integer);
INSERT INTO "AutoCtrlInfo" VALUES(1,50.0,30.0,3,2,5,0.1,10,30);
CREATE TABLE StandardCfgInfo_1(id integer,set_time text,temperature_1 integer,temperature_2 integer,temperature_3 integer,temperature_4 integer,temperature_5 integer,temperature_6 integer,temperature_7 integer,temperature_8 integer,temperature_9 integer,temperature_10 integer,temperature_11 integer,temperature_12 integer,temperature_13 integer,temperature_14 integer,temperature_15 integer,temperature_16 integer,temperature_17 integer,temperature_18 integer,temperature_19 integer,temperature_20 integer,temperature_21 integer);
INSERT INTO "StandardCfgInfo_1" VALUES(1,'2013-01-04 20:19:00',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
CREATE TABLE ChannelCfgInfo(id integer primary key,tem_idx integer,phy_idx integer,kiln_idx integer,chn_type integer,car_loc integer,chn_name text,adj_gain double,adj_zero double);
INSERT INTO "ChannelCfgInfo" VALUES(1,0,0,1,0,5,'�Զ��ź�',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(2,0,1,1,0,10,'�Զ��ź�',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(3,0,2,1,0,15,'�Զ��ź�',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(4,0,3,1,0,35,'�Զ��ź�',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(5,0,4,1,0,50,'�Զ��ź�',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(6,0,5,1,0,65,'�Զ��ź�',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(7,0,6,1,0,78,'�Զ��ź�',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(8,0,7,1,0,84,'�Զ��ź�',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(9,0,8,1,0,90,'�Զ��ź�',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(10,0,9,1,0,96,'96���ڶ�',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(11,1,0,1,0,96,'96���ⶥ',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(12,1,1,1,0,102,'102���ڶ�',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(13,1,2,1,0,102,'102���ⶥ',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(14,1,3,1,0,108,'108���ڶ�',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(15,1,4,1,0,108,'108���ⶥ',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(16,1,5,1,0,114,'114���ڶ�',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(17,1,6,1,0,114,'114���ⶥ',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(18,1,7,1,0,120,'�ų�3',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(19,1,8,1,0,120,'����',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(20,1,9,1,0,126,'�Զ��ź�',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(21,2,0,1,0,132,'�Զ��ź�',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(22,2,1,1,1,0,'102���ڱ�',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(23,2,2,1,1,1,'102�����',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(24,2,3,1,1,2,'108���ڱ�',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(25,2,4,1,1,3,'108�����',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(26,2,5,1,1,4,'114���ڱ�',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(27,2,6,1,1,5,'114���ڱ�',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(28,2,7,1,1,6,'�ų�1',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(29,2,8,1,1,7,'�ų�2',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(30,2,9,1,1,8,'����1',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(31,3,0,1,1,9,'����2',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(32,6,0,1,4,0,'1�ŵ��',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(33,6,1,1,4,1,'2�ŵ��',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(34,4,0,1,5,0,'�������',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(35,5,0,1,6,0,'�������',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(36,7,0,1,7,0,'�������',1.00,0.00);
INSERT INTO "ChannelCfgInfo" VALUES(37,7,1,1,7,1,'�������',1.00,0.00);
CREATE TABLE OutputsInfo(kiln_idx integer, out_time text, condition integer, up_serve integer);
CREATE TABLE History_1(id integer primary key,save_time text,temperature_1 integer,temperature_2 integer,temperature_3 integer,temperature_4 integer,temperature_5 integer,temperature_6 integer,temperature_7 integer,temperature_8 integer,temperature_9 integer,temperature_10 integer,temperature_11 integer,temperature_12 integer,temperature_13 integer,temperature_14 integer,temperature_15 integer,temperature_16 integer,temperature_17 integer,temperature_18 integer,temperature_19 integer,temperature_20 integer,temperature_21 integer,temperaturea_1 integer,temperaturea_2 integer,temperaturea_3 integer,temperaturea_4 integer,temperaturea_5 integer,temperaturea_6 integer,temperaturea_7 integer,temperaturea_8 integer,temperaturea_9 integer,temperaturea_10 integer,motor_1 double,motor_2 double);
COMMIT;
