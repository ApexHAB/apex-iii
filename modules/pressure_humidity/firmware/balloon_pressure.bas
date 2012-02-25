'/**
' * balloon_pressure.bas
' *
' * Part of the Apex III project
' * http://www.apexhab.org/apex-iii/
' *
' * Philip Warren
' *
' * (c) Copyright ApexHAB 2011
' * team@apexhab.org
' */

init:
	disconnect

main:
	readadc c.4, b0
	b1 = b0*200/255
	serout c.0, T2400_4, ("P", b1)
	pause 75
	goto main