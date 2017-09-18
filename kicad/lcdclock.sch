EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:lcdclock
LIBS:lcdclock-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "LCD Clock"
Date "2017-09-18"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L GND #PWR01
U 1 1 59B7CB4A
P 700 1650
F 0 "#PWR01" H 700 1400 50  0001 C CNN
F 1 "GND" H 700 1500 50  0000 C CNN
F 2 "" H 700 1650 50  0001 C CNN
F 3 "" H 700 1650 50  0001 C CNN
	1    700  1650
	1    0    0    -1  
$EndComp
$Comp
L +BATT #PWR02
U 1 1 59B7CCCA
P 700 750
F 0 "#PWR02" H 700 600 50  0001 C CNN
F 1 "+BATT" H 700 890 50  0000 C CNN
F 2 "" H 700 750 50  0001 C CNN
F 3 "" H 700 750 50  0001 C CNN
	1    700  750 
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 59B9D9D7
P 1600 5050
F 0 "#PWR03" H 1600 4800 50  0001 C CNN
F 1 "GND" H 1600 4900 50  0000 C CNN
F 2 "" H 1600 5050 50  0001 C CNN
F 3 "" H 1600 5050 50  0001 C CNN
	1    1600 5050
	1    0    0    -1  
$EndComp
$Comp
L L_Core_Ferrite L1
U 1 1 59B9F09D
P 1600 3500
F 0 "L1" H 1550 3500 50  0000 C CNN
F 1 "470u" V 1710 3500 50  0000 C CNN
F 2 "LCD Clock:WE_470uH_Inductor" H 1600 3500 50  0001 C CNN
F 3 "" H 1600 3500 50  0001 C CNN
	1    1600 3500
	1    0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 59B9F416
P 1350 3750
F 0 "C5" H 1375 3850 50  0000 L CNN
F 1 "100u" H 1375 3650 50  0000 L CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" H 1388 3600 50  0001 C CNN
F 3 "" H 1350 3750 50  0001 C CNN
	1    1350 3750
	0    -1   -1   0   
$EndComp
$Comp
L 3.6V_Lithium_AA BT1
U 1 1 59BA71F1
P 700 1450
F 0 "BT1" H 800 1550 50  0000 L CNN
F 1 "3.6V Lithium AA" H 800 1450 50  0000 L CNN
F 2 "LCD Clock:CONNECTOR_2x1" V 700 1510 50  0001 C CNN
F 3 "" V 700 1510 50  0001 C CNN
	1    700  1450
	1    0    0    -1  
$EndComp
$Comp
L TEST TP4
U 1 1 59BAD22D
P 1900 3650
F 0 "TP4" H 1900 3950 50  0000 C BNN
F 1 "TVCC" H 1900 3900 50  0000 C CNN
F 2 "Measurement_Points:Measurement_Point_Round-SMD-Pad_Big" H 1900 3650 50  0001 C CNN
F 3 "" H 1900 3650 50  0001 C CNN
	1    1900 3650
	1    0    0    -1  
$EndComp
$Comp
L TEST TP5
U 1 1 59BAD5C4
P 2100 4250
F 0 "TP5" H 2100 4550 50  0000 C BNN
F 1 "TSIG" H 2100 4500 50  0000 C CNN
F 2 "Measurement_Points:Measurement_Point_Round-SMD-Pad_Big" H 2100 4250 50  0001 C CNN
F 3 "" H 2100 4250 50  0001 C CNN
	1    2100 4250
	1    0    0    -1  
$EndComp
$Comp
L D D1
U 1 1 59BAD8D9
P 700 1000
F 0 "D1" H 700 1100 50  0000 C CNN
F 1 "BAT60A" H 700 900 50  0000 C CNN
F 2 "Diodes_SMD:D_SOD-323_HandSoldering" H 700 1000 50  0001 C CNN
F 3 "" H 700 1000 50  0001 C CNN
	1    700  1000
	0    -1   1    0   
$EndComp
Wire Wire Line
	700  1550 700  1650
Wire Wire Line
	700  750  700  850 
Wire Wire Line
	2000 4350 2200 4350
Wire Wire Line
	1600 3650 1600 3850
Wire Wire Line
	1500 3750 2200 3750
Connection ~ 1600 3750
Wire Wire Line
	1200 3750 1100 3750
Wire Wire Line
	1100 3750 1100 4950
Wire Wire Line
	1600 4850 1600 5050
Wire Wire Line
	700  1250 700  1150
$Comp
L LCD_DE377 U4
U 1 1 59B92408
P 10900 3350
F 0 "U4" H 10650 6100 60  0000 L CNN
F 1 "LCD_DE377" H 11150 400 60  0000 R CNN
F 2 "LCD Clock:LCD-DIP" H 10650 5100 60  0001 C CNN
F 3 "" H 10650 5100 60  0001 C CNN
	1    10900 3350
	1    0    0    -1  
$EndComp
$Comp
L STM32L073xx_LQFP100 U1
U 1 1 59C01C12
P 3500 1850
F 0 "U1" V 2850 1850 60  0000 C CNN
F 1 "STM32L073V8" V 4150 1850 60  0000 C CNN
F 2 "Housings_QFP:LQFP-100_14x14mm_Pitch0.5mm" H 3750 1600 60  0001 C CNN
F 3 "" H 3750 1600 60  0001 C CNN
	1    3500 1850
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 59C01DE2
P 2650 1850
F 0 "C6" H 2675 1950 50  0000 L CNN
F 1 "100n" H 2675 1750 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 2688 1700 50  0001 C CNN
F 3 "" H 2650 1850 50  0001 C CNN
	1    2650 1850
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 59C01E75
P 2400 1850
F 0 "C4" H 2425 1950 50  0000 L CNN
F 1 "100n" H 2425 1750 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 2438 1700 50  0001 C CNN
F 3 "" H 2400 1850 50  0001 C CNN
	1    2400 1850
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 59C01EB0
P 2150 1850
F 0 "C3" H 2175 1950 50  0000 L CNN
F 1 "100n" H 2175 1750 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 2188 1700 50  0001 C CNN
F 3 "" H 2150 1850 50  0001 C CNN
	1    2150 1850
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 59C01F1C
P 1900 1850
F 0 "C2" H 1925 1950 50  0000 L CNN
F 1 "100n" H 1925 1750 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 1938 1700 50  0001 C CNN
F 3 "" H 1900 1850 50  0001 C CNN
	1    1900 1850
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 59C01F4C
P 1650 1850
F 0 "C1" H 1675 1950 50  0000 L CNN
F 1 "10u" H 1675 1750 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 1688 1700 50  0001 C CNN
F 3 "" H 1650 1850 50  0001 C CNN
	1    1650 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 2500 2650 2000
Wire Wire Line
	2400 2500 2400 2000
Wire Wire Line
	2150 2500 2150 2000
Wire Wire Line
	1900 2000 1900 2500
Wire Wire Line
	2650 1200 2650 1700
Wire Wire Line
	2400 1200 2400 1700
Wire Wire Line
	2150 1200 2150 1700
Wire Wire Line
	1900 1200 1900 1700
Wire Wire Line
	1650 1100 1650 1700
$Comp
L C C8
U 1 1 59C025EE
P 4350 1900
F 0 "C8" H 4375 2000 50  0000 L CNN
F 1 "100n" H 4375 1800 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 4388 1750 50  0001 C CNN
F 3 "" H 4350 1900 50  0001 C CNN
	1    4350 1900
	1    0    0    -1  
$EndComp
$Comp
L C C9
U 1 1 59C02751
P 4600 1900
F 0 "C9" H 4625 2000 50  0000 L CNN
F 1 "1u" H 4625 1800 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 4638 1750 50  0001 C CNN
F 3 "" H 4600 1900 50  0001 C CNN
	1    4600 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 1200 4350 1750
Wire Wire Line
	3700 1100 3700 1300
Wire Wire Line
	3800 1300 3800 1200
Connection ~ 3800 1200
Wire Wire Line
	3700 2400 3700 2500
Wire Wire Line
	4350 2500 4350 2050
Wire Wire Line
	4600 2050 4600 2600
Connection ~ 4350 2500
Wire Wire Line
	4600 1200 4600 1750
Connection ~ 4350 1200
Wire Wire Line
	3800 2400 3800 2500
Connection ~ 3800 2500
Wire Wire Line
	3300 2400 3300 2600
Connection ~ 1900 2500
Connection ~ 2150 2500
Connection ~ 2400 2500
Connection ~ 2650 2500
Wire Wire Line
	3000 2500 3000 2400
Connection ~ 3000 2500
Wire Wire Line
	3100 2500 3100 2400
Connection ~ 3100 2500
Wire Wire Line
	3200 2500 3200 2400
Connection ~ 3200 2500
Wire Wire Line
	3300 1200 3300 1300
Wire Wire Line
	3200 1200 3200 1300
Connection ~ 3200 1200
Wire Wire Line
	3100 1200 3100 1300
Connection ~ 3100 1200
Wire Wire Line
	3000 1200 3000 1300
Connection ~ 3000 1200
Connection ~ 2650 1200
Connection ~ 2400 1200
Connection ~ 2150 1200
Connection ~ 1900 1200
Connection ~ 3300 1200
$Comp
L +BATT #PWR04
U 1 1 59C03416
P 1650 1100
F 0 "#PWR04" H 1650 950 50  0001 C CNN
F 1 "+BATT" H 1650 1240 50  0000 C CNN
F 2 "" H 1650 1100 50  0001 C CNN
F 3 "" H 1650 1100 50  0001 C CNN
	1    1650 1100
	1    0    0    -1  
$EndComp
Connection ~ 1650 1200
Connection ~ 3700 1200
Connection ~ 3300 2500
Wire Wire Line
	1650 2500 3300 2500
Wire Wire Line
	1650 1200 3500 1200
$Comp
L GND #PWR05
U 1 1 59C035FC
P 3300 2600
F 0 "#PWR05" H 3300 2350 50  0001 C CNN
F 1 "GND" H 3300 2450 50  0000 C CNN
F 2 "" H 3300 2600 50  0001 C CNN
F 3 "" H 3300 2600 50  0001 C CNN
	1    3300 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 1200 4600 1200
Wire Wire Line
	3700 2500 4600 2500
$Comp
L +BATT #PWR06
U 1 1 59C03738
P 3700 1100
F 0 "#PWR06" H 3700 950 50  0001 C CNN
F 1 "+BATT" H 3700 1240 50  0000 C CNN
F 2 "" H 3700 1100 50  0001 C CNN
F 3 "" H 3700 1100 50  0001 C CNN
	1    3700 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 2500 1650 2000
$Comp
L GND #PWR07
U 1 1 59C038C1
P 4600 2600
F 0 "#PWR07" H 4600 2350 50  0001 C CNN
F 1 "GND" H 4600 2450 50  0000 C CNN
F 2 "" H 4600 2600 50  0001 C CNN
F 3 "" H 4600 2600 50  0001 C CNN
	1    4600 2600
	1    0    0    -1  
$EndComp
Connection ~ 4600 2500
$Comp
L +BATT #PWR08
U 1 1 59C03A59
P 4000 750
F 0 "#PWR08" H 4000 600 50  0001 C CNN
F 1 "+BATT" H 4000 890 50  0000 C CNN
F 2 "" H 4000 750 50  0001 C CNN
F 3 "" H 4000 750 50  0001 C CNN
	1    4000 750 
	1    0    0    -1  
$EndComp
$Comp
L C C7
U 1 1 59C03A8D
P 4250 850
F 0 "C7" H 4275 950 50  0000 L CNN
F 1 "100n" H 4275 750 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 4288 700 50  0001 C CNN
F 3 "" H 4250 850 50  0001 C CNN
	1    4250 850 
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR09
U 1 1 59C03B38
P 4500 950
F 0 "#PWR09" H 4500 700 50  0001 C CNN
F 1 "GND" H 4500 800 50  0000 C CNN
F 2 "" H 4500 950 50  0001 C CNN
F 3 "" H 4500 950 50  0001 C CNN
	1    4500 950 
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 950  4500 850 
Wire Wire Line
	4500 850  4400 850 
Wire Wire Line
	4100 850  4000 850 
Wire Wire Line
	4000 750  4000 1300
Connection ~ 4000 850 
Wire Wire Line
	3500 1200 3500 1300
$Comp
L STM32L073xx_LQFP100 U1
U 2 1 59C07792
P 8900 3400
F 0 "U1" H 8450 6200 60  0000 L CNN
F 1 "STM32L073V8" H 9000 600 60  0000 R CNN
F 2 "Housings_QFP:LQFP-100_14x14mm_Pitch0.5mm" H 9150 3150 60  0001 C CNN
F 3 "" H 9150 3150 60  0001 C CNN
	2    8900 3400
	1    0    0    -1  
$EndComp
$Comp
L STM32L073xx_LQFP100 U1
U 3 1 59C10A96
P 5700 1450
F 0 "U1" H 5350 1750 60  0000 L CNN
F 1 "STM32L073V8" H 5700 1150 60  0000 R CNN
F 2 "Housings_QFP:LQFP-100_14x14mm_Pitch0.5mm" H 5950 1200 60  0001 C CNN
F 3 "" H 5950 1200 60  0001 C CNN
	3    5700 1450
	1    0    0    -1  
$EndComp
$Comp
L SW_Push SW1
U 1 1 59C114FC
P 6000 1000
F 0 "SW1" H 6050 1100 50  0000 L CNN
F 1 "SW_Push" H 6000 940 50  0000 C CNN
F 2 "" H 6000 1200 50  0001 C CNN
F 3 "" H 6000 1200 50  0001 C CNN
	1    6000 1000
	0    1    1    0   
$EndComp
Wire Wire Line
	6000 800  6000 700 
Wire Wire Line
	5900 1500 6650 1500
Wire Wire Line
	5900 1600 6650 1600
Wire Wire Line
	5900 1300 6250 1300
Wire Wire Line
	6000 1200 6000 1900
$Comp
L Conn_JLink_SWD J1
U 1 1 59C131D8
P 7200 1600
F 0 "J1" H 6850 2150 50  0000 L CNN
F 1 "Conn_JLink_SWD" H 7550 950 50  0000 R CNN
F 2 "" H 7200 1600 50  0001 C CNN
F 3 "" H 7200 1600 50  0001 C CNN
	1    7200 1600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR010
U 1 1 59C146AA
P 7850 2200
F 0 "#PWR010" H 7850 1950 50  0001 C CNN
F 1 "GND" H 7850 2050 50  0000 C CNN
F 2 "" H 7850 2200 50  0001 C CNN
F 3 "" H 7850 2200 50  0001 C CNN
	1    7850 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 700  7850 2200
Wire Wire Line
	7850 2100 7750 2100
Wire Wire Line
	7850 2000 7750 2000
Connection ~ 7850 2100
Wire Wire Line
	7750 1900 7850 1900
Connection ~ 7850 2000
Wire Wire Line
	7750 1800 7850 1800
Connection ~ 7850 1900
Wire Wire Line
	7750 1700 7850 1700
Connection ~ 7850 1800
Wire Wire Line
	7750 1600 7850 1600
Connection ~ 7850 1700
Wire Wire Line
	7750 1500 7850 1500
Connection ~ 7850 1600
Wire Wire Line
	7750 1400 7850 1400
Connection ~ 7850 1500
Wire Wire Line
	7750 1300 7850 1300
Connection ~ 7850 1400
NoConn ~ 7750 1200
NoConn ~ 6650 2100
NoConn ~ 6650 2000
NoConn ~ 6650 1700
NoConn ~ 6650 1400
NoConn ~ 6650 1300
NoConn ~ 6650 1800
$Comp
L +BATT #PWR011
U 1 1 59C14D27
P 6550 1100
F 0 "#PWR011" H 6550 950 50  0001 C CNN
F 1 "+BATT" H 6550 1240 50  0000 C CNN
F 2 "" H 6550 1100 50  0001 C CNN
F 3 "" H 6550 1100 50  0001 C CNN
	1    6550 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 700  7850 700 
Connection ~ 7850 1300
Wire Wire Line
	6000 1900 6650 1900
Connection ~ 6000 1300
$Comp
L PWR_FLAG #FLG012
U 1 1 59C167A9
P 1050 750
F 0 "#FLG012" H 1050 825 50  0001 C CNN
F 1 "PWR_FLAG" H 1050 900 50  0000 C CNN
F 2 "" H 1050 750 50  0001 C CNN
F 3 "" H 1050 750 50  0001 C CNN
	1    1050 750 
	1    0    0    -1  
$EndComp
Wire Wire Line
	700  800  1050 800 
Wire Wire Line
	1050 800  1050 750 
Connection ~ 700  800 
$Comp
L C C10
U 1 1 59C17FCD
P 6250 1000
F 0 "C10" H 6275 1100 50  0000 L CNN
F 1 "100n" H 6275 900 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 6288 850 50  0001 C CNN
F 3 "" H 6250 1000 50  0001 C CNN
	1    6250 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 1300 6250 1150
Wire Wire Line
	6250 850  6250 700 
Connection ~ 6250 700 
Wire Wire Line
	6550 1100 6550 1200
Wire Wire Line
	6550 1200 6650 1200
Wire Wire Line
	9200 750  10450 750 
Wire Wire Line
	10350 750  10350 1050
Wire Wire Line
	10350 850  10450 850 
Connection ~ 10350 750 
Wire Wire Line
	10350 950  10450 950 
Connection ~ 10350 850 
Wire Wire Line
	10350 1050 10450 1050
Connection ~ 10350 950 
NoConn ~ 9200 6050
NoConn ~ 9200 5950
NoConn ~ 9200 5850
$Comp
L STM32L073xx_LQFP100 U1
U 4 1 59C1D3F8
P 2650 4350
F 0 "U1" H 2400 4500 60  0000 L CNN
F 1 "STM32L073V8" H 2900 4200 60  0000 R CNN
F 2 "Housings_QFP:LQFP-100_14x14mm_Pitch0.5mm" H 2900 4100 60  0001 C CNN
F 3 "" H 2900 4100 60  0001 C CNN
	4    2650 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 4350 2100 4250
Connection ~ 2100 4350
Wire Wire Line
	3100 4350 3200 4350
Wire Wire Line
	3200 4350 3200 3250
Wire Wire Line
	3200 3250 1600 3250
Wire Wire Line
	1600 3250 1600 3350
$Comp
L PWR_FLAG #FLG013
U 1 1 59C1FB1E
P 2200 3550
F 0 "#FLG013" H 2200 3625 50  0001 C CNN
F 1 "PWR_FLAG" H 2200 3700 50  0000 C CNN
F 2 "" H 2200 3550 50  0001 C CNN
F 3 "" H 2200 3550 50  0001 C CNN
	1    2200 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 3750 1900 3650
Wire Wire Line
	2200 3750 2200 3550
Connection ~ 1900 3750
Wire Wire Line
	1100 4950 1600 4950
Connection ~ 1600 4950
$Comp
L STM32L073xx_LQFP100 U1
U 5 1 59C22F18
P 4450 3700
F 0 "U1" H 4150 4100 60  0000 L CNN
F 1 "STM32L073V8" H 4650 3300 60  0000 R CNN
F 2 "Housings_QFP:LQFP-100_14x14mm_Pitch0.5mm" H 4700 3450 60  0001 C CNN
F 3 "" H 4700 3450 60  0001 C CNN
	5    4450 3700
	1    0    0    -1  
$EndComp
$Comp
L Crystal Y1
U 1 1 59C2348A
P 5000 3700
F 0 "Y1" H 5000 3850 50  0000 C CNN
F 1 "32.768kHz 12.5pF" H 5000 3550 50  0000 C CNN
F 2 "" H 5000 3700 50  0001 C CNN
F 3 "" H 5000 3700 50  0001 C CNN
	1    5000 3700
	0    -1   1    0   
$EndComp
$Comp
L C C11
U 1 1 59C23559
P 5450 3450
F 0 "C11" H 5475 3550 50  0000 L CNN
F 1 "15p" H 5475 3350 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 5488 3300 50  0001 C CNN
F 3 "" H 5450 3450 50  0001 C CNN
	1    5450 3450
	0    -1   -1   0   
$EndComp
$Comp
L C C12
U 1 1 59C235C8
P 5450 3950
F 0 "C12" H 5475 4050 50  0000 L CNN
F 1 "15p" H 5475 3850 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 5488 3800 50  0001 C CNN
F 3 "" H 5450 3950 50  0001 C CNN
	1    5450 3950
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR014
U 1 1 59C23625
P 5700 4050
F 0 "#PWR014" H 5700 3800 50  0001 C CNN
F 1 "GND" H 5700 3900 50  0000 C CNN
F 2 "" H 5700 4050 50  0001 C CNN
F 3 "" H 5700 4050 50  0001 C CNN
	1    5700 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 3450 5700 4050
Wire Wire Line
	5700 3950 5600 3950
Wire Wire Line
	5700 3450 5600 3450
Connection ~ 5700 3950
Wire Wire Line
	4850 3450 5300 3450
Wire Wire Line
	5000 3450 5000 3550
Connection ~ 5000 3450
Wire Wire Line
	4850 3950 5300 3950
Wire Wire Line
	5000 3950 5000 3850
Connection ~ 5000 3950
$Comp
L ELV_DCF-2 U2
U 1 1 59C24F4D
P 1600 4350
F 0 "U2" H 1400 4750 60  0000 L CNN
F 1 "ELV_DCF-2" H 1600 3950 60  0000 L CNN
F 2 "" H 1600 4350 60  0001 C CNN
F 3 "" H 1600 4350 60  0001 C CNN
	1    1600 4350
	1    0    0    -1  
$EndComp
$EndSCHEMATC
