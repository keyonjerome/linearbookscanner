#include <Bounce2.h>
#include "comms_protocol.pb.h"
#include "comms.h"
#define STEP_PIN 3
#define DIR_PIN 2
#define ENABLE_PIN 4
#define LIMIT_SWITCH1 5
#define LIMIT_SWITCH2 6
#define START_STOP_BUTTON_PIN 7
#define SWITCH_DIR_BUTTON_PIN 8

Bounce limitSwitch1 = Bounce();
Bounce limitSwitch2 = Bounce();
Bounce start_stop_button = Bounce();
Bounce switch_dir_button = Bounce();

bool motorRunning = false;
bool direction = true; // true = forward, false = reverse
const int stepDelay = 250; // Microseconds for low speed

int seq = 0;

void setup() {
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);
    pinMode(LIMIT_SWITCH1, INPUT_PULLUP);
    pinMode(LIMIT_SWITCH2, INPUT_PULLUP);
    pinMode(START_STOP_BUTTON_PIN, INPUT_PULLUP);
    pinMode(SWITCH_DIR_BUTTON_PIN, INPUT_PULLUP);
    
    limitSwitch1.attach(LIMIT_SWITCH1);
    limitSwitch1.interval(20);
    limitSwitch2.attach(LIMIT_SWITCH2);
    limitSwitch2.interval(5);
    
    start_stop_button.attach(START_STOP_BUTTON_PIN);
    start_stop_button.interval(30);

    switch_dir_button.attach(SWITCH_DIR_BUTTON_PIN);
    switch_dir_button.interval(30);
    
    digitalWrite(ENABLE_PIN, LOW); // Enable the driver
    Serial.begin(9600); // Initialize UART
}

void loop() {
    limitSwitch1.update();
    limitSwitch2.update();
    start_stop_button.update();
    switch_dir_button.update();

    if (start_stop_button.fell()) {
        motorRunning = !motorRunning;
    }
    CommsMessage message = {0};
    create_status_message(seq,comms_protocol_DeviceState_RUNNING,0,&message);

    if (motorRunning) {
        if (limitSwitch1.fell()) {
            direction = !direction;
            create_flip_page_message(seq,&message);
        }
        if (limitSwitch2.fell()) {
            direction = !direction;
            create_flip_page_message(seq,&message);
        }
        if(switch_dir_button.fell()) {
            direction = !direction;
            create_flip_page_message(seq,&message);
        }
        digitalWrite(DIR_PIN, direction);
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(stepDelay);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(stepDelay);
    }
    // Asynchronous UART transmission
    if (Serial.available()) {
        CommsMessage received = {0};
        decode_command(&received);
    }
    encode_command(&message);
    seq++;
}
