
#include <Servo.h> 
 
Servo servoUD;  // create servo object to control Up/Down Servo
Servo servoLR; // create servo object to control Left/Right Servo
 
int ServoUD_pin = 10;  // analog pin used to connect the potentiometer
int ServoLR_pin = 9;
int ServoUD_position;    // variable to read the value from the analog pin 
int ServoLR_position;

void setup()
{
  servoUD.attach(ServoUD_pin);
  servoLR.attach(ServoLR_pin);
}

void loop()
{
  for(ServoUD_position = 0; ServoUD_position < 180; ServoUD_position += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    servoUD.write(ServoUD_position);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  for(ServoUD_position = 180; ServoUD_position>=1; ServoUD_position-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    servoUD.write(ServoUD_position);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  }
 for(ServoLR_position = 0; ServoLR_position < 180; ServoLR_position += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    servoLR.write(ServoLR_position);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  for(ServoLR_position = 180; ServoLR_position>=1; ServoLR_position-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    servoLR.write(ServoLR_position);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
}
