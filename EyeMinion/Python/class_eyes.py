#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
Eyes Library (OctoWS2812)

This library is for communicate with eyes via serial

Author : Fabien0102
Last update : May 2015
"""
import serial

import os
import time

BAUD_RATE = 9600

class Eyes():
    def __init__(self, port ='auto'):
        if (port == 'auto'):
            self._ser = None
            self._serOpen = False
            self._serPort = None
            self._auto_connect() # Auto connect serial port
        else:
            self._ser = serial.Serial(port, BAUD_RATE)
            self._serOpen = True
            self._serPort = port
            
        self._debug = False
        
            
    """
    ------------------
    --- ANIMATIONS ---
    ------------------
    """
    def k2000(self):
        self._run("A01")    
    def heart(self):
        self._run("A02")
    def clap(self):
        self._run("A03")
    def rainbow(self):
        self._run("A04")
    def silon(self):
        self._run("A05")
    def eyes(self):
        self._run("A06")
    def look_around(self):
        self._run("A08")
    def flash(self):
        self._run("A10")
    def look_left(self):
        self._run("A11")
	def look_rigth(self):
		self._run("A12")
		
	def text(self, text_to_display):
		self._run("T"+text_to_display)
    
    """
    ------------
    --- CORE ---
    ------------
    """
    def _help(self):
        print "------------"
        print "Animations :"
        print "------------"
        for anim in dir(self):
            if "_" not in anim:
                print anim
                
        print "------------"
        print "debug (on|off)"
    def _set_debug(self, value):
        self._debug = value
    def _auto_connect(self):
        for port in self._list_serial_ports():
            try:
                ser = serial.Serial(port, baudrate= BAUD_RATE, timeout=2)
                ser.write("V\n")
                result = ser.readline()
                if "MIB Eyes" in result:
                        print "Connect Successful! Connected on port:",port
                        self._ser = ser
                        self._ser.flush()
                        self._serOpen = True
                        self._serPort = port
                        break
            except:
                    pass
        if self._serOpen == False:
            print "Connection Failed :("
    def _list_serial_ports(self):
        # Windows
        if os.name == 'nt':
            # Scan for available ports.
            available = []
            for i in range(256):
                try:
                    ser = serial.Serial(i)
                    available.append('COM'+str(i + 1))
                    ser.close()
                except serial.SerialException:
                    pass
            return available
        else:
            # Mac / Linux
            # return [port[0] for port in list_ports.comports()]
            return 0
    def _run(self, command):
        if self._serOpen == False:
            print "Connection Failed :("
        else:
            # Send command into serial
            self._ser.write(str(command)+"\n")
            
            # Print respond of Eyes (Debug only)
            if self._ser.readable() and self._debug:
                read = self._ser.readline()
                if len(read) == 0:
                    print "No respond of MIB Eyes :( "
                else:
                    print "[%s] %s"%(self._serPort, str(read))
        
if __name__ == '__main__':
    # CLI Interface
    quit = False
    print "MIB Eyes 0.0.1"
    print "Type \"help\" for listing animations available"
    
    eyes = Eyes()
    while not quit:
        cmd = raw_input(">>> ")
        
        if cmd in ["quit", "q", "exit", ":q"]:
            quit = True
        elif cmd in dir(eyes): #cmd is in defines functions
            eval("eyes."+cmd+"()") #exec cmd as function :)
        elif cmd == "help":
            eyes._help()
        elif cmd == "debug on":
            eyes._set_debug(True)
        elif cmd == "debug off":
            eyes._set_debug(False)
        else:
            eyes._run(cmd)
            
    print "Good bye :)"
