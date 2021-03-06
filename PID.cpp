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
 * \fn PID::PID(float mP, float, mI, int mLar, int mHaut)
 * \brief Constructeur : coefficient P, coefficient I, largeur de l'image, hauteur de l'image
 *
 */

PID::PID(float mP, float mI, float mD, int mLar, int mHaut)
{
    p = mP;
    i = mI;
    d = mD;
    lar = mLar;
    haut = mHaut;
    integraleX = 0;
    integraleY = 0;
    deriveX = 0;
    deriveY = 0;
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

    integraleX += deltaX;
    integraleY += deltaY;

    int deriveXLocal = deltaX - deriveX;
    deriveX = deltaX;

    int deriveYLocal = deltaY - deriveY;
    deriveY = deltaY;

    int cmdX = p * deltaX + i * integraleX + d * deriveXLocal;
    int cmdY = p * deltaY + i * integraleY + d * deriveYLocal;

    if (cmdX < 0) result[0] = 0; //sens1
    else result[0] = 1; //sens2
    result[1] = convert(abs(cmdX), 0, lar, 0, 7);

    if (cmdY < 0) result[2] = 0; //sens1
    else result[2] = 1; //sens2
    result[3] = convert(abs(cmdY), 0, haut, 0, 7);
}
