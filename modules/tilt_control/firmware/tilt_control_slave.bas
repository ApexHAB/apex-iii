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

SYMBOL POTUP = 42
SYMBOL POTSIDE = 105

ORIENTATION = SIDE

SYMBOL PWMPIN = 2
SYMBOL DIRPIN = 0
SYMBOL PERIOD = 1000
SYMBOL DUTY = w3
low PWMPIN
low DIRPIN

main:
  
    if pin4 = 1 then gosub rotateToUp
    if pin4 = 0 then gosub rotateToSide
  
    
    goto main

rotateToUp:
    
    readadc 1,POTPOSITION
    if POTPOSITION < POTUP then
        ORIENTATION = UP
        return
    endif

    high DIRPIN

    DO UNTIL POTPOSITION < POTUP
        readadc 1,POTPOSITION
        DUTY = POTPOSITION
        DUTY = DUTY * 2 max 400
        pwmout PWMPIN, 100, DUTY
    LOOP

    low DIRPIN
    pwmout PWMPIN, OFF

    ORIENTATION = UP
    return
    
rotateToSide:
    
    readadc 1,POTPOSITION
    if POTPOSITION > POTSIDE then
        ORIENTATION = SIDE
        return
    endif

    low DIRPIN

    DO UNTIL POTPOSITION > POTSIDE
        readadc 1,POTPOSITION
        DUTY = 189 - POTPOSITION - 42
        DUTY = DUTY * 3 max 400
        'DUTY = 400
        pwmout PWMPIN, 100, DUTY
        
    LOOP

    low DIRPIN
    pwmout PWMPIN, OFF

    ORIENTATION = SIDE
    return
    


