'/**
' * tilt_control_slave.bas
' *
' * Part of the Apex III project
' * http://www.apexhab.org/apex-iii/
' *
' * Daniel Saul
' *
' * (c) Copyright ApexHAB 2011
' * team@apexhab.org
' */

symbol POTPOSITION = b0
symbol COMMAND = b1
symbol ORIENTATION = b2

symbol UP = 1
symbol SIDE = 0

SYMBOL POTUP = 200
SYMBOL POTSIDE = 100

ORIENTATION = SIDE

SYMBOL PWMPIN = 2
SYMBOL DIRPIN = 0
SYMBOL PERIOD = 1000
SYMBOL DUTY = w3
low PWMPIN
low DIRPIN

main:
    serin 4,T2400_4,("TILT"),COMMAND

    if COMMAND = UP AND ORIENTATION = SIDE then gosub rotateToUp
    if COMMAND = SIDE AND ORIENTATION = UP then gosub rotateToSide

    goto main

rotateToUp:
    
    readadc 1,POTPOSITION
    if POTPOSITION > POTUP then
        ORIENTATION = UP
        return
    endif

    high DIRPIN

    DO UNTIL POTPOSITION > POTUP
        readadc 1,POTPOSITION
        b3 = 0 - 155 - POTPOSITION
        b3 = b3 * 4
        DUTY = POTPOSITION max 400
        pwmout PWMPIN, 100, DUTY
    LOOP

    low DIRPIN
    pwmout PWMPIN, OFF

    ORIENTATION = UP
    return
    
rotateToSide:
    
    readadc 1,POTPOSITION
    if POTPOSITION < POTSIDE then
        ORIENTATION = SIDE
        return
    endif

    low DIRPIN

    DO UNTIL POTPOSITION < POTSIDE
        readadc 1,POTPOSITION
        b3 = POTPOSITION - 100
        b3 = b3 * 4
        DUTY = POTPOSITION max 400
        pwmout PWMPIN, 100, DUTY
    LOOP

    low DIRPIN
    pwmout PWMPIN, OFF

    ORIENTATION = SIDE
    return
    


