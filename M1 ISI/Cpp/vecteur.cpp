#ifndef VECTEUR_H
#define VECTEUR_H

#include <string>  

class Vecteur{

public:

	Temps();
	Temps(float);

private:
    int x;
    int y;
    int z;


};

#endif


Temps::Temps()
{
    second = 0;
}

Temps::Temps(float s)
{
    second = s;
}