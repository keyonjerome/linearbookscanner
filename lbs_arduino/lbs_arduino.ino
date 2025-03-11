#include <Bounce2.h>
#include "comms_protocol.pb.h"
#include "comms.h"

// Pin definitions
#define STEP_PIN 2
#define DIR_PIN 3
#define ENABLE_PIN 4
#define LIMIT_SWITCH1 5
#define LIMIT_SWITCH2 6
#define START_STOP_BUTTON_PIN 7
#define SWITCH_DIR_BUTTON_PIN 8

// Bounce2 button/switch objects
Bounce limitSwitch1 = Bounce();
Bounce limitSwitch2 = Bounce();
Bounce start_stop_button = Bounce();
Bounce switch_dir_button = Bounce();

// Motor control variables
bool motorRunning = true;  // Start motor in ON state
bool direction = true; // true = forward, false = reverse
const int stepDelay = 250; // Microseconds for step pulse delay
unsigned long lastStepTime = 0;
unsigned int seq = 0;
CommsMessage msg;

void setup() {
    // Configure motor control pins
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);
    
    // Configure input pins (normally open switches)
    pinMode(LIMIT_SWITCH1, INPUT_PULLUP);
    pinMode(LIMIT_SWITCH2, INPUT_PULLUP);
    pinMode(START_STOP_BUTTON_PIN, INPUT_PULLUP);
    pinMode(SWITCH_DIR_BUTTON_PIN, INPUT_PULLUP);
    
    // Attach buttons/switches with debounce
    limitSwitch1.attach(LIMIT_SWITCH1);
    limitSwitch1.interval(5);
    
    limitSwitch2.attach(LIMIT_SWITCH2);
    limitSwitch2.interval(30);
    
    start_stop_button.attach(START_STOP_BUTTON_PIN);
    start_stop_button.interval(30);

    switch_dir_button.attach(SWITCH_DIR_BUTTON_PIN);
    switch_dir_button.interval(30);
    
    // Enable stepper motor driver
    digitalWrite(ENABLE_PIN, LOW); 

    // Initialize serial for debugging
    Serial.begin(115200);

    // Set initial direction
    digitalWrite(DIR_PIN, direction);
}

void loop() {
    // Update button states
    limitSwitch1.update();
    limitSwitch2.update();
    start_stop_button.update();
    switch_dir_button.update();

    // Handle start/stop button
    if (start_stop_button.fell()) {
        motorRunning = !motorRunning;
        Serial.print("Motor running: ");
        Serial.println(motorRunning ? "ON" : "OFF");
    }

    // Handle direction switch button
    if (switch_dir_button.fell()) {
        direction = !direction;
        Serial.print("Direction manually changed: ");
        Serial.println(direction ? "FORWARD" : "REVERSE");
        digitalWrite(DIR_PIN, direction);
    }

    // Handle limit switches (reverse direction when triggered)
    if (limitSwitch1.fell()) {
        direction = false;  // Reverse direction
        digitalWrite(DIR_PIN, direction);
        Serial.println("Limit Switch 1 hit! Reversing direction.");
    }

    if (limitSwitch2.fell()) {
        direction = true;  // Reverse direction
        digitalWrite(DIR_PIN, direction);
        Serial.println("Limit Switch 2 hit! Reversing direction.");
    }

    // Step motor if running
    if (motorRunning) {
        unsigned long now = micros();
        if (now - lastStepTime >= stepDelay) {
            lastStepTime = now;
            digitalWrite(STEP_PIN, HIGH);
            delayMicroseconds(5); // Short pulse
            digitalWrite(STEP_PIN, LOW);
        }
    } 
    comms_protocol_CommsMessage message = comms_protocol_CommsMessage_init_zero;

    // Initialize the message
    message.sequence_number = 1;
    message.type = comms_protocol_MessageType_STATUS;
    message.device = comms_protocol_Device_STM32;
    message.which_msg = comms_protocol_CommsMessage_status_tag;
    message.msg.status.state = comms_protocol_DeviceState_RUNNING;
    message.msg.status.ticks = 12345;  // Example tick value (timestamp)
    // Asynchronous UART transmission
    if (Serial.available()) {
        CommsMessage received = {0};
        decode_command(&received);
    }
//    encode_command(&message);
    seq++;
}
