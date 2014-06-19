// MyView.h
#pragma once
#include <osgViewer/Viewer>
#include <osg/PositionAttitudeTransform>
#include <osg/Switch>
#include "Model.h"
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osgText/Text>
#include <osgViewer/Viewer>
#include <iostream>
#include "KeyController.h"
#include <Windows.h>
#include <osgDB/ReadFile>
#include "ButtonArrowController.h"
#include <math.h>
#include <osg/MatrixTransform>
#include <vector>
#include <osg/StateAttribute>
#include <osg/Switch>

using namespace osg;
using namespace std;


class MyView
{
private:
	static MyView *instance;		// Instance de la View pour le pattern singleton
	ref_ptr<Camera> camera1;		// reference la camera1 pour pouvoir y accéder
public:
	ref_ptr<osgViewer::Viewer> buildSceneGraph(); // Construit le graphe de scène
	ref_ptr<MatrixTransform> creationPion(char* nomPion,Vec4);
	ref_ptr<MatrixTransform> creationBoutonType2(int x,int y,int z,int numeroBouton,ref_ptr<Texture2D> texture, ref_ptr<Geode> unBouton);
	ref_ptr<MatrixTransform> creationBoutonType2AvecSwitch(int x,int y,int z,int numeroBouton,ref_ptr<Texture2D> texture,
		ref_ptr<Camera> camera,ref_ptr<Geode> unBouton,ref_ptr<Geode> unBouton2);
	ref_ptr<Texture2D> creationTexture(char* nom);
	void creerSwitchOnMatrix(ref_ptr<Camera> camera,ref_ptr<MatrixTransform> unBouton,ref_ptr<MatrixTransform> unBouton2); // crée un switch comportant les deux MT passées en param
	void creerSwitchOnGeode(ref_ptr<MatrixTransform> matrix,
		ref_ptr<Geode> geode,ref_ptr<Geode> geodeAlt); // crée un switch comportant les deux geodes passées en param
	ref_ptr<MatrixTransform> creationBoutonType1(double angle, double x, double y, double z, int numBouton,ref_ptr<Geode> unBouton);
	ref_ptr<MatrixTransform> creationBoutonType1AvecSwitch(double angle, double x, double y, double z, int numBouton,ref_ptr<Camera> camera,ref_ptr<Geode> unBouton,ref_ptr<Geode> unBoutonAlt);
	ref_ptr<Geode> MyView::creationGeode(ref_ptr<Drawable> objet);
	ref_ptr<MatrixTransform> creationMurAvecSwitch(int j,Vec3 posInitmur,double x, double y,
		ref_ptr<Texture2D> texture,ref_ptr<Geode> geode,ref_ptr<Geode> geodeAlt); // créé une matrix mur dont le fils est un switch comportant deux geodes
	void definitEclairageAmbient(ref_ptr<Camera> cam,int numLight,StateAttribute::GLMode num);	//definit  un éclairage ambient et directionnel
	void definitEclairageDiffuse(ref_ptr<Camera> cam,int numLight,StateAttribute::GLMode num);	//definit un éclairage diffus et positionnel
	ref_ptr<LightSource> associeLightASource(ref_ptr<Light> light,ref_ptr<Camera> camera); // associe un objet light a un objet lightsource
	void createObjetPersonnalise(ref_ptr<Camera> cam,double yBoite);
	ref_ptr<Camera> getCamera1();
	// Pattern singleton
	MyView();
	static MyView *getInstance();
};