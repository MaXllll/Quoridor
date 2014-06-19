// IA.h
#pragma once
#include "Model.h"
#include <unordered_set>
#include <stack>
#include "Case.h"

class IA
{
private:
	struct Hash {
		size_t operator()(const Case& ) const;
	};
	static void succ(Case& c, Model& m, stack<Case>&, unordered_set<Case, Hash>&);

public:
	static bool estBloqué(Case& c, Model& m, int);

};