#!/usr/bin/python
import os
import datetime

path = '/data2/mu3e/IKAR2017part2/infos'
base_name = 'monitoring_file'
extension = 'histos.root'
import sqlite3

def do_ls():
  tempfilename = 'tempfile'
  cmd="ls " + path + "/" + base_name + "*" + extension + " > " + tempfilename
  #print cmd
  os.system(cmd)
  file1 = open(tempfilename,"r")
  files=[]
  for line in file1:
    tocut=path+'/'
    #print line[:-1]
    files.append(line[:-1].split(tocut)[1])
  return files
  
def modification_date(filename):
    t = os.path.getmtime(filename)
    return datetime.datetime.fromtimestamp(t)
    
files = do_ls()

conn = sqlite3.connect('/home/fwauters/Analysis/Hindenburg/runDB.db')
print "Opened database successfully";

for f in files:
	full_filename = path + "/" + f
	timestamp = modification_date(full_filename)
	run_number = f.split(base_name)[1][0:6].lstrip('0')
	if (run_number != "" and int(run_number) > 5130):
	  sql_cmd = "INSERT INTO runs (run_nr,time) VALUES (" + run_number + ",'" + str(timestamp)  + "')"
	  print sql_cmd
	  conn.execute(sql_cmd)
	
conn.commit()
print "Records created successfully"
conn.close()
