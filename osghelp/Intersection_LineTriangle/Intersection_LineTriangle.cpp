// Intersection_LineTriangle.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulGeomLine.h>
#include <SigmaUtil/SulGeodeSphere.h>
#include <SigmaUtil/SulGeomTriangle.h>
#include <SigmaUtil/SulIntTriangle.h>
#include <osg/group>
#include <osgViewer/Viewer>

osg::Group* createScene()
{
	osg::Group* pGroup = new osg::Group;

	pGroup->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

	// create triangle
	CSulGeomTriangle* pGeomTriangle = new CSulGeomTriangle( 2.0f );
	pGeomTriangle->setColor( 0, 0.7f, 0, 1 );
	pGroup->addChild( pGeomTriangle );
	
	// create line
	osg::Vec3 p1( 0, 0, -2  );
	osg::Vec3 p2( 0, 0.5, 2 );
	osg::LineSegment* pLineSeg = new osg::LineSegment( p1, p2 );
	CSulGeomLine* pLine = new CSulGeomLine( *pLineSeg );
	pGroup->addChild( pLine );

	// calc intersection
	osg::Vec3 vHit;
	sulIntTriangle( *pLineSeg, pGeomTriangle->getTriangle(), vHit );

	// create sphere and put it at the intersection position
	CSulGeodeSphere* pGeomSphere = new CSulGeodeSphere( 0.1f, vHit );
	pGroup->addChild( pGeomSphere );

	return pGroup;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

    rViewer->setUpViewInWindow( 32, 32, 800, 600 );

	rViewer->setSceneData( createScene() );

	return rViewer->run();
}
