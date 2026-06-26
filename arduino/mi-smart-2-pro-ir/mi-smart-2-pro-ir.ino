/*
 * mi-smart-2-pro-ir.ino
 *
 * IR-to-USB bridge for the Xiaomi Mi Smart Projector 2 Pro.
 *
 * The projector has no real "remote power on" feature, but it does react to
 * standard USB HID input. This sketch turns an Arduino Pro Micro (ATmega32U4)
 * into a tiny USB keyboard that listens for two infrared button presses:
 *
 *   - SLEEP button  -> sends the USB "System Sleep" event  -> projector sleeps
 *   - WAKE  button  -> sends the ENTER key                 -> projector wakes
 *
 * Any NEC-protocol IR remote works (e.g. a Broadlink RM, a spare LG TV remote,
 * or a cheap dedicated remote). Just read the command codes printed on the
 * serial monitor and plug them into the CMD_SLEEP / CMD_WAKE constants below.
 *
 * Hardware:
 *   - Arduino Pro Micro (ATmega32U4) -- native USB, required for HID
 *   - IR receiver (e.g. TSOP38238 / VS1838B) on IR_RECEIVE_PIN
 *
 * Libraries (install via the Arduino Library Manager):
 *   - IRremote  by Armin Joachimsmeyer  (>= 4.x)
 *   - HID-Project by NicoHood
 *
 * Author: vzol24
 * License: MIT
 */

/*
 * Only enable the NEC decoder. Limiting the active protocols keeps the build
 * smaller and decoding faster. NEC also covers Apple and Onkyo remotes.
 */
#define DECODE_NEC

#include <Arduino.h>
#include "HID-Project.h"

// Provides IR_RECEIVE_PIN and other per-board pin defaults.
#include "PinDefinitionsAndMore.h"

#include <IRremote.hpp>

// ---------------------------------------------------------------------------
// Configuration
// ---------------------------------------------------------------------------

// IR command codes for the two buttons. Read these off the serial monitor
// (115200 baud) while pressing the buttons on your own remote, then update them.
constexpr uint8_t CMD_SLEEP = 177;  // button that puts the projector to sleep
constexpr uint8_t CMD_WAKE  = 176;  // button that wakes the projector up

// Time to wait after acting on a command, to avoid a single long press or the
// remote's NEC repeat frames triggering the action multiple times (debounce).
constexpr unsigned long ACTION_COOLDOWN_MS = 2000;

// Short settle delay between starting an HID device and ending it.
constexpr unsigned long HID_SETTLE_MS = 300;

// ---------------------------------------------------------------------------
// Actions
// ---------------------------------------------------------------------------

// Send the USB "System Sleep" power event. The projector treats this like
// pressing its own power/standby button.
void sendSleep() {
    Serial.println(F("-> SLEEP"));
    System.begin();
    System.write(SYSTEM_SLEEP);
    delay(HID_SETTLE_MS);
    System.end();
}

// Send an ENTER key press to wake the projector from sleep.
void sendWake() {
    Serial.println(F("-> WAKE"));
    Keyboard.begin();
    Keyboard.write(KEY_ENTER);
    delay(HID_SETTLE_MS);
    Keyboard.end();
}

// ---------------------------------------------------------------------------
// Setup / Loop
// ---------------------------------------------------------------------------

void setup() {
    Serial.begin(115200);
    // Give the host a moment to (re)enumerate the USB serial port.
    delay(2000);
    Serial.println(F("START " __FILE__ " from " __DATE__
                     "\r\nUsing IRremote library version " VERSION_IRREMOTE));

    // Start the IR receiver; ENABLE_LED_FEEDBACK blinks the built-in LED on RX.
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

    Serial.print(F("Ready to receive IR signals of protocols: "));
    printActiveIRProtocols(&Serial);
    Serial.print(F("at pin "));
    Serial.println(IR_RECEIVE_PIN);
}

void loop() {
    // Nothing to do until a full IR frame has been decoded.
    if (!IrReceiver.decode()) {
        return;
    }

    // Print a one-line summary of what was received. Handy for discovering the
    // command codes of a new remote, and for debugging in general.
    IrReceiver.printIRResultShort(&Serial);
    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
        // Unknown protocol: dump the raw timing so it can be analysed.
        IrReceiver.printIRResultRawFormatted(&Serial, true);
    }

    // Ignore NEC auto-repeat frames so holding a button does not fire repeatedly.
    const bool isRepeat = IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT;
    const uint8_t command = IrReceiver.decodedIRData.command;

    if (!isRepeat) {
        if (command == CMD_SLEEP) {
            sendSleep();
            delay(ACTION_COOLDOWN_MS);
        } else if (command == CMD_WAKE) {
            sendWake();
            delay(ACTION_COOLDOWN_MS);
        }
    }

    // Re-enable the receiver for the next frame. Receiving is paused after each
    // decoded packet, so this call is required.
    IrReceiver.resume();
}
