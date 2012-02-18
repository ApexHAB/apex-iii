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

symbol CUTDOWN_CYCLES = b10
CUTDOWN_CYCLES = 0

symbol ALTITUDE_EEPROM = 0
symbol ALTITUDE_STORED = b11
' << End >>

main:
	serin MOSI, T2400_4, ("#", $01, "+"), DESTINATION_ADDRESS, COMMAND, ALTITUDE_LOW, ALTITUDE_HIGH

	if DESTINATION_ADDRESS = MODULE_ADDRESS then
		gosub reply
	elseif DESTINATION_ADDRESS = $00 then
		gosub reading
	endif
	
	gosub incoming
	
	debug
	
	goto main

' << Custom subs >>

' << End >>

reading:
	' << Take a reading from the sensor >>
	OUT0 = $00
	OUT1 = $00
	OUT2 = $00
	OUT3 = $00
	' << End >>
	return

incoming:
	' << Deal with incoming data from the core >>
	if ALTITUDE > 3000 then
		read ALTITUDE_EEPROM, ALTITUDE_STORED
		if ALTITUDE_STORED = $00 then
			ALTITUDE_STORED = $FF
			write ALTITUDE_EEPROM, ALTITUDE_STORED
		endif
		low SIREN
	else
		read ALTITUDE_EEPROM, ALTITUDE_STORED
		if ALTITUDE_STORED = $FF then
			high SIREN
		else
			low SIREN
		endif
	endif
	
	if COMMAND = $05 then
		ALTITUDE_STORED = $00
		write ALTITUDE_EEPROM, ALTITUDE_STORED
		low SIREN
	endif
	
	if COMMAND = $03 then
		low SIREN
	elseif COMMAND = $04 then
		high SIREN
	endif

	if COMMAND = $02 then
		CUTDOWN_CYCLES = 2
	endif
	
	if CUTDOWN_CYCLES > 0 then
		high CUTDOWN
		if DESTINATION_ADDRESS = $00 then
			CUTDOWN_CYCLES = CUTDOWN_CYCLES - 1
		endif
	else
		low CUTDOWN
	end if	
	' << End >>
	return

reply:
	high MISO
	pause 25
	serout MISO, T2400_4, ($00, "#", MODULE_ADDRESS, "+", OUT0, OUT1, OUT2, OUT3)
	return