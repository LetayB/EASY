#include "Coureur.h"

using namespace std;

Coureur::Coureur(){
	
}
Coureur::Coureur(int d,string n){ 
	dossard = d;
	name = n;
}


Temps Coureur::run(){
	return Temps(float(rand()%10000) + float(rand()%10)/10);
}