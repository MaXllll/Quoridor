#include <osg/MatrixTransform>
#include <osgUtil/LineSegmentIntersector>
#include "ButtonArrowController.h"
#include "ModeleVue.h"
//#include <iostream>

using namespace osgUtil;

ButtonArrowController::ButtonArrowController(){
}

void ButtonArrowController::operator()(Node* node, NodeVisitor* nv)
{		
	Model* model = Model::getInstance(); // Récupère le modèle
	ModeleVue* modelV = ModeleVue::getInstance(); // Récupère le modèle	
	//La classe osgGA::GUIEventAdapter gère tous les types d'évènemens qui peuvent survenir
	std::list<ref_ptr<GUIEventAdapter>> events; // Structure de type liste (librairie standard)
	EventVisitor* ev = dynamic_cast<EventVisitor*>(nv); // Particularise le "visiteur" de noeuds
	if(ev) events = ev->getEvents(); // Récupère les "évènements" dans la liste
	if(!ev){
		cout<<"ev mal initialisé"<<endl;	
	}
	ref_ptr<GUIEventAdapter> ea = events.back(); // Récupère le dernier élément de la liste
	events.pop_back(); // Supprime l'élément récupéré de la liste

	// boolean servant a déterminer si le mvt doit etre conservé dans la table des couts joués
	int coupAEnregister = AUCUN;
	switch(ea->getEventType()){
	case GUIEventAdapter::MOVE:
		//magic don't touch , ne fonctionne pas sans... 
		if(true){
			// Intersecteur à partir du point de l'écran (en pixels) au moment du click
			ref_ptr<LineSegmentIntersector> intersector = new LineSegmentIntersector(Intersector::WINDOW,ea->getX(),ea->getY());
			// Visite des noeuds spécifiés pour déterminer si des Drawables entrent en intersection
			// avec la "ligne de mire" du click de la souris
			IntersectionVisitor iv(intersector.get());
			// chaque noeud visité sera traité pour la sélection via un click de la souris
			node->accept(iv);
			// si au moins un Drawable est sélectionné
			if(intersector->containsIntersections()){
				// on récupère le plus proche (?) et ses coordonnées dans l'univers de dessin
				LineSegmentIntersector::Intersection result = intersector->getFirstIntersection();
				float x = result.getWorldIntersectPoint().x();
				float y = result.getWorldIntersectPoint().y();
				coupAEnregister=model->testIntersectButton(x,y,MOVE);		
			}else{
				modelV->unSwitchBouton();
			}
		}
		break;
	case GUIEventAdapter::PUSH: // Pression sur la souris
		if(ea->getButton()==GUIEventAdapter::LEFT_MOUSE_BUTTON){ // bouton du milieu (roulette)
			// Intersecteur à partir du point de l'écran (en pixels) au moment du click
			ref_ptr<LineSegmentIntersector> intersector = 
				new LineSegmentIntersector(Intersector::WINDOW,ea->getX(),ea->getY());
			// Visite des noeuds spécifiés pour déterminer si des Drawables entrent en intersection
			// avec la "ligne de mire" du click de la souris
			IntersectionVisitor iv(intersector.get());
			// chaque noeud visité sera traité pour la sélection via un click de la souris
			node->accept(iv);
			// si au moins un Drawable est sélectionné
			if(intersector->containsIntersections()){
				// on récupère le plus proche (?) et ses coordonnées dans l'univers de dessin
				LineSegmentIntersector::Intersection result = intersector->getFirstIntersection();
				float x = result.getWorldIntersectPoint().x();
				float y = result.getWorldIntersectPoint().y();
				coupAEnregister=model->testIntersectButton(x,y,CLICK);		
			}
		}
		if(coupAEnregister!=AUCUN){
			model->enregistreCoup(coupAEnregister);	
		}
		model->finDuJeu();
		break;
	
	default:
		break;
	}

}

