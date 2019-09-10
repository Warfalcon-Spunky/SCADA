PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE Login(id integer primary key, name text, pwd text, pwr integer, phone text, mail text);
INSERT INTO "Login" VALUES(1, 'admin', '123456', 0, '13983089826', 'imf_no.1@hotmail.com');
INSERT INTO "Login" VALUES(2, 'guest', '789012', 1, '13982201898', 'imf_no.1@hotmail.com');

CREATE TABLE Config(roast_num integer, bake_num integer, factory_name text, factory_phone text);
INSERT INTO "Config" VALUES(2, 2, '重庆华岩页岩机砖厂', '13982201898');

CREATE TABLE ParaCfg(port_num integer, baudrate integer, parity text, data_bits integer, stop_bits integer, timeout integer, scan_time integer, m_serve_ip integer, m_serve_port integer, s_serve_ip integer, s_serve_port integer);
INSERT INTO "ParaCfg" VALUES(1, 19200, 'N', 8, 1, 1000, 3000, 3232235521, 65003, 3232235522, 65004);

CREATE TABLE TerminalCfg(id integer primary key, cmd_code integer, terminal_id integer, channels integer, reg_addr integer);
INSERT INTO "TerminalCfg" VALUES(1, 4, 15, 10, 30001);
INSERT INTO "TerminalCfg" VALUES(2, 4, 16, 6, 30001);
INSERT INTO "TerminalCfg" VALUES(3, 4, 17, 10, 30001);
INSERT INTO "TerminalCfg" VALUES(4, 4, 18, 10, 30001);
INSERT INTO "TerminalCfg" VALUES(5, 4, 19, 10, 30001);
INSERT INTO "TerminalCfg" VALUES(6, 4, 20, 3, 30001);
INSERT INTO "TerminalCfg" VALUES(7, 4, 21, 10, 30001);
INSERT INTO "TerminalCfg" VALUES(8, 4, 21, 7, 30001);

CREATE TABLE ResCfg(temp_num integer, press_num integer, humidity_num integer, motor_num integer, total_cars integer, standard_index integer);
INSERT INTO "ResCfg" VALUES(16, 4, 0, 2, 40, 0);
INSERT INTO "ResCfg" VALUES(18, 2, 0, 1 ,36, 1);
INSERT INTO "ResCfg" VALUES(8, 4, 2, 0, 24, 2);
INSERT INTO "ResCfg" VALUES(7, 0, 2, 0, 22, 3);

CREATE TABLE CarCfg_1(car_1 integer, car_2 integer, car_3 integer, car_4 integer, car_5 integer, car_6 integer, car_7 integer, car_8 integer, car_9 integer, car_10 integer, car_11 integer, car_12 integer, car_13 integer, car_14 integer, car_15 integer, car_16 integer);
INSERT INTO "CarCfg_1"VALUES(2, 4, 6, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 26);

CREATE TABLE CarCfg_2(car_1 integer, car_2 integer, car_3 integer, car_4 integer, car_5 integer, car_6 integer, car_7 integer, car_8 integer, car_9 integer, car_10 integer, car_11 integer, car_12 integer, car_13 integer, car_14 integer, car_15 integer, car_16 integer, car_17 integer, car_18 integer);
INSERT INTO "CarCfg_2"VALUES(2, 4, 6, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 26, 27, 28);

CREATE TABLE CarCfg_3(car_1 integer, car_2 integer, car_3 integer, car_4 integer, car_5 integer, car_6 integer, car_7 integer, car_8 integer);
INSERT INTO "CarCfg_3"VALUES(5, 6, 7, 8, 9, 10, 11, 12);

CREATE TABLE CarCfg_4(car_1 integer, car_2 integer, car_3 integer, car_4 integer, car_5 integer, car_6 integer, car_7 integer);
INSERT INTO "CarCfg_4"VALUES(2, 4, 6, 12, 13, 14, 15);

CREATE TABLE Standard_1(id integer, set_time text, temperature_1 integer, temperature_2 integer, temperature_3 integer, temperature_4 integer, temperature_5 integer, temperature_6 integer, temperature_7 integer, temperature_8 integer, temperature_9 integer, temperature_10 integer, temperature_11 integer, temperature_12 integer, temperature_13 integer, temperature_14 integer, temperature_15 integer, temperature_16 integer);
INSERT INTO "Standard_1" VALUES(1, '2012-7-7 13:00:03', 20, 40, 60, 300, 500, 650, 800, 980, 1020, 1000, 950, 700, 600, 400, 300, 200);

CREATE TABLE Standard_2(id integer, set_time text, temperature_1 integer, temperature_2 integer, temperature_3 integer, temperature_4 integer, temperature_5 integer, temperature_6 integer, temperature_7 integer, temperature_8 integer, temperature_9 integer, temperature_10 integer, temperature_11 integer, temperature_12 integer, temperature_13 integer, temperature_14 integer, temperature_15 integer, temperature_16 integer, temperature_17 integer, temperature_18 integer);
INSERT INTO "Standard_2" VALUES(1, '2012-7-9 13:00:03', 20, 40, 60, 300, 500, 650, 800, 980, 1020, 1025, 1100, 1000, 950, 700, 600, 400, 300, 200);
INSERT INTO "Standard_2" VALUES(2, '2012-7-10 13:00:03', 20, 40, 60, 300, 500, 650, 800, 980, 1020, 1025, 1100, 1000, 950, 700, 600, 400, 300, 200);

CREATE TABLE Standard_3(id integer, set_time text, temperature_1 integer, temperature_2 integer, temperature_3 integer, temperature_4 integer, temperature_5 integer, temperature_6 integer, temperature_7 integer, temperature_8 integer);
INSERT INTO "Standard_3" VALUES(1, '2012-7-8 13:00:03', 20, 40, 60, 300, 500, 650, 800, 980);
INSERT INTO "Standard_3" VALUES(2, '2012-7-9 14:00:03', 20, 40, 60, 300, 500, 650, 800, 980);
INSERT INTO "Standard_3" VALUES(3, '2012-7-10 13:00:03', 20, 40, 60, 300, 500, 650, 800, 980);

CREATE TABLE Standard_4(id integer, set_time text, temperature_1 integer, temperature_2 integer, temperature_3 integer, temperature_4 integer, temperature_5 integer, temperature_6 integer, temperature_7 integer);
INSERT INTO "Standard_4" VALUES(1, '2012-7-11 13:00:03', 20, 40, 60, 300, 500, 650, 800);
INSERT INTO "Standard_4" VALUES(2, '2012-7-12 13:00:03', 20, 40, 60, 300, 500, 650, 800);
INSERT INTO "Standard_4" VALUES(3, '2012-7-13 13:00:03', 20, 40, 60, 300, 500, 650, 800);
INSERT INTO "Standard_4" VALUES(4, '2012-7-14 13:00:03', 20, 40, 60, 300, 500, 650, 800);

CREATE TABLE History_1(id integer primary key, save_time text, temperature_1 integer, temperature_2 integer, temperature_3 integer, temperature_4 integer, temperature_5 integer, temperature_6 integer, temperature_7 integer, temperature_8 integer, temperature_9 integer, temperature_10 integer, temperature_11 integer, temperature_12 integer, temperature_13 integer, temperature_14 integer, temperature_15 integer, temperature_16 integer, press_1 integer, press_2 integer, press_3 integer, press_4 integer, motor_1 float, motor_2 float);
INSERT INTO "History_1" VALUES(1, '2012-8-1 13:00:03', 20, 40, 60, 300, 500, 650, 800, 980, 1020, 1000, 950, 700, 600, 400, 300, 200, 50, 100, 150, 200, 30.2, 40.3);
INSERT INTO "History_1" VALUES(2, '2012-8-2 13:00:03', 20, 40, 60, 300, 500, 650, 800, 980, 1020, 1000, 950, 700, 600, 400, 300, 200, 50, 100, 150, 200, 30.2, 40.3);
INSERT INTO "History_1" VALUES(3, '2012-8-3 13:00:03', 20, 40, 60, 300, 500, 650, 800, 980, 1020, 1000, 950, 700, 600, 400, 300, 200, 50, 100, 150, 200, 30.2, 40.3);
INSERT INTO "History_1" VALUES(4, '2012-8-4 13:00:03', 20, 40, 60, 300, 500, 650, 800, 980, 1020, 1000, 950, 700, 600, 400, 300, 200, 50, 100, 150, 200, 30.2, 40.3);
INSERT INTO "History_1" VALUES(5, '2012-8-5 13:00:03', 20, 40, 60, 300, 500, 650, 800, 980, 1020, 1000, 950, 700, 600, 400, 300, 200, 50, 100, 150, 200, 30.2, 40.3);
INSERT INTO "History_1" VALUES(6, '2012-8-6 13:00:03', 20, 40, 60, 300, 500, 650, 800, 980, 1020, 1000, 950, 700, 600, 400, 300, 200, 50, 100, 150, 200, 30.2, 40.3);
INSERT INTO "History_1" VALUES(7, '2012-8-7 13:00:03', 20, 40, 60, 300, 500, 650, 800, 980, 1020, 1000, 950, 700, 600, 400, 300, 200, 50, 100, 150, 200, 30.2, 40.3);
INSERT INTO "History_1" VALUES(8, '2012-8-8 13:00:03', 20, 40, 60, 300, 500, 650, 800, 980, 1020, 1000, 950, 700, 600, 400, 300, 200, 50, 100, 150, 200, 30.2, 40.3);

CREATE TABLE History_2(id integer primary key, save_time text, temperature_1 integer, temperature_2 integer, temperature_3 integer, temperature_4 integer, temperature_5 integer, temperature_6 integer, temperature_7 integer, temperature_8 integer, temperature_9 integer, temperature_10 integer, temperature_11 integer, temperature_12 integer, temperature_13 integer, temperature_14 integer, temperature_15 integer, temperature_16 integer, temperature_17 integer, temperature_18 integer, press_1 integer, press_2 integer, motor_1 float);
INSERT INTO "History_2" VALUES(1, '2012-7-11 13:00:03', 20, 40, 60, 300, 500, 650, 800, 980, 1020, 1050, 1090, 1000, 950, 700, 600, 400, 300, 200, 50, 100, 40.3);
INSERT INTO "History_2" VALUES(2, '2012-7-12 13:00:03', 20, 40, 60, 300, 500, 650, 800, 980, 1020, 1050, 1090, 1000, 950, 700, 600, 400, 300, 200, 50, 100, 40.3);
INSERT INTO "History_2" VALUES(3, '2012-7-13 13:00:03', 20, 40, 60, 300, 500, 650, 800, 980, 1020, 1050, 1090, 1000, 950, 700, 600, 400, 300, 200, 50, 100, 40.3);
INSERT INTO "History_2" VALUES(4, '2012-7-14 13:00:03', 20, 40, 60, 300, 500, 650, 800, 980, 1020, 1050, 1090, 1000, 950, 700, 600, 400, 300, 200, 50, 100, 40.3);
INSERT INTO "History_2" VALUES(5, '2012-7-15 13:00:03', 20, 40, 60, 300, 500, 650, 800, 980, 1020, 1050, 1090, 1000, 950, 700, 600, 400, 300, 200, 50, 100, 40.3);
INSERT INTO "History_2" VALUES(6, '2012-7-16 13:00:03', 20, 40, 60, 300, 500, 650, 800, 980, 1020, 1050, 1090, 1000, 950, 700, 600, 400, 300, 200, 50, 100, 40.3);
INSERT INTO "History_2" VALUES(7, '2012-7-17 13:00:03', 20, 40, 60, 300, 500, 650, 800, 980, 1020, 1050, 1090, 1000, 950, 700, 600, 400, 300, 200, 50, 100, 40.3);
INSERT INTO "History_2" VALUES(8, '2012-7-18 13:00:03', 20, 40, 60, 300, 500, 650, 800, 980, 1020, 1050, 1090, 1000, 950, 700, 600, 400, 300, 200, 50, 100, 40.3);

CREATE TABLE History_3(id integer primary key, save_time text, temperature_1 integer, temperature_2 integer, temperature_3 integer, temperature_4 integer, temperature_5 integer, temperature_6 integer, temperature_7 integer, temperature_8 integer, press_1 integer, press_2 integer, press_3 integer, press_4 integer, humidity_1 float, humidity_2 float);

CREATE TABLE History_4(id integer primary key, save_time text, temperature_1 integer, temperature_2 integer, temperature_3 integer, temperature_4 integer, temperature_5 integer, temperature_6 integer, temperature_7 integer, humidity_1 float, humidity_2 float);


CREATE TABLE CarHeat_1(car_id integer, heat double);
INSERT INTO "CarHeat_1" VALUES(1, 300.6);
INSERT INTO "CarHeat_1" VALUES(2, 350.6);
INSERT INTO "CarHeat_1" VALUES(3, 350.6);
INSERT INTO "CarHeat_1" VALUES(4, 350.6);
INSERT INTO "CarHeat_1" VALUES(5, 350.6);
INSERT INTO "CarHeat_1" VALUES(6, 350.6);
INSERT INTO "CarHeat_1" VALUES(7, 350.6);
INSERT INTO "CarHeat_1" VALUES(8, 350.6);
INSERT INTO "CarHeat_1" VALUES(9, 350.6);
INSERT INTO "CarHeat_1" VALUES(10, 350.6);
INSERT INTO "CarHeat_1" VALUES(11, 350.6);
INSERT INTO "CarHeat_1" VALUES(12, 350.6);
INSERT INTO "CarHeat_1" VALUES(13, 350.6);
INSERT INTO "CarHeat_1" VALUES(14, 350.6);
INSERT INTO "CarHeat_1" VALUES(15, 350.6);
INSERT INTO "CarHeat_1" VALUES(16, 350.6);
INSERT INTO "CarHeat_1" VALUES(17, 350.6);
INSERT INTO "CarHeat_1" VALUES(18, 350.6);
INSERT INTO "CarHeat_1" VALUES(19, 350.6);
INSERT INTO "CarHeat_1" VALUES(20, 350.6);
INSERT INTO "CarHeat_1" VALUES(21, 300.6);
INSERT INTO "CarHeat_1" VALUES(22, 350.6);
INSERT INTO "CarHeat_1" VALUES(23, 350.6);
INSERT INTO "CarHeat_1" VALUES(24, 350.6);
INSERT INTO "CarHeat_1" VALUES(25, 350.6);
INSERT INTO "CarHeat_1" VALUES(26, 350.6);
INSERT INTO "CarHeat_1" VALUES(27, 350.6);
INSERT INTO "CarHeat_1" VALUES(28, 350.6);
INSERT INTO "CarHeat_1" VALUES(29, 350.6);
INSERT INTO "CarHeat_1" VALUES(30, 350.6);
INSERT INTO "CarHeat_1" VALUES(31, 350.6);
INSERT INTO "CarHeat_1" VALUES(32, 450.6);
INSERT INTO "CarHeat_1" VALUES(33, 450.6);
INSERT INTO "CarHeat_1" VALUES(34, 450.6);
INSERT INTO "CarHeat_1" VALUES(35, 450.6);
INSERT INTO "CarHeat_1" VALUES(36, 450.6);
INSERT INTO "CarHeat_1" VALUES(37, 450.6);
INSERT INTO "CarHeat_1" VALUES(38, 450.6);
INSERT INTO "CarHeat_1" VALUES(39, 450.6);
INSERT INTO "CarHeat_1" VALUES(40, 450.6);

CREATE TABLE CarHeat_2(car_id integer, heat double);
INSERT INTO "CarHeat_2" VALUES(1, 300.6);
INSERT INTO "CarHeat_2" VALUES(2, 350.6);
INSERT INTO "CarHeat_2" VALUES(3, 350.6);
INSERT INTO "CarHeat_2" VALUES(4, 350.6);
INSERT INTO "CarHeat_2" VALUES(5, 350.6);
INSERT INTO "CarHeat_2" VALUES(6, 350.6);
INSERT INTO "CarHeat_2" VALUES(7, 350.6);
INSERT INTO "CarHeat_2" VALUES(8, 350.6);
INSERT INTO "CarHeat_2" VALUES(9, 350.6);
INSERT INTO "CarHeat_2" VALUES(10, 350.6);
INSERT INTO "CarHeat_2" VALUES(11, 350.6);
INSERT INTO "CarHeat_2" VALUES(12, 350.6);
INSERT INTO "CarHeat_2" VALUES(13, 350.6);
INSERT INTO "CarHeat_2" VALUES(14, 350.6);
INSERT INTO "CarHeat_2" VALUES(15, 350.6);
INSERT INTO "CarHeat_2" VALUES(16, 350.6);
INSERT INTO "CarHeat_2" VALUES(17, 350.6);
INSERT INTO "CarHeat_2" VALUES(18, 350.6);
INSERT INTO "CarHeat_2" VALUES(19, 350.6);
INSERT INTO "CarHeat_2" VALUES(20, 350.6);
INSERT INTO "CarHeat_2" VALUES(21, 300.6);
INSERT INTO "CarHeat_2" VALUES(22, 350.6);
INSERT INTO "CarHeat_2" VALUES(23, 350.6);
INSERT INTO "CarHeat_2" VALUES(24, 350.6);
INSERT INTO "CarHeat_2" VALUES(25, 350.6);
INSERT INTO "CarHeat_2" VALUES(26, 350.6);
INSERT INTO "CarHeat_2" VALUES(27, 350.6);
INSERT INTO "CarHeat_2" VALUES(28, 350.6);
INSERT INTO "CarHeat_2" VALUES(29, 350.6);
INSERT INTO "CarHeat_2" VALUES(30, 350.6);
INSERT INTO "CarHeat_2" VALUES(31, 350.6);
INSERT INTO "CarHeat_2" VALUES(32, 450.6);
INSERT INTO "CarHeat_2" VALUES(33, 450.6);
INSERT INTO "CarHeat_2" VALUES(34, 450.6);
INSERT INTO "CarHeat_2" VALUES(35, 450.6);
INSERT INTO "CarHeat_2" VALUES(36, 450.6);

CREATE TABLE Outputs(roast_index integer, out_time text, car_id integer, heat double, condition integer);
INSERT INTO "Outputs" VALUES(1, "2012-8-9 12:00:09", 40, 450.6, 0);
INSERT INTO "Outputs" VALUES(2, "2012-8-9 12:00:09", 36, 450.6, 0);

COMMIT;
