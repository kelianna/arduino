const byte ledPin = 2;       // Builtin-LED pin
const byte interruptPin = 0; // BOOT/IO0 button pin
volatile byte state = LOW;

#include <IRremote.hpp>
#include <Arduino.h>

#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp>


void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
  Serial.begin(115200);
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

    // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

    Serial.print(F("Ready to receive IR signals of protocols: "));
    printActiveIRProtocols(&Serial);
    Serial.print(F("at pin "));
    Serial.println(IR_RECEIVE_PIN);
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);

    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

    /*
     * The IR library setup. That's all!
     */
    IrSender.begin(); // Start with IR_SEND_PIN as send pin and if NO_LED_FEEDBACK_CODE is NOT defined, enable feedback LED at default feedback LED pin

    Serial.print(F("Ready to send IR signals at pin "));
    Serial.println(IR_SEND_PIN);
}
uint16_t sAddress = 0x0102;
uint8_t sCommand = 0x34;
uint8_t sRepeats = 0;
void loop() {
  digitalWrite(ledPin, state);
  if (IrReceiver.decode()) {

        // Print a short summary of received data
        IrReceiver.printIRResultShort(&Serial);
        if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
            // We have an unknown protocol here, print more info
            IrReceiver.printIRResultRawFormatted(&Serial, true);
        }
        Serial.println();

        /*
         * !!!Important!!! Enable receiving of the next value,
         * since receiving has stopped after the end of the current received data packet.
         */
        IrReceiver.resume(); // Enable receiving of the next value

        /*
         * Finally, check the received data and perform actions according to the received command
         */
        if (IrReceiver.decodedIRData.command == 0x10) {
            // do something
        } else if (IrReceiver.decodedIRData.command == 0x11) {
            // do something else
        }
    }
    Serial.println();
    Serial.print(F("Send now: address=0x"));
    Serial.print(sAddress, HEX);
    Serial.print(F(" command=0x"));
    Serial.print(sCommand, HEX);
    Serial.print(F(" repeats="));
    Serial.print(sRepeats);
    Serial.println();

    Serial.println(F("Send NEC with 16 bit address"));
    Serial.flush();

    // Results for the first loop to: Protocol=NEC Address=0x102 Command=0x34 Raw-Data=0xCB340102 (32 bits)
    IrSender.sendNEC(sAddress, sCommand, sRepeats);

    /*
     * If you cannot avoid to send a raw value directly like e.g. 0xCB340102 you must use sendNECRaw()
     */
//    Serial.println(F("Send NECRaw 0xCB340102"));
//    IrSender.sendNECRaw(0xCB340102, sRepeats);
    /*
     * Increment send values
     * Also increment address just for demonstration, which normally makes no sense
     */
    sAddress += 0x0101;
    sCommand += 0x11;
    sRepeats++;
    // clip repeats at 4
    if (sRepeats > 4) {
        sRepeats = 4;
    }

    delay(1000);
}

void blink() {
  state = !state;
}
