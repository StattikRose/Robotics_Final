#include <Sparki.h>
#include "SparkFunISL29125.h"
#include <Wire.h>

SFE_ISL29125 RGB_sensor;

void setup() {
    RGB_sensor.init();
}

void loop() {
    sparki.clearLCD();
    sparki.println("IR:");
    sparki.println(sparki.lineCenter());
    sparki.println("COLOR:");
    sparki.println(RGB_sensor.readStatus());
    sparki.updateLCD();
    delay(3000);
    sparki.clearLCD();
    sparki.println("RED:");
    sparki.println(RGB_sensor.readRed());
    sparki.println("GREEN:");
    sparki.println(RGB_sensor.readGreen());
    sparki.println("BLUE:");
    sparki.println(RGB_sensor.readBlue());
    sparki.updateLCD();
    delay(3000);
}
