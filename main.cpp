#include <math.h>
#include <iostream>
#include <sys/time.h>
#include <opencv/cv.hpp>
#include "Serial.hpp"
#include "PID.hpp"

using namespace std;

int main(int argc, char **argv)
{
    Serial arduino;
	arduino.Init();

	char *buff = new char[1];
	char *buffUD = new char[1];
    char *buffLR = new char[1];

    PID adapterCommand(1, 0.4, 400, 200); //coeff P, largeur img, hauteur img
    int resultAdapter[4];

    /*
        Reception des coordonnées ici
    */

    /*
        Traitement & Convert
    */
    adapterCommand.Calcul(300, 5, 5, 100, resultAdapter); //x1, y1, x2, y2

    if (resultAdapter[0] == 0) sprintf(buffLR, "%c", (resultAdapter[1] | 0b00000000)<<4);
    else sprintf(buffLR, "%c", (resultAdapter[1] | 0b00001000)<<4);
    
    if (resultAdapter[2] == 0) sprintf(buffUD, "%c", (resultAdapter[3] | 0b00000000));
    else sprintf(buffUD, "%c", (resultAdapter[3] | 0b00001000));

    sprintf(buff, "%c", buffLR[0] | buffUD[0]);
    printf("%d", buff[0]);

    /*
        Envoie commande
    */
    arduino.Write(buff);
    
    arduino.Close();

    return 0;
}
