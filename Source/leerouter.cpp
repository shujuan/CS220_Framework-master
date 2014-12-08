/*
 * LeeRouter.cpp
 *
 *  Created on: Nov 14, 2014
 *      Author: jmilet
 */

#include <list>

#include "../Headers/LeeRouter.h"
#include "../Headers/GRNode.h"
#include "../Headers/Routerdefinitions.h"

using std::list ;

Utilities::LeeRouter::LeeRouter(GRGrid *grid):maze(grid) {

}


bool Utilities::LeeRouter::routeNet(GRNode * target, GRNode * source) {

	bool pathfound = findPath( target, source );

	if ( pathfound )  {

		vector<GRNode*> pathnodes= this->maze->retraceGRPath( source, target ) ;

		Path * path = this->maze->createPath(pathnodes) ;

		this->maze->add_path( path ) ;


	}

	maze->deleteLeeMarks() ;

	return pathfound ;


}

bool Utilities::LeeRouter::findPath(GRNode * target, GRNode * source) {

	list<GRNode*> propagationList;
	list<GRNode*> waveFrontList ;

	propagationList.push_front(source) ;

	USHORT waveNumber = 1 ;

	bool pathExist = false ;

	while (propagationList.size( ) > 0 )  {

		// For each vertex in the propagation list

		for(list<GRNode*>::iterator iter=propagationList.begin(); iter != propagationList.end(); ++iter) {

			list<GRNode*> neiborhTBLR = maze->findNeighborFree( (*iter)->getX(), (*iter)->getY() ) ;

			// For each vertex in the neighborhood of Vi Do

			for(list<GRNode*>::iterator itn=neiborhTBLR.begin(); itn != neiborhTBLR.end(); ++itn) {

				if ( (*itn)->getLeeMark() ==0  )  {

					(*itn)->setLeeMark(waveNumber) ;

					waveFrontList.push_front( (*itn) )  ;

					if ( (*itn) == target ) {

						pathExist = true ;

						goto pathFound;

					}


				}

			} // End For exploring wavefront

		} // End exploration current front wave

		waveNumber++ ;
		propagationList.clear() ;
		propagationList.splice( propagationList.begin(), waveFrontList ) ;

	}   // End while propagation list

	pathFound :

	return pathExist ;

}


Utilities::LeeRouter::~LeeRouter() {

}

