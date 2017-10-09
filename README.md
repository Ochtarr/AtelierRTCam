# AtelierRTCam

Atelier M2 : Contrôle d'une caméra et stabiisation de flux vidéo

## Interface d'échange entre le conrôlleur arduino & le PC source

1 octet pour le contrôle des deux servomoteurs : 
4ers bits pour le servo haut/bas
4 derniers bits pour le servo gauche/droite

sur les 4 bits : 
- le 1er contrôle le sens de rotation
- les 3 autres contrôlent la vitesse de rotation (000->vitesse nulle;111->vitesse maximum du servomoteurs)


