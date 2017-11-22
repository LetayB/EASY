#include "Commissaire.h" 


void Commissaire::addRunner(Coureur &c){
	Participant p;
	p.runner = c;
	p.time = Temps();
	runners.push_back(p);
	nb_runner_register ++;
}

void Commissaire::race(){
	int distance = 0; 
	Temps tmp_time;
	while(distance <20){
		distance ++;
		for(runnersIterator = runners.begin();runnersIterator != runners.end();runnersIterator++)
		{
			runnersIterator->time.addTime(runnersIterator->runner.run()); 
		}
	}
}