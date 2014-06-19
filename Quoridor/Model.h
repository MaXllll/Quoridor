// Model.h
#pragma once
#include <osg/Vec4>
#include <osgText/Text>
#include <string>
#include <iostream>
#include <osg/MatrixTransform>
#include <fstream>
#include <stack> 


using namespace osg;
using namespace std;

#define NB_BOUTONS 16

enum {MHAUT, MBAS, MGAUCHE, MDROITE,MURPOSE,AUCUN}; // 0, 1, 2, 3, 4, 5
enum {HAUT, BAS, GAUCHE, DROITE,MODEMUR,MODEPION,POSEMUR,ROTATE,SAVE,RESET,UNDO,UPLOAD,ZOOM,DEZOOM,DEUXD,TROISD}; 
enum {CLICK, MOVE}; // enum qui définit le type d'action ayant été effectuée
enum {NONE,JOUEUR1,JOUEUR2}; //j1 = 1 et j2 = 2

static const Vec4 VERT = Vec4(0,1,0,1); // constante pour definir la couleur VERT (pour le texte)

class Model
{
private:
	static Model *instance;			// Instance du Model pour le pattern singleton
	string texteTour; // texte pour les tours
	string joueur1;					// Le nom du joueur1
	string joueur2;					// Le nom du joueur2
	Vec2 positionPion1;				// Vecteur retenant la position du pion du joueur 1
	Vec2 positionPion2;				// Vecteur retenant la position du pion du joueur 2
	int nbrCases;					// nombre de cases du plateau
	bool tour;						// true pour le joueur 1 et false pour le joueur 2 
	bool mode;						// true=mode pion, false=mode mur		
	bool fin;						// flag de fin de jeu	
	int murRestantJ1;				// Le nombre de mur restant au joueur 1
	int murRestantJ2;				// Le nombre de mur restant au joueur 2
	vector<vector<int>> intersections; // Le vecteur de vecteur de int qui retient a quel endroit un mur est placé (0 -> pas de mur, 1 -> mur vertical, 2 -> mur horizontal)
	bool murCourant;				// Boolean qui permet de savoir si un mur est deja en train d'etre placé
	Vec2 posMurCourant;				// La position du mur entrain d'etre bouge
	bool orientationMurCourant;		// L'orientation du mur courant , true pour vertical et false pour horizontal
	vector<Vec3> vecteurPositionBoutons; // vecteur retenant les positions des différents boutons
	Vec3 positionButtonHaut;		// bouton haut
	Vec3 positionButtonGauche;		// bouton gauche
	Vec3 positionButtonBas;			// bouton bas
	Vec3 positionButtonDroite;		// bouton droite
	Vec3 positionButtonModePion;	// bouton pion
	Vec3 positionButtonModeMur;		// bouton mur
	Vec3 positionButtonPoseMur;		// bouton confirmation d'emplacement
	Vec3 positionButtonRotate;		// bouton rotation
	Vec3 positionButtonDisquette;	// bouton sauvegarde
	Vec3 positionButtonReset;		// bouton reset
	Vec3 positionButtonUndo;		// bouton undo
	Vec3 positionButtonUpload;		// bouton upload
	Vec3 positionButtonZoom;		// bouton zoom
	Vec3 positionButtonDezoom;		// bouton dezoom
	Vec3 positionButton2D;			// bouton perspective 2D
	Vec3 positionButton3D;			// bouton perspective 3D
	Vec3 positionMurInitial;		// position du mur en haut a gauche
	int mvtDuneCase;				// valeur d'un mouvement d'une case
	stack<int> tableCoupsJoues;		// pile des coups joués durant la partie

public:

	string getTexteTour(){ // renvoie le texte du message de tour (et des erreurs , fin de jeu etc)
		return texteTour;
	}
	void initModel(int NbrCases);	// initialise le modele en fonction du nombre de cases sur le plateau 
	void Model::setModeleVue();		// attrbution de l'instance de ModeleVue dans une méthode a part car certaines valeurs doivent etre init avant de pouvoir la récupérer
	string getJoueur1();			// renvoie le nom du joueur1
	string getJoueur2();			// renvoie le nom du joueur2
	int getMurRestantJ1();			// renvoie le nombre de murs libres restants pour le joueur 1
	int getMurRestantJ2();			// renvoie le nombre de murs libres restants pour le joueur 2	
	void setJoueur1(string j1); // set le nom du joueur1
	void setJoueur2(string j2); // set le nom du joueur2
	Vec2* getPosPion1();			// renvoie un pointeur sur le Vec2 contenant la position x,y du pion1
	void setPosPion1(int x,int y); // set la positon du pion 1 avec les param x,y
	Vec2* getPosPion2();			// renvoie un pointeur sur le Vec2 contenant la position x,y du pion2
	void setPosPion2(int x,int y); // set la positon du pion 2 avec les param x,y
	int getNbrCases();				// renvoie le nombre de cases
	bool getTour();					// renvoie le tour (vrai si joueur1 , faux si joueur 2)
	bool getMode();					// renvoie le mode activé (pion = true, mur = false)
	void changeTour();				// change le tour 
	void changeMode();				// change le mode 
	void changeTitre(string);		// Change le titre et prévient le modèle de la vue
	void changeText(string texte);	// change le message affiché (pour les tours, messages d'erreur, fin de tour etc) et prévient le modèle de la vue
	void setText(string texte);		// change le message affiché (pour les tours, messages d'erreur, fin de tour etc)
	void changeTextMode();			// change le message affiché (pour les modes)
	void setPosButton(int numero,double x,double y,double z); // set la position du bouton en fonction de son numero
	vector<vector<int>> getIntersections();
	//Les méthodes qui suivent renvoient un int pour savoir quel input a été effectué afin de l'enregister dans la liste des coups le cas échéant
	int keyUp();					// deplace le pion vers le haut
	int keyLeft();					// deplace le pion vers la gauche
	int keyRight();				// deplace le pion vers la droite
	int keyDown();					// deplace le pion vers le bas
	int keyEnterMur();			    // méthode qui permet de positionner le mur définitivement

	void keyUpMur();				// deplace le mur vers le haut
	void keyLeftMur();				// deplace le mur vers la gauche
	void keyRightMur();				// deplace le mur vers la droite
	void keyDownMur();				// deplace le mur vers le bas	

	void rotate();					// permet de faire tourner un mur
	void setPosInit(Vec3 vec);		// set la position initale du mur courant qu'on va bouger pour pouvoir le remettre la ou il etait si le joueur ne confirme pas
	void changeTextMvtImp();		// set un texte pour mouvement impossible
	void changeTextEmpImp();		// set un texte pour emplacement impossible
	void changeTextCoupAnnule();	// set un texte adapté a une annulation de coup
	void finDuJeu();				// vérifie si un des deux pions a atteint son but et si c'est la cas , change la valeur du booléen fin
	bool getFin();					// renvoie le booleen fin
	bool verifierPositionMur();		// verifie si le mur courant est bien dans le plateau de jeu (appelé par keyReturnMur)
	int testIntersectButton(double x,double y, int action); // test si les deux parametres x et y (position du click)
	ofstream ouvrirFichierE(string s);			//Ouvre le fichier pour la lecture
	ifstream ouvrirFichierL(string s);			//Ouvre le fichier pour l'ecriture
	void reset();					// méthode permettant de relancer une partie
	void save();					// méthode permettant de sauvegarder la partie
	void load();					// méthode permettant de charger une partie
	void setTour(bool);
	void setFin(bool);
	bool to_bool(std::string const& s);
	void setMurRestantJ1(int);
	void setMurRestantJ2(int);
	void setNombreCases(int);
	void setIntersections(vector<vector<int>>);
	void changeTextPionBloqué(int);	//Change le message d'erreur en fonction du pion
	// sont compris dans un des drawables de la viewport2 en faisant les tests un par un

	void initMurCourant();			// initialise le mur courant
	Vec2* getPosMurCourant();		// renvoie le mur courant

	Vec2* getPosInitPion1();		//renvoie la position initiale du pion1
	Vec2* getPosInitPion2();		//renvoie la position initiale du pion2
	void initPosInitPion1();		//initialise la position initiale du pion1
	void initPosInitPion2();		//initialise la position initiale du pion2
	bool getMurCourant();			//renvoie le booleen MurCourant indiquant si on est en train de manipuler un mur
	bool getOrientationMurCourant();
	bool pasDeMurEnCollision(int, int, int); // Teste si aucun mur n'est dans le chemin du mouvement du pion en
	//prenant seulement en paramètre la direction du mouvement, vu que le tour en cours et la position du pion sont
	//stockés dans la classe
	bool murCourantPasEnCollision(int, int, int); // Teste si le mur courant ne rentre pas en collision avec le mouvement voulu
	bool verifierMouvement(int, int, int);
	void enregistreCoup(int coupAEnregistrer); //enregistre le coup passé en paramètre
	void annuleDernierCoup();				   //annule le dernier coup effectué par le joueur en cours
	stack<int> geTableCoupsJoues();

	friend ostream &operator << (ostream& o, Model m); //Surcharge de l'opérauteur << afin d'écrire dans un fichier
	friend istream &operator>>(istream& i, Model& m); //Surcharge de l'opérauteur >> afin de lire dans un fichier
	
	// Pattern singleton
	Model();
	Model(int nbrCases);
	static Model *getInstance();
};
