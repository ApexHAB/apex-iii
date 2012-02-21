'/**
' * adc_lemon_slave.bas
' *
' * Part of the Apex III project
' * http://www.apexhab.org/apex-iii/
' *
' * Daniel Saul
' *
' * (c) Copyright ApexHAB 2011
' * team@apexhab.org
' */

symbol ADC2 = b0
symbol TEMP1 = b1
symbol TEMP2 = b2

main:

    serin 0,T2400_4,("JUICY")
    readadc 4,ADC2
    ADC2 = ADC2 / 255 * 330

    ser


