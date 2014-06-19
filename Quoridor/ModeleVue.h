// Model.h
#pragma once
#include <osg/Vec4>
#include <osgText/Text>
#include <string>
#include <iostream>
#include <osg/MatrixTransform>
#include <osg/AnimationPath>
#include <cmath>
#include "Model.h"
#include "MyView.h"

using namespace osg;
using namespace std;

#define BLANC Vec4(1,1,1,1)
#define BEIGE Vec4(0.78,0.67,0.49,1)
#define NOIR Vec4(0,0,0,1)
#define BLEUCLAIR Vec4(0.66,0.92,0.99,1)
#define OCRE Vec4(.5,.3,0,1)
#define BRUN Vec4(0.18,0.11,0.05,1)	  //brun foncé	
#define BRUN2 Vec4(0.39,0.17,0.040,1) // brun moyen
#define BISTRE Vec4(0.52,0.42,0.30,1) // brun - beige
#define AUBURN Vec4(0.61,0.24,0.04,1) // brun orange
#define MADURO Vec4(0.21,0.18,0.14,1) //gris foncé
#define GRIS Vec4(0.37,0.37,0.37,1)	// gris


#define COULEURVIEWPORT23 BRUN

#define TAILLE_POLICE 1.15
#define yEyeFrustum -9
#define zEyeFrustum 12
#define EYEFRUSTUM Vec3(0,yEyeFrustum,zEyeFrustum)  // eye initial utilisé en vue frustum
#define	EYEORTHO Vec3(0,0,13)     // eye initial utilisé en vue ortho
#define	EYELIBRE Vec3(0,20,13)	  // eye initial utilisé en vue libre

#define NB_CASE 9
#define NB_JOUEUR 2

#define LARGEUR_ESPACE 32
#define HAUTEUR_ESPACE 32

#define XCAM2 0
#define YCAM2 670
#define LARGESP_CAM2 32
#define HAUTESP_CAM2 1.6

#define XCAM3 0
#define YCAM3 0
#define LARGESP_CAM3 34.28
#define HAUTESP_CAM3 4
#define NEARCAM3 0.1
#define FARCAM3 5

#define POSINIT_ALL Vec3(0,0,0)

#define POS_XV1 0
#define POS_YV1 70
#define LARGEURFENETRE 600
#define HAUTEURFENETRE 700

#define LARGEURVIEWPORT1 600
#define LARGEURVIEWPORT2 600
#define LARGEURVIEWPORT3 600

#define HAUTEURVIEWPORT1 600
#define HAUTEURVIEWPORT2 30
#define HAUTEURVIEWPORT3 70

#define POS_INIT_ALL

#define POS_BOUTONHAUT Vec3(-4.25,1,0)


#define Y_BOUTONSDEP -0.75
#define INCR_BOUTONDEPLACEMENT 1.75

#define EPAISSEUR_PLATEAU 1.0
#define EPAISSEURCASE 0.2
#define HAUTEUR_PION 0.9
#define TAILLE_PLATEAU 15.5
#define HAUT_DU_PLATEAU TAILLE_PLATEAU/2.;
#define GAUCHE_DU_PLATEAU -TAILLE_PLATEAU/2.;

#define INCREMENTEYE -(yEyeFrustum/4.)
#define INCREMENTUP 0.25

#define UP Vec3(0,1,0)				
#define CENTER Vec3(0,0,0)

#define EYE2 Vec3(0,0,1.5)
#define UP2 Vec3(0,1,0)

#define EYE3 Vec3(0,0,1.5)
#define UP3 Vec3(0,1,0)

#define XBOUTON1 1.5
#define YBOUTON1 1.5

#define XBOUTON2 1.8
#define YBOUTON2 1.8

#define ZBOUTON 0.1

#define YBOUTON2HAUT 1
#define YBOUTON2BAS -1

enum { ORTHO, FRUSTUM }; // 0, 1

class ModeleVue
{
private :	
	static ModeleVue *instance;		// Instance du Model pour le pattern singleton
	Model* model;					//instance de Model pour pouvoir appeller certaines méthodes requises
	double taille_case_ecart;		// la taille d'un ecart entre deux cases plus la taille d'une case
	double taille_case;				// la taille d'une case
	double taille_ecart;			// la taille d'un ecart entre deux cases
	double taille_mur;				// la taille d'un mur
	
	int largeurRectProj1;			//largeur du rectangle de projection de camera1
	int pointDEntreeCamera1;		//point d'entrée de la projection de la camera1
	int pointFinalCamera1;			//point final de la projection de la camera1

	Vec3 eyeFrustum;				// eye utilisé en vue frustum
	Vec3 eyeOrtho;					// eye utilisé en vue ortho
	Vec3 up;						

	int modeDeProjection;			// voir l'enum
	int positionCamera;				// permet de déterminer la position de la camera
	int directionCourante;			// indique la direction actuelle de la camera
	vector<ref_ptr<MatrixTransform>> mursJ1; // vecteur référençant les différentes matrix transform 
			// des murs du joueur1 dans les rangements pour pouvoir les supprimer une fois un mur posé
	vector<ref_ptr<MatrixTransform>> mursJ2; //	vecteur référençant les différentes matrix transform des murs
			// des murs du joueur2 dans les rangements pour pouvoir les supprimer une fois un mur posé

	Vec3 posIniMur;  // position réelle initiale d'un mur (en haut a gauche) pour pouvoir faire la conversion logique -> réelle
	Vec3 posInitPion;  // position réelle initiale d'un pion (en haut a gauche) pour pouvoir faire la conversion logique -> réelle
	double taille_plateau;			// la taille du plateau
	ref_ptr<MatrixTransform> matrixPion1;	// pointeur sur la matrix transform du pion1
	ref_ptr<MatrixTransform> matrixPion2;	// pointeur sur la matrix transform du pion2
	vector<Vec2> vecPosMurInitJ1;	//vecteur des positions des murs dans les rangements pr J1
	vector<Vec2> vecPosMurInitJ2;	//vecteur des positions des murs dans les rangements pr J2
	ref_ptr<osgText::Text> texteTour;	// ref des messages affichés lors du jeu
	ref_ptr<Camera> camera1;		// reference la camera1 pour pouvoir changer le point de vue
	int deplacementDeCamera;		//détermine les pas de zoom de camera	
	vector<ref_ptr<Switch>> vecteurDeSwitch; //vecteur retenant tous les switchs des boutons graphiques

public :	
	void ModeleVue::intModeleVue();
	void ajouterMur(ref_ptr<MatrixTransform> MTmur, int joueur); // ajoute un matrix transform d'un mur dans le vecteur mursJ1 ou mursJ2 en fonction du joueur
	vector<ref_ptr<MatrixTransform>> getMurs(int joueur); // renvoie la structure de donnee qui stock les murs du joueur 1 ou 2
	void supprimerMurRangement(int index, int joueur);	// supprime un mur en stock dans les rangements a l'indice passé en paramètre pour le joueur passé en param
	double getTaille_case_ecart();	// renvoie taille_case_ecart
	double getTaille_case();		// renvoie taille_case
	double getTaille_ecart();		// renvoie ecarts
	double getTaille_mur();			// renvoie la taille du mur
	int getLargeurRectProj1();		// renvoie la largeur du rectangle de projection
	void setLargeurRectProj1(int largeur); //set la largeur du rectangle de projection
	int getPointDEntreeCamera1();	// renvoie le point d'entree de la projection dans l'espace
	void setPointDEntreeCamera1(int pointEntree);	// set le point d'entree de la projection dans l'espace
	int getPointFinalCamera1();		// renvoie le point final de la projection dans l'espace
	void creationMurCourant();		// crée un mur courant pour pouvoir le manipuler 
	void remettreMurAuRangement(int mur);		// remettre le mur en paramètre à sa place
	Vec3* getPosInitMur(); 
	Vec3* getPosInitPion();
	void ajoutVecPosMurInit(double x, double y,int joueur);	//ajoute un Vec2 dans le vecPosMurInitJ1 correspondant
	void setMatrixPion1(ref_ptr<MatrixTransform> matrixPion1);
	void setMatrixPion2(ref_ptr<MatrixTransform> matrixPion2);
	void setMatrixMurCourant(ref_ptr<MatrixTransform> matrixMurCourantParam);
	ref_ptr<MatrixTransform> rotate(ref_ptr<MatrixTransform>);					// applique une rotation de 90° sur le mur courant
	void reset();			// relance une partie
	void changeText(string s);
	ref_ptr<osgText::Text> getTexteTour();
	// Test pour changer l'angle de vue a chaque tour mais ca rend le jeu plus confus donc pas utilisé , a améliorer
	void setCamera1(ref_ptr<Camera> camera);	//set la camera1
	int getDeplacementCamera();					//renvoie deplacement de camera
	void switchCamera();						//permet d'inverser la vue pour avoir une vue différenciée pour les deux joueurs ( A AMELIORER)
	void changerDistanceDeVue(int changement);	//permet de zoomer et dezoomer
	void changeModeProjection(int mode);				//passe de ortho a frustum et vice versa
	void setProjection();						//set la projection en fonction des variables de la classe
	void changeTextureBoutonPespect();		// change la texture du bouton de perspective
	void tournerCamera(int sens);			// tourne la camera autour du plateau
	void moveUpCamera(int incr);					// bouge la camera vers les Y positifs
	void moveDownCamera(int incr);					// bouge la camera vers les Y négatifs
	void moveLeftCamera(int incr);					// bouge la camera vers les X négatifs
	void moveRightCamera(int incr);					// bouge la camera vers les X positifs
	void incrementeCompteurCamera(int incr);		// incremente le compteur de la camera
	void rafraichirMurs();							// Méthode qui va rafraichir l'affichage pour tous les murs par rapport au modèle
	void rafraichirPions();							// Méthode qui va rafraichir l'affichir des pions par rapport au modèle
	void ajouterSwitch(ref_ptr<Switch> switchP);	// Méthode permettant d'ajout un switch dans le vecteur de switch
	void switchBouton(int numBouton);				// permet de changer le child actif du switch pour ce bouton 
	void unSwitchBouton();							// remet tous les switch a zero
	void switchMur(ref_ptr<MatrixTransform> mur);	// change la couleur du mur a la couleur des murs sur le plateau
	void unSwitchMur(ref_ptr<MatrixTransform> mur);	// permet de remettre le mur a la couleur des murs de rangement
	// Pattern singleton
	ModeleVue();
	ModeleVue(int nbrCases);
	static ModeleVue *getInstance();
	void animer(Vec3 dest, ref_ptr<MatrixTransform> matrix, bool); //Méthode s'occupant d'animer la matrixTransform recue 
	void animerMur(Vec2 dest, bool);				//Méthode qui va transformer le vec2 passé en paramètre en Vec3 et définir si il faut pivoter le mur afin d'appeler la méthode animer
	void animerPion(Vec2 dest);						//Méthode qui va transformer le vec2 passé en paramètre en Vec3 afin d'appler la méthode animer pour ce pion
	void initPosPions();
};
