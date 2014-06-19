//Case.h
#pragma once
#include "Model.h"

class Case
{
private:


public:
	Case(int, int);
	Case();
	int getX();
	int getY();
	int x;
	int y;
	bool estUnBut(int pion, Model &m);
	bool equals(const Case&) const;
	bool operator == (Case c)const{ return equals(c);};
	static bool equal_to (Case & c1, Case & c2){ return c1.equals(c2);};
};