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
    sparki.print(sparki.lineCenter());
    sparki.println("COLOR:");
    sparki.print(RGB_sensor.readStatus());
    sparki.println("RED:");
    sparki.print(RGB_sensor.readRed());
    sparki.println("GREEN:");
    sparki.print(RGB_sensor.readGreen());
    sparki.println("BLUE:");
    sparki.print(RGB_sensor.readBlue());
    sparki.updateLCD();
}
