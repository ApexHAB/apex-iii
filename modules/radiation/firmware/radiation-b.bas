'/**
' * adc_lemon.bas
' *
' * Part of the Apex III project
' * http://www.apexhab.org/apex-iii/
' *
' * Daniel Saul & Priyesh Patel
' *
' * (c) Copyright ApexHAB 2011
' * team@apexhab.org
' */

symbol MOSI_PIN = pin2
symbol MISO_PIN = pin1

symbol MOSI = 2
symbol MISO = 1

symbol DESTINATION_ADDRESS = b0
symbol COMMAND = b1
symbol ALTITUDE_LOW = b2
symbol ALTITUDE_HIGH = b3
symbol ALTITUDE = w1

symbol MODULE_ADDRESS = b4
MODULE_ADDRESS = $08

symbol OUT0 = b6
symbol OUT1 = b7
symbol OUTWORD0 = w3
symbol OUT2 = b8
symbol OUT3 = b9
symbol OUTWORD1 = w4



main:
	
    count 4,20000,OUTWORD0
    OUTWORD0 = OUTWORD0 * 3
    
    serin MOSI, T2400_4, ("#", $01, "+", "MODULE_ADDRESS"), COMMAND, ALTITUDE_LOW, ALTITUDE_HIGH

	gosub reply
	
	goto main

reply:
	high MISO
	pause 25
	serout MISO, T2400_4, ($00, "#", MODULE_ADDRESS, "+", OUT0, OUT1, $00, $00)
	return
