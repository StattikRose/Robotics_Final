# Robotics_Final
CSCI 3302: Robotics

## Overview
Sparki Bot will navigate around an open are filled with golf balls. Sparki will then retrieve and sort the golf balls based on their color into bins.

## Steps
* Sparki will start in the South-West corner of the open area and will navigate back and forth across the area. Using UltraSound Sparki will detect objects.
* Once an object is detected Sparki will move to the object and grab it with its grippers. We will mount a RGB sensor on the front of Sparki to detect the color.
* Once the color is detected and the object is in Sparki's grippers it will then navigate back to the starting point.
* From the starting point Sparki will move up a color coded line to find the correct bin to deposit the ball into and deposit it.
* Sparki will then move back to the starting point and from there out to the location where it picked up the last ball and continue on its path across the open area.
* Once Sparki has reached the North-East cornor or the area it will assume all balls were found and will celebrate.

## Sparki State Codes and Coding Assignments
1. State 1 (Jared Jolton)
    * Search path, object detection, check if end
2. State 2 (Brian Kidd)
    * Retrieve object, grab, determin color
3. State 3 (Zack Allen)
    * Go to SBLI, while checking left IR sensor to prevent fall
4. State 4 (Jennifer Dooley)
    * Go up color coded line, fix odometry with localization, find correct bin & deposit, return to SBLI
5. State 5 (Brian Salmon)
    * Goes from SBLI to where ball was found, clear global vars and set state to 1
6. State 6 (All) 
	* Celabrate!

## State Input/Output requirements
| State | Input | Output |
| :---- | :---------------- | :----------------------- |
| 1. | None | Approximate location of Object |
| 2. | Approx. Loc of obj | ID color and return Location |
| 3. | None | None |
| 4. | ID color | None |
| 5. | return location | None |
| 6. | None | None |

## Platform Layout
![Platform Sketch.png]