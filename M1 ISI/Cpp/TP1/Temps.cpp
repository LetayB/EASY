#include "Temps.h"

using namespace std;

Temps::Temps()
{ 
    second = 0.0;
}

Temps::Temps(float s)
{
    second = s;
}

void Temps::addTime(Temps t){
	second += t.getTimeInSecond();
}

float Temps::getTimeInSecond(){
	return second;
}
void Temps::setTime(float s){
   	second = s;
}

void Temps::print(){
   	cout << " " << "	" << "m " << second << "ss" << std::endl;
}



