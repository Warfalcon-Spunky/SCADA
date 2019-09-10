copy .\db\RunDB.DB D:\iControl\RunBakDB.sql.db
.\db\sqlite3.exe D:\iControl\RunBakDB.sql.db .dump > d:\iControl\RunBakDB.sql
del D:\iControl\RunBakDB.sql.db