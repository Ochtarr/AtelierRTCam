
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
	Serial.begin(9600);
	servoUD.attach(ServoUD_pin);
	servoLR.attach(ServoLR_pin);
	//getServoPosMax();


	//Init au milieu
	servoUD.write(posInitServoUD);
	servoLR.write(posInitServoLR);

	dataServo = B00000000; 
}

/*
Amelioration voir README.MD

servoMove(boolean sensUD, int vitesseUD, boolean sensLR, int vitesseLR)
*/

// servo : 0 servo UD / 1 servo LR
// sens de direction : 0 left or up / 1 right or down
// integer to change delay
void servoMove(boolean servo, boolean sens, int vitesse) {
	 //cas ou la vitesse est nulle
	 if(vitesse==0 && servo == false)
		 servoUD.write(currentPositionUD);
	 else if(vitesse==0 && servo == true)
		 servoLR.write(currentPositionLR);
	 else{
		 if(servo==false) // servoUD
		 {
			 if(sens == 0)
			 {
				 currentPositionUD--;
			 }
			 else
			 {
				 currentPositionUD++;
			 }
		 }
		 else //servoLR
		 {
			 if(sens == 0)
			 {
				 currentPositionLR--;
			 }
			 else
			 {
				 currentPositionLR++;
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
	
	// MODE DEBUG : Affichage position courantes 
	 Serial.print("Pos. courante axe V: ");
	 Serial.print(currentPositionUD);
	 Serial.print(" / axe h: ");
	 Serial.println(currentPositionLR);

	// Réglage de la vitesse : vitesse 0 à 7, échelle de 0 à 100
	 vitesse = map(vitesse, 0, 7, 0, 100);
	// Attente de l'envoi des données
	 delay(vitesse);
} 

void loop()
{
	
 if( Serial.available() )
 {
	 dataServo = Serial.read();
	 Serial.println(dataServo);
 } 
 
 //dataServo = B10001011;
 Serial.println(dataServo);
 
 byte dataCalcUD = dataServo >> 4;
 byte dataCalcLR = dataServo << 4;
 dataCalcLR = dataCalcLR >> 4;
 
 /*
 Serial.println("debut");
 Serial.println(dataCalcUD);
 Serial.println(dataCalcLR);
 */
 
 byte sensUD = dataCalcUD >> 3;
 byte sensLR = dataCalcLR >> 3;
 
 byte vitesseUD = dataCalcUD & B0111;
 byte vitesseLR = dataCalcLR & B0111;
 
 servoMove(0, sensUD, vitesseUD);
 servoMove(1, sensLR, vitesseLR);
}
	 
	
void getServoPosMax()
{
	//Init au milieu
	servoUD.write(posInitServoUD);
	servoLR.write(posInitServoLR);
	
	//va en haut a gauche
	servoUD.write(posMaxUp);
	servoLR.write(posMaxLeft);
	
	for(ServoLR_position = posMaxLeft; ServoLR_position < posMaxRight; ServoLR_position += 1)  // goes from 0 degrees to 180 degrees 
	{                                  // in steps of 1 degree 
		servoLR.write(ServoLR_position);              // tell servo to go to position in variable 'pos' 
		delay(15);                       // waits 15ms for the servo to reach the position 
	}
	
	delay(50);
	
	
	for(ServoUD_position = posMaxUp; ServoUD_position < posMaxDown; ServoUD_position += 1)  // goes from 0 degrees to 180 degrees 
	{                                  // in steps of 1 degree 
		servoUD.write(ServoUD_position);              // tell servo to go to position in variable 'pos' 
		delay(15);                       // waits 15ms for the servo to reach the position 
	}
	
	
	delay(50);
	
	for(ServoLR_position = posMaxRight; ServoLR_position >= posMaxLeft; ServoLR_position -= 1)  // goes from 0 degrees to 180 degrees 
	{                                  // in steps of 1 degree 
		servoLR.write(ServoLR_position);              // tell servo to go to position in variable 'pos' 
		delay(15);                       // waits 15ms for the servo to reach the position 
	}
	
	delay(50);
	
	
	for(ServoUD_position = posMaxDown; ServoUD_position >= posMaxUp; ServoUD_position -= 1)  // goes from 0 degrees to 180 degrees 
	{                                  // in steps of 1 degree 
		servoUD.write(ServoUD_position);              // tell servo to go to position in variable 'pos' 
		delay(15);                       // waits 15ms for the servo to reach the position 
	}
	
	
	//Init au milieu
	servoUD.write(posInitServoUD);
	servoLR.write(posInitServoLR);
	
}

