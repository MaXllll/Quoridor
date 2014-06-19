// quoridor.cpp
#pragma once
#include "MyView.h"
using namespace osg;

int main(void)
{
	ref_ptr<osgViewer::Viewer> viewer = MyView::getInstance()->buildSceneGraph();

	// Lancement de la boucle de rendu associ�e au viewer d�fini 
	return viewer->run();
}