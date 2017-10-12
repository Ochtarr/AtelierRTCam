/**
 * \file PID.cpp
 * \brief Classe PID
 * \author Damien.L
 * \version 0.1
 * \date 11 octobre 2017
 *
 * Classe permettant d'adapter la commande à envoyer à la liaison série suivant une quantité d'erreur
 *
 */

#include "PID.hpp"

/**
 * \fn PID::PID(float mP, int mLar, int mHaut)
 * \brief Constructeur : coefficient P, largeur de l'image, hauteur de l'image
 *
 */

PID::PID(float mP, int mLar, int mHaut)
{
    p = mP;
    lar = mLar;
    haut = mHaut;
}

/**
 * \fn int convert(int x, int in_min, int in_max, int out_min, int out_max)
 * \brief Fonction d'adaptation d'une valeur selon la plage d'entrée et la plage de sortie. Equivalence ==> fonction map sur arduino
 *
 * \return Résultat de la conversion
 */

int convert(int x, int in_min, int in_max, int out_min, int out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * \fn void PID::Calcul(int x1, int y1, int x2, int y2, int *result)
 * \brief Le résultat des calculs est dans le tableau "result". Indice 0 et 2 : sens, indice 1 et 3 : pas convertie de 0 à 7
 *
 */

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
