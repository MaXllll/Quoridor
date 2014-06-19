//Case.cpp

#include "Case.h"

Case::Case(int x, int y){
	this->x = x;
	this->y = y;

}

Case::Case(){

}

int Case::getX(){
	return x;
}

int Case::getY(){
	return y;
}

bool Case::estUnBut(int pion, Model &m){
	if(pion == 1){
		return y == 0;
	}else{
		return y == m.getNbrCases()-1;
	}
}

//Redéfinition de la methode equals qui compare deux cases sur base de leur coordonnées
bool Case::equals(const Case& c) const{
	return this->x == c.x && this->y == c.y;
}