#pragma once
#include <osgGA/GUIEventAdapter>
#include <osgGA/EventVisitor>
#include "Model.h"

using namespace osg;
using namespace osgGA;

// Classe qui va servir à définir un "event callback" lié au click sur le bouton graphique
class ButtonArrowController :
	public NodeCallback
{
public:
	ButtonArrowController();
	// C'est l'opérateur ci-dessous, hérité de NodeCallback, qui doit être redéfini
	// afin de gérer des évènements liés à un Node dans le cadre du parcours d'un NodeVisitor
	virtual void operator()(Node* node, NodeVisitor* nv);
	//~EventController();
};
