# AtelierRTCam

AMELIORATION A PROPOSER : au lieu de traiter les mouvements séparément, il faudrait regrouper l'execution de ces mouvements, de manière à ce qu'ils soient exécutés parallèlement.

Atelier M2 : Contrôle d'une caméra et stabiisation de flux vidéo

## Echange des données entre l'arduino et l'ordinateur


## Structure des données d'échange entre le contrôlleur arduino & le PC source

1 octet pour le contrôle des deux servomoteurs : 
- 4 premiers bits pour le servo haut/bas
- 4 derniers bits pour le servo gauche/droite

Sur ces deux groupes de 4 bits, nous avons de décomposer la structure ainsi : 
- le 1er bit contrôle le sens de rotation
- les 3 autres contrôlent les 8 vitesses de rotation possibles du servomoteurs.

(000->vitesse nulle;111->vitesse maximum du servomoteurs)

l'échelle pouvant être modifiée.


