#ifndef COMMISSAIRE_H
#define COMMISSAIRE_H

#define NB_MAX_RUNNER 100

#include <vector>	
#include <string>
#include "Coureur.h"  
#include "Temps.h"

struct Participant{
    Coureur runner;
    Temps time;
};

class Commissaire{ 

public:
	Commissaire();
	void addRunner(Coureur &c);
	void race();
	float endCourse();	

private:
    int nb_runner_register;
    std::vector<Participant> runners;
	std::vector<Participant>::iterator runnersIterator;


};

#endif