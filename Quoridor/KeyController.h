// EventController.h
#pragma once
#include <osgGA/GUIEventAdapter>
#include <osgGA/EventVisitor>
#include "Model.h"

using namespace osg;
using namespace osgGA;

// Classe qui va servir � d�finir un "event callback" li� aux actions de l'utilisateur
class KeyController :
	public NodeCallback
{
public:
	KeyController();
	// C'est l'op�rateur ci-dessous, h�rit� de NodeCallback, qui doit �tre red�fini
	// afin de g�rer des �v�nements li�s � un Node dans le cadre du parcours d'un NodeVisitor
	virtual void operator()(Node* node, NodeVisitor* nv);
	//~EventController();
};

