EESchema Schematic File Version 4
LIBS:w-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Microchip_ATtiny:ATtiny85-20PU U1
U 1 1 606E0204
P 4050 3800
F 0 "U1" H 3520 3846 50  0000 R CNN
F 1 "ATtiny85-20PU" H 3520 3755 50  0000 R CNN
F 2 "Package_DIP:DIP-8_W7.62mm" H 4050 3800 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/atmel-2586-avr-8-bit-microcontroller-attiny25-attiny45-attiny85_datasheet.pdf" H 4050 3800 50  0001 C CNN
	1    4050 3800
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:AMS1117 U2
U 1 1 606E0662
P 3400 1650
F 0 "U2" H 3400 1892 50  0000 C CNN
F 1 "AMS1117" H 3400 1801 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 3400 1850 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 3500 1400 50  0001 C CNN
	1    3400 1650
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:2N2219 Q2
U 1 1 606E08CD
P 7000 4600
F 0 "Q2" H 7191 4646 50  0000 L CNN
F 1 "BC547C" H 7191 4555 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-39-3" H 7200 4525 50  0001 L CIN
F 3 "http://www.onsemi.com/pub_link/Collateral/2N2219-D.PDF" H 7000 4600 50  0001 L CNN
	1    7000 4600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 606E4660
P 3400 2050
F 0 "#PWR0101" H 3400 1800 50  0001 C CNN
F 1 "GND" H 3405 1877 50  0000 C CNN
F 2 "" H 3400 2050 50  0001 C CNN
F 3 "" H 3400 2050 50  0001 C CNN
	1    3400 2050
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0103
U 1 1 606E4A9A
P 3800 1650
F 0 "#PWR0103" H 3800 1500 50  0001 C CNN
F 1 "VCC" H 3817 1823 50  0000 C CNN
F 2 "" H 3800 1650 50  0001 C CNN
F 3 "" H 3800 1650 50  0001 C CNN
	1    3800 1650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 606F6D05
P 7100 4050
F 0 "R3" H 7170 4096 50  0000 L CNN
F 1 "100k" H 7170 4005 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P1.90mm_Vertical" V 7030 4050 50  0001 C CNN
F 3 "~" H 7100 4050 50  0001 C CNN
	1    7100 4050
	1    0    0    -1  
$EndComp
Text GLabel 8200 4600 2    50   Input ~ 0
O3V
Wire Wire Line
	8200 4600 7700 4600
$Comp
L power:GND #PWR0106
U 1 1 606F6F58
P 7100 4800
F 0 "#PWR0106" H 7100 4550 50  0001 C CNN
F 1 "GND" H 7105 4627 50  0000 C CNN
F 2 "" H 7100 4800 50  0001 C CNN
F 3 "" H 7100 4800 50  0001 C CNN
	1    7100 4800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 606F7085
P 6450 4600
F 0 "R1" V 6243 4600 50  0000 C CNN
F 1 "1k" V 6334 4600 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P1.90mm_Vertical" V 6380 4600 50  0001 C CNN
F 3 "~" H 6450 4600 50  0001 C CNN
	1    6450 4600
	0    1    1    0   
$EndComp
Text GLabel 6300 4600 0    50   Input ~ 0
ALFA
Text GLabel 4800 3800 2    50   Input ~ 0
ALFA
$Comp
L power:VCC #PWR0112
U 1 1 606F8E0E
P 4050 3100
F 0 "#PWR0112" H 4050 2950 50  0001 C CNN
F 1 "VCC" H 4067 3273 50  0000 C CNN
F 2 "" H 4050 3100 50  0001 C CNN
F 3 "" H 4050 3100 50  0001 C CNN
	1    4050 3100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0113
U 1 1 606F8EC7
P 4050 4500
F 0 "#PWR0113" H 4050 4250 50  0001 C CNN
F 1 "GND" H 4055 4327 50  0000 C CNN
F 2 "" H 4050 4500 50  0001 C CNN
F 3 "" H 4050 4500 50  0001 C CNN
	1    4050 4500
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0114
U 1 1 606F90BB
P 3000 1650
F 0 "#PWR0114" H 3000 1500 50  0001 C CNN
F 1 "+5V" H 3015 1823 50  0000 C CNN
F 2 "" H 3000 1650 50  0001 C CNN
F 3 "" H 3000 1650 50  0001 C CNN
	1    3000 1650
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0115
U 1 1 606F923E
P 1050 1900
F 0 "#PWR0115" H 1050 1750 50  0001 C CNN
F 1 "+5V" H 1065 2073 50  0000 C CNN
F 2 "" H 1050 1900 50  0001 C CNN
F 3 "" H 1050 1900 50  0001 C CNN
	1    1050 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 606F92B5
P 1050 2200
F 0 "C1" H 1165 2246 50  0000 L CNN
F 1 "C" H 1165 2155 50  0000 L CNN
F 2 "Capacitor_THT:CP_Axial_L10.0mm_D6.0mm_P15.00mm_Horizontal" H 1088 2050 50  0001 C CNN
F 3 "~" H 1050 2200 50  0001 C CNN
	1    1050 2200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0116
U 1 1 606F933F
P 1050 2400
F 0 "#PWR0116" H 1050 2150 50  0001 C CNN
F 1 "GND" H 1055 2227 50  0000 C CNN
F 2 "" H 1050 2400 50  0001 C CNN
F 3 "" H 1050 2400 50  0001 C CNN
	1    1050 2400
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 606F93D1
P 1700 2200
F 0 "C2" H 1815 2246 50  0000 L CNN
F 1 "C" H 1815 2155 50  0000 L CNN
F 2 "Capacitor_THT:CP_Axial_L10.0mm_D6.0mm_P15.00mm_Horizontal" H 1738 2050 50  0001 C CNN
F 3 "~" H 1700 2200 50  0001 C CNN
	1    1700 2200
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0117
U 1 1 606F94BE
P 1700 1900
F 0 "#PWR0117" H 1700 1750 50  0001 C CNN
F 1 "VCC" H 1717 2073 50  0000 C CNN
F 2 "" H 1700 1900 50  0001 C CNN
F 3 "" H 1700 1900 50  0001 C CNN
	1    1700 1900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0118
U 1 1 606F951D
P 1700 2450
F 0 "#PWR0118" H 1700 2200 50  0001 C CNN
F 1 "GND" H 1705 2277 50  0000 C CNN
F 2 "" H 1700 2450 50  0001 C CNN
F 3 "" H 1700 2450 50  0001 C CNN
	1    1700 2450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0120
U 1 1 606FA1B3
P 5500 4250
F 0 "#PWR0120" H 5500 4000 50  0001 C CNN
F 1 "GND" H 5505 4077 50  0000 C CNN
F 2 "" H 5500 4250 50  0001 C CNN
F 3 "" H 5500 4250 50  0001 C CNN
	1    5500 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4650 4000 5150 4000
Wire Wire Line
	1050 2350 1050 2400
Wire Wire Line
	3700 1650 3800 1650
Wire Wire Line
	3400 2050 3400 1950
Wire Wire Line
	3000 1650 3100 1650
Wire Wire Line
	1700 2450 1700 2350
Wire Wire Line
	1050 2050 1050 1900
Wire Wire Line
	4050 4400 4050 4500
Wire Wire Line
	4050 3100 4050 3200
Wire Wire Line
	1700 1900 1700 2050
$Comp
L Connector_Generic_MountingPin:Conn_01x08_MountingPin J5
U 1 1 607A075A
P 8900 5600
F 0 "J5" H 8987 5516 50  0000 L CNN
F 1 "Conn_01x08_MountingPin" H 8987 5425 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 8900 5600 50  0001 C CNN
F 3 "~" H 8900 5600 50  0001 C CNN
	1    8900 5600
	1    0    0    -1  
$EndComp
Text GLabel 8450 5300 0    50   Input ~ 0
VIN
Text GLabel 8450 5400 0    50   Input ~ 0
GND
Text GLabel 8450 5500 0    50   Input ~ 0
O5V
Text GLabel 8450 5600 0    50   Input ~ 0
GND
Text GLabel 8450 5700 0    50   Input ~ 0
O3V
Text GLabel 8450 5800 0    50   Input ~ 0
GND
Text GLabel 8450 5900 0    50   Input ~ 0
CLK
Text GLabel 8450 6000 0    50   Input ~ 0
DAT
Wire Wire Line
	8450 5300 8700 5300
Wire Wire Line
	8450 5400 8700 5400
Wire Wire Line
	8450 5500 8700 5500
Wire Wire Line
	8450 5600 8700 5600
Wire Wire Line
	8450 5700 8700 5700
Wire Wire Line
	8450 5800 8700 5800
Wire Wire Line
	8450 5900 8700 5900
Wire Wire Line
	8450 6000 8700 6000
$Comp
L Transistor_FET:IRF9540N Q?
U 1 1 608DD22E
P 7600 4200
F 0 "Q?" H 7805 4154 50  0000 L CNN
F 1 "IRF9540N" H 7805 4245 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 7800 4125 50  0001 L CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irf9540n.pdf" H 7600 4200 50  0001 L CNN
	1    7600 4200
	1    0    0    1   
$EndComp
Wire Wire Line
	7100 4200 7100 4400
Wire Wire Line
	7400 4200 7100 4200
Connection ~ 7100 4200
Wire Wire Line
	7700 4400 7700 4600
Wire Wire Line
	6600 4600 6800 4600
$Comp
L power:+5V #PWR0111
U 1 1 606F82C6
P 7700 3650
F 0 "#PWR0111" H 7700 3500 50  0001 C CNN
F 1 "+5V" H 7715 3823 50  0000 C CNN
F 2 "" H 7700 3650 50  0001 C CNN
F 3 "" H 7700 3650 50  0001 C CNN
	1    7700 3650
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0110
U 1 1 606F8274
P 7100 3650
F 0 "#PWR0110" H 7100 3500 50  0001 C CNN
F 1 "+5V" H 7115 3823 50  0000 C CNN
F 2 "" H 7100 3650 50  0001 C CNN
F 3 "" H 7100 3650 50  0001 C CNN
	1    7100 3650
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D?
U 1 1 608E08B0
P 5250 3600
F 0 "D?" H 5242 3345 50  0000 C CNN
F 1 "LED" H 5242 3436 50  0000 C CNN
F 2 "" H 5250 3600 50  0001 C CNN
F 3 "~" H 5250 3600 50  0001 C CNN
	1    5250 3600
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 608E098A
P 5500 3850
F 0 "R?" H 5570 3896 50  0000 L CNN
F 1 "330" H 5570 3805 50  0000 L CNN
F 2 "" V 5430 3850 50  0001 C CNN
F 3 "~" H 5500 3850 50  0001 C CNN
	1    5500 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 4000 5500 4250
Wire Wire Line
	5400 3600 5500 3600
Wire Wire Line
	5500 3600 5500 3700
Wire Wire Line
	4650 3600 5100 3600
Wire Wire Line
	7700 3650 7700 4000
Wire Wire Line
	7100 3650 7100 3900
Wire Wire Line
	4650 3800 4800 3800
$EndSCHEMATC
