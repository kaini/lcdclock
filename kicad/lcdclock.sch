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
Date ""
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
P 3400 2350
F 0 "#PWR01" H 3400 2100 50  0001 C CNN
F 1 "GND" H 3400 2200 50  0000 C CNN
F 2 "" H 3400 2350 50  0001 C CNN
F 3 "" H 3400 2350 50  0001 C CNN
	1    3400 2350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 59B9D9D7
P 1250 7250
F 0 "#PWR02" H 1250 7000 50  0001 C CNN
F 1 "GND" H 1250 7100 50  0000 C CNN
F 2 "" H 1250 7250 50  0001 C CNN
F 3 "" H 1250 7250 50  0001 C CNN
	1    1250 7250
	1    0    0    -1  
$EndComp
$Comp
L L_Core_Ferrite L1
U 1 1 59B9F09D
P 1250 5700
F 0 "L1" H 1200 5700 50  0000 C CNN
F 1 "470u" V 1360 5700 50  0000 C CNN
F 2 "Resistors_SMD:R_1812_HandSoldering" H 1250 5700 50  0001 C CNN
F 3 "" H 1250 5700 50  0001 C CNN
	1    1250 5700
	1    0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 59B9F416
P 1000 5950
F 0 "C5" H 1025 6050 50  0000 L CNN
F 1 "100u" H 1025 5850 50  0000 L CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" H 1038 5800 50  0001 C CNN
F 3 "" H 1000 5950 50  0001 C CNN
	1    1000 5950
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1650 6550 1850 6550
Wire Wire Line
	1250 5850 1250 6050
Wire Wire Line
	1150 5950 1850 5950
Connection ~ 1250 5950
Wire Wire Line
	850  5950 750  5950
Wire Wire Line
	750  5950 750  7150
Wire Wire Line
	1250 7050 1250 7250
$Comp
L C C6
U 1 1 59C01DE2
P 5100 1900
F 0 "C6" H 5125 2000 50  0000 L CNN
F 1 "100n" H 5125 1800 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 5138 1750 50  0001 C CNN
F 3 "" H 5100 1900 50  0001 C CNN
	1    5100 1900
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 59C01E75
P 4850 1900
F 0 "C4" H 4875 2000 50  0000 L CNN
F 1 "100n" H 4875 1800 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 4888 1750 50  0001 C CNN
F 3 "" H 4850 1900 50  0001 C CNN
	1    4850 1900
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 59C01EB0
P 4600 1900
F 0 "C3" H 4625 2000 50  0000 L CNN
F 1 "100n" H 4625 1800 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 4638 1750 50  0001 C CNN
F 3 "" H 4600 1900 50  0001 C CNN
	1    4600 1900
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 59C01F4C
P 4350 1900
F 0 "C1" H 4375 2000 50  0000 L CNN
F 1 "10u" H 4375 1800 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 4388 1750 50  0001 C CNN
F 3 "" H 4350 1900 50  0001 C CNN
	1    4350 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 2950 5100 2050
Wire Wire Line
	4850 2950 4850 2050
Wire Wire Line
	4600 2950 4600 2050
Wire Wire Line
	5100 1450 5100 1750
Wire Wire Line
	4850 1450 4850 1750
Wire Wire Line
	4600 1450 4600 1750
Wire Wire Line
	4350 1350 4350 1750
$Comp
L C C8
U 1 1 59C025EE
P 6500 1900
F 0 "C8" H 6525 2000 50  0000 L CNN
F 1 "100n" H 6525 1800 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 6538 1750 50  0001 C CNN
F 3 "" H 6500 1900 50  0001 C CNN
	1    6500 1900
	1    0    0    -1  
$EndComp
$Comp
L C C9
U 1 1 59C02751
P 6750 1900
F 0 "C9" H 6775 2000 50  0000 L CNN
F 1 "1u" H 6775 1800 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 6788 1750 50  0001 C CNN
F 3 "" H 6750 1900 50  0001 C CNN
	1    6750 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 1450 6500 1750
Wire Wire Line
	6500 2950 6500 2050
Wire Wire Line
	6750 2050 6750 3050
Connection ~ 6500 2950
Wire Wire Line
	6750 1450 6750 1750
Connection ~ 6500 1450
Connection ~ 4600 2950
Connection ~ 4850 2950
Connection ~ 5100 2950
Wire Wire Line
	5550 2950 5550 2850
Connection ~ 5550 2950
Wire Wire Line
	5650 2950 5650 2850
Connection ~ 5650 2950
Wire Wire Line
	5750 2850 5750 3050
Connection ~ 5750 2950
Wire Wire Line
	5750 1450 5750 1550
Wire Wire Line
	5650 1450 5650 1550
Connection ~ 5650 1450
Wire Wire Line
	5550 1450 5550 1550
Connection ~ 5550 1450
Connection ~ 5100 1450
Connection ~ 4850 1450
Connection ~ 4600 1450
Connection ~ 4350 1450
Wire Wire Line
	4350 1450 5750 1450
$Comp
L GND #PWR03
U 1 1 59C035FC
P 5750 3050
F 0 "#PWR03" H 5750 2800 50  0001 C CNN
F 1 "GND" H 5750 2900 50  0000 C CNN
F 2 "" H 5750 3050 50  0001 C CNN
F 3 "" H 5750 3050 50  0001 C CNN
	1    5750 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 1450 6750 1450
Wire Wire Line
	5950 2950 6750 2950
Wire Wire Line
	4350 2950 4350 2050
$Comp
L GND #PWR04
U 1 1 59C038C1
P 6750 3050
F 0 "#PWR04" H 6750 2800 50  0001 C CNN
F 1 "GND" H 6750 2900 50  0000 C CNN
F 2 "" H 6750 3050 50  0001 C CNN
F 3 "" H 6750 3050 50  0001 C CNN
	1    6750 3050
	1    0    0    -1  
$EndComp
Connection ~ 6750 2950
$Comp
L C C7
U 1 1 59C03A8D
P 6350 1050
F 0 "C7" H 6375 1150 50  0000 L CNN
F 1 "100n" H 6375 950 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 6388 900 50  0001 C CNN
F 3 "" H 6350 1050 50  0001 C CNN
	1    6350 1050
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR05
U 1 1 59C03B38
P 6600 1150
F 0 "#PWR05" H 6600 900 50  0001 C CNN
F 1 "GND" H 6600 1000 50  0000 C CNN
F 2 "" H 6600 1150 50  0001 C CNN
F 3 "" H 6600 1150 50  0001 C CNN
	1    6600 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 1150 6600 1050
Wire Wire Line
	6600 1050 6500 1050
Wire Wire Line
	6200 1050 6100 1050
$Comp
L SW_Push SW1
U 1 1 59C114FC
P 4550 5650
F 0 "SW1" H 4600 5750 50  0000 L CNN
F 1 "SW_Push" H 4550 5590 50  0000 C CNN
F 2 "Buttons_Switches_SMD:SW_SPST_B3S-1000" H 4550 5850 50  0001 C CNN
F 3 "" H 4550 5850 50  0001 C CNN
	1    4550 5650
	0    1    1    0   
$EndComp
Wire Wire Line
	4550 5450 4550 5350
Wire Wire Line
	4450 6150 5200 6150
Wire Wire Line
	4450 6250 5200 6250
Wire Wire Line
	4450 5950 4800 5950
Wire Wire Line
	4550 5850 4550 6550
$Comp
L Conn_JLink_SWD J1
U 1 1 59C131D8
P 5750 6250
F 0 "J1" H 5400 6800 50  0000 L CNN
F 1 "Conn_JLink_SWD" H 6100 5600 50  0000 R CNN
F 2 "Connectors:IDC_Header_Straight_20pins" H 5750 6250 50  0001 C CNN
F 3 "" H 5750 6250 50  0001 C CNN
	1    5750 6250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 59C146AA
P 6400 6850
F 0 "#PWR06" H 6400 6600 50  0001 C CNN
F 1 "GND" H 6400 6700 50  0000 C CNN
F 2 "" H 6400 6850 50  0001 C CNN
F 3 "" H 6400 6850 50  0001 C CNN
	1    6400 6850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 5350 6400 6850
Wire Wire Line
	6400 6750 6300 6750
Wire Wire Line
	6400 6650 6300 6650
Connection ~ 6400 6750
Wire Wire Line
	6300 6550 6400 6550
Connection ~ 6400 6650
Wire Wire Line
	6300 6450 6400 6450
Connection ~ 6400 6550
Wire Wire Line
	6300 6350 6400 6350
Connection ~ 6400 6450
Wire Wire Line
	6300 6250 6400 6250
Connection ~ 6400 6350
Wire Wire Line
	6300 6150 6400 6150
Connection ~ 6400 6250
Wire Wire Line
	6300 6050 6400 6050
Connection ~ 6400 6150
Wire Wire Line
	6300 5950 6400 5950
Connection ~ 6400 6050
NoConn ~ 6300 5850
NoConn ~ 5200 6750
NoConn ~ 5200 6650
NoConn ~ 5200 6350
NoConn ~ 5200 6050
NoConn ~ 5200 5950
NoConn ~ 5200 6450
Wire Wire Line
	4550 5350 6400 5350
Connection ~ 6400 5950
Wire Wire Line
	4550 6550 5200 6550
Connection ~ 4550 5950
$Comp
L C C10
U 1 1 59C17FCD
P 4800 5650
F 0 "C10" H 4825 5750 50  0000 L CNN
F 1 "100n" H 4825 5550 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 4838 5500 50  0001 C CNN
F 3 "" H 4800 5650 50  0001 C CNN
	1    4800 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 5950 4800 5800
Wire Wire Line
	4800 5500 4800 5350
Connection ~ 4800 5350
Wire Wire Line
	5100 5750 5100 5850
Wire Wire Line
	5100 5850 5200 5850
Wire Wire Line
	2750 6550 2850 6550
Wire Wire Line
	2850 6550 2850 5450
Wire Wire Line
	2850 5450 1250 5450
Wire Wire Line
	1250 5450 1250 5550
$Comp
L PWR_FLAG #FLG07
U 1 1 59C1FB1E
P 1850 5750
F 0 "#FLG07" H 1850 5825 50  0001 C CNN
F 1 "PWR_FLAG" H 1850 5900 50  0000 C CNN
F 2 "" H 1850 5750 50  0001 C CNN
F 3 "" H 1850 5750 50  0001 C CNN
	1    1850 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 5950 1850 5750
Wire Wire Line
	750  7150 1250 7150
Connection ~ 1250 7150
$Comp
L Crystal Y1
U 1 1 59C2348A
P 2050 3950
F 0 "Y1" H 2050 4100 50  0000 C CNN
F 1 "32.768kHz 12.5pF" H 2050 3800 50  0000 C CNN
F 2 "Crystals:Crystal_SMD_SeikoEpson_MC405-2pin_9.6x4.1mm" H 2050 3950 50  0001 C CNN
F 3 "" H 2050 3950 50  0001 C CNN
	1    2050 3950
	0    -1   1    0   
$EndComp
$Comp
L C C11
U 1 1 59C23559
P 2500 3700
F 0 "C11" H 2525 3800 50  0000 L CNN
F 1 "15p" H 2525 3600 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 2538 3550 50  0001 C CNN
F 3 "" H 2500 3700 50  0001 C CNN
	1    2500 3700
	0    -1   -1   0   
$EndComp
$Comp
L C C12
U 1 1 59C235C8
P 2500 4200
F 0 "C12" H 2525 4300 50  0000 L CNN
F 1 "15p" H 2525 4100 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 2538 4050 50  0001 C CNN
F 3 "" H 2500 4200 50  0001 C CNN
	1    2500 4200
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR08
U 1 1 59C23625
P 2750 4300
F 0 "#PWR08" H 2750 4050 50  0001 C CNN
F 1 "GND" H 2750 4150 50  0000 C CNN
F 2 "" H 2750 4300 50  0001 C CNN
F 3 "" H 2750 4300 50  0001 C CNN
	1    2750 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 3700 2750 4300
Wire Wire Line
	2750 4200 2650 4200
Wire Wire Line
	2750 3700 2650 3700
Connection ~ 2750 4200
Wire Wire Line
	1900 3700 2350 3700
Wire Wire Line
	2050 3700 2050 3800
Connection ~ 2050 3700
Wire Wire Line
	1900 4200 2350 4200
Wire Wire Line
	2050 4200 2050 4100
Connection ~ 2050 4200
$Comp
L ELV_DCF-2 U2
U 1 1 59C24F4D
P 1250 6550
F 0 "U2" H 1050 6900 60  0000 L CNN
F 1 "ELV_DCF-2" H 1250 6200 60  0000 L CNN
F 2 "LCD Clock:CONNECTOR_3x1" H 1250 6550 60  0001 C CNN
F 3 "" H 1250 6550 60  0001 C CNN
	1    1250 6550
	1    0    0    -1  
$EndComp
$Comp
L TPS706xx U3
U 1 1 59C2A669
P 2600 1700
F 0 "U3" H 2350 2000 60  0000 L CNN
F 1 "TPS70630" H 2600 1400 60  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5_HandSoldering" H 2600 1700 60  0001 C CNN
F 3 "" H 2600 1700 60  0001 C CNN
	1    2600 1700
	1    0    0    -1  
$EndComp
$Comp
L C C14
U 1 1 59C2C059
P 3400 1950
F 0 "C14" H 3425 2050 50  0000 L CNN
F 1 "1u" H 3425 1850 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 3438 1800 50  0001 C CNN
F 3 "" H 3400 1950 50  0001 C CNN
	1    3400 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 1150 2600 1250
Wire Wire Line
	2600 2250 2600 2150
Wire Wire Line
	3050 1700 3650 1700
Wire Wire Line
	3150 1700 3150 1800
Wire Wire Line
	3150 2250 3150 2100
Connection ~ 2600 2250
$Comp
L C C13
U 1 1 59C2C8A9
P 3150 1950
F 0 "C13" H 3175 2050 50  0000 L CNN
F 1 "2.2u" H 3175 1850 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 3188 1800 50  0001 C CNN
F 3 "" H 3150 1950 50  0001 C CNN
	1    3150 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 1050 3400 1800
Connection ~ 2600 1150
Wire Wire Line
	3400 2100 3400 2350
Connection ~ 3150 2250
Wire Wire Line
	1800 1150 3400 1150
Wire Wire Line
	850  2250 3400 2250
Wire Wire Line
	2150 1700 2050 1700
Wire Wire Line
	2050 1700 2050 1150
Connection ~ 2050 1150
Connection ~ 3150 1700
$Comp
L VCC #PWR09
U 1 1 59C304BD
P 3650 1700
F 0 "#PWR09" H 3650 1550 50  0001 C CNN
F 1 "VCC" H 3650 1850 50  0000 C CNN
F 2 "" H 3650 1700 50  0001 C CNN
F 3 "" H 3650 1700 50  0001 C CNN
	1    3650 1700
	0    1    1    0   
$EndComp
$Comp
L VCC #PWR010
U 1 1 59C306DD
P 4350 1350
F 0 "#PWR010" H 4350 1200 50  0001 C CNN
F 1 "VCC" H 4350 1500 50  0000 C CNN
F 2 "" H 4350 1350 50  0001 C CNN
F 3 "" H 4350 1350 50  0001 C CNN
	1    4350 1350
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR011
U 1 1 59C3072B
P 5950 1350
F 0 "#PWR011" H 5950 1200 50  0001 C CNN
F 1 "VCC" H 5950 1500 50  0000 C CNN
F 2 "" H 5950 1350 50  0001 C CNN
F 3 "" H 5950 1350 50  0001 C CNN
	1    5950 1350
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR013
U 1 1 59C30C48
P 5100 5750
F 0 "#PWR013" H 5100 5600 50  0001 C CNN
F 1 "VCC" H 5100 5900 50  0000 C CNN
F 2 "" H 5100 5750 50  0001 C CNN
F 3 "" H 5100 5750 50  0001 C CNN
	1    5100 5750
	1    0    0    -1  
$EndComp
Connection ~ 3400 2250
$Comp
L LCD U4
U 1 1 59C35B16
P 10150 2050
F 0 "U4" H 9700 2950 60  0000 L CNN
F 1 "LCD" H 10750 1050 60  0000 R CNN
F 2 "Connectors:IDC_Header_Straight_16pins" H 9900 3800 60  0001 C CNN
F 3 "" H 9900 3800 60  0001 C CNN
	1    10150 2050
	1    0    0    -1  
$EndComp
$Comp
L Battery BT1
U 1 1 59C366BC
P 850 1700
F 0 "BT1" H 950 1800 50  0000 L CNN
F 1 "4xAAA 6.4-3.2V" H 950 1700 50  0000 L CNN
F 2 "LCD Clock:CONNECTOR_2x1" V 850 1760 50  0001 C CNN
F 3 "" V 850 1760 50  0001 C CNN
	1    850  1700
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG014
U 1 1 59C37520
P 3400 1050
F 0 "#FLG014" H 3400 1125 50  0001 C CNN
F 1 "PWR_FLAG" H 3400 1200 50  0000 C CNN
F 2 "" H 3400 1050 50  0001 C CNN
F 3 "" H 3400 1050 50  0001 C CNN
	1    3400 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	850  1900 850  2350
$Comp
L PWR_FLAG #FLG015
U 1 1 59C3864E
P 850 2350
F 0 "#FLG015" H 850 2425 50  0001 C CNN
F 1 "PWR_FLAG" H 850 2500 50  0000 C CNN
F 2 "" H 850 2350 50  0001 C CNN
F 3 "" H 850 2350 50  0001 C CNN
	1    850  2350
	-1   0    0    1   
$EndComp
Connection ~ 850  2250
$Comp
L STM32L073xx_48 U1
U 1 1 59C3F145
P 5950 2200
F 0 "U1" V 5400 2650 60  0000 R CNN
F 1 "STM32L073xx_48" V 6250 1850 60  0000 L CNN
F 2 "Housings_QFP:LQFP-48_7x7mm_Pitch0.5mm" H 6200 1950 60  0001 C CNN
F 3 "" H 6200 1950 60  0001 C CNN
	1    5950 2200
	1    0    0    -1  
$EndComp
$Comp
L STM32L073xx_48 U1
U 2 1 59C3F1AE
P 8350 2400
F 0 "U1" H 7850 3650 60  0000 L CNN
F 1 "STM32L073xx_48" H 8400 950 60  0000 R CNN
F 2 "Housings_QFP:LQFP-48_7x7mm_Pitch0.5mm" H 8600 2150 60  0001 C CNN
F 3 "" H 8600 2150 60  0001 C CNN
	2    8350 2400
	1    0    0    -1  
$EndComp
$Comp
L STM32L073xx_48 U1
U 3 1 59C3F229
P 4250 6100
F 0 "U1" H 3900 6400 60  0000 L CNN
F 1 "STM32L073xx_48" H 4250 5800 60  0000 R CNN
F 2 "Housings_QFP:LQFP-48_7x7mm_Pitch0.5mm" H 4500 5850 60  0001 C CNN
F 3 "" H 4500 5850 60  0001 C CNN
	3    4250 6100
	1    0    0    -1  
$EndComp
$Comp
L STM32L073xx_48 U1
U 4 1 59C3F29A
P 2300 6550
F 0 "U1" H 2050 6700 60  0000 L CNN
F 1 "STM32L073xx_48" H 2550 6400 60  0000 R CNN
F 2 "Housings_QFP:LQFP-48_7x7mm_Pitch0.5mm" H 2550 6300 60  0001 C CNN
F 3 "" H 2550 6300 60  0001 C CNN
	4    2300 6550
	1    0    0    -1  
$EndComp
$Comp
L STM32L073xx_48 U1
U 5 1 59C3F32D
P 1500 3950
F 0 "U1" H 1200 4350 60  0000 L CNN
F 1 "STM32L073xx_48" H 1700 3550 60  0000 R CNN
F 2 "Housings_QFP:LQFP-48_7x7mm_Pitch0.5mm" H 1750 3700 60  0001 C CNN
F 3 "" H 1750 3700 60  0001 C CNN
	5    1500 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 2950 5750 2950
Wire Wire Line
	5950 2950 5950 2850
Wire Wire Line
	5950 1350 5950 1550
Connection ~ 5950 1450
Wire Wire Line
	8600 1800 9500 1800
Wire Wire Line
	8600 1900 9500 1900
Wire Wire Line
	9500 2000 8600 2000
Wire Wire Line
	8600 2100 9500 2100
Wire Wire Line
	9500 2200 8600 2200
Wire Wire Line
	8600 2300 9500 2300
Wire Wire Line
	9500 2400 8600 2400
Wire Wire Line
	8600 2500 9500 2500
Wire Wire Line
	9500 2600 8600 2600
Wire Wire Line
	8600 2700 9500 2700
Wire Wire Line
	9500 2800 8600 2800
Wire Wire Line
	8600 2900 9500 2900
NoConn ~ 8600 3000
NoConn ~ 8600 3100
NoConn ~ 8600 3200
NoConn ~ 8600 3300
NoConn ~ 8600 3400
NoConn ~ 8600 3500
Wire Wire Line
	8600 1600 9500 1600
Wire Wire Line
	8600 1400 9500 1400
Wire Wire Line
	8600 1300 9100 1300
Wire Wire Line
	9400 1500 9500 1500
Wire Wire Line
	9100 1300 9400 1500
Wire Wire Line
	8600 1500 9050 1500
Wire Wire Line
	9050 1500 9250 1300
Wire Wire Line
	9250 1300 9500 1300
$Comp
L Q_PMOS_GSD Q1
U 1 1 59C3A539
P 1600 1250
F 0 "Q1" H 1800 1300 50  0000 L CNN
F 1 "BSH205" H 1800 1200 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 1800 1350 50  0001 C CNN
F 3 "" H 1600 1250 50  0001 C CNN
	1    1600 1250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1400 1150 850  1150
Connection ~ 1600 2250
Connection ~ 3400 1150
Wire Wire Line
	850  1150 850  1500
$Comp
L R R2
U 1 1 59C3B737
P 1600 1850
F 0 "R2" V 1680 1850 50  0000 C CNN
F 1 "100k" V 1600 1850 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 1530 1850 50  0001 C CNN
F 3 "" H 1600 1850 50  0001 C CNN
	1    1600 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 1700 1600 1450
Wire Wire Line
	1600 2000 1600 2250
Wire Wire Line
	5250 2200 5250 2950
Connection ~ 5250 2950
$Comp
L GND #PWR016
U 1 1 59C46279
P 10500 3450
F 0 "#PWR016" H 10500 3200 50  0001 C CNN
F 1 "GND" H 10500 3300 50  0000 C CNN
F 2 "" H 10500 3450 50  0001 C CNN
F 3 "" H 10500 3450 50  0001 C CNN
	1    10500 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	10500 3350 10500 3450
Wire Wire Line
	10400 3350 10500 3350
$Comp
L LED D1
U 1 1 59C4572E
P 10250 3350
F 0 "D1" H 10250 3450 50  0000 C CNN
F 1 "LED" H 10250 3250 50  0000 C CNN
F 2 "LCD Clock:CONNECTOR_2x1" H 10250 3350 50  0001 C CNN
F 3 "" H 10250 3350 50  0001 C CNN
	1    10250 3350
	-1   0    0    1   
$EndComp
$Comp
L R R1
U 1 1 59C46694
P 9850 3350
F 0 "R1" V 9930 3350 50  0000 C CNN
F 1 "?" V 9850 3350 50  0001 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 9780 3350 50  0001 C CNN
F 3 "" H 9850 3350 50  0001 C CNN
	1    9850 3350
	0    1    1    0   
$EndComp
Wire Wire Line
	10100 3350 10000 3350
Wire Wire Line
	9200 3350 9700 3350
Wire Wire Line
	9200 3700 9200 3350
Wire Wire Line
	8600 3700 9200 3700
Text Notes 8600 3900 0    60   ~ 0
Backlight\nDo not connect if not needed.
Wire Wire Line
	6100 1050 6100 1550
$EndSCHEMATC
