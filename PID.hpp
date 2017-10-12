/*
 * PID.h
 *
 *  Created on: 11 oct. 2017
 *      Author: damien
 */

 #ifndef PID_HPP_
 #define PID_HPP_
 #include <stdlib.h>
 
 class PID {
 protected:
     float p; 	        /*!< Coefficient p*/
     float i;           /*!< Coefficient i*/
     float d;           /*!< Coefficient d*/
     int lar;	        /*!< Largeur de l'image*/
     int haut;  	    /*!< Hauteur de l'image*/

     int integraleX;    /*!< Intégrale en X*/
     int integraleY;    /*!< Intégrale en Y*/
     int deriveX;    /*!< Intégrale en X*/
     int deriveY;    /*!< Intégrale en Y*/
 public:
    PID(float mP, float mI, float mD, int mLar, int mHaut);
    void Calcul(int x1, int y1, int x2, int y2, int *result);
 };
 
 #endif /* PID_HPP_ */
