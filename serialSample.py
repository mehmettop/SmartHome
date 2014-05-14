#! /usr/bin/python
import serial
import string
import MySQLdb

db = MySQLdb.connect("192.168.2.5","raspberry","1234","rpi")
curs=db.cursor()
if(db):
 print "Database Connection O.K."
else:
 print "Databse Connection Fail"

try:
    curs.execute ("INSERT INTO log(date,time,exp) values(CURRENT_DATE(),NOW(),'RPI-Connection-Established')")
    db.commit()
except:
    print "Error : Rollback"
    db.rollback()

port = serial.Serial("/dev/ttyAMA0",9600,timeout=1)
port.open()
port.write('HelloArduino')

def readLine(port):
    str = ""
    while 1:
       ch = port.read(1)
       if(ch == '\n' or ch == '\r' or ch==''):
          break
       str += ch

    return str

while True:
    line = readLine(port)
    print line
    if(line == 'Password O.K.'):
       try:
         curs.execute ("INSERT INTO log(date,time,exp) values(CURRENT_DATE(),NOW(),'Password Correct Alarm Disabled')")
         db.commit()
       except:
         print "Error : Rollback"
         db.rollback()
    else: 
       if(line == 'stop'):
          print "Stoped"
          port.close()
   
    if(line == 'Password Fail'):
      try:
        curs.execute ("INSERT INTO log(date,time,exp) values(CURRENT_DATE(),NOW(),'Acces Failure Alarm Actived')")
        db.commit()
      except:
        print "Error : Rollback"
        db.rollback()
