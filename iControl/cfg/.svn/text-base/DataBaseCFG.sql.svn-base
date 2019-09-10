PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE Login(id integer primary key, name text, pwd text, pwr integer, phone text, mail text);
INSERT INTO "Login" VALUES(1,'admin','spunky',0,'13983089826','imf_no.1@hotmail.com');
CREATE TABLE Config(roast_num integer, bake_num integer, factory_name text, factory_phone text);
INSERT INTO "Config" VALUES(1,0,'','');
CREATE TABLE ParaCfg(port_num integer, baudrate integer, parity text, data_bits integer, stop_bits integer, timeout integer, scan_time integer, m_serve_ip integer, m_serve_port integer, s_serve_ip integer, s_serve_port integer);
INSERT INTO "ParaCfg" VALUES(1, 19200, 'N', 8, 1, 1000, 3000, 3232235521, 65003, 3232235522, 65004);CREATE TABLE TerminalCfg(id integer primary key, cmd_code integer, terminal_id integer, channels integer, reg_addr integer);
INSERT INTO "TerminalCfg" VALUES(1,4,1,10,30001);
CREATE TABLE ResCfg(temp_num integer, press_num integer, humidity_num integer, motor_num integer, total_cars integer, standard_index integer);
INSERT INTO "ResCfg" VALUES(16,0,0,0,20,0);
CREATE TABLE CarCfg_1(car_1 integer,car_2 integer,car_3 integer,car_4 integer,car_5 integer,car_6 integer,car_7 integer,car_8 integer,car_9 integer,car_10 integer,car_11 integer,car_12 integer,car_13 integer,car_14 integer,car_15 integer,car_16 integer);
INSERT INTO "CarCfg_1" VALUES(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
CREATE TABLE Standard_1(id integer,set_time text,temperature_1 integer,temperature_2 integer,temperature_3 integer,temperature_4 integer,temperature_5 integer,temperature_6 integer,temperature_7 integer,temperature_8 integer,temperature_9 integer,temperature_10 integer,temperature_11 integer,temperature_12 integer,temperature_13 integer,temperature_14 integer,temperature_15 integer,temperature_16 integer);
INSERT INTO "Standard_1" VALUES(1,'2012-9-9 00:00:00',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
CREATE TABLE History_1(id integer primary key,save_time text,temperature_1 integer,temperature_2 integer,temperature_3 integer,temperature_4 integer,temperature_5 integer,temperature_6 integer,temperature_7 integer,temperature_8 integer,temperature_9 integer,temperature_10 integer,temperature_11 integer,temperature_12 integer,temperature_13 integer,temperature_14 integer,temperature_15 integer,temperature_16 integer);
CREATE TABLE CarHeat_1(car_id integer, heat double)
INSERT INTO "CarHeat_1" VALUES(1,0.0);
INSERT INTO "CarHeat_1" VALUES(2,0.0);
INSERT INTO "CarHeat_1" VALUES(3,0.0);
INSERT INTO "CarHeat_1" VALUES(4,0.0);
INSERT INTO "CarHeat_1" VALUES(5,0.0);
INSERT INTO "CarHeat_1" VALUES(6,0.0);
INSERT INTO "CarHeat_1" VALUES(7,0.0);
INSERT INTO "CarHeat_1" VALUES(8,0.0);
INSERT INTO "CarHeat_1" VALUES(9,0.0);
INSERT INTO "CarHeat_1" VALUES(10,0.0);
INSERT INTO "CarHeat_1" VALUES(11,0.0);
INSERT INTO "CarHeat_1" VALUES(12,0.0);
INSERT INTO "CarHeat_1" VALUES(13,0.0);
INSERT INTO "CarHeat_1" VALUES(14,0.0);
INSERT INTO "CarHeat_1" VALUES(15,0.0);
INSERT INTO "CarHeat_1" VALUES(16,0.0);
CREATE TABLE Outputs(roast_index integer, out_time text, car_id integer, heat double, condition integer);COMMIT;
