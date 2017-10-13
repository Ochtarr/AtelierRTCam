#include <Servo.h>
 
/* déclaration des servomoteurs */
Servo servoUD;  // create servo object to control Up/Down Servo
Servo servoLR; // create servo object to control Left/Right Servo
int ServoUD_pin = 9;  // analog pin used to connect the potentiometer
int ServoLR_pin = 10;
int ServoUD_position;    // variable to read the value from the analog pin 
int ServoLR_position;

/* Initialisation */
int posInitServoUD = 115;
int posInitServoLR = 115;
int posMaxUp = 15;
int posMaxDown = 175;
int posMaxRight = 175;
int posMaxLeft = 5;

byte dataServo = 0;

int currentPositionUD = posInitServoUD;
int currentPositionLR = posInitServoLR;

void setup()
{
  Serial.begin(115200);
  servoUD.attach(ServoUD_pin);
  servoLR.attach(ServoLR_pin);

  //Init au milieu
  servoUD.write(posInitServoUD);
  servoLR.write(posInitServoLR);

  dataServo = B00000000; 
}

// servo : 0 servo UD / 1 servo LR
// sens de direction : 0 left or up / 1 right or down
// integer to change delay
void servoMove(boolean servo, boolean sens, int vitesse) {
   //cas ou la vitesse est nulle
   int pasPos = 1;
   if(vitesse==0 && servo == false)
     servoUD.write(currentPositionUD);
   else if(vitesse==0 && servo == true)
     servoLR.write(currentPositionLR);
   else{
     if(servo==false) // servoUD
     {
       if(sens == 0)
       {
         currentPositionUD-=pasPos;
       }
       else
       {
         currentPositionUD+=pasPos;
       }
     }
     else //servoLR
     {
       if(sens == 0)
       {
         currentPositionLR-=pasPos;
       }
       else
       {
         currentPositionLR+=pasPos;
       }
     }
   }
   
   if(currentPositionLR >= posMaxRight)
     currentPositionLR = posMaxRight;
   if(currentPositionLR <= posMaxLeft)
     currentPositionLR = posMaxLeft;
   if(currentPositionUD >= posMaxDown)
     currentPositionUD = posMaxDown;
   if(currentPositionUD <= posMaxUp)
     currentPositionUD = posMaxUp;
     
   servoUD.write(currentPositionUD);
   servoLR.write(currentPositionLR);
} 

void loop()
{
 if( Serial.available() )
 {
   dataServo = Serial.read();
   Serial.flush();
   Serial.println(dataServo);
 
   byte dataCalcUD = dataServo >> 4;
   byte dataCalcLR = dataServo << 4;
   dataCalcLR = dataCalcLR >> 4;
   
   byte sensUD = dataCalcUD >> 3;
   byte sensLR = dataCalcLR >> 3;
   
   byte vitesseUD = dataCalcUD & B0111;
   byte vitesseLR = dataCalcLR & B0111;
   
   servoMove(0, sensUD, vitesseUD);
   servoMove(1, sensLR, vitesseLR);
 }
}
