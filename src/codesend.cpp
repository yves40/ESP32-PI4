/*
Usage: ./codesend decimalcode [protocol] [pulselength]
decimalcode - As decoded by RFSniffer
protocol    - According to rc-switch definitions
pulselength - pulselength in microseconds

 'codesend' hacked from 'send' by @justy
 
 - The provided rc_switch 'send' command uses the form systemCode, unitCode, command
   which is not suitable for our purposes.  Instead, we call 
   send(code, length); // where length is always 24 and code is simply the code
   we find using the RF_sniffer.ino Arduino sketch.

(Use RF_Sniffer.ino to check that RF signals are being produced by the RPi's transmitter 
or your remote control)
*/
#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>

#define VERSION "codesend 1.06, May 05 2020"

int main(int argc, char *argv[])
{

    int loops = 1;

    // This pin is not the first pin on the RPi GPIO header!
    // Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
    // for more information.
    int PIN = 0;

    printf("%s\n\n", VERSION);

    // Parse the first parameter to this command as an integer
    int protocol = 0; // A value of 0 will use rc-switch's default value
    int pulseLength = 0;

    // If no command line argument is given, print the help text
    if (argc == 1)
    {
        printf("Usage: %s decimalcode [loops] [protocol] [pulselength]\n", argv[0]);
        printf("decimalcode\t- As decoded by RFSniffer\n");
        printf("protocol\t- According to rc-switch definitions\n");
        printf("pulselength\t- pulselength in microseconds\n");
        return -1;
    }

    // Change protocol and pulse length accroding to parameters
    int code = atoi(argv[1]);
    // Any number of loops requestd ?
    if (argc == 3)
    {
        loops = atoi(argv[2]);
    }
    if (argc == 4)
        protocol = atoi(argv[2]);
    if (argc == 5)
        pulseLength = atoi(argv[3]);

    if (wiringPiSetup() == -1)
        return 1;
    RCSwitch mySwitch = RCSwitch();
    if (protocol != 0)
        mySwitch.setProtocol(protocol);
    if (pulseLength != 0)
        mySwitch.setPulseLength(pulseLength);
    for (int i = 0; i < loops; ++i)
    {
        printf("Enable transmission on GPIO_GEN%i\n", PIN);
        mySwitch.enableTransmit(PIN);
        printf("sending code[%i] \n", code);
        mySwitch.send(code, 24);
        printf("Closing transmission GPIO now\n\n");
        mySwitch.disableTransmit();
        delay(1000);
    }
    return 0;
}
