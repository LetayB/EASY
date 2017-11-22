#!/bin/bash

pkill -9 navire
killall -9 navire > /dev/null 2>&1


if [ $1 -eq 10 ] 
then
    rm ../Fichiers/mer.bin
    cp ../Fichiers/mer_10_10_vierge.bin ../Fichiers/mer.bin
fi

if [ $1 -eq 5 ] 
then
rm ../Fichiers/mer.bin
cp ../Fichiers/mer_5_5_vierge.bin ../Fichiers/mer.bin
fi


exit 0
