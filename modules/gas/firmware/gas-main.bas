'/**
' * gas-main.bas
' *
' * Part of the Apex III project
' * http://www.apexhab.org/apex-iii/
' *
' * Philip Warren & Priyesh Patel
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
' Change this module address to the assigned address
MODULE_ADDRESS = $07

symbol OUT0 = b6
symbol OUT1 = b7
symbol OUTWORD0 = w3
symbol OUT2 = b8
symbol OUT3 = b9
symbol OUTWORD1 = w4

' << Custom symbols >>
' << End >>

main:
	serin MOSI, T2400_4, ("#", $01, "+"), DESTINATION_ADDRESS, COMMAND, ALTITUDE_LOW, ALTITUDE_HIGH

	if DESTINATION_ADDRESS = MODULE_ADDRESS then
		gosub reply
	elseif DESTINATION_ADDRESS = $00 then
		gosub reading
	endif

	gosub incoming

	'debug

	goto main

' << Custom subs >>

' << End >>

reading:


	'The baud rate is fixed at 4800,n,8,1 

	serin [300], c.3, T2400_4,  ("G"), OUT0
	readadc c.4,OUT1
	
	
	' << Take a reading from the sensor >>
	' << Place data in OUT0 to OUT3 >>

	' << End >>
	return

incoming:
	' << Deal with incoming data from the core >>
	' << End >>
	return

reply:
	high MISO
	pause 25
	serout MISO, T2400_4, ($00, "#", MODULE_ADDRESS, "+", OUT0, OUT1, OUT2, OUT3)
	return
