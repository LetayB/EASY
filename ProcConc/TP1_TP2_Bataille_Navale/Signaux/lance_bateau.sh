#!/bin/bash

#Usage : ./lance_bateau.sh <nb bateaux a lancer>

alphabet[1]='A'
alphabet[2]='B'
alphabet[3]='C'
alphabet[4]='D'
alphabet[5]='E'
alphabet[6]='F'
alphabet[7]='G'
alphabet[8]='H'
alphabet[9]='I'
alphabet[10]='J'
alphabet[11]='K'
alphabet[12]='L'
alphabet[13]='M'
alphabet[14]='N'
alphabet[15]='O'
alphabet[16]='P'
alphabet[17]='Q'
alphabet[18]='R'
alphabet[19]='S'
alphabet[20]='T'
alphabet[21]='U'
alphabet[22]='V'
alphabet[23]='W'
alphabet[24]='X'
alphabet[25]='Y'
alphabet[26]='Z'


i=1

while [ $i -le $1 ]
do
    echo Lancement du bateau ${alphabet[$i]}
    
    ./navire ../Fichiers/bateaux.bin  ${alphabet[$i]} &
    
    let i=1+$i
done 

exit 0

#./navire ../Fichiers/bateaux.bin A& ./navire ../Fichiers/bateaux.bin B& ./navire ../Fichiers/bateaux.bin C& ./navire ../Fichiers/bateaux.bin D& ./navire ../Fichiers/bateaux.bin E& ./navire ../Fichiers/bateaux.bin F& ./navire ../Fichiers/bateaux.bin G& ./navire ../Fichiers/bateaux.bin H& ./navire ../Fichiers/bateaux.bin I& ./navire ../Fichiers/bateaux.bin J
