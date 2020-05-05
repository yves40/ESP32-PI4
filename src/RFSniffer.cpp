/*
  RFSniffer

  Usage: ./RFSniffer [<pulseLength>]
  [] = optional

  Hacked from http://code.google.com/p/rc-switch/
  by @justy to provide a handy RF code sniffer
*/

#include "RCSwitch.h"
#include "wiringPi.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define VERSION "RFSniffer 1.10, May 05 2020"

RCSwitch mySwitch;

int main(int argc, char *argv[])
{

  // This pin is not the first pin on the RPi GPIO header!
  // Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
  // for more information.
  int PIN = 2;
  int maxloops = 50;
  int loops = 0;

  printf("%s\n\n", VERSION);

  if (wiringPiSetup() == -1)
  {
    printf("wiringPiSetup failed, exiting...");
    exit(1);
  }

  int pulseLength = 0;
  if (argv[1] != NULL)
    pulseLength = atoi(argv[1]);

  mySwitch = RCSwitch();
  if (pulseLength != 0)
    mySwitch.setPulseLength(pulseLength);
  printf("Listen on GPIO_GEN%i\n", PIN);
  mySwitch.enableReceive(PIN); // Receiver on interrupt 0 => that is pin #2

  while (loops < maxloops)
  {

    if (mySwitch.available())
    {
      printf("Resetting loops, which was currently set to %i\n", loops);
      loops = 0; // Reste loops as we received something
      int value = mySwitch.getReceivedValue();

      if (value == 0)
      {
        printf("Unknown encoding\n");
      }
      else
      {

        printf("Received %i\n", mySwitch.getReceivedValue());
        printf("Bits %i\n", mySwitch.getReceivedBitlength());
        printf("Protocol %i\n", mySwitch.getReceivedProtocol());
      }

      fflush(stdout);
      mySwitch.resetAvailable();
    }
    ++loops;
    if ((loops % 10) == 0)
    {
      printf(".");
      fflush(stdout);
    }
    delay(500);
  }
  printf("\n\n");

  exit(0);
}
