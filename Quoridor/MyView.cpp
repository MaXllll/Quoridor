// MyView.cpp
#include "MyView.h"
#include "ModeleVue.h"
#include "Model.h"
using namespace osg;
using namespace std;

// Les membres static non constants doient Ítres initialises dans le .cpp

MyView *MyView::instance = 0;


MyView::MyView(){
}

MyView *MyView::getInstance(){
	if (instance == 0)
		instance = new MyView();
	return instance;
}

ref_ptr<osgViewer::Viewer> MyView::buildSceneGraph()

{
	// On instancie les différentes entités dans l'ordre pour qu'elles référencent des bonnes instances chacunes
	Model* model = Model::getInstance();	
	model->initModel(NB_CASE);
	ModeleVue* modelV = ModeleVue::getInstance();
	model->setModeleVue();

	// Récupération de variables utilisées souvent pour qu'elles soient plus facilement utilisables
	double tailleMur = modelV->getTaille_mur();
	double tailleCase = modelV->getTaille_case();
	double tailleCaseEcart = modelV->getTaille_case_ecart();
	double tailleEcart = modelV->getTaille_ecart();
	double gauche_plateau = GAUCHE_DU_PLATEAU;
	double taillePlateau = TAILLE_PLATEAU;
	double haut_plateau = HAUT_DU_PLATEAU;

	// Definition d'un viewer et d'une fenÍtre d'ecran associee
	ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
	viewer->setUpViewInWindow(LARGEUR_ESPACE,HAUTEUR_ESPACE,LARGEURFENETRE,HAUTEURFENETRE);
	// Desactive le "manipulator" par defaut du viewer
	viewer->getCamera()->setAllowEventFocus(false);

	// *** Camera1 : l'aire de jeu, affiche la plateau, les cases, les murs, etc ***
	// Definition de la camera et de ses caracteristiques
	camera1 = new Camera;
	modelV->setCamera1(camera1);
	modelV->setProjection();
	//aspect ratio de 1.2
	camera1->setViewport(new Viewport(POS_XV1,POS_YV1,LARGEURVIEWPORT1,HAUTEURVIEWPORT1)); 
	camera1->setReferenceFrame(Camera::ABSOLUTE_RF);
	camera1->setClearColor(BEIGE);

	// Definition du parallelepipËde rectangle et d'un Geode pour l'accueillir
	ref_ptr<ShapeDrawable> plateau = new ShapeDrawable;
	plateau->setShape(new Box(POSINIT_ALL,TAILLE_PLATEAU,TAILLE_PLATEAU,EPAISSEUR_PLATEAU));
	plateau->setColor(BRUN2);
	ref_ptr<Geode> unPlateau = creationGeode(plateau.get());

	// Definition de transformations geometriques ‡ appliquer au parallelepipËde
	ref_ptr<MatrixTransform> mPlateau = new MatrixTransform;
	mPlateau->addChild(unPlateau.get());
	ref_ptr<Texture2D> textureBois = creationTexture("woodtexture1.jpg");
	mPlateau->getOrCreateStateSet()->setTextureAttributeAndModes(0,textureBois.get());
	// Associe le noeud contenant les transformations geometriques ‡ la camera1 
	camera1->addChild(mPlateau.get());

	// Definition d'une petite case (emplacement des pions)
	ref_ptr<ShapeDrawable> caseJeu = new ShapeDrawable;
	// on définit les trois positions initiales utilisées pour toutes les cases
	double xInitial = gauche_plateau+((2*tailleCase)+tailleEcart*2)+(tailleCase/2);
	double yInitial = haut_plateau-((tailleCase*2)+(tailleEcart*2)+(tailleCase/2));
	double zInitial = (EPAISSEUR_PLATEAU/2)+(EPAISSEURCASE/2);
	Vec3 posInitCase = Vec3(xInitial,yInitial,zInitial);
	// on crée une case en 0, 0, 0 avec sa taille calculée dans le modele de la vue
	caseJeu->setShape(new Box(POSINIT_ALL,tailleCase,tailleCase,EPAISSEURCASE));
	caseJeu->setColor(BRUN);
	ref_ptr<Geode> uneCaseJeu = creationGeode(caseJeu.get());

	double x = 0;
	double y = 0;
	double increment = tailleCaseEcart;
	for(int i=0;i<model->getNbrCases();i++){
		for(int j=0;j<model->getNbrCases();j++){
			ref_ptr<MatrixTransform> mTransCase = new MatrixTransform;
			//on positionne la case a la position initiale de toutes les cases
			mTransCase->setMatrix(Matrix::translate(posInitCase));
			//on le transfere ensuite a sa position calculée grace au x et y qui s'incrémentent par les tours de boucle
			mTransCase->postMult(Matrix::translate(x,y,0));
			mTransCase->getOrCreateStateSet()->setTextureAttributeAndModes(0,textureBois.get());
			mTransCase->addChild(uneCaseJeu.get());
			camera1->addChild(mTransCase.get());
			y-= increment;
		}
		x+= increment;
		y = 0;
	}
	
	//Definition d'une grande case (case de rangement)
	ref_ptr<ShapeDrawable> rectangle = new ShapeDrawable;
	double xInitialGrandeCase = gauche_plateau+((2*tailleCase)+tailleEcart*2)+(tailleCase/2);
	double yInitialGrandeCase = haut_plateau-(((tailleCase*2)+tailleEcart)/2);
	double zInitialGrandeCase = (EPAISSEUR_PLATEAU/2)+(EPAISSEURCASE/2);
	Vec3 posInitGrandeCase = Vec3(xInitialGrandeCase,yInitialGrandeCase,zInitialGrandeCase);
	//la hauteur d'une grande case correspond a 2 fois un case normale plus un écart
	rectangle->setShape(new Box(POSINIT_ALL,tailleCase,(tailleCase*2)+tailleEcart,EPAISSEURCASE));

	rectangle->setColor(BRUN);
	ref_ptr<Geode> unRectangle = creationGeode(rectangle.get());

	x = 0;
	y = 0;
	increment = tailleCaseEcart;
	// MEME fonctionnement que pour les petites cases
	for(int i=0;i<model->getNbrCases()*2;i++){
			//cases du haut
			ref_ptr<MatrixTransform> mTransGrandeCase = new MatrixTransform;
			mTransGrandeCase->addChild(unRectangle.get());
			mTransGrandeCase->getOrCreateStateSet()->setTextureAttributeAndModes(0,textureBois.get());
			mTransGrandeCase->setMatrix(Matrix::translate(posInitGrandeCase));
			mTransGrandeCase->postMult(Matrix::translate(x,y,0));			
			camera1->addChild(mTransGrandeCase.get());
			x+= increment;
			// on repart a x = 0 et on descend dans le plateau pour définir les grandes cases du bas du plateau
			if(i == model->getNbrCases()-1){
				x = 0;
				y = (-TAILLE_PLATEAU/2+((tailleCase*2)+tailleEcart)/2)*2;
			}
			
	}
	// Definition d'un mur
	ref_ptr<ShapeDrawable> mur = new ShapeDrawable;
	mur->setShape(new Box(POSINIT_ALL,tailleEcart,tailleMur,tailleCase));
	mur->setColor(BEIGE);
	ref_ptr<Geode> unMur = creationGeode(mur.get());

	// Definition d'un mur alternatif
	ref_ptr<ShapeDrawable> murAlt = new ShapeDrawable;
	murAlt->setShape(new Box(POSINIT_ALL,tailleEcart,tailleMur,tailleCase));
	murAlt->setColor(GRIS);
	ref_ptr<Geode> unMurAlt = creationGeode(murAlt.get());

	// on définit le x et y initial de tous les murs 
	double xInitialMur = -TAILLE_PLATEAU/2+((2*tailleCase)+(tailleEcart+(tailleEcart/2)));
	double yInitialMur = TAILLE_PLATEAU/2;
	double zInitialMur = (EPAISSEUR_PLATEAU/2)+(tailleCase/2);
	Vec3 posInitMur = Vec3(xInitialMur,yInitialMur,zInitialMur);
	model->setPosInit(posInitMur);

	// ce xMurT et yMurT vont eux servir décaler les positions des murs pour tous ne soient pas a la meme position 
	// au début. On les incrémente au fur et a mesure de la boucle pour constituer les deux rangées de murs
	double xMurT = 0;
	double yMurT = -(tailleMur)/2;
	double incrementMurRX =  tailleCaseEcart;
	// on boucle sur le nombre de cases + 1 car on a ce nombre de murs par rangée 
	for(int i=0;i<model->getNbrCases()+1;i++){
		// on parcourt sur le nombre de 2 correspondant au nombre de joueurs pour mettre chaque mur dans toutes les rangées (ici 2)
		for(int j=0;j<NB_JOUEUR ;j++){
			// on crée un mur avec un switch pour pouvoir changer sa couleur quand on le pose
			creationMurAvecSwitch(j,posInitMur,xMurT,yMurT,textureBois,unMur,unMurAlt);
			// on diminue le y pour qu'au deuxieme tour de la boucle on place le mur plus haut (dans les rangements du joueur2)
			// car on travaille avec un repere en haut a gauche mais nous ne voulions pas avoir des y négatifs dont ils sont inversés
			yMurT -= TAILLE_PLATEAU-((((tailleCase*2)+tailleEcart)/2)*2);
		}
		// on réajuste le x pour passer a la colonne suivante et le Y pour repasser dans les rangements du joueur1
		xMurT+= incrementMurRX;
		yMurT = -((tailleCase*2)+tailleEcart)/2;
	}

	// DEFINITION DES PIONS
	// les positions de ces pions sont initialisées dans le constructeur du modele donc pas besoin de les passer ici
	ref_ptr<MatrixTransform> pion1 = creationPion("pion1",BISTRE);
	pion1->getOrCreateStateSet()->setTextureAttributeAndModes(0,textureBois.get());
	modelV->setMatrixPion1(pion1);
	camera1->addChild(pion1);	
	ref_ptr<MatrixTransform> pion2 = creationPion("pion2",AUBURN);
	pion2->getOrCreateStateSet()->setTextureAttributeAndModes(0,textureBois.get());
	modelV->setMatrixPion2(pion2);
	camera1->addChild(pion2); 
	modelV->initPosPions();

	// Associe un "event callback" 
	pion1->setEventCallback(new KeyController);

	// *** Camera2 : pour afficher les messages du jeu ***

	// Definition de la camera et de ses caracteristiques
	ref_ptr<Camera> camera2 = new Camera;	
	camera2->setViewMatrixAsLookAt(EYE2,CENTER,UP2);
	camera2->setProjectionMatrixAsOrtho2D(-LARGESP_CAM2/2.,LARGESP_CAM2/2.,-HAUTESP_CAM2/2.,HAUTESP_CAM2/2.);
	camera2->setViewport(new Viewport(XCAM2,YCAM2,LARGEURVIEWPORT2,HAUTEURVIEWPORT2));
	camera2->setReferenceFrame(Camera::ABSOLUTE_RF);
	camera2->setClearColor(COULEURVIEWPORT23);	

	// Definition d'un Geode pour accueillir le message du Tour	
	ref_ptr<Geode> messageTour = creationGeode(modelV->getTexteTour());

	// Associe le noeud contenant le message 
	camera2->addChild(messageTour.get());

	// *** Camera3 : pour afficher les boutons cliquables ***

	// Definition de la camera et de ses caracteristiques
	ref_ptr<Camera> camera3 = new Camera;
	camera3->setViewMatrixAsLookAt(EYE3,CENTER,UP3);
	camera3->setProjectionMatrixAsOrtho(-LARGESP_CAM3/2.,LARGESP_CAM3/2.,-HAUTESP_CAM3/2.,HAUTESP_CAM3/2.,NEARCAM3,FARCAM3);
	camera3->setViewport(new Viewport(XCAM3,YCAM3,LARGEURVIEWPORT3,HAUTEURVIEWPORT3));
	camera3->setReferenceFrame(Camera::ABSOLUTE_RF);
	camera3->setClearColor(COULEURVIEWPORT23);
	//On applique le callback des clicks sur la camera3 car elle contient les boutons graphiques
	camera3->setEventCallback(new ButtonArrowController);

	//Definition de la texture du bouton pour le mur
	ref_ptr<Texture2D> texture1= creationTexture("arrow.jpg");
	//Definition de la texture du bouton pour le mur
	ref_ptr<Texture2D> texture2= creationTexture("wall.png");
	//Definition de la texture du bouton pour le pion
	ref_ptr<Texture2D> texture3= creationTexture("pion.png");
	//Definition de la texture du bouton pour le check
	ref_ptr<Texture2D> texture4= creationTexture("check.png");
	//Definition de la texture du bouton pour la rotation
	ref_ptr<Texture2D> texture5= creationTexture("rotation.png");
	//Definition de la texture du bouton pour la rotation
	ref_ptr<Texture2D> textureDisquette = creationTexture("save-icon.png");
	//Definition de la texture du bouton pour le reset du jeu
	ref_ptr<Texture2D> textureReset = creationTexture("reset.png");
	//Definition de la texture du bouton pour le reset du jeu
	ref_ptr<Texture2D> textureUndo = creationTexture("undo.png");
	//Definition de la texture du bouton pour le chargement d'une sauvegarde
	ref_ptr<Texture2D> textureUpload = creationTexture("upload.png");
	//Definition de la texture du bouton pour le zoom
	ref_ptr<Texture2D> textureZoom = creationTexture("zoom.png");
	//Definition de la texture du bouton pour le dezoom
	ref_ptr<Texture2D> textureDezoom = creationTexture("dezoom.png");
	//Definition de la texture du bouton pour le passage en 3D
	ref_ptr<Texture2D> texture3d = creationTexture("3d.png");
	//Definition de la texture du bouton pour le passage en 2D
	ref_ptr<Texture2D> texture2d = creationTexture("2d.jpg");

	// Définition d'un bouton graphique de mouvement
	ref_ptr<ShapeDrawable> bouton1 = new ShapeDrawable;
	bouton1->setShape(new Box(POSINIT_ALL,XBOUTON1,YBOUTON1,ZBOUTON));
	bouton1->setColor(BLANC);

	// Définition d'un bouton graphique de mouvement alternatif
	ref_ptr<ShapeDrawable> bouton1Alt = new ShapeDrawable;
	bouton1Alt->setShape(new Box(POSINIT_ALL,XBOUTON1,YBOUTON1,ZBOUTON));
	bouton1Alt->setColor(BEIGE);

	//Definition de la geode accueillant le bouton graphique de mouvement
	ref_ptr<Geode> unBouton1 = creationGeode(bouton1);
	//Definition de la geode accueillant le bouton graphique de mouvement
	ref_ptr<Geode> unBouton1Alt = creationGeode(bouton1Alt);
	//Application de la texture sur le bouton
	unBouton1->getOrCreateStateSet()->setTextureAttributeAndModes(0,texture1.get());
	unBouton1Alt->getOrCreateStateSet()->setTextureAttributeAndModes(0,texture1.get());

	//Definition d'une matrix transform pour le bouton1
	ref_ptr<MatrixTransform> mBouton = creationBoutonType1AvecSwitch(3*(PI/2),POS_BOUTONHAUT.x(),
		POS_BOUTONHAUT.y(),POS_BOUTONHAUT.z(),HAUT,camera3,unBouton1,unBouton1Alt);
	
	double increment2 = INCR_BOUTONDEPLACEMENT;
	x = POS_BOUTONHAUT.x() - INCR_BOUTONDEPLACEMENT;
	y = Y_BOUTONSDEP;
	
	int j=0;
	//Boucle pour définir les trois autres boutons en translatant le premier a trois autres positions et dans un sens différent grace a la rotation
	for(j;j<3;j++){
		ref_ptr<MatrixTransform> mBouton = creationBoutonType1AvecSwitch(j*(PI/2),x,y,0,j+1,camera3,unBouton1,unBouton1Alt);
		x+=increment2;
	}

	// Définition d'un bouton graphique de type 2 (plus gros)
	ref_ptr<ShapeDrawable> bouton2 = new ShapeDrawable;
	bouton2->setShape(new Box(POSINIT_ALL,XBOUTON2,YBOUTON2,ZBOUTON));
	bouton2->setColor(BLANC);

	// Définition d'un bouton graphique alternatif de couleur différente pour le switch
	ref_ptr<ShapeDrawable> bouton2Alt = new ShapeDrawable;
	bouton2Alt->setShape(new Box(POSINIT_ALL,XBOUTON2,YBOUTON2,ZBOUTON));
	bouton2Alt->setColor(BEIGE);

	//Definition de la geode accueillant le bouton graphique de type 2
	ref_ptr<Geode> unBouton2 = creationGeode(bouton2.get());
	//Definition de la geode accueillant le bouton graphique alternatif de couleur différente pour le switch
	ref_ptr<Geode> unBouton2Alt = creationGeode(bouton2Alt.get());

	//Bouton pour basculer en mode mur
	creationBoutonType2AvecSwitch(-14,YBOUTON2HAUT,0,MODEMUR,texture2,camera3,unBouton2,unBouton2Alt);	
	//Bouton pour basculer en mode pion
	creationBoutonType2AvecSwitch(-12,YBOUTON2HAUT,0,MODEPION,texture3,camera3,unBouton2,unBouton2Alt);
	//Bouton pour confirmer le choix d'emplacement du mur
	creationBoutonType2AvecSwitch(5,YBOUTON2HAUT,0,POSEMUR,texture4,camera3,unBouton2,unBouton2Alt);
	//Bouton pour rotate un mur
	creationBoutonType2AvecSwitch(7,YBOUTON2HAUT,0,ROTATE,texture5,camera3,unBouton2,unBouton2Alt);
	//Bouton pour la sauvegarde
	creationBoutonType2AvecSwitch(9,YBOUTON2HAUT,0,SAVE,textureDisquette,camera3,unBouton2,unBouton2Alt);
	//Bouton pour le reset
	creationBoutonType2AvecSwitch(11,YBOUTON2HAUT,0,RESET,textureReset,camera3,unBouton2,unBouton2Alt);
	//Bouton pour le retour en arrière
	creationBoutonType2AvecSwitch(5,YBOUTON2BAS,0,UNDO,textureUndo,camera3,unBouton2,unBouton2Alt);
	//Bouton pour l'upload
	creationBoutonType2AvecSwitch(7,YBOUTON2BAS,0,UPLOAD,textureUpload,camera3,unBouton2,unBouton2Alt);
	//Bouton pour le zoom
	creationBoutonType2AvecSwitch(15,YBOUTON2HAUT,0,ZOOM,textureZoom,camera3,unBouton2,unBouton2Alt);
	//Bouton pour le dezoom
	creationBoutonType2AvecSwitch(15,YBOUTON2BAS,0,DEZOOM,textureDezoom,camera3,unBouton2,unBouton2Alt);
	//Bouton pour le changement de perspective
	creationBoutonType2AvecSwitch(-14,YBOUTON2BAS,0,DEUXD,texture2d,camera3,unBouton2,unBouton2Alt);
	//Bouton pour le changement de perspective
	creationBoutonType2AvecSwitch(-12,YBOUTON2BAS,0,TROISD,texture3d,camera3,unBouton2,unBouton2Alt);

	//Définition d'un objet 3D
	//On a choisi de créer les lignes de séparation entre le plateau et les rangements 
	createObjetPersonnalise(camera1,(taillePlateau/2.) -tailleCaseEcart-tailleCase);
	createObjetPersonnalise(camera1,(-taillePlateau/2.) +tailleCaseEcart*2);

	//definition de l'éclairage
	definitEclairageAmbient(camera1,1,GL_LIGHT1); //directionnel et ambient
	definitEclairageDiffuse(camera1,2,GL_LIGHT2); //positionnel et diffus

	// Creation d'un noeud racine de type Group chapeautant les 3 cameras
	ref_ptr<Group> root = new Group;
	root->addChild(camera1.get());
	root->addChild(camera2.get());
	root->addChild(camera3.get());

	// Demarrage du graphe de scene au noeud correspondant ‡ la camera 
	viewer->setSceneData(root.get());

	// Lancement de la boucle de rendu associee au viewer defini 
	return viewer.get();
}


ref_ptr<MatrixTransform> MyView::creationPion(char* name,Vec4 color){
	// DEFINITION DU PION
	StateSet* stateset;
	ref_ptr<ShapeDrawable> pion = new ShapeDrawable;
	pion->setShape(new Cone(POSINIT_ALL,0.4,0.9));
	pion->setColor(color);
	ref_ptr<Geode> unPion = new Geode;
	unPion->addDrawable(pion.get());
	stateset = unPion->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING,StateAttribute::ON);
	// Définit un noeud de type transformation géométrique qui gérera le déplacement du pion1
	ref_ptr<MatrixTransform> gestionPion = new MatrixTransform;	
	// Dans le graphe de scène, lie le noeud précédant à celui accueillant le parallélépipède
	gestionPion->postMult(Matrix::translate(0,0,(EPAISSEUR_PLATEAU/2)+(HAUTEUR_PION/2)));
	gestionPion->addChild(unPion);
	gestionPion->setName(name);
	return gestionPion;
}

ref_ptr<MatrixTransform> MyView::creationBoutonType2(int x, int y, int z, int numero, ref_ptr<Texture2D> texture,ref_ptr<Geode> unBouton){
	Model* model = Model::getInstance();
	ref_ptr<MatrixTransform> mBouton = new MatrixTransform;
	mBouton->setMatrix(Matrix::identity());
	mBouton->postMult(Matrix::translate(x,y,z));
	model->setPosButton(numero,x,y,z);
	mBouton->getOrCreateStateSet()->setTextureAttributeAndModes(0,texture.get());
	mBouton->addChild(unBouton);
	return mBouton;
}

ref_ptr<MatrixTransform> MyView::creationBoutonType2AvecSwitch(int x, int y, int z, int numero, ref_ptr<Texture2D> texture, ref_ptr<Camera> camera,
	ref_ptr<Geode> unBouton,ref_ptr<Geode> unBoutonAlt){
	//definitionde la matrix de base
	ref_ptr<MatrixTransform> mBouton = creationBoutonType2(x,y,z,numero,texture,unBouton);
	//definition de la deuxieme matrix transform vers laquelle on switch lorsqu'on passe la souris sur le bouton
	ref_ptr<MatrixTransform> mBoutonAlt = creationBoutonType2(x,y,z,numero,texture,unBoutonAlt);
	creerSwitchOnMatrix(camera,mBouton,mBoutonAlt);
	return mBouton;
}

ref_ptr<MatrixTransform> MyView::creationBoutonType1(double angle, double x, double y, double z, int numBouton,ref_ptr<Geode> unBouton){
	Model* model = Model::getInstance();
	ref_ptr<MatrixTransform> mBouton = new MatrixTransform;
	mBouton->setMatrix(Matrix::identity());
	mBouton->setMatrix(Matrix::rotate(angle,Z_AXIS));
	mBouton->postMult(Matrix::translate(x,y,z));
	model->setPosButton(numBouton,x,y,z);
	mBouton->addChild(unBouton);
	return mBouton;
}

ref_ptr<MatrixTransform> MyView::creationBoutonType1AvecSwitch(double angle, double x, double y, double z, int numBouton,ref_ptr<Camera> camera,ref_ptr<Geode> unBouton,ref_ptr<Geode> unBoutonAlt){
	//definitionde la matrix de base
	ref_ptr<MatrixTransform> mBouton = creationBoutonType1(angle,x,y,z,numBouton,unBouton);
	//definition de la deuxieme matrix transform vers laquelle on switch lorsqu'on passe la souris sur le bouton
	ref_ptr<MatrixTransform> mBoutonAlt = creationBoutonType1(angle,x,y,z,numBouton,unBoutonAlt);
	creerSwitchOnMatrix(camera,mBouton,mBoutonAlt);
	return mBouton;
}

// créé une matrix mur dont le fils est un switch comportant deux geodes
ref_ptr<MatrixTransform> MyView::creationMurAvecSwitch(int j,Vec3 posInitMur,double x, double y,ref_ptr<Texture2D> texture,ref_ptr<Geode> geode,ref_ptr<Geode> geode2){
	ModeleVue* modelV = ModeleVue::getInstance();
	ref_ptr<MatrixTransform> MTmur = new MatrixTransform;
	// on fait un setMatrix a la position initiale de tous les murs
	MTmur->setMatrix(Matrix::translate(posInitMur));
	// on le transfere ensuite a sa position a lui grace a xMurT et yMurT
	MTmur->postMult(Matrix::translate(x,y,0));
	// on attribue une texture bois au mur
	MTmur->getOrCreateStateSet()->setTextureAttributeAndModes(0,texture.get());
	// on crée le switch
	creerSwitchOnGeode(MTmur, geode, geode2);	
	if(j==0){
		// si on est encore dans la rangée du joueur 2, on ajoute ce mur dans la structure mursJ2 conservée dans le modele de la vue
		// et on set aussi sa position initiale pour pouvoir le remettre la ou il etait en cas de besoin
		modelV->ajouterMur(MTmur, JOUEUR2);
		modelV->ajoutVecPosMurInit(posInitMur.x()+x,posInitMur.y()+y,JOUEUR2);
	}else if(j==1){
		// idem pour le joueur1
		modelV->ajouterMur(MTmur, JOUEUR1);
		modelV->ajoutVecPosMurInit(posInitMur.x()+x,posInitMur.y()+y,JOUEUR1);
	}
	camera1->addChild(MTmur.get());
	return MTmur;
}

void MyView::creerSwitchOnGeode(ref_ptr<MatrixTransform> matrix,ref_ptr<Geode> geode,ref_ptr<Geode> geodeAlt){
	ModeleVue* modelV = ModeleVue::getInstance();
	ref_ptr<Switch> switchB = new Switch;
	// ajout des deux matrix au switch
	switchB->addChild(geode,true);
	switchB->addChild(geodeAlt,false);
	// ajout du switch a la camera
	matrix->addChild(switchB);
}

void MyView::creerSwitchOnMatrix(ref_ptr<Camera> camera,ref_ptr<MatrixTransform> mBouton,ref_ptr<MatrixTransform> mBoutonAlt){
	ModeleVue* modelV = ModeleVue::getInstance();
	ref_ptr<Switch> switchB = new Switch;
	// ajout des deux matrix au switch
	switchB->addChild(mBouton,true);
	switchB->addChild(mBoutonAlt,false);
	// ajout du switch a la camera
	camera->addChild(switchB);
	modelV->ajouterSwitch(switchB);
}

ref_ptr<Texture2D> MyView::creationTexture(char* path){
	// Lecture d'un fichier contenant l'image à utiliser pour les textures
	ref_ptr<Image> image = osgDB::readImageFile(path);
	if (!image) {
		std::cout << "Couldn't load texture." << std::endl;
		return NULL;
	}

	//Definition de la texture du bouton pour le mur
	ref_ptr<Texture2D> texture= new Texture2D;
	texture->setImage(image);
	texture->setWrap(Texture::WRAP_S,Texture::CLAMP);
	texture->setWrap(Texture::WRAP_T,Texture::CLAMP);
	return texture;
}

ref_ptr<Geode> MyView::creationGeode(ref_ptr<Drawable> objet){
	StateSet* stateset;
	ref_ptr<Geode> unBouton = new Geode;
	stateset = unBouton->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING,StateAttribute::ON);
	unBouton->addDrawable(objet.get());
	return unBouton;
}

ref_ptr<Camera> MyView::getCamera1(){
	return camera1;	
}

//definit  un éclairage ambient
void MyView::definitEclairageAmbient(ref_ptr<Camera> camera, int numLight,StateAttribute::GLMode numMode){
	ref_ptr<Light> light = new Light;
	light->setLightNum(numLight);
	light->setPosition(Vec4(0,0,2,0)); 
	light->setAmbient(Vec4(0.3,0.3,0.3,1));
	light->setDiffuse(Vec4(0,0,0,1));
	light->setSpecular(Vec4(0,0,0,1));
	associeLightASource(light,camera);
	camera->getOrCreateStateSet()->setMode(numMode,StateAttribute::ON); // Active la lumière
}

//definit un éclairage diffus
void MyView::definitEclairageDiffuse(ref_ptr<Camera> camera, int numLight,StateAttribute::GLMode numMode){
	ref_ptr<Light> light = new Light;
	light->setLightNum(numLight);
	light->setPosition(Vec4(-2,-2,2,1));
	light->setAmbient(Vec4(0,0,0,1));
	light->setDiffuse(Vec4(0.4,0.4,0.4,1));
	light->setSpecular(Vec4(0,0,0,1));
	associeLightASource(light,camera);
	camera->getOrCreateStateSet()->setMode(numMode,StateAttribute::ON); // Active la lumière
}

// associe un objet light a un objet lightsource
ref_ptr<LightSource> MyView::associeLightASource(ref_ptr<Light> light,ref_ptr<Camera> camera){
	ref_ptr<LightSource> source = new LightSource;
	source->setLight(light);
	if(light->getLightNum() == 0){
		source->setReferenceFrame(LightSource::ABSOLUTE_RF);
	}
	camera->addChild(source);	
	return source;
}
//Méthode qui crée la ligne de séparation entre le rangement et les cases de jeu en définissant entièrement l'objet 3D
// Le deuxieme paramètre est pour définir la ligne du haut ou la ligne du bas
void MyView::createObjetPersonnalise(ref_ptr<Camera> camera, double yBoiteP){

	ModeleVue* modelV = ModeleVue::getInstance();
	ref_ptr<Vec3Array> vertexArray = new Vec3Array;
	double tailleEcart = modelV->getTaille_ecart();
	double tailleCase = modelV->getTaille_case();
	double tailleCaseEcart = modelV->getTaille_case_ecart();
	double tailleMur = modelV->getTaille_mur();	
	double taillePlateau = TAILLE_PLATEAU;
	double xBoite = (taillePlateau/2.) - 2*tailleCaseEcart; 
	double yBoite = yBoiteP; 
	double zBoite = EPAISSEURCASE/2+EPAISSEUR_PLATEAU/2;
	ref_ptr<Geometry> rectanglePerso = new Geometry;
	//BIND_OVERALL sur la color car je ne peux pas de différenciation des couleurs par sommet
	rectanglePerso->setColorBinding(Geometry::BIND_OVERALL);
	rectanglePerso->setNormalBinding(Geometry::BIND_OVERALL);

	ref_ptr<Vec4Array> colorArray = new Vec4Array;
	colorArray->push_back(BEIGE);
	rectanglePerso->setColorArray(colorArray);

	//rectanglePerso->setNormalArray(normalArray);
	//4 points formant la 'base' de la forme
	vertexArray->push_back(Vec3(xBoite,yBoite,zBoite)); //down back right 0
	vertexArray->push_back(Vec3(-xBoite,yBoite-tailleEcart,zBoite)); //down front left 1
	vertexArray->push_back(Vec3(-xBoite,yBoite,zBoite)); //down back left 2
	vertexArray->push_back(Vec3(xBoite,yBoite-tailleEcart,zBoite)); //down front right 3
	
	//4 points formant le 'toit' de la forme
	vertexArray->push_back(Vec3(xBoite,yBoite,zBoite+tailleEcart)); //top back right 4
	vertexArray->push_back(Vec3(-xBoite,yBoite-tailleEcart,zBoite+tailleEcart)); //top front left 5
	vertexArray->push_back(Vec3(-xBoite,yBoite,zBoite+tailleEcart)); //top back left 6
	vertexArray->push_back(Vec3(xBoite,yBoite-tailleEcart,zBoite+tailleEcart));	//top front right 7

	rectanglePerso->setVertexArray(vertexArray);
	
	//ref_ptr<Vec3Array> normalArray = new Vec3Array;
	//normalArray->push_back(Vec3(0,0,1));


	// Explicite comment il faut utiliser les sommets pour créer la surface requise 
	// On respecte bien l'ordre anti-horlogique
	// On crée chacunes des faces pour créer l'objet en 3D
	ref_ptr<DrawElementsUInt> base = new DrawElementsUInt(GL_QUADS);
	base->push_back(3);
	base->push_back(0);
	base->push_back(2);
	base->push_back(1);	
	rectanglePerso->addPrimitiveSet(base);
	ref_ptr<DrawElementsUInt> toit = new DrawElementsUInt(GL_QUADS);
	toit->push_back(7);
	toit->push_back(4);
	toit->push_back(6);
	toit->push_back(5);
	rectanglePerso->addPrimitiveSet(toit);
	ref_ptr<DrawElementsUInt> back = new DrawElementsUInt(GL_QUADS);
	back->push_back(0);
	back->push_back(4);
	back->push_back(6);
	back->push_back(2);
	rectanglePerso->addPrimitiveSet(back);
	ref_ptr<DrawElementsUInt> front = new DrawElementsUInt(GL_QUADS);
	front->push_back(3);
	front->push_back(7);
	front->push_back(5);
	front->push_back(1);
	rectanglePerso->addPrimitiveSet(front);
	ref_ptr<DrawElementsUInt> left = new DrawElementsUInt(GL_QUADS);
	left->push_back(1);
	left->push_back(5);
	left->push_back(6);
	left->push_back(2);
	rectanglePerso->addPrimitiveSet(left);
	ref_ptr<DrawElementsUInt> right = new DrawElementsUInt(GL_QUADS);
	right->push_back(3);
	right->push_back(7);
	right->push_back(4);
	right->push_back(0);
	rectanglePerso->addPrimitiveSet(right);

	//Instanciation de la Geometry	
	ref_ptr<Geode> unRectanglePerso = new Geode;
	StateSet* stateset;
	stateset = unRectanglePerso->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING,StateAttribute::OFF);
	unRectanglePerso->addDrawable(rectanglePerso.get());

	// ne fait pas ce qu'on veut mais la couleur rend bien avec la texture dessus
	ref_ptr<Texture2D> textureBois = creationTexture("woodtexture1.jpg");
	unRectanglePerso->getOrCreateStateSet()->setTextureAttributeAndModes(0,textureBois.get());

	camera->addChild(unRectanglePerso);
}