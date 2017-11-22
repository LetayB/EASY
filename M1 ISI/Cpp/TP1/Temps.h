#ifndef TEMPS_H
#define TEMPS_H

#include <iostream>	
#include <string> 

class Temps{

public:
	Temps();
	Temps(float);
	float getTimeInSecond(); 
	void addTime(Temps);
	void setTime(float);
	void print();

private:
	float second;

};

#endif