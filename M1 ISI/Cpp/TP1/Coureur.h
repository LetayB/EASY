#ifndef COUREUR_H
#define COUREUR_H

#include <string>  
#include <iostream>
#include <cstdlib>

#include "Temps.h"

class Coureur{

public:
	Coureur();
 	Coureur(int,std::string);
	void askRegister();
	Temps run();

private:
    std::string name;
    int dossard;


};

#endif