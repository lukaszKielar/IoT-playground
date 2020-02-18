#!/usr/bin/python3

import os
import glob
import time
import socket
from config import api_key
from datetime import datetime
import RPi.GPIO as GPIO
from Adafruit_CharLCD import Adafruit_CharLCD

#  Initialize LCD (must specify pinout and dimensions)
lcd = Adafruit_CharLCD(rs=26, en=19, d4=13, d5=6, d6=5, d7=11, cols=16, lines=2)

os.system('modprobe w1-gpio')
os.system('modprobe w1-therm')

base_dir = '/sys/bus/w1/devices/'
device_folder = glob.glob(base_dir + '28*')[0]
device_file = device_folder + '/w1_slave'

def read_temp():
    with open(device_file, 'r') as f:
        lines = f.readlines()
    while lines[0].strip()[-3:] != 'YES':
        time.sleep(0.2)
    equals_pos = lines[1].find('t=')
    if equals_pos != -1:
        temp_string = lines[1][equals_pos+2:]
        temp_c = "{0:.2f}".format(float(temp_string)/1000)
        return temp_c

while True:
    try:
        lcd.clear()
        temp = read_temp()
        thingspeak = urllib.request.urlopen("https://api.thingspeak.com/update?api_key={}&field1={}".format(api_key, temp))
        reading = 'Temp: {}{}C'.format(temp, chr(186))
        print(reading)
        lcd.message(reading)
        time.sleep(1)

    except KeyboardInterrupt:
        print('CTRL-C pressed. Program exiting...')
        break

    finally:
        lcd.clear()
        GPIO.cleanup()
