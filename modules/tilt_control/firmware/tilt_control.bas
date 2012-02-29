'/**
' * tilt_control.bas
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
MODULE_ADDRESS = $05

symbol OUT0 = b6
symbol OUT1 = b7
symbol OUTWORD0 = w3
symbol OUT2 = b8
symbol OUT3 = b9
symbol OUTWORD1 = w4

symbol UP = 1
symbol SIDE = 0

low 4

main:
	serin MOSI, T2400_4, ("#", $01, "+"), DESTINATION_ADDRESS, COMMAND, ALTITUDE_LOW, ALTITUDE_HIGH

	if DESTINATION_ADDRESS = MODULE_ADDRESS then
		gosub reply
	endif
	
	gosub incoming
	
	goto main

incoming:
	
    if ALTITUDE < 24000 then gosub rotateToSide
    if ALTITUDE > 25000 then gosub rotateToUp
   
    return

rotateToSide:
    
    low 4
    return

rotateToUp:
    
    high 4
    return

reply:
	high MISO
	pause 25
	serout MISO, T2400_4, ($00, "#", MODULE_ADDRESS, "+", $00, $00, $00, $00)
	return
