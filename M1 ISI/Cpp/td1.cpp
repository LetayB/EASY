using namespace std;
#include <iostream>

class Point{
	private :
		float m_x, m_y;
	public :
		inline float getX() {return m_x;}
		inline void setX(float x) {m_x = x;}
		inline float getY() {return m_y;}
		inline void setY(float y) {m_y = y;}

};

void raz(Point p){
	p.setX(0);
	p.setY(0);
}

int main(){
	Point p1;
	p1.setX(10);
	p1.setY(10);
	raz(p1);
	cout << "x = " << p1.getX() << "y = " << p1.getY() << endl;
}