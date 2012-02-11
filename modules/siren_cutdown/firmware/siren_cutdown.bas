'/**
' * siren_cutdown.bas
' *
' * Part of the Apex III project
' * http://www.apexhab.org/apex-iii/
' *
' * Priyesh Patel
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
MODULE_ADDRESS = $02

symbol OUT0 = b6
symbol OUT1 = b7
symbol OUTBYTE0 = w3
symbol OUT2 = b8
symbol OUT3 = b9
symbol OUTBYTE1 = w4

' << Custom symbols >>
symbol CUTDOWN = 0
symbol SIREN = 4
' << End >>

main:
	serin MOSI, T2400_4, ("#", $01, "+"), b0, b1, b2, b3


	if DESTINATION_ADDRESS = MODULE_ADDRESS then
		gosub reply
	else
		gosub reading
	endif
	
	gosub incoming
	
	debug
	
	goto main

' << Custom subs >>

' << End >>

reading:
	' << Take a reading from the sensor >>
	OUT0 = $AB
	OUT1 = $CD
	OUT2 = $EF
	OUT3 = $12
	' << End >>
	return

incoming:
	' << Deal with incoming data from the core >>
	if ALTITUDE < 3000 then
		high SIREN
	else
		low SIREN
	endif
	
	if COMMAND = $02 then
		high CUTDOWN
	else
		low CUTDOWN
	endif 
	' << End >>
	return

reply:
	high MISO
	pause 25
	serout MISO, T2400_4, ($00, "#", MODULE_ADDRESS, "+", OUT0, OUT1, OUT2, OUT3)
	return