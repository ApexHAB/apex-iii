'/**
' * light.bas
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

symbol MODULE_ADDRESS_A = b4
' Change this module address to the assigned address
MODULE_ADDRESS = $03

symbol MODULE_ADDRESS_B = b5
' Change this module address to the assigned address
MODULE_ADDRESS = $0A

symbol OUT0 = b6
symbol OUT1 = b7
symbol OUTWORD0 = w3 ' R
symbol OUT2 = b8
symbol OUT3 = b9
symbol OUTWORD1 = w4 ' G
symbol OUT4 = b10
symbol OUT5 = b11
symbol OUTWORD0 = w5 ' B
symbol OUT6 = b12
symbol OUT7 = b13
symbol OUTWORD1 = w6 ' Normal

' << Custom symbols >>
' << End >>

main:
	serin MOSI, T2400_4, ("#", $01, "+"), DESTINATION_ADDRESS, COMMAND, ALTITUDE_LOW, ALTITUDE_HIGH

	if DESTINATION_ADDRESS = MODULE_ADDRESS_A then
		gosub reply_a
	elseif DESTINATION_ADDRESS = MODULE_ADDRESS_B then
		gosub reply_b
	elseif DESTINATION_ADDRESS = $00 then
		gosub reading
	endif
	
	gosub incoming
	
	'debug
	
	goto main

' << Custom subs >>

' << End >>

reading:
	' << Take a reading from the sensor >>
	' << Place data in OUT0 to OUT3 >>

	' << End >>
	return

incoming:
	' << Deal with incoming data from the core >>
	' << End >>
	return

reply_a:
	high MISO
	pause 25
	serout MISO, T2400_4, ($00, "#", MODULE_ADDRESS, "+", OUT0, OUT1, OUT2, OUT3)
	return

reply_b:
	high MISO
	pause 25
	serout MISO, T2400_4, ($00, "#", MODULE_ADDRESS, "+", OUT4, OUT5, OUT6, OUT7)
	return