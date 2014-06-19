// IA.cpp
#pragma once
#include "IA.h"

using namespace std;

size_t IA::Hash::operator()(const Case& c) const{
	return ((51 + std::hash<int>()(c.x)) * 51 + std::hash<int>()(c.y));
}

//Implémente l'algorithme A*
bool IA::estBloqué(Case &c, Model &m, int pion){
	unordered_set<Case, Hash> V;
	stack<Case> F;
	F.push(c);
	Case caseCourante;
	while(!F.empty()){
		caseCourante = F.top();
		F.pop();
		V.insert(caseCourante);
		succ(caseCourante, m, F, V);
		if(caseCourante.estUnBut(pion, m)){
			return false;
		}
	}
	return true;
}

void IA::succ(Case &c, Model &m, stack<Case> &f, unordered_set<Case, Hash> &V){
	if(m.verifierMouvement(c.getX(),c.getY(), HAUT)){
		int x = c.getX();
		int y = c.getY();
		Case* c = new Case(x, y-1);
		if(V.count(*c) == 0){
			f.push(*c);
		}
	}
	if(m.verifierMouvement(c.getX(),c.getY(), BAS)){
		int x = c.getX();
		int y = c.getY();
		Case* c = new Case(x, y+1);
		if(V.count(*c) == 0){
			f.push(*c);
		}
	}
	if(m.verifierMouvement(c.getX(),c.getY(), GAUCHE)){
		int x = c.getX();
		int y = c.getY();
		Case* c = new Case(x-1, y);
		if(V.count(*c) == 0){
			f.push(*c);
		}
	}
	if(m.verifierMouvement(c.getX(),c.getY(), DROITE)){
		int x = c.getX();
		int y = c.getY();
		Case* c = new Case(x+1, y);
		if(V.count(*c) == 0){
			f.push(*c);
		}
	}
}



