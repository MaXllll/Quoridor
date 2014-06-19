// quoridor.cpp
#pragma once
#include "MyView.h"
using namespace osg;

int main(void)
{
	ref_ptr<osgViewer::Viewer> viewer = MyView::getInstance()->buildSceneGraph();

	// Lancement de la boucle de rendu associée au viewer défini 
	return viewer->run();
}