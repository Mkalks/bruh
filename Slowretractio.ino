#include <ESP32Servo.h>
#include <FRPPMReceiverSensor.h>

const int servo1pin = 27;
const int PINPPM = 4;
const int NUMBEROFCHANNELS = 8;

PPMReceiverSensor MyReceiver(PINPPM, NUMBEROFCHANNELS);

Servo servo1;
Servo servo2;
int channelValues[NUMBEROFCHANNELS];
unsigned long previousMillis = 0;
unsigned long interval = 20;   
int currentPos = 0;
int targetPos = 0;
int stepSize = 1; 

void setup() {
    MyReceiver.Init();
    Serial.begin(115200);
    servo1.setPeriodHertz(50);
    servo1.attach(servo1pin);
    servo2.setPeriodHertz(50);
    servo2.attach(14);
}

void loop() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        for (int i = 0; i < NUMBEROFCHANNELS; i++) {
            channelValues[i] = MyReceiver.ReadChannel(i);
            if (channelValues[i] <= 600) {
                targetPos = 0;
            } else if (channelValues[i] >= 1599) {
                targetPos = 120;
            }

            if (i == 0) {
                int servoposition = map(channelValues[i], 599, 1600, 60, 120);
                servo2.write(servoposition);
            }
        }

        if (currentPos != targetPos) {
            if (currentPos < targetPos) {
                currentPos += stepSize;
            } else {
                currentPos -= stepSize;
            }
            servo1.write(currentPos);
        }
    }
}
