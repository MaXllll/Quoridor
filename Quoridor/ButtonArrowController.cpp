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
	Model* model = Model::getInstance(); // R�cup�re le mod�le
	ModeleVue* modelV = ModeleVue::getInstance(); // R�cup�re le mod�le	
	//La classe osgGA::GUIEventAdapter g�re tous les types d'�v�nemens qui peuvent survenir
	std::list<ref_ptr<GUIEventAdapter>> events; // Structure de type liste (librairie standard)
	EventVisitor* ev = dynamic_cast<EventVisitor*>(nv); // Particularise le "visiteur" de noeuds
	if(ev) events = ev->getEvents(); // R�cup�re les "�v�nements" dans la liste
	if(!ev){
		cout<<"ev mal initialis�"<<endl;	
	}
	ref_ptr<GUIEventAdapter> ea = events.back(); // R�cup�re le dernier �l�ment de la liste
	events.pop_back(); // Supprime l'�l�ment r�cup�r� de la liste

	// boolean servant a d�terminer si le mvt doit etre conserv� dans la table des couts jou�s
	int coupAEnregister = AUCUN;
	switch(ea->getEventType()){
	case GUIEventAdapter::MOVE:
		//magic don't touch , ne fonctionne pas sans... 
		if(true){
			// Intersecteur � partir du point de l'�cran (en pixels) au moment du click
			ref_ptr<LineSegmentIntersector> intersector = new LineSegmentIntersector(Intersector::WINDOW,ea->getX(),ea->getY());
			// Visite des noeuds sp�cifi�s pour d�terminer si des Drawables entrent en intersection
			// avec la "ligne de mire" du click de la souris
			IntersectionVisitor iv(intersector.get());
			// chaque noeud visit� sera trait� pour la s�lection via un click de la souris
			node->accept(iv);
			// si au moins un Drawable est s�lectionn�
			if(intersector->containsIntersections()){
				// on r�cup�re le plus proche (?) et ses coordonn�es dans l'univers de dessin
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
			// Intersecteur � partir du point de l'�cran (en pixels) au moment du click
			ref_ptr<LineSegmentIntersector> intersector = 
				new LineSegmentIntersector(Intersector::WINDOW,ea->getX(),ea->getY());
			// Visite des noeuds sp�cifi�s pour d�terminer si des Drawables entrent en intersection
			// avec la "ligne de mire" du click de la souris
			IntersectionVisitor iv(intersector.get());
			// chaque noeud visit� sera trait� pour la s�lection via un click de la souris
			node->accept(iv);
			// si au moins un Drawable est s�lectionn�
			if(intersector->containsIntersections()){
				// on r�cup�re le plus proche (?) et ses coordonn�es dans l'univers de dessin
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

