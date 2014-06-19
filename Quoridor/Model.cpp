// Model.cpp

#include "Model.h"
#include <cmath>
#include <osg/MatrixTransform>
#include "ModeleVue.h"
#include <sstream>
#include "IA.h"

// Les membres static non constants doient etres initialises dans le .cpp

Model *Model::instance = 0;
ModeleVue * modelV = 0; 
// Implementation classique du pattern singleton

Model::Model(){	
	joueur1 = "Maxime";
	joueur2 =  "Mathieu";
	tour = true;
	mode = true;
	mvtDuneCase = 1;
}

//initialise le modele en fonction du nombre de cases sur le plateau 
void Model::initModel(int nombreCases){	
	fin = false;
	nbrCases = nombreCases;
	murRestantJ1 = nbrCases+1;
	murRestantJ2 = nbrCases+1;
	intersections.resize(nbrCases-1);
	int i=0;
	for(i; i<nbrCases-1;i++){
		intersections[i].resize(nbrCases-1);
	}
	int j=0;
	for(j=0; j<NB_BOUTONS; j++){
		vecteurPositionBoutons.push_back(Vec3(0,0,0));
	}
	posMurCourant[0] = 0;
	posMurCourant[1] = 0;
	murCourant = false;
	initPosInitPion1();
	initPosInitPion2();
}

void Model::setModeleVue(){
	modelV = ModeleVue::getInstance();
}

//retourne l'instance du modele
Model *Model::getInstance(){
	if (instance == 0)
		instance = new Model();
	return instance;
}

// renvoie le nom du joueur1
string Model::getJoueur1(){
	return joueur1;
}

// renvoie le nom du joueur2
string Model::getJoueur2(){
	return joueur2;
}

//renvoie le nombre de murs libres restants pour le joueur 1
int Model::getMurRestantJ1(){
	return murRestantJ1;
}

//renvoie le nombre de murs libres restants pour le joueur 2
int Model::getMurRestantJ2(){
	return murRestantJ2;
}

// set le nom du joueur1
void Model::setJoueur1(string j1){
	joueur1 = j1;
}

// set le nom du joueur2
void Model::setJoueur2(string j2){
	joueur2 = j2;
}

// renvoie un pointeur sur le Vec3 contenant la position x,y,z du pion1
Vec2* Model::getPosPion1(){
	Vec2* ptrPP1;
	ptrPP1 = &positionPion1;
	return ptrPP1;
}

// set la positon du pion 1 avec les param x,y,z
void Model::setPosPion1(int x,int y){
	positionPion1.set(x,y);
}

// renvoie un pointeur sur le Vec3 contenant la position x,y,z du pion2
Vec2* Model::getPosPion2(){
	Vec2* ptrPP2;
	ptrPP2 = &positionPion2;
	return ptrPP2;
}

// set la positon du pion 2 avec les param x,y,z
void Model::setPosPion2(int x,int y){
	positionPion2.set(x,y);
}

// renvoie le nombre de cases
int Model::getNbrCases(){
	return nbrCases;
}

// renvoie le tour (vrai si joueur1 , faux si joueur 2)
bool Model::getTour(){
	return tour;
}

// renvoie le mode activé (pion = true, mur = false)
bool Model::getMode(){
	return mode;
}

// change le tour 
void Model::changeTour(){
	tour = !tour;
	//switchCamera();
}

// change le mode 
void Model::changeMode(){
	if(!mode && murCourant){
		if(tour) modelV->remettreMurAuRangement(murRestantJ1-1);
		else modelV->remettreMurAuRangement(murRestantJ2-1);
		murCourant = false;
	}
	mode = !mode;
}

// change le message affiché (pour les tours, messages d'erreur, fin de tour etc)
void Model::changeText(string texte){
	texteTour = texte;
	modelV->changeText(texte);
}

// set le message affiché (pour les tours, messages d'erreur, fin de tour etc) mais ne change pas a l'affichage
void Model::setText(string texte){
	texteTour = texte;
}

// change le message affiché (pour les modes)
void Model::changeTextMode(){
	string s;
	if(getTour()){
		s = "Tour de "+joueur1;
	}else{
		s = "Tour de "+joueur2;
	}
	if(getMode()){
		s = s + ": Mode pion";
	}
	else{
		s = s + ": Mode mur";
	}
	changeText((char*)s.c_str());
}

// set un texte pour mouvement impossible
void Model::changeTextMvtImp(){
	if(tour){
		changeText(joueur1+" : Mouvement impossible");
	}else{
		changeText(joueur2+" : Mouvement impossible");
	}
}

// set un texte pour emplacement impossible
void Model::changeTextEmpImp(){
	if(tour){
		changeText(joueur1+" : Emplacement impossible");
	}else{
		changeText(joueur2+" : Emplacement impossible");
	}
}

// set un texte pour emplacement impossible
void Model::changeTextCoupAnnule(){
	if(tour){
		changeText("Dernier Coup annulé : Tour de "+joueur1);
	}else{
		changeText("Dernier Coup annulé : Tour du "+joueur2);
	}
}

// set la position du bouton en fonction de son numero
void Model::setPosButton(int numero, double x, double y, double z){
	vecteurPositionBoutons[numero].set(Vec3(x,y,z));
}

// test si les deux parametres x et y (position du click) sont compris dans un des drawables de la viewport2 en faisant 
// les tests un par un
int Model::testIntersectButton(double x, double y,int action){
	if(!fin){
		//Si on est au dessus du bouton haut
		if(x>vecteurPositionBoutons[HAUT].x()-0.75&&x<vecteurPositionBoutons[HAUT].x()+0.75&&y>vecteurPositionBoutons[HAUT].y()-0.75&&y<vecteurPositionBoutons[HAUT].y()+0.75){
			if(action==CLICK){
				if(mode){					
					return keyUp();
				}else{
					keyUpMur();
				}
			}else if(action==MOVE){
				modelV->switchBouton(HAUT);
			}
		//Si on est au dessus du bouton gauche
		}else if(x>vecteurPositionBoutons[BAS].x()-0.75&&x<vecteurPositionBoutons[BAS].x()+0.75&&y>vecteurPositionBoutons[BAS].y()-0.75&&y<vecteurPositionBoutons[BAS].y()+0.75){
			if(action==CLICK){
				if(mode){
					return keyLeft();
				}else{
					keyLeftMur();
				}
			}else if(action==MOVE){
				modelV->switchBouton(BAS);
			}
		//Si on est au dessus du bouton bas
		}else if(x>vecteurPositionBoutons[GAUCHE].x()-0.75&&x<vecteurPositionBoutons[GAUCHE].x()+0.75&&y>vecteurPositionBoutons[GAUCHE].y()-0.75&&y<vecteurPositionBoutons[GAUCHE].y()+0.75){
			if(action==CLICK){	
				if(mode){
					return keyDown();
				}else{
					keyDownMur();
				}
			}else if(action==MOVE){
				modelV->switchBouton(GAUCHE);
			}
		//Si on est au dessus du bouton droite
		}else if(x>vecteurPositionBoutons[DROITE].x()-0.75&&x<vecteurPositionBoutons[DROITE].x()+0.75&&y>vecteurPositionBoutons[DROITE].y()-0.75&&y<vecteurPositionBoutons[DROITE].y()+0.75){
			if(action==CLICK){
				if(mode){
					return keyRight();
				}else{
					keyRightMur();
				}
			}else if(action==MOVE){
				modelV->switchBouton(DROITE);
			}
		//Si on est au dessus du bouton pour être en mode mur
		}else if(x>vecteurPositionBoutons[MODEMUR].x()-1&&x<vecteurPositionBoutons[MODEMUR].x()+1&&y>vecteurPositionBoutons[MODEMUR].y()-1&&y<vecteurPositionBoutons[MODEMUR].y()+1){
			// si on est pas deja en mode mur alors
			if(action==CLICK){
				if(mode==true){
					changeMode();
					changeTextMode();
				}else{
					changeText("Vous êtes déjà en mode mur");	
				}
			}else if(action==MOVE){
				modelV->switchBouton(MODEMUR);
			}
		//Si on est au dessus du bouton pour être en mode pion
		}else if(x>vecteurPositionBoutons[MODEPION].x()-1&&x<vecteurPositionBoutons[MODEPION].x()+1&&y>vecteurPositionBoutons[MODEPION].y()-1&&y<vecteurPositionBoutons[MODEPION].y()+1){
			// si on est pas deja en mode pion alors
			if(action==CLICK){
				if(mode==false){
					changeMode();
					changeTextMode();
				}else{
					changeText("Vous êtes déjà en mode pion");	
				}
			}else if(action==MOVE){
				modelV->switchBouton(MODEPION);
			}
		//Si on est au dessus du bouton pour confirmer le déplacement d'un mur
		}else if(x>vecteurPositionBoutons[POSEMUR].x()-1&&x<vecteurPositionBoutons[POSEMUR].x()+1&&y>vecteurPositionBoutons[POSEMUR].y()-1&&y<vecteurPositionBoutons[POSEMUR].y()+1){
			// si on est pas en mode mur alors
			if(action==CLICK){
				if(mode==false){
					changeText("Emplacement du mur confirmé");
					return keyEnterMur();
				}else{
					changeText("Vous devez être en mode mur");	
				}
			}else if(action==MOVE){
				modelV->switchBouton(POSEMUR);
			}
		//Si on est au dessus du bouton pour pivoter le mur
		}else if(x>vecteurPositionBoutons[ROTATE].x()-1&&x<vecteurPositionBoutons[ROTATE].x()+1&&y>vecteurPositionBoutons[ROTATE].y()-1&&y<vecteurPositionBoutons[ROTATE].y()+1){
			// si on est pas en mode mur alors
			if(action==CLICK){
				if(mode==false){
					changeText("Rotation du mur confirmée");
					rotate();
				}else{
					changeText("Vous devez être en mode mur");	
				}
			}else if(action==MOVE){
				modelV->switchBouton(ROTATE);
			}
		//Si on est au dessus du bouton pour sauvegarder
		}else if(x>vecteurPositionBoutons[SAVE].x()-1&&x<vecteurPositionBoutons[SAVE].x()+1&&y>vecteurPositionBoutons[SAVE].y()-1&&y<vecteurPositionBoutons[SAVE].y()+1){
			// si on est pas en mode mur alors
			if(action==CLICK){
				save();
			}else if(action==MOVE){
				modelV->switchBouton(SAVE);
			}
		//Si on est au dessus du bouton pour annuler un coup
		}else if(x>vecteurPositionBoutons[UNDO].x()-1&&x<vecteurPositionBoutons[UNDO].x()+1&&y>vecteurPositionBoutons[UNDO].y()-1&&y<vecteurPositionBoutons[UNDO].y()+1){
			//on appelle la méthode pour annuler un coup
			if(action==CLICK){
				annuleDernierCoup();
			}else if(action==MOVE){
				modelV->switchBouton(UNDO);
			}
		//Si on est au dessus du bouton pour zoomer
		}else if(x>vecteurPositionBoutons[ZOOM].x()-1&&x<vecteurPositionBoutons[ZOOM].x()+1&&y>vecteurPositionBoutons[ZOOM].y()-1&&y<vecteurPositionBoutons[ZOOM].y()+1){
			if(action==CLICK){
				modelV->changerDistanceDeVue(-modelV->getDeplacementCamera());
			}else if(action==MOVE){
				modelV->switchBouton(ZOOM);
			}
		//Si on est au dessus du bouton pour dezoomer
		}else if(x>vecteurPositionBoutons[DEZOOM].x()-1&&x<vecteurPositionBoutons[DEZOOM].x()+1&&y>vecteurPositionBoutons[DEZOOM].y()-1&&y<vecteurPositionBoutons[DEZOOM].y()+1){
			if(action==CLICK){
				modelV->changerDistanceDeVue(modelV->getDeplacementCamera());
			}else if(action==MOVE){
				modelV->switchBouton(DEZOOM);
			}
			
		//Si on est au dessus du bouton pour être en 2d
		}else if(x>vecteurPositionBoutons[DEUXD].x()-1&&x<vecteurPositionBoutons[DEUXD].x()+1&&y>vecteurPositionBoutons[DEUXD].y()-1&&y<vecteurPositionBoutons[DEUXD].y()+1){
			if(action==CLICK){
				modelV->changeModeProjection(ORTHO);
			}else if(action==MOVE){
				modelV->switchBouton(DEUXD);
			}
		//Si on est au dessus du bouton pour être en 2d
		}else if(x>vecteurPositionBoutons[TROISD].x()-1&&x<vecteurPositionBoutons[TROISD].x()+1&&y>vecteurPositionBoutons[TROISD].y()-1&&y<vecteurPositionBoutons[TROISD].y()+1){
			if(action==CLICK){
				modelV->changeModeProjection(FRUSTUM);
			}else if(action==MOVE){
				modelV->switchBouton(TROISD);
			}
		}
	}
	if(x>vecteurPositionBoutons[RESET].x()-1&&x<vecteurPositionBoutons[RESET].x()+1&&y>vecteurPositionBoutons[RESET].y()-1&&y<vecteurPositionBoutons[RESET].y()+1){
		//On enlève tous les murs et les pions
		if(action==CLICK){
			reset();
		}else if(action==MOVE){
			modelV->switchBouton(RESET);
		}
	}else if(x>vecteurPositionBoutons[UPLOAD].x()-1&&x<vecteurPositionBoutons[UPLOAD].x()+1&&y>vecteurPositionBoutons[UPLOAD].y()-1&&y<vecteurPositionBoutons[UPLOAD].y()+1){
			if(action==CLICK){
				load();
			}else if(action==MOVE){
				modelV->switchBouton(UPLOAD);
			}
	}
	return AUCUN;
}

// bouge le pion vers le haut
int Model::keyUp(){	
	// si joueur 1
	if(tour){
		// si on est pas deja sur le bord du plateau (dans le sens ou le mouvement nous ferait sortir)
		if(positionPion1.y() != 0 && pasDeMurEnCollision(positionPion1.x(), positionPion1.y(), HAUT)){
			// si un autre pion est la ou est censé aller 
			if(positionPion1.y()-mvtDuneCase == positionPion2.y() && positionPion1.x() == positionPion2.x()){
				//si le fait de sauter de deux cases ne nous fait pas sortir du plateau
				if(positionPion1.y()-2*mvtDuneCase >= 0 && pasDeMurEnCollision(positionPion1.x(), positionPion1.y()-mvtDuneCase, HAUT)){
					//on fait deux mouvements pour sauter au dessus de l'autre
					setPosPion1(positionPion1.x(),positionPion1.y()-2*mvtDuneCase);
					modelV->animerPion(Vec2(positionPion1.x(),positionPion1.y()));
				}else{
					// si ca nous faisait sortir alors on affiche mvt impossible
					changeTextMvtImp();
					return AUCUN;
				}
			}else{
				// on fait un mouvement dans la bonne direction
				setPosPion1(positionPion1.x(),positionPion1.y()-mvtDuneCase);
				modelV->animerPion(Vec2(positionPion1.x(),positionPion1.y()));
			}
		}else{
			// si on est deja sur le bord alors on affiche mouvement impossible
			changeTextMvtImp();
			return AUCUN;
		}
		//le code ci dessous est plus ou moins le meme mais pour le joueur2	
	}else{	
		if(positionPion2.y() != 0 && pasDeMurEnCollision(positionPion2.x(), positionPion2.y(),HAUT)){
			if(positionPion2.y()-mvtDuneCase == positionPion1.y() && positionPion1.x() == positionPion2.x()){
				if(positionPion2.y()-2*mvtDuneCase >= 0 && pasDeMurEnCollision(positionPion2.x(), positionPion2.y()-mvtDuneCase, HAUT)){

					setPosPion2(positionPion2.x(),positionPion2.y()-2*mvtDuneCase);
					modelV->animerPion(Vec2(positionPion2.x(),positionPion2.y()));
				}else{
					changeTextMvtImp();
					return AUCUN;
				}
			}else{
				setPosPion2(positionPion2.x(),positionPion2.y()-mvtDuneCase);
				modelV->animerPion(Vec2(positionPion2.x(),positionPion2.y()));
			}
		}else{
			changeTextMvtImp();
			return AUCUN;
		}
	}
	// on change le tour et on change le message du tour
	this->changeTour();
	changeTextMode();
	return HAUT;
}

// bouge le pion vers la gauche
// explications voir keyUp()
int Model::keyLeft(){
	if(tour){
		if(positionPion1.x() != 0 && pasDeMurEnCollision(positionPion1.x(), positionPion1.y(),GAUCHE)){
			if(positionPion1.x()-mvtDuneCase == positionPion2.x() && positionPion1.y() == positionPion2.y()){
				if(positionPion1.x()-2*mvtDuneCase >= 0 && pasDeMurEnCollision(positionPion1.x()-mvtDuneCase, positionPion1.y(), GAUCHE)){
					setPosPion1(positionPion1.x()-2*mvtDuneCase,positionPion1.y());
					modelV->animerPion(Vec2(positionPion1.x(),positionPion1.y()));
				}else{
					changeTextMvtImp();
					return AUCUN;
				}
			}else{
				setPosPion1(positionPion1.x()-mvtDuneCase,positionPion1.y());
				modelV->animerPion(Vec2(positionPion1.x(),positionPion1.y()));
			}
		}else{
			changeTextMvtImp();
			return AUCUN;
		}	
	}else{	
		if(positionPion2.x() != 0 && pasDeMurEnCollision(positionPion2.x(), positionPion2.y(),GAUCHE)){
			if(positionPion2.x()-mvtDuneCase == positionPion1.x() && positionPion1.y() == positionPion2.y()){
				if(positionPion2.x()-2*mvtDuneCase >= 0 && pasDeMurEnCollision(positionPion2.x()-mvtDuneCase, positionPion2.y(), GAUCHE)){
					setPosPion2(positionPion2.x(),positionPion2.y());
					modelV->animerPion(Vec2(positionPion2.x()-2*mvtDuneCase,positionPion2.y()));
				}else{
					changeTextMvtImp();
					return AUCUN;
				}
			}else{
				setPosPion2(positionPion2.x()-mvtDuneCase,positionPion2.y());
				modelV->animerPion(Vec2(positionPion2.x(),positionPion2.y()));
			}
		}else{
			changeTextMvtImp();
			return AUCUN;
		}
	}
	// on change le tour et on change le message du tour
	this->changeTour();
	changeTextMode();
	return GAUCHE;
}

// bouge le pion vers la droite
// explications voir keyUp()
int Model::keyRight(){
	if(tour){
		if(positionPion1.x() != nbrCases-1 && pasDeMurEnCollision(positionPion1.x(), positionPion1.y(),DROITE)){
			if(positionPion1.x()+mvtDuneCase == positionPion2.x() && positionPion1.y() == positionPion2.y()){
				if(positionPion1.x()+2*mvtDuneCase <= nbrCases-1 && pasDeMurEnCollision(positionPion1.x()+mvtDuneCase, positionPion1.y(), DROITE)){
					setPosPion1(positionPion1.x()+2*mvtDuneCase,positionPion1.y());
					modelV->animerPion(Vec2(positionPion1.x(),positionPion1.y()));
				}else{
					changeTextMvtImp();
					return AUCUN;
				}
			}else{
				setPosPion1(positionPion1.x()+mvtDuneCase,positionPion1.y());
				modelV->animerPion(Vec2(positionPion1.x(),positionPion1.y()));
			}
		}else{
			changeTextMvtImp();
			return AUCUN;
		}	
	}else{	
		if(positionPion2.x() != nbrCases-1 && pasDeMurEnCollision(positionPion2.x(), positionPion2.y(),DROITE)){
			if(positionPion2.x()+mvtDuneCase == positionPion1.x() && positionPion1.y() == positionPion2.y()){
				if(positionPion2.x()+2*mvtDuneCase <= nbrCases-1 && pasDeMurEnCollision(positionPion2.x()+mvtDuneCase, positionPion2.y(), DROITE)){
					setPosPion2(positionPion2.x()+2*mvtDuneCase,positionPion2.y());
					modelV->animerPion(Vec2(positionPion2.x(),positionPion2.y()));
				}else{
					changeTextMvtImp();
					return AUCUN;
				}
			}else{
				setPosPion2(positionPion2.x()+mvtDuneCase,positionPion2.y());
				modelV->animerPion(Vec2(positionPion2.x(),positionPion2.y()));
			}
		}else{
			changeTextMvtImp();
			return AUCUN;
		}
	}
	// on change le tour et on change le message du tour
	this->changeTour();
	changeTextMode();
	return DROITE;
}
//mvmt vers le bas
// explications voir keyUp()
int Model::keyDown(){
	if(tour){
		if(positionPion1.y() != nbrCases-1 && pasDeMurEnCollision(positionPion1.x(), positionPion1.y(),BAS)){
			if(positionPion1.y()+mvtDuneCase == positionPion2.y() && positionPion1.x() == positionPion2.x()){
				if(positionPion1.y()+2*mvtDuneCase <= nbrCases-1 && pasDeMurEnCollision(positionPion1.x(), positionPion1.y()+mvtDuneCase, BAS)){
					setPosPion1(positionPion1.x(),positionPion1.y()+2*mvtDuneCase);
					modelV->animerPion(Vec2(positionPion1.x(),positionPion1.y()));
				}else{
					changeTextMvtImp();
					return AUCUN;
				}
			}else{
				setPosPion1(positionPion1.x(),positionPion1.y()+mvtDuneCase);
				modelV->animerPion(Vec2(positionPion1.x(),positionPion1.y()));
			}
		}else{
			changeTextMvtImp();
			return false;
		}	
	}else{	
		if(positionPion2.y() != nbrCases-1 && pasDeMurEnCollision(positionPion2.x(), positionPion2.y(),BAS)){
			if(positionPion2.y()+mvtDuneCase == positionPion1.y() && positionPion1.x() == positionPion2.x()){
				if(positionPion2.y()+2*mvtDuneCase <= nbrCases-1 && pasDeMurEnCollision(positionPion2.x(), positionPion2.y()+mvtDuneCase, BAS)){
					setPosPion2(positionPion2.x(),positionPion2.y()+2*mvtDuneCase);
					modelV->animerPion(Vec2(positionPion2.x(),positionPion2.y()));
				}else{
					changeTextMvtImp();
					return AUCUN;
				}
			}else{
				setPosPion2(positionPion2.x(),positionPion2.y()+mvtDuneCase);
				modelV->animerPion(Vec2(positionPion2.x(),positionPion2.y()));
			}
		}else{
			changeTextMvtImp();
			return AUCUN;
		}
	}
	// on change le tour et on change le message du tour
	this->changeTour();
	changeTextMode();
	return BAS;
}

//Méthode bougeant le mur courant vers le haut
void Model::keyUpMur(){
	// On initialise la position du mur courant
	if(!murCourant){
		initMurCourant();
	}else{
		if(posMurCourant.y()!=0){
			modelV->animerMur(Vec2(posMurCourant.x(), posMurCourant.y()-1), false);
			posMurCourant[1] -= 1;
		}else{
			changeTextMvtImp();
		}
	}
}

//Méthode bougeant le mur courant vers la gauche
void Model::keyLeftMur(){
	// On initialise la position du mur courant
	if(!murCourant){
		initMurCourant();
	}else{
		if(posMurCourant.x()!=0){
			modelV->animerMur(Vec2(posMurCourant.x()-1, posMurCourant.y()), false);
			posMurCourant[0] -= 1;
		}else{
			changeTextMvtImp();
		}
	}
}

//Méthode bougeant le mur courant vers la droite
void Model::keyRightMur(){
	// On initialise la position du mur courant
	if(!murCourant){
		initMurCourant();
	}else{
		if(posMurCourant.x()!=nbrCases-2){
			modelV->animerMur(Vec2(posMurCourant.x()+1, posMurCourant.y()), false);
			posMurCourant[0] += 1;
		}else{
			changeTextMvtImp();
		}
	}
}

//Méthode bougeant le mur courant vers le bas
void Model::keyDownMur(){
	// On initialise la position du mur courant
	if(!murCourant){
		initMurCourant();
	}else{
		// si on est pas sur un bord
		if(posMurCourant.y() != nbrCases-2){
			modelV->animerMur(Vec2(posMurCourant.x(), posMurCourant.y()+1), false);
			posMurCourant[1] += 1;
		}else{
			changeTextMvtImp();
		}
	}
}

//Méthode de confirmation de positonnement d'un mur
int Model::keyEnterMur(){	
	//on vérifie si la position voulue est valide
	if(verifierPositionMur()){
		// on teste si un des deux pions n'est pas bloqués si on pose ce mur
		if(!IA::estBloqué(*(new Case(positionPion1.x(), positionPion1.y())), *this, 1)){
			if(!IA::estBloqué(*(new Case(positionPion2.x(), positionPion2.y())), *this, 2)){
				//il n'y plus de mur courant puisqu'on pose celui-ci donc il restera a sa place
				murCourant = false;
				//on met a jour la structure de données retenant les emplacements des intersections
				if(orientationMurCourant) intersections[posMurCourant.x()][posMurCourant.y()] = 1;
				else intersections[posMurCourant.x()][posMurCourant.y()] = 2;
				if(tour){
					//si c'est le tour du joueur 1, on supprime le premier mur disponible dans la liste
					murRestantJ1-=1;
				}else{
					//sinn même chose pour le joueur 2
					murRestantJ2-=1;
				}
				this->changeMode();
				this->changeTour();
				changeTextMode();
				return MURPOSE;
			}else{
				changeTextPionBloqué(2);
			}
		}else{
			changeTextPionBloqué(1);
		}
	}else{
		changeTextEmpImp();
		return AUCUN;
	}
	return AUCUN;
}

void Model::changeTextPionBloqué(int i){
	if(i==1){
		changeText("Le pion 1 est bloqué!");
	}else{
		changeText("Le pion 2 est bloqué!");
	}
}
//Methode verifiant si le mur courant est a une position correcte.
bool Model::verifierPositionMur(){
	// on teste si l'intersection n'est pas deja prise
	if(intersections[posMurCourant.x()][posMurCourant.y()] != 0){		
		return false;
	}
	// si le mur est vertical , on teste si l'intersection en haut ou en bas est occupé par un mur a la verticale
	// sinon on teste si l'intersection à gauche ou à droite est occupé par un mur a l'horizontale
	// on test aussi si on est bord sur le bord d'un plateau avant de tester à coté pour ne pas faire d'out of bound 
	if(orientationMurCourant){
		if((posMurCourant.y()!=nbrCases-2&&intersections[posMurCourant.x()][posMurCourant.y()+1] == 1) || (posMurCourant.y()!=0 &&intersections[posMurCourant.x()][posMurCourant.y()-1] == 1)){
			return false;
		}
	}else{
		if((posMurCourant.x()!=nbrCases-2&&intersections[posMurCourant.x()+1][posMurCourant.y()] == 2) || (posMurCourant.x()!=0 && intersections[posMurCourant.x()-1][posMurCourant.y()] == 2)){
			return false;
		}
	}
	return true;
}

//// permet de faire tourner un mur
void Model::rotate(){	
	orientationMurCourant = !orientationMurCourant;
	modelV->animerMur(posMurCourant, true);
}

// set la position initale du mur courant qu'on va bouger pour pouvoir le remettre la ou il etait si le joueur ne confirme pas	
void Model::setPosInit(Vec3 vec){
	positionMurInitial = vec;
};

// vérifie si un des deux pions a atteint son but et si c'est la cas , change la valeur du booléen fin
void Model::finDuJeu(){
	if(positionPion1.y()==0){
		fin = true;
		changeText("Fin du jeu : "+joueur1+" a gagné!");
	}else if(positionPion2.y()==nbrCases-1){
		fin = true;
		changeText("Fin du jeu : "+joueur2+" a gagné!");
	}else{
		fin = false;	
	}
}

// renvoie le booleen fin
bool Model::getFin(){
	return fin;
}


// initialise la position du mur courant au milieu en fonction du nombre de cases
void Model::initMurCourant(){
	//l'orientation du mur est verticale a l'initialisation
	orientationMurCourant = true;
	if(!murCourant){
		// Si c'est le tour du joueur 1
		if(tour){
			//si tous les murs ont déjà été placé , on affiche un message d'erreur et on ne fait rien
			if(murRestantJ1 == 0){
				changeText(joueur1+" : Vous n'avez plus de murs disponible");
				return;
			}
			posMurCourant[0] = (int)(nbrCases/2)-1;
			posMurCourant[1] = nbrCases-2;
			murCourant = true;
			// c'est le tour du joueur 2
		}else{
			if(murRestantJ2 == 0){
				changeText(joueur2+" : Vous n'avez plus de murs disponible");
				return;
			}
			//Si un le nombre de cases est pair
			if((nbrCases%2)==0){
				posMurCourant[0] = (int)(nbrCases/2)-1;
				posMurCourant[1] = 0;
				//Si un le nombre de cases est impair on décale de un par la droite pour que ca soit symétrique
			} else {
				posMurCourant[0] = ((int)nbrCases/2);
				posMurCourant[1] = 0;
			}
			murCourant = true;
		}
	}
	modelV->creationMurCourant();
}

//Renvoie le vecteur contenant la position courante
Vec2* Model::getPosMurCourant(){
	return &posMurCourant;
}

//initialise la position du pion1
void Model::initPosInitPion1(){
	// on décale d'une case vers la droite dans le cas ou le nombre de cases est pair
	if(nbrCases%2!=0){
		positionPion1 = Vec2(nbrCases/2,nbrCases-1);
	}else{
		positionPion1 = Vec2((nbrCases/2)-1,nbrCases-1);
	}
}
//initialise la position du pion2
void Model::initPosInitPion2(){
	//ici par besoin de décaler on le mets d'office la comme ca le plateau sera symétrique
	positionPion2 = Vec2(nbrCases/2,0);
}

bool Model::getMurCourant(){
	return murCourant;
}

bool Model::getOrientationMurCourant(){
	return orientationMurCourant;
}

stack<int> Model::geTableCoupsJoues(){
	return tableCoupsJoues;
}

//Surcharge de l'opérauteur << afin d'écrire dans un fichier
ostream &operator<<(ostream& o, Model m){
	o << m.getTexteTour() << endl << m.getPosPion1()->x() << endl << m.getPosPion1()->y() << endl << m.getPosPion2()->x() << endl << m.getPosPion2()->y() << endl <<
		m.getTour() << endl << m.getFin() << endl << m.getMurRestantJ1() << endl << m.getMurRestantJ2() << endl << m.getNbrCases() << endl;
	vector<vector<int>> intersections = m.getIntersections();
	for(int i=0;i<intersections.size();i++){
		for(int j=0;j<intersections.size();j++){
			o << intersections[i][j] << " ";
		}
	}
	o << endl;
	//On va écrire dans le fichier la pile de coups joués
	//On commence par écrire sa taille
	o << m.geTableCoupsJoues().size() << endl;
	//On met tous les éléments de la pile dans un vector pour pouvoir écrire son contenu mais à l'envers
	stack<int> stack = m.geTableCoupsJoues();
	vector<int> stackV;
	while(!stack.empty()){
		stackV.push_back(stack.top());
		stack.pop();
	}
	for(int i=stackV.size()-1;i>=0;i--){
		o << stackV[i] << " ";
	}
	return o << endl;	
}

//Surcharge de l'opérauteur >> afin de lire dans un fichier
istream &operator>>(istream& i, Model& m){
	//On lit le texte du tour
	string textTour;
	getline(i, textTour);
	m.changeText(textTour);
	//On lit la position du pion 1
	string xP1x;
	getline(i, xP1x);
	string xP1y;
	getline(i, xP1y);
	int xp1x;
	istringstream (xP1x) >> xp1x;
	int xp1y;
	istringstream (xP1y) >> xp1y;
	m.setPosPion1(xp1x, xp1y);
	//On lit la position du pion 2
	string xP2x;
	getline(i, xP2x);
	string xP2y;
	getline(i, xP2y);
	int xp2x;
	istringstream (xP2x) >> xp2x;
	int xp2y;
	istringstream (xP2y) >> xp2y;
	m.setPosPion2(xp2x, xp2y);
	//On lit le boolean tour
	string stour;
	getline(i, stour);
	m.setTour(m.to_bool(stour));
	//On lit le boolean fin
	string sfin;
	getline(i, sfin);
	m.setFin(m.to_bool(sfin));
	//On lit le nombre de mur restant pour le j1
	string sMurRestantJ1;
	getline(i, sMurRestantJ1);
	int murRestantJ1;
	istringstream (sMurRestantJ1) >> murRestantJ1;
	m.setMurRestantJ1(murRestantJ1);
	//On lit le nombre de mur restant pour le j2
	string sMurRestantJ2;
	getline(i, sMurRestantJ2);
	int murRestantJ2;
	istringstream (sMurRestantJ2) >> murRestantJ2;
	m.setMurRestantJ2(murRestantJ2);
	//On lit le nombre de cases
	string sNbrCases;
	getline(i, sNbrCases);
	int nbrCases;
	istringstream (sNbrCases) >> nbrCases;
	m.setNombreCases(nbrCases);
	//On lit la structures de donnée intersections
	vector<vector<int>> intersections = m.getIntersections();
	//On redimentionne la structure de donnée en fonction de la sauvegarde
	intersections.resize(nbrCases-1);
	for(int i=0; i<nbrCases-1;i++){
		intersections[i].resize(nbrCases-1);
	}
	string inter;
	getline(i, inter);
	istringstream iss(inter);
	for(int i=0;i<intersections.size();i++){
		for(int j=0;j<intersections.size();j++){
			string sNbr;
			getline(iss, sNbr, ' ');
			int nbr;
			istringstream (sNbr) >> nbr;
			intersections[i][j] = nbr;
		}
	}
	m.setIntersections(intersections);
	//On lit le nombre de coups joués
	string sNbrCoups;
	getline(i, sNbrCoups);
	int nbrCoups;
	istringstream (sNbrCoups) >> nbrCoups;
	//on lit la stack de coups joués
	string coup;
	getline(i, coup);
	istringstream iss2(coup);
	for(int i=0;i< nbrCoups;i++){
		string sCoup;
		getline(iss2, sCoup, ' ');
		int coup;
		istringstream (sCoup) >> coup;
		m.enregistreCoup(coup);
	}
	return i;
}
vector<vector<int>> Model::getIntersections(){
	return intersections;
}

ofstream Model::ouvrirFichierE(string s){
	ofstream monfichier(s);
	return monfichier;
}

ifstream Model::ouvrirFichierL(string s){
	ifstream monfichier(s);
	return monfichier;
}

//Valeurs de direction : 0 = haut, 1 = bas, 2 = gauche , 3 = droite
bool Model::pasDeMurEnCollision(int pX, int pY, int direction){
	Vec2 positionPionEnCours;

	//switch case testant la direction recue en paramètre
	switch(direction){
	case HAUT:
		// si le pion va vers le haut on teste les deux intersections en haut a gauche et en haut a droite
		// Mais si le pion est sur la bord gauche on ne fait pas le test en haut a gauche 
		// et si le pion est sur le bord droit on ne fait pas le test en haut à droite
		// on ne teste pas si il est contre la paroi du haut car le test est juste avant dans la méthode keyUp()
		if((pX!=0&&intersections[pX-1][pY-1]==2) || (pX!=nbrCases-1&&intersections[pX][pY-1]==2)){
			return false;
		}			
		break;
	case BAS:
		if((pX!=0&&intersections[pX-1][pY]==2) || (pX!=nbrCases-1&&intersections[pX][pY]==2)){
			return false;
		}			
		break;
	case GAUCHE:
		if((pY!=0&&intersections[pX-1][pY-1]==1) || (pY!=nbrCases-1&&intersections[pX-1][pY]==1)){
			return false;
		}
		break;
	case DROITE:
		if((pY!=0&&intersections[pX][pY-1]==1) || (pY!=nbrCases-1&&intersections[pX][pY]==1)){
			return false;
		}
		break;
	default:
		break;
	}
	return true;
}

//Méthode qui vérifie si le mouvement est possible en fonction d'une direction
bool Model::verifierMouvement(int x, int y, int direction){
	switch(direction){
	case HAUT:
		if(y != 0 && pasDeMurEnCollision(x, y, HAUT) && murCourantPasEnCollision(x, y, HAUT)){
			return true;
		}
		break;
	case GAUCHE:
		if(x != 0 && pasDeMurEnCollision(x, y, GAUCHE) && murCourantPasEnCollision(x, y, GAUCHE)){
			return true;
		}
		break;
	case DROITE:
		if(x != nbrCases-1 && pasDeMurEnCollision(x, y,DROITE) && murCourantPasEnCollision(x, y, DROITE)){
			return true;
		}
		break;
	case BAS:
		if(y != nbrCases-1 && pasDeMurEnCollision(x, y,BAS) && murCourantPasEnCollision(x, y, BAS)){
			return true;
		}
		break;
	}

	return false;
}

bool Model::murCourantPasEnCollision(int x, int y, int direction){
	switch(direction){
	case HAUT:
		//Si le mur courant est horizontal
		if(!orientationMurCourant){
			//Si le mur courant au dessus a gauche ou a droite
			if((posMurCourant.x() == x-1 || posMurCourant.x() == x) && posMurCourant.y() == y-1){
				return false;
			}
		}
		break;
	case BAS:
		//Si le mur courant est horizontal
		if(!orientationMurCourant){
			//Si le mur courant en dessous a gauche ou a droite
			if((posMurCourant.x() == x-1 || posMurCourant.x() == x) && posMurCourant.y() == y){
				return false;
			}
		}
		break;
	case GAUCHE:
		//Si le mur courant est vertical
		if(orientationMurCourant){
			//Si le mur courant est a gauche, en haut ou en bas
			if((posMurCourant.y() == y-1 || posMurCourant.y() == y) && posMurCourant.x() == x-1){
				return false;
			}
		}
		break;
	case DROITE:
		//Si le mur courant est vertical
		if(orientationMurCourant){
			//Si le mur courant en dessous a droite, et haut ou en bas
			if((posMurCourant.y() == y-1 || posMurCourant.y() == y) && posMurCourant.x() == x){
				return false;
			}
		}
		break;
	}

	return true;
}

//enregistre le coup passé en paramètre dans la queue du joueur précédent (car a l'appel de la méthode on est déjà passé au joueur suivant)
void Model::enregistreCoup(int coupAEnregistre){
	tableCoupsJoues.push(coupAEnregistre);
}

//annule le dernier coup effectué par le joueur en cours
void Model::annuleDernierCoup(){
	int dernierCoup;
	int murRestant;
	//on teste d'abord si un coup a été joué
	if(tableCoupsJoues.size()!=0){
		dernierCoup = tableCoupsJoues.top();
		tableCoupsJoues.pop();
		//si tour = true c'est le tour du J1 mais on inverse vu que le dernier coup est celui de l'adversaire
		if(tour) murRestant = murRestantJ2;		
		else murRestant = murRestantJ1;
	}else{
		changeTextMvtImp();
		return;
	}		
	// on change de tour car le dernier coup joué est forcément un coup de l'adversaire sinon ca ne serait pas notre tour
	this->changeTour();
	switch(dernierCoup){
		// en fonction du coup recu, on va faire le coup inverse pour revenir en arriere
	case MHAUT:
		keyDown();
		break;
	case MBAS:
		keyUp();
		break;
	case MGAUCHE:
		keyRight();
		break;
	case MDROITE:
		keyLeft();
		break;
	case MURPOSE:
		modelV->remettreMurAuRangement(murRestant);
		if(tour) 
			murRestantJ1+=1;
		else
			murRestantJ2+=1;			
		//this->changeMode();
		this->changeTour();
		changeTextMode();
		break;
	default :
		break;
	}
	// on rechange de tour a la fin de la méthode car les méthodes keyUp/Dow/Left/Right
	// appellent changeTour() or si on a undo le coup de l'adversaire ce n'est pas a nous mais a lui de
	// pouvoir jouer
	this->changeTour();
	this->changeTextCoupAnnule();
}

void Model::setFin(bool finP){
	fin = finP;
}

void Model::setTour(bool tourP){
	tour = tourP;
}

bool Model::to_bool(std::string const& s) {
	return s != "0";
}

void Model::setMurRestantJ1(int murs){
	murRestantJ1 = murs;
}

void Model::setMurRestantJ2(int murs){
	murRestantJ2 = murs;
}

void Model::setNombreCases(int cases){
	nbrCases = cases;
}


void Model::setIntersections(vector<vector<int>> inter){
	intersections = inter;
}
// méthode permettant de relancer une partie
void Model::reset(){
	tour = true;
	mode = true;
	mvtDuneCase = 1;
	intersections.clear();
	initModel(getNbrCases());
	modelV->reset();
	changeText("Nouvelle partie !");
}
// méthode permettant de sauvegarder la partie
void Model::save(){
	ofstream fichier = ouvrirFichierE("ma_sauvegarde.txt");
	if(fichier){
		fichier << *getInstance();
		changeText("La partie a bien été sauvegardée");
	}else{
		changeText("Problème lors de la sauvegarde du fichier");
	}
}
// méthode permettant de charger une partie
void Model::load(){
	ifstream fichier = ouvrirFichierL("ma_sauvegarde.txt");
	if(fichier){
		fichier >> *getInstance();
		modelV->rafraichirMurs();
		modelV->rafraichirPions();
		changeText("La partie a bien été loadée");
	}else{
		changeText("Problème lors de la lecture du fichier");
	}
}