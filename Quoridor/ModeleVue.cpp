// Model.cpp

#include "ModeleVue.h"
#include <osg/ShapeDrawable>


ModeleVue *ModeleVue::instance=0;

ModeleVue::ModeleVue(){	
	model = Model::getInstance();
	taille_plateau = 15.5;
	taille_case = taille_plateau/((model->getNbrCases()+4.0)+((model->getNbrCases()+4.0)/4.0));
	taille_ecart = taille_plateau/(((model->getNbrCases()+3.0)*4.0)+(model->getNbrCases()+3.0));
	taille_case_ecart = taille_case + taille_ecart;
	taille_mur = taille_case*2+taille_ecart;
	largeurRectProj1 = 22;
	pointDEntreeCamera1 = 13;
	pointFinalCamera1 = 25;
	posIniMur[0] = -(taille_plateau/2) + (taille_case*3) + (taille_ecart*2) + (taille_ecart/2);
	posIniMur[1] = (taille_plateau/2) - ((taille_case*3) + (taille_ecart*2) + (taille_ecart/2));
	posIniMur[2] = EPAISSEUR_PLATEAU;
	posInitPion = Vec3(-(taille_plateau/2)+(2.5*taille_case)+(2*taille_ecart),(taille_plateau/2)-((taille_case*2.5)+(taille_ecart*2)),(EPAISSEUR_PLATEAU/2)+(HAUTEUR_PION/2));
	deplacementDeCamera = 2;	
	ref_ptr<osgText::Font> maPolice = osgText::readFontFile("fonts/arial.ttf"); // True Type Font

	modeDeProjection = FRUSTUM;
	eyeFrustum = EYEFRUSTUM;
	eyeOrtho = EYEORTHO;				
	up = UP;			
	positionCamera = 4;
	texteTour = new osgText::Text;
	texteTour->setFont(maPolice.get());
	texteTour->setCharacterSize(TAILLE_POLICE); // PAS des pixels
	texteTour->setPosition(Vec3(-8,-.25,0)); // PAS des pixels
	model->setText("Tour de "+model->getJoueur1());
	texteTour->setText("Tour de "+model->getJoueur1());
	texteTour->setColor(BLANC);
	texteTour->setDataVariance(Object::DYNAMIC);
	double xInitialMur = -7.75+((2*getTaille_case())+(getTaille_ecart()+(getTaille_ecart()/2)));
}

// ajoute un matrix transform d'un mur dans le vecteur mursJ1 ou mursJ2 en fonction du joueur
void ModeleVue::ajouterMur(ref_ptr<MatrixTransform> m, int joueur){
	if(joueur == 1){
		mursJ1.push_back(m);
	}else{
		mursJ2.push_back(m);
	}
}

// renvoie le dernier mur du vecteur mursJ1 ou mursJ1 en fonction du joueur recu en param
vector<ref_ptr<MatrixTransform>> ModeleVue::getMurs(int joueur){
	if(joueur == 1){
		return mursJ1;
	} else {
		return mursJ2;
	}
}


// renvoie taille_case_ecart
double ModeleVue::getTaille_case_ecart(){
	return taille_case_ecart;
}

// renvoie taille_case
double ModeleVue::getTaille_case(){
	return taille_case;
}

// renvoie taille_mur
double ModeleVue::getTaille_mur(){
	return taille_mur;
}


// renvoie ecart
double ModeleVue::getTaille_ecart(){
	return taille_ecart;
}

// renvoie largeurRectProj1
int ModeleVue::getLargeurRectProj1(){
	return largeurRectProj1;
}

// set largeurRectProj1
void ModeleVue::setLargeurRectProj1(int largeur){
	largeurRectProj1 = largeur;
}

// renvoie pointDEntreeCamera1
int ModeleVue::getPointDEntreeCamera1(){
	return pointDEntreeCamera1;
}

// renvoie pointDEntreeCamera1
void ModeleVue::setPointDEntreeCamera1(int pointEntre){
	pointDEntreeCamera1 = pointEntre;
}


// renvoie pointFinalCamera1
int ModeleVue::getPointFinalCamera1(){
	return pointFinalCamera1;
}

// renvoie deplacementDeCamera
int ModeleVue::getDeplacementCamera(){
	return deplacementDeCamera;
}

//retourne l'instance du modele vue
ModeleVue *ModeleVue::getInstance(){
	if (instance == 0)
		instance = new ModeleVue();
	return instance;
}

void ModeleVue::ajoutVecPosMurInit(double x, double y,int joueur){
	if(joueur==1){
		vecPosMurInitJ1.push_back(Vec2(x,y));
	}else{
		vecPosMurInitJ2.push_back(Vec2(x,y));
	}
}

// Methode permettant de creer un mur courant
void ModeleVue::creationMurCourant(){
	Vec2 posMurCourant = *model->getPosMurCourant();
	//on calculer les mouvements réels
	double xMur = posMurCourant.x() * taille_case_ecart;
	double yMur = posMurCourant.y() * taille_case_ecart;
	// on récupère la matrix du mur a bouger en fonction du nombre de murs restants et du joueur courant
	ref_ptr<MatrixTransform> maTransformation;
	if(model->getTour()){
		maTransformation = mursJ1[model->getMurRestantJ1()-1].get();
	}else{
		maTransformation = mursJ2[model->getMurRestantJ2()-1].get();
	}
	// on change la couleur du mur vu qu'on le place sur le plateau
	switchMur(maTransformation);
	animer(Vec3(posIniMur.x() + xMur, posIniMur.y()-yMur,(EPAISSEUR_PLATEAU/2)+(getTaille_case()/2)), maTransformation, false);
}

void ModeleVue::remettreMurAuRangement(int mur){
	ref_ptr<MatrixTransform> maTransformation;
	Vec2 vecPos;
	if(model->getTour()){
		maTransformation = mursJ1[mur].get();
		vecPos = vecPosMurInitJ1.at(mur);
	}else{
		maTransformation = mursJ2[mur].get();
		vecPos = vecPosMurInitJ2.at(mur);
	}
	// on remet le mur a la couleur des rangements
	unSwitchMur(maTransformation);
	bool pivoter = false;
	Quat q = maTransformation->getMatrix().getRotate();
	if(!q.zeroRotation()){
		pivoter = true;
	}
	animer(Vec3(vecPos,(EPAISSEUR_PLATEAU/2)+(getTaille_case()/2)), maTransformation, pivoter);
}

Vec3* ModeleVue::getPosInitMur(){
	return &posIniMur;
}

Vec3* ModeleVue::getPosInitPion(){
	return &posInitPion;
}

void ModeleVue::setMatrixPion1(ref_ptr<MatrixTransform> matrixPion1Param){
	matrixPion1 = matrixPion1Param;
}

void ModeleVue::setMatrixPion2(ref_ptr<MatrixTransform> matrixPion2Param){
	matrixPion2 = matrixPion2Param;
}

ref_ptr<MatrixTransform> ModeleVue::rotate(ref_ptr<MatrixTransform> matrix){
	matrix->postMult(Matrix::rotate(PI/2,Z_AXIS));
	return matrix;
}

void ModeleVue::reset(){
	rafraichirMurs();
	rafraichirPions();
}

//Méthode initialisant la position des pions
void ModeleVue::initPosPions(){
	Vec2 posLogiquePion1 = *(model->getPosPion1());	
	Vec3 posReelPion1 = Vec3(posInitPion[0]+(posLogiquePion1[0]*taille_case_ecart),posInitPion[1]-(posLogiquePion1[1]*taille_case_ecart),posInitPion[2]);	
	Vec2 posLogiquePion2 = *(model->getPosPion2());
	Vec3 posReelPion2 = Vec3(posInitPion[0]+(posLogiquePion2[0]*taille_case_ecart),posInitPion[1]-(posLogiquePion2[1]*taille_case_ecart),posInitPion[2]);
	matrixPion1->setMatrix(Matrix::identity()); // réinitialisation
	matrixPion1->postMult(Matrix::translate(posReelPion1));
	matrixPion2->setMatrix(Matrix::identity()); // réinitialisation
	matrixPion2->postMult(Matrix::translate(posReelPion2));
}

//Méthode qui va animer les pions en fonction des positions contenues dans le modèle
void ModeleVue::rafraichirPions(){
	Vec2 posLogiquePion1 = *(model->getPosPion1());	
	Vec3 posReelPion1 = Vec3(posInitPion[0]+(posLogiquePion1[0]*taille_case_ecart),posInitPion[1]-(posLogiquePion1[1]*taille_case_ecart),posInitPion[2]);	
	Vec2 posLogiquePion2 = *(model->getPosPion2());
	Vec3 posReelPion2 = Vec3(posInitPion[0]+(posLogiquePion2[0]*taille_case_ecart),posInitPion[1]-(posLogiquePion2[1]*taille_case_ecart),posInitPion[2]);
	animer(posReelPion1, matrixPion1, false);
	animer(posReelPion2, matrixPion2, false);
}

//Méthode qui va animer les murs en fonction des positions contenues dans le modèle
void ModeleVue::rafraichirMurs(){
	vector<vector<int>> intersections = model->getIntersections();
	//On va remplir le rangement de murs pour le joueur1
	double xMur = posIniMur.x()-taille_case_ecart;
	double yMur = posIniMur.y()-(model->getNbrCases()*taille_case_ecart);
	for(int i=0;i<model->getMurRestantJ1();i++){
		ref_ptr<MatrixTransform> maTransformation;
		maTransformation = mursJ1[i].get();
		bool pivoter = false;
		Quat q = maTransformation->getMatrix().getRotate();
		if(!q.zeroRotation()){
			pivoter = true;
		}
		unSwitchMur(maTransformation);
		animer(Vec3(xMur, yMur, posIniMur.z()),maTransformation, pivoter); 
		xMur += taille_case_ecart; 
	}
	//On va remplir le rangement de murs pour le joueur2
	xMur = posIniMur.x()-taille_case_ecart;
	yMur = posIniMur.y();
	for(int i=0;i<model->getNbrCases()+1;i++){
		ref_ptr<MatrixTransform> maTransformation;
		maTransformation = mursJ2[i].get();		bool pivoter = false;
		Quat q = maTransformation->getMatrix().getRotate();
		if(!q.zeroRotation()){
			pivoter = true;
		}
		unSwitchMur(maTransformation);
		animer(Vec3(xMur, yMur+(2*taille_case_ecart), posIniMur.z()),maTransformation, pivoter); 
		xMur += taille_case_ecart; 
	}
	//On parcours le tableau d'intersections pour connaitre la position des murs
	vector<Vec3> indiceMurs;
	for(int i=0;i<intersections.size();i++){
		for(int j=0;j<intersections.size();j++){
			int k = intersections[i][j];
			if(k != 0){
				indiceMurs.push_back(Vec3(i,j,k));
			}
		}
	}
	ref_ptr<MatrixTransform> maTransformation;
	//On va placer les murs sur le plateau de jeu
	for(int i=model->getMurRestantJ1();i<model->getNbrCases()+1;i++){
		Vec3 v = indiceMurs.back();
		indiceMurs.pop_back();
		xMur = v.x() * taille_case_ecart;
		yMur = v.y() * taille_case_ecart;
		maTransformation = mursJ1[i].get();
		bool pivoter = false;
		Quat q = maTransformation->getMatrix().getRotate();
		if(!q.zeroRotation() && v.z() == 1){
			pivoter = true;
		}else if(q.zeroRotation() && v.z() == 2){
			pivoter = true;
		}
		switchMur(maTransformation);
		animer(Vec3(posIniMur.x()+xMur,posIniMur.y()-yMur, posIniMur.z()),maTransformation, pivoter); 
	}
	for(int i=model->getMurRestantJ2();i<model->getNbrCases()+1;i++){
		Vec3 v = indiceMurs.back();
		indiceMurs.pop_back();
		xMur = v.x() * taille_case_ecart;
		yMur = v.y() * taille_case_ecart;
		maTransformation = mursJ2[i].get();
		bool pivoter = false;
		Quat q = maTransformation->getMatrix().getRotate();
		if(!q.zeroRotation() && v.z() == 1){
			pivoter = true;
		}else if(q.zeroRotation() && v.z() == 2){
			pivoter = true;
		}
		switchMur(maTransformation);
		animer(Vec3(posIniMur.x()+xMur,posIniMur.y()-yMur, posIniMur.z()),maTransformation, pivoter); 
	}
}

//Méthode qui va animer la matrixTransform recue vers le position contenue dans le Vec3 et le pivoter si demandé
void ModeleVue::animer(Vec3 dest, ref_ptr<MatrixTransform> matrix, bool pivoter){
	ref_ptr<AnimationPath> myPath = new AnimationPath();
	Quat q = matrix->getMatrix().getRotate();
	myPath->insert(0, AnimationPath::ControlPoint(matrix->getMatrix().getTrans(), q)); // Positionnement de départ
	if(pivoter){
		Quat q2;
		if(q.zeroRotation()){
			q2 = Quat(1.57, Z_AXIS);
		}else{
			q2 = Quat(0, Z_AXIS);
		}
		myPath->insert(0.2, AnimationPath::ControlPoint(dest, q2)); // Positionnement d'arrivée
	}else{
		myPath->insert(0.2, AnimationPath::ControlPoint(dest, q)); // Positionnement d'arrivée
	}
	myPath->setLoopMode(AnimationPath::NO_LOOPING); // L'animation a lieu une seule fois
	// Le CallBack de gestion de l'animation est créé et associé au noeud de transformation ad hoc
	ref_ptr<AnimationPathCallback> myAnimationCallback = new AnimationPathCallback(myPath);
	matrix->setUpdateCallback(myAnimationCallback);
}

//Méthode qui va transformer le vec2 passé en paramètre en Vec3 et définir si il faut pivoter le mur afin d'appeler la méthode animer
void ModeleVue::animerMur(Vec2 dest, bool pivoter){
	double xDest = dest.x() * taille_case_ecart;
	double yDest = dest.y() * taille_case_ecart;
	Vec3 newPosDest = Vec3(posIniMur.x()+xDest,posIniMur.y()-yDest,posIniMur.z());
	ref_ptr<MatrixTransform> matrixMurCourant;
	if(model->getTour()){
		matrixMurCourant = mursJ1[model->getMurRestantJ1()-1].get();
	}else{
		matrixMurCourant = mursJ2[model->getMurRestantJ2()-1].get();;
	}
	if(pivoter){
		animer(newPosDest, matrixMurCourant, true);
	}else{
		animer(newPosDest, matrixMurCourant, false);
	}
}

//Méthode qui va transformer le vec2 passé en paramètre en Vec3 afin d'appler la méthode animer pour ce pion
void ModeleVue::animerPion(Vec2 dest){
	Vec3 newPosDest = Vec3(posInitPion[0]+(dest[0]*taille_case_ecart),posInitPion[1]-(dest[1]*taille_case_ecart),posInitPion[2]);
	ref_ptr<MatrixTransform> matrixPionCourant;
	if(model->getTour()){
		matrixPionCourant = matrixPion1;
	}else{
		matrixPionCourant = matrixPion2;
	}
	animer(newPosDest, matrixPionCourant, false);
}

void ModeleVue::changeText(string s){
	texteTour->setText(s);
}



ref_ptr<osgText::Text> ModeleVue::getTexteTour(){
	return texteTour;
}

//Les 2 méthodes ne sont pas utilisée pour l'instant
//set la camera passée en parametre
void ModeleVue::setCamera1(ref_ptr<Camera> camera){
	camera1 = camera;
}

//permet d'inverser la vue pour avoir une vue différenciée pour les deux joueurs ( A AMELIORER)
void ModeleVue::switchCamera(){
	if(model->getTour()){
		Vec3 eye = Vec3(0,-7,11);
		Vec3 up = Vec3(0,1,0);
		Vec3 center = Vec3(0,0,0);
		camera1->setViewMatrixAsLookAt(eye,center,up);	
	}else{
		Vec3 eye = Vec3(0,7,11);
		Vec3 up = Vec3(0,-1,0);
		Vec3 center = Vec3(0,0,0);
		camera1->setViewMatrixAsLookAt(eye,center,up);
	}
}

//permet de zoomer et dezoomer
void ModeleVue::changerDistanceDeVue(int changement){
	if(largeurRectProj1+changement>17 && largeurRectProj1+changement<35){
		setLargeurRectProj1(largeurRectProj1+changement);
		setProjection();
	}
}

//passe de ortho a frustum et vice versa
void ModeleVue::changeModeProjection(int mode){
	if(modeDeProjection == mode){
		model->changeText("Vous êtes déjà dans ce mode de projection");
	}else{
		modeDeProjection = mode;
		setProjection();
	}
}

//set la projection en fonction des variables de la classe
void ModeleVue::setProjection(){
	if(modeDeProjection == FRUSTUM){
		camera1->setViewMatrixAsLookAt(eyeFrustum,CENTER,up);
		camera1->setProjectionMatrixAsFrustum(-largeurRectProj1/2,largeurRectProj1/2,-largeurRectProj1/2,
			largeurRectProj1/2,pointDEntreeCamera1,pointFinalCamera1);
	}else{
		camera1->setViewMatrixAsLookAt(eyeOrtho,CENTER,up);
		// on enleve 10 pour que ca soit adapté a l'ortho
		int largeurRectProj = largeurRectProj1-10;
		camera1->setProjectionMatrixAsOrtho(-largeurRectProj,largeurRectProj,-largeurRectProj,
			largeurRectProj,pointDEntreeCamera1,pointFinalCamera1);
	}
}

//Méthode qui s'occupe de tourner la camera en fonction du paramètre recu
void ModeleVue::tournerCamera(int sens){
	if(positionCamera==0){
		if(sens==GAUCHE) moveUpCamera(-1);
		else moveRightCamera(1);
	}else if(positionCamera>0 && positionCamera<8 ){		
		if(sens==GAUCHE) moveLeftCamera(-1);
		else moveRightCamera(1);
	}else if(positionCamera==8){
		if(sens==GAUCHE) moveLeftCamera(-1);
		else moveUpCamera(1);
	}else if(positionCamera>8 && positionCamera<16){
		if(sens==GAUCHE) moveDownCamera(-1);
		else moveUpCamera(1);
	}else if(positionCamera==16){
		if(sens==GAUCHE) moveDownCamera(-1);
		else moveLeftCamera(1);
	}else if(positionCamera>16 && positionCamera<24){
		if(sens==GAUCHE) moveRightCamera(-1);
		else moveLeftCamera(1);		
	}else if(positionCamera==24){
		if(sens==GAUCHE) moveRightCamera(-1);
		else moveDownCamera(1);
	}else if(positionCamera>24 && positionCamera<=31){
		if(sens==GAUCHE) moveUpCamera(-1);
		else moveDownCamera(1);
	}
	setProjection();
}
// bouge la camera vers les Y positifs
void ModeleVue::moveUpCamera(int incr){
	up = Vec3(up.x(),up.y()-INCREMENTUP,up.z());
	eyeFrustum = Vec3(eyeFrustum.x(),eyeFrustum.y()+INCREMENTEYE,eyeFrustum.z());
	incrementeCompteurCamera(incr);
}
// bouge la camera vers les Y négatifs
void ModeleVue::moveDownCamera(int incr){
	up = Vec3(up.x(),up.y()+INCREMENTUP,up.z());
	eyeFrustum = Vec3(eyeFrustum.x(),eyeFrustum.y()-INCREMENTEYE,eyeFrustum.z());
	incrementeCompteurCamera(incr);
}
// bouge la camera vers les X négatifs
void ModeleVue::moveLeftCamera(int incr){
	up = Vec3(up.x()+INCREMENTUP,up.y(),up.z());
	eyeFrustum = Vec3(eyeFrustum.x()-INCREMENTEYE,eyeFrustum.y(),eyeFrustum.z());
	incrementeCompteurCamera(incr);
}
// bouge la camera vers les X positifs
void ModeleVue::moveRightCamera(int incr){
	up = Vec3(up.x()-INCREMENTUP,up.y(),up.z());
	eyeFrustum = Vec3(eyeFrustum.x()+INCREMENTEYE,eyeFrustum.y(),eyeFrustum.z());
	incrementeCompteurCamera(incr);	
}

//méthode qui va changer les coordoné de la camera en fonction du paramètr recu
void ModeleVue::incrementeCompteurCamera(int incr){
	if(positionCamera == 0&&incr<0){
		positionCamera = 31;
	}else if(positionCamera==31&&incr>0){
		positionCamera = 0;
	}else{
		positionCamera+=incr;
	}
}

// ajoute un matrix transform d'un mur dans le vecteur mursJ1 ou mursJ2 en fonction du joueur
void ModeleVue::ajouterSwitch(ref_ptr<Switch> switchP){
	vecteurDeSwitch.push_back(switchP);
}

//La couleur du mur est changé par celle définie pour les murs sur le plateau
void ModeleVue::switchBouton(int numeroBouton){
	vecteurDeSwitch[numeroBouton].get()->setValue(0,false);
	vecteurDeSwitch[numeroBouton].get()->setValue(1,true);
}

//La couleur du mur est changé par celle définie pour les murs rangés
void ModeleVue:: unSwitchBouton(){
	for(int i=0; i<vecteurDeSwitch.size() ; i++){
		vecteurDeSwitch[i].get()->setValue(0,true);
		vecteurDeSwitch[i].get()->setValue(1,false);
	}
}
// change la couleur du mur a la couleur des murs sur le plateau
void ModeleVue::switchMur(ref_ptr<MatrixTransform> mur){
	Switch* switchB = dynamic_cast<Switch*>(mur->getChild(0));
	switchB->setValue(0,false);
	switchB->setValue(1,true);
}
// permet de remettre le mur a la couleur des murs de rangement
void ModeleVue::unSwitchMur(ref_ptr<MatrixTransform> mur){
	Switch* switchB = dynamic_cast<Switch*>(mur->getChild(0));
	switchB->setValue(0,true);
	switchB->setValue(1,false);
}