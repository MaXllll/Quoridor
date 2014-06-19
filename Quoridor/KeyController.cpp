// EventController.cpp
#include <osg/MatrixTransform>
#include "KeyController.h"
#include <iostream>
#include "Model.h"
#include "ModeleVue.h"

KeyController::KeyController(){
}

void KeyController::operator()(Node* node, NodeVisitor* nv)
{	
	Model* model = Model::getInstance(); // Récupère le modèle	
	ModeleVue* modelV = ModeleVue::getInstance(); // Récupère le modèle	
	bool mode = model->getMode(); // true si pion, false si mur	

	//on teste si on est dans le bon mode ou si on est pas déjà a la fin du jeu
	if(model->getFin()){
		return;
	}
	//La classe osgGA::GUIEventAdapter gère tous les types d'évènemens  qui peuvent survenir
	std::list<ref_ptr<GUIEventAdapter>> events; // Structure de type liste (librairie standard)
	EventVisitor* ev = dynamic_cast<EventVisitor*>(nv); // Particularise le "visiteur" de noeuds
	if(ev) events = ev->getEvents(); // Récupère les "évènements" dans la liste
	ref_ptr<GUIEventAdapter> ea = events.back(); // Récupère le dernier élément de la liste
	events.pop_back(); // Supprime l'élément récupéré de la liste

	// boolean servant a déterminer si le mvt doit etre conservé dans la table des couts joués
	int coupAEnregister = AUCUN;

	switch(ea->getEventType())
	{
	case(GUIEventAdapter::KEYDOWN):
		{	
			model->changeTextMode();	
			switch(ea->getKey()){

			case GUIEventAdapter::KEY_Up:
				if(mode) coupAEnregister = model->keyUp();
				else model->keyUpMur();
				break;
			case GUIEventAdapter::KEY_Left:
				if(mode) coupAEnregister = model->keyLeft();
				else model->keyLeftMur();
				break;
			case GUIEventAdapter::KEY_Down:
				if(mode) coupAEnregister = model->keyDown();
				else model->keyDownMur();
				break;
			case GUIEventAdapter::KEY_Right:			
				if(mode) coupAEnregister = model->keyRight();
				else model->keyRightMur();
				break;

			case 'm':
				if(mode){
					model->changeMode();
					model->changeTextMode();
				}
			case 'p':
				if(!mode){
					model->changeMode();
					model->changeTextMode();
				}
				break;

			case 'r':
				if(!mode){
					model->rotate();
				}
				break;

			case 'u':
				model->annuleDernierCoup();
				break;
			case 'z':
				modelV->changerDistanceDeVue(-modelV->getDeplacementCamera());
				break;
			case 's':
				modelV->changerDistanceDeVue(modelV->getDeplacementCamera());
				break;
			case 'q':
				modelV->tournerCamera(GAUCHE);
				break;
			case 'd':
				modelV->tournerCamera(DROITE);
				break;
			case 'c':
				model->load();
				break;
			case 'e':
				model->save();
				break;
			case 'n':
				model->reset();
				break;
			case 'o':
				modelV->changeModeProjection(ORTHO);
				break;
			case 'f':
				modelV->changeModeProjection(FRUSTUM);
				break;
			case GUIEventAdapter::KEY_Return:
				if(!mode){
					coupAEnregister = model->keyEnterMur();
				}
				break;

			default:
				break;
			} 
		}

	case(GUIEventAdapter::SCROLL):
		switch(ea->getScrollingMotion()){
		case GUIEventAdapter::SCROLL_DOWN:
			modelV->changerDistanceDeVue(-modelV->getDeplacementCamera());
			break;
		case GUIEventAdapter::SCROLL_UP:
			modelV->changerDistanceDeVue(modelV->getDeplacementCamera());
			break;
		default:
			break;
		}
	if(coupAEnregister!=AUCUN){
		model->enregistreCoup(coupAEnregister);	
	}
	model->finDuJeu();
	default:
		break;	
	}
	traverse(node,nv);
}


