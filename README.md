# Rpismartdog
Attiny85 as external watchdog for Raspberry Pi. 

Raspberry Pi Zero hangs from time to time.
Internal watchdog is scary - infinite reboot loop / 

attiny85 should implement:
- i2c as a way to send heartbits
- configurable powercycle delay
- power on after a given period
- power off on request 
