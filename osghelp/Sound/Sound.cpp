// Sound.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulGeomGrid.h>
#include <SigmaUtil/SulAudioManager.h>
#include <SigmaUtil/SulAudioListener.h>
#include <SigmaUtil/SulAudioSource.h>
#include <SigmaUtil/SulGeomAxis.h>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>

#include "al.h"
#include "alc.h"
#include "alut.h"

#include <iostream>

CSulAudioManager* audioManager = 0;

enum ESHAPE
{
	SHAPE_SPHERE,
	SHAPE_BOX,
	SHAPE_CONE,
	SHAPE_CYLINDER,
	SHAPE_CAPSULE
};

osg::Node* createShape( const osg::Vec3& pos, ESHAPE eShape, const CSulString& bufferName )
{
	osg::Geode* geode = new osg::Geode;

	switch ( eShape )
	{
		case SHAPE_SPHERE:
			geode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),0.5f) ) );
			break;

		case SHAPE_BOX:
			geode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(2.0f,0.0f,0.0f),2.0f) ) );
			break;

		case SHAPE_CONE:
			geode->addDrawable( new osg::ShapeDrawable( new osg::Cone(osg::Vec3(4.0f,0.0f,0.0f),0.5f,3.0f) ) );
			break;

		case SHAPE_CYLINDER:
			geode->addDrawable( new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(6.0f,0.0f,0.5f),0.5f,3.0f) ) );
			break;

		case SHAPE_CAPSULE:
			geode->addDrawable( new osg::ShapeDrawable( new osg::Capsule(osg::Vec3(8.0f,0.0f,1.0f),0.5f,3.0f) ) );
			break;
	}

	osg::MatrixTransform* mt = new osg::MatrixTransform;
	mt->addChild( geode );

	osg::Matrix m;
	m.setTrans( pos );
	mt->setMatrix( m );

	CSulAudioSource* audioSource = new CSulAudioSource( audioManager->getBuffer( bufferName ) );
	audioSource->init();
	audioSource->play();
	mt->addUpdateCallback( audioSource );

	return mt;
}

osg::Node* createScene()
{
	// using the sigma utility library to create the grid
	CSulGeomGrid* pGeomGrid = new CSulGeomGrid;
	pGeomGrid->Create( osg::Vec3(-15,-15,0), 30, 30, 1, 1, 5, 5 );

	return pGeomGrid;
}

CSulString getALErrorString( ALenum error )
{
	switch ( error )
	{
		case AL_INVALID_NAME:		return "Invalid name parameter.";
		case AL_INVALID_ENUM:		return "Invalid parameter.";
		case AL_INVALID_VALUE:		return "Invalid enum parameter value.";
		case AL_INVALID_OPERATION:	return "Illegal call.";
		case AL_OUT_OF_MEMORY:		return "Unable to allocate memory.";
	}

	return "UNKNOWN";
}

void errorCheck( CSulString prefixString )
{
	ALenum error;
	if ( (error = alGetError()) != AL_NO_ERROR ) 
	{
		CSulString s = getALErrorString( error );
		std::cout << "ERROR: " << prefixString << " " << s << std::endl;
		exit(-1);
	}
}

void setupSound( osgViewer::Viewer* viewer )
{
	// setup the audio manager
	audioManager = new CSulAudioManager;
	audioManager->init();
	//audioManager->createBuffer( "background", "c:/ThunderStormRain_S08WT.99_short.wav" );
	audioManager->createBuffer( "background", "C:/Projects/sigmaOsg/osghelp/Data/tank.wav" );
	
	// there is only one listen for each application (that being you), we attach our listener to the camera
	CSulAudioListener* audioListener = new CSulAudioListener;
	audioListener->init();
	viewer->getCamera()->addUpdateCallback( audioListener );

	// show an axis at 0,0,0
	osg::Group* group = viewer->getSceneData()->asGroup();
	group->addChild( new CSulGeomAxis( 1.0f ) );

	group->addChild( createShape( osg::Vec3(10,10,0), SHAPE_SPHERE, "background" ) );

	/*
	osg::Node* s2 = createShape( osg::Vec3(10,-10,0), SHAPE_BOX );
	group->addChild( s2 );
	CSulAudioSource* audioSource2 = new CSulAudioSource;
	audioSource2->init();
	s2->addUpdateCallback( audioSource2 );

	osg::Node* s3 = createShape( osg::Vec3(-10,10,0), SHAPE_CONE );
	group->addChild( s3 );
	CSulAudioSource* audioSource3 = new CSulAudioSource;
	audioSource3->init();
	s3->addUpdateCallback( audioSource3 );
		*/
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

	// set the scene-graph data the viewer will render
	osg::Group* group = new osg::Group;
	
	group->addChild( createScene() );
  
	viewer->setSceneData( group );

	// setup sound
	setupSound( viewer );

	// execute main loop
	return viewer->run();
}
