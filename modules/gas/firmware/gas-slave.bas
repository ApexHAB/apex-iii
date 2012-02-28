'/**
' * gas-slave.bas
' *
' * Part of the Apex III project
' * http://www.apexhab.org/apex-iii/
' *
' * Philip Warren
' *
' * (c) Copyright ApexHAB 2011
' * team@apexhab.org
' */

main:
	readadc c.2,b0
	disconnect
	serout c.0, T2400_4, ("G", b0)
	reconnect
	pause 100
    goto main
