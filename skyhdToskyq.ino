/*
Sky HD to Sky Q mapper

Allows use of Sky HD remote with the Sky Q

Chet Kelly
www.chet.ie
*/

#include <IRremote.h>

//base value of all Sky HD codes
#define SKYHDBASE 0xC05C00

//base value of all Sky Q codes
#define SKYQBASE 0xC0081A00

byte buttonVal;

long sendButton;

/*
* Default is Arduino pin D11.
* You can change this to another available Arduino Pin.
* Your IR receiver should be connected to the pin defined here
*/
int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);
IRsend irsend;
decode_results results;

void setup()
{
irrecv.enableIRIn(); // Start the receiver
}
void process(decode_results *results) {

//We are only interested in Sky HD codes which are all 24 bit RC6
if (results->decode_type == RC6 && results->bits == 24) {

//assign 24 bit value to 8 bit variable to take least significant byte
buttonVal = results->value;

//check if its likely to be a valid Sky HD code by comparing the base
/*
check if its likely to be a valid Sky HD code by comparing the base e.g.

received = 0xC05C5C

assigned to buttonVal = 0x5C

0xC05C5C - 0x5C = 0xC05C00 = Sky HD base

*/
if(results->value - buttonVal == SKYHDBASE){

//Sky HD least significant byte matches corresponding button on Sky Q
sendButton = SKYQBASE + buttonVal;

//send IR
irsend.sendRC6(sendButton, 32);
//we need to re-enable receiving after sending
irrecv.enableIRIn();
}
}
}

void loop() {
if (irrecv.decode(&results)) {
process(&results);
irrecv.resume();
}
}
