import serial
from time import *
from math import *

ser = serial.Serial("/dev/ttyAMA0")
ser.baudrate = 9600

send_time_once = 0
send_display_on_once = 0
send_display_off_once = 0

while 1:
	lt = localtime()
	lt_old = lt
	h, m, s, d = lt[3:7]

	#print "t%i:%i:%i:%i" % (h, m, s, d)

	# send time every minute
	if (s == 0) and (send_time_once == 0):
		ser.write("t%02i:%02i:%02i:%02i\n" % (h, m, s, d))
		send_time_once = 1
	
	if (s == 1):
		send_time_once = 0
	
	# display off from 22:00 to 6:00 every 5 minutes
	if ((h >= 22) or (h <= 6)) and (m % 5 == 0) and (s == 0) and (send_display_off_once == 0):
		ser.write("d0\n")
		send_display_off_once = 1
	
	# display on at 06:00 Mo-Fr and 10:00 Sa-So
	if (((d <= 4) and (h == 6)) or ((d >= 5) and (h == 10))) and (m == 0) and (s == 0) and (send_display_on_once == 0):
		ser.write("d1\n")
		send_display_on_once = 1
	
	if (s == 1):
		send_display_on_once = 0
		send_display_off_once = 0

ser.close()
