/*
 * Serial.cpp
 *
 *  Created on: 11 oct. 2017
 *      Author: damien
 */

 #include "PID.hpp"

 PID::PID(float mP, int mLar, int mHaut)
 {
     p = mP;
     lar = mLar;
     haut = mHaut;
 }

int convert(int x, int in_min, int in_max, int out_min, int out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void PID::Calcul(int x1, int y1, int x2, int y2, int *result)
{
    int deltaX = x1 - x2;
    int deltaY = y1 - y2;
    int errorX = p * deltaX;
    int errorY = p * deltaY;

    if (errorX < 0) result[0] = 0; //sens1
    else result[0] = 1; //sens2
    result[1] = convert(abs(errorX), 0, lar, 0, 7);

    if (errorY < 0) result[2] = 0; //sens1
    else result[2] = 1; //sens2
    result[3] = convert(abs(errorY), 0, haut, 0, 7);
}