#!/usr/bin/python
import os
import datetime
import sqlite3

path = '/home/fwauters/Analysis/Hindenburg'
filename = 'Keithley2611B_228.log'
    
conn = sqlite3.connect('/home/fwauters/Analysis/Hindenburg/runDB.db')
print "Opened database successfully";

f=open(path+"/"+filename,"r")

for line in f:
	timestamp = line.split("	")[0] 
	hv = line.split("	")[2]
	if(timestamp != "Time"):
		sql_cmd = "INSERT INTO hv (time,hv) VALUES ('" + str(timestamp) + "'," + hv  + ");"
		print sql_cmd
		#break
		conn.execute(sql_cmd)
	
conn.commit()
print "Records created successfully"
conn.close()
