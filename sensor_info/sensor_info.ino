#include <Sparki.h>
#include "SparkFunISL29125.h"
#include <Wire.h>

SFE_ISL29125 RGB_sensor;

void setup() {
    RGB_sensor.init();
}

void loop() {
    /*
    sparki.clearLCD();
    sparki.println("IR:");
    sparki.println(sparki.lineCenter());
    sparki.println("COLOR:");
    sparki.println(RGB_sensor.readStatus());
    sparki.updateLCD();
    delay(3000);
    */
    sparki.clearLCD();
    sparki.println("RED:");
    sparki.println(RGB_sensor.readRed());
    sparki.println("GREEN:");
    sparki.println(RGB_sensor.readGreen());
    sparki.println("BLUE:");
    sparki.println(RGB_sensor.readBlue());
    sparki.updateLCD();
    //delay(3000);
    //sparki.clearLCD();
    //unsigned int red = RGB_sensor.readRed();
    //unsigned int green = RGB_sensor.readGreen();
    //unsigned int blue = RGB_sensor.readBlue();
    unsigned int red = (RGB_sensor.readRed() + RGB_sensor.readRed() + RGB_sensor.readRed() + RGB_sensor.readRed() + RGB_sensor.readRed()) / 5;
    unsigned int green = (RGB_sensor.readGreen() + RGB_sensor.readGreen() + RGB_sensor.readGreen() + RGB_sensor.readGreen() + RGB_sensor.readGreen()) / 5;
    unsigned int blue = (RGB_sensor.readBlue() + RGB_sensor.readBlue() + RGB_sensor.readBlue() + RGB_sensor.readBlue() + RGB_sensor.readBlue()) / 5;
    int sum = red + green + blue;
    sum = sum*10;
    String color;
    if (sum < 170) {
          color = "blue";
    }
    else if (sum > 350) {
          color = "white";  
    }
    else {
          color = "pink";  
    }
    sparki.println(sum);
    sparki.println(color);
    sparki.updateLCD();
    delay(3000);
}
