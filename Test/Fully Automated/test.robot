*** Settings ***
Documentation     Human Machine Interface Test
Library           SerialLibrary    encoding=ascii
Library           String
Suite Setup       Open Serial Connection
Suite Teardown    Close Serial Connection

*** Variables ***
${PORT}           COM11
${BAUDRATE}       9600
${TIMEOUT}        5
${NEWLINE}        \r\n

*** Keywords ***
Open Serial Connection
    Add Port    ${PORT}    baudrate=${BAUDRATE}    timeout=${TIMEOUT}
    Open Port   ${PORT}
    Flush Port  ${PORT}
    Sleep    3s
    Log To Console    Connected to ${PORT}

Close Serial Connection
    Delete All Ports
    Log To Console    Disconnected

Send Command
    [Arguments]    ${cmd}
    Flush Port    ${PORT}
    Write Data    ${cmd}${NEWLINE}
    Sleep    0.5s
    Log To Console    Sent: ${cmd}

Read Response
    [Arguments]    ${timeout}=3
    ${resp}=    Read Until    ${NEWLINE}    timeout=${timeout}
    ${resp}=    Strip String    ${resp}
    ${resp}=    Replace String    ${resp}    \r    ${EMPTY}
    Log To Console    Received: ${resp}
    [Return]    ${resp}

Verify Contains
    [Arguments]    ${text}    ${expected}
    Should Contain    ${text}    ${expected}

Get Status
    Send Command    get_status
    ${resp}=    Read Response
    [Return]    ${resp}

Verify Status
    [Arguments]    ${expected_blue}    ${expected_red}    ${expected_fan_buzzer}
    ${status}=    Get Status
    Should Contain    ${status}    BLUE=${expected_blue}
    Should Contain    ${status}    RED=${expected_red}
    Should Contain    ${status}    FAN_AND_BUZZER=${expected_fan_buzzer}

*** Test Cases ***
1 System Initialization
    Sleep   5s
    ${msg}=    Read Response    5
    Verify Contains    ${msg}    SYSTEM READY

2 Temperature Displayed on LCD
    Send Command    get_temp
    ${resp}=    Read Response
    WHILE    $resp == "Type 'help' for commands." or $resp == ""
        ${resp}=    Read Response
    END
    Verify Contains    ${resp}    TEMP:

3 Temperature Updates Every 2 Seconds
    Send Command    get_temp
    ${t1}=    Read Response
    Sleep    2.5s
    Send Command    get_temp
    ${t2}=    Read Response
    Verify Contains    ${t1}    TEMP:
    Verify Contains    ${t2}    TEMP:


4 High and Low Limits Displayed
    Send Command    print_limits
    ${resp}=    Read Response
    WHILE    '${resp}' == ''
        ${resp}=    Read Response
    END
    Should Contain    ${resp}    LOW:
    Should Contain    ${resp}    HIGH:


5 OK/NOK Display on LCD
    [Documentation]    Verify OK appears when temp is in range, NOK when out
    
    # Set safe range
    Send Command    set_low=20
    ${resp}=    Read Response
    Verify Contains    ${resp}    OK
    Send Command    set_high=30
    ${resp}=    Read Response
    Verify Contains    ${resp}    OK
    Sleep    1s

    # Test OK
    Send Command    set_temp=25
    ${resp}=    Read Response
    Verify Contains    ${resp}    OK
    Send Command    read_lcd
    ${lcd}=    Read Response
    Should Contain    ${lcd}    LCD:
    Should Contain    ${lcd}    OK
    Should Contain    ${lcd}    Temp: 25

    # Test NOK (below)
    Send Command    set_temp=15
    ${resp}=    Read Response
    Verify Contains    ${resp}    OK
    Send Command    read_lcd
    ${lcd}=    Read Response
    Should Contain    ${lcd}    NOK
    Should Contain    ${lcd}    Temp: 15

    # Test NOK (above)
    Send Command    set_temp=35
    ${resp}=    Read Response
    Verify Contains    ${resp}    OK
    Send Command    read_lcd
    ${lcd}=    Read Response
    Should Contain    ${lcd}    NOK
    Should Contain    ${lcd}    Temp: 35

    # Return to safe
    Send Command    set_temp=25
    
6 Decimal Temperature Values Accepted
    Send Command    set_temp=27.5
    ${resp}=    Read Response
    WHILE    "OK" not in $resp
        ${resp}=    Read Response
    END
    Verify Contains    ${resp}    OK

    Send Command    get_temp
    ${resp}=    Read Response
    Should Contain    ${resp}    27.5

7 High Limit Increases 
    Send Command    set_high=31
    ${resp}=    Read Response
    WHILE    "OK" not in $resp
        ${resp}=    Read Response
    END
    Verify Contains    ${resp}    OK

8 High Limit Decreases 
    Send Command    set_high=29
    ${resp}=    Read Response
    Verify Contains    ${resp}    OK

9 Low Limit Increases 
    Send Command    set_low=21
    ${resp}=    Read Response
    Verify Contains    ${resp}    OK

10 Low Limit Decreases 
    Send Command    set_low=19
    ${resp}=    Read Response
    Verify Contains    ${resp}    OK

11 LEDs Off in Safe Range
    Send Command    set_temp=25
    ${resp}=    Read Response
    WHILE    "OK" not in $resp
        ${resp}=    Read Response
    END
    Verify Status     OFF     OFF     OFF

12 Blue LED ON Below Low Limit
    Send Command    set_temp=17
    ${resp}=    Read Response
    Verify Status     ON     OFF     OFF

13 Blue LED Turns On At Exact Low Limit
    Send Command    set_low=20
    ${resp}=    Read Response
    WHILE    "OK" not in $resp
        ${resp}=    Read Response
    END

    Send Command    set_temp=20
    ${resp}=    Read Response
    Verify Status     ON     OFF     OFF

14 Blue LED Off above Low Limit
    Send Command    set_temp=21
    ${resp}=    Read Response
    Verify Status     OFF     OFF     OFF

15 Red LED, Fan and Buzzer ON Above High Limit
    Send Command    set_temp=36
    ${resp}=    Read Response
    Verify Status     OFF     ON     ON

16 Red LED, Fan and Buzzer Turn On At Exact High Limit
    Send Command    set_low=20
    ${resp}=    Read Response
    WHILE    "OK" not in $resp
        ${resp}=    Read Response
    END

    Send Command    set_high=30
    ${resp}=    Read Response
    WHILE    "OK" not in $resp
        ${resp}=    Read Response
    END

    Send Command    set_temp=30
    ${resp}=    Read Response
    Verify Status     OFF     ON     ON

17 System Returns to Safe State
    Send Command    set_temp=28
    ${resp}=    Read Response
    Verify Status     OFF     OFF     OFF

