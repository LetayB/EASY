#include <iostream>
#include "Temps.h"
#include "Coureur.h"
#include <cstdlib>
#include <time.h>

using namespace std;

int main (){   

	srand(time(0));
	Temps t;
	t.print();
	t.setTime(50);
	t.print();
	Temps t2(60);
	t2.print();
	Coureur c = Coureur(1,"Amaury");
	c.run().print();
	cout << "Hello world!" << endl;
	return 0;
} 