
/*
 * Specify which protocol(s) should be used for decoding.
 * If no protocol is defined, all protocols are active.
 */
//#define DECODE_DENON        // Includes Sharp
//#define DECODE_JVC
//#define DECODE_KASEIKYO
//#define DECODE_PANASONIC    // the same as DECODE_KASEIKYO
//#define DECODE_LG
#define DECODE_NEC          // Includes Apple and Onkyo
//#define DECODE_SAMSUNG
//#define DECODE_SONY
//#define DECODE_RC5
//#define DECODE_RC6

//#define DECODE_BOSEWAVE
//#define DECODE_LEGO_PF
//#define DECODE_MAGIQUEST
//#define DECODE_WHYNTER

//#define DECODE_DISTANCE     // universal decoder for pulse width or pulse distance protocols
//#define DECODE_HASH         // special decoder for all protocols

//#define DEBUG // Activate this for lots of lovely debug output from the decoders.
//#define INFO                // To see valuable informations from universal decoder for pulse width or pulse distance protocols

#include <Arduino.h>
#include "HID-Project.h"

/*
 * Define macros for input and output pin etc.
 */
#include "PinDefinitionsAndMore.h"

#include <IRremote.hpp>

void setup() {
    Serial.begin(115200);
    delay(5000);
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

    // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

    Serial.print(F("Ready to receive IR signals of protocols: "));
    printActiveIRProtocols(&Serial);
    Serial.print(F("at pin "));
    Serial.println(IR_RECEIVE_PIN);
}

void loop() {
    /*
     * Check if received data is available and if yes, try to decode it.
     * Decoded result is in the IrReceiver.decodedIRData structure.
     *
     * E.g. command is in IrReceiver.decodedIRData.command
     * address is in command is in IrReceiver.decodedIRData.address
     * and up to 32 bit raw data in IrReceiver.decodedIRData.decodedRawData
     */
    if (IrReceiver.decode()) {

        // Print a short summary of received data
        IrReceiver.printIRResultShort(&Serial);
        if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
            // We have an unknown protocol here, print more info
            IrReceiver.printIRResultRawFormatted(&Serial, true);
        }
        Serial.println();

  
//        Serial.println(IrReceiver.decodedIRData.command);
        /*
         * Finally, check the received data and perform actions according to the received command
         */
        if (IrReceiver.decodedIRData.command == 177) {
            // do stop
            Serial.println("stop");

            System.begin();
            System.write(SYSTEM_SLEEP);
            // Simple debounce
            delay(300);
            System.end();
            delay(2000);
        } else if (IrReceiver.decodedIRData.command == 176) {
            // do start
            Serial.println("start");

            Keyboard.begin();
            
            Keyboard.println(KEY_ENTER);
            // Simple debounce
            delay(300);
            Keyboard.end();

            delay(2000);
        }

        /*
         * !!!Important!!! Enable receiving of the next value,
         * since receiving has stopped after the end of the current received data packet.
         */
        IrReceiver.resume(); // Enable receiving of the next value
      
    }
}
