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
MODULE_ADDRESS = $06

symbol OUT0 = b6
symbol OUT1 = b7
symbol OUTWORD0 = w3
symbol OUT2 = b8
symbol OUT3 = b9
symbol OUTWORD1 = w4

symbol ADC1 = b10


main:
	serin MOSI, T2400_4, ("#", $01, "+"), DESTINATION_ADDRESS, COMMAND, ALTITUDE_LOW, ALTITUDE_HIGH

	if DESTINATION_ADDRESS = MODULE_ADDRESS then
		gosub reply
    elseif DESTINATION_ADDRESS = $00 then
        gosub reading
	endif
	
	gosub incoming
	
	goto main

reading:

    readadc 4,ADC1
    OUT0 = ADC1 / 255 * 330
    serout 3,T2400_4,("JUICY")
    serin 3,T2400_4,("SQUEEZY",OUT1,OUT2,OUT3)

incoming:
	
    return

reply:
	high MISO
	pause 25
	serout MISO, T2400_4, ($00, "#", MODULE_ADDRESS, "+", OUT0, OUT1, OUT2, OUT3)
	return
