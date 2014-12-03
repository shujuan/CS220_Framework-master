/*
 * grbaserouter.cpp
 *
 *  Created on: Nov 28, 2014
 *      Author: jmilet
 */

#include "..\Headers\grbaserouter.h"
#include "..\Headers\grnode.h"
#include "..\Headers\GRNetList.h"
#include "..\Headers\routerdefinitions.h"

#include <algorithm>

using Utilities::Grid;
using Utilities::GRNode;
using Utilities::GRNetList;
using std::cout;

Utilities::GRBaseRouter::GRBaseRouter(GRGrid *grid,
		vector<Utilities::GRNetList*> list ): maze(grid), inputNets(list)  {

}

GRNetList * Utilities::GRBaseRouter::getGRNetObject(int netid ) {

	GRNetList * net ;
	GRNetList *cnet ;

	for ( int k=0; k < inputNets.size() ; k++ )  {

		cnet = inputNets.at(k) ;
		if ( cnet->getNetID() == netid )  {
			net = cnet ;
			break ;
		}
	}

	return net ;

}

void Utilities::GRBaseRouter::markGenNeckCells(
		vector<GRNode*> * pathnodes, GRNode* source, GRNode* target ) {

	// Starting from the second element all the way to the previous-last
	// element we have to identify all bottle necks

	int savedNetID = 0 ;
	GRNode * pathNode ;

	for ( int index=0; index < pathnodes->size(); index++ ) {

		pathNode = pathnodes->at(index);

		if (!( pathNode == source )) {

			if (!( pathNode == target )) {

				savedNetID = pathNode->getNetID() ;

				pathNode->setNetID(0) ;

				this->maze->deleteLeeMarks( source->getNetID() ) ;

				bool pathExist = this->findGRPath( target, source, 3 ) ;

				pathNode->setNetID(savedNetID) ;

				pathNode->setBottleNeck( !pathExist ) ;

			}

		}

	}

}



bool compareNet(GRNetList* a, GRNetList* b) {

	return (*a < *b);

}

bool compareNode(GRNode* a, GRNode* b) {

	return (a->getPriority() < b->getPriority() );

}


void Utilities::GRBaseRouter::sortNets(vector<Utilities::GRNetList*> * nets) {

	std::sort ( nets->begin(), nets->end(), compareNet );

}


GRNetList * Utilities::GRBaseRouter::getHPriorityNet(vector<GRNode*> listofnodes) {

	vector<Utilities::GRNetList*> nlist ;

	for ( int k=0; k < listofnodes.size() ; k++  )  {

		GRNode * grnode = listofnodes.at(k);

		if ( grnode->getNetID() >0 ) {

			GRNetList * net = getGRNetObject( grnode->getNetID() );

			nlist.push_back( net ) ;

		}

	}

	// Ordering
	sortNets( &nlist ) ;

	// finding the node
	return nlist.at( 0 ) ;

}

void Utilities::GRBaseRouter::sortNodes(vector<Utilities::GRNode*> * nodes ) {

	std::sort ( nodes->begin(), nodes->end(), compareNode );

}

GRNode * Utilities::GRBaseRouter::getHPriorityNode(vector<GRNode*> listofnodes) {

	vector<Utilities::GRNode*> nlist ;

	for ( int k=0; k < listofnodes.size() ; k++  )  {

		GRNode * grnode = listofnodes.at(k);

		if ( grnode->getNetID() >0 ) {

			nlist.push_back( grnode ) ;

		}

	}

	// Ordering
	sortNodes( &nlist ) ;

	if ( listofnodes.size() >1 ) {

		return nlist.at( listofnodes.size() - 1 ) ;

	} else {

		return nlist.at( 0 ) ;

	}

}

void Utilities::GRBaseRouter::retraceGRPath(GRNode * source, GRNode * sink ) {

	GRNode    * pnode ;

	vector<GRNode*> pathnodes= this->maze->retraceGRPath( source, sink ) ;

	this->markGenNeckCells( &pathnodes, source, sink ) ;

	for ( int n=0; n < pathnodes.size(); n++ )  {

		pnode = pathnodes.at(n) ;
		pnode->setInPath(true) ;
		pnode->displayNode() ;

	}

}


int Utilities::GRBaseRouter::findGRPaths( int cycle, int p  ) {

	GRNetList *cnet ;
	GRNode    * pnode ;

	int npaths = 0 ;

	for ( int k=0; k < inputNets.size() ; k++ )  {

		cnet = inputNets.at(k) ;

		GRNode * source = (GRNode*) this->maze->get_node( cnet->get_source() ) ;
		GRNode * sink   = (GRNode*) this->maze->get_node( cnet->getSink() ) ;

		cout << " Searching path "  << "\n" ;

		source->displayNode() ;
		sink->displayNode() ;

		bool pathfound = this->findGRPath( sink, source, cycle ) ;

		if ( pathfound ) {

			cout << " Path found ************* "  << "\n" ;

			retraceGRPath( source, sink );

			cnet->setConnected( true ) ;
			cnet->setPriority( p ) ;
			npaths ++ ;

		}

	} // End for

	return npaths ;

}


bool Utilities::GRBaseRouter::findGRPath(GRNode * target, GRNode * source, int cycle ) {

	list<GRNode*> propagationCells;
	list<GRNode*> waveFrontCells ;

	source->setLeeMark(1) ;

	propagationCells.push_front(source) ;

	USHORT waveNumber = 2 ;

	bool pathExist = false ;

	int index = 0 ;

	int maxWave = 2*(this->maze->get_width() +this->maze->get_height() ) ;

	while ( (propagationCells.size( ) > 0)	&& (index < maxWave) )  {

		// For each vertex in the propagation list

		for(list<GRNode*>::iterator miter=propagationCells.begin();
				miter != propagationCells.end(); miter++) {

			vector<GRNode*> neiborhTBLR = maze->findGRNeighbor(
					(*miter)->getX(), (*miter)->getY(), source->getNetID() ) ;

			// For each vertex in the neighborhood of Vi

			for(vector<GRNode*>::iterator itn=neiborhTBLR.begin(); itn != neiborhTBLR.end(); itn++) {

				if ( (*itn)->getLeeMark() == 0 ) {  // We have to deal with un-explorer nodes

					(*itn)->setLeeMark(waveNumber) ;

					waveFrontCells.push_front( (*itn) )  ;

					if ( (*itn) == target ) {

						pathExist = true ;

						goto pathFound;

					}

				}

			} // End For exploring wavefront

		} // End exploration current front wave

		waveNumber++ ;
		propagationCells.clear() ;
		propagationCells.splice( propagationCells.begin(), waveFrontCells ) ;
		waveFrontCells.clear() ;

		index++ ;

		//break ;

	}   // End while propagation list

	pathFound :

	return pathExist ;

}



Utilities::GRBaseRouter::~GRBaseRouter() {

}

