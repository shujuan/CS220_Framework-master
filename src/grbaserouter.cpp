/*
 * grbaserouter.cpp
 *
 *  Created on: Nov 28, 2014
 *      Author: jmilet
 */

#include "../Headers/grbaserouter.h"
#include "../Headers/grnode.h"
#include "../Headers/grnetlist.h"

#include <algorithm>

using Utilities::GRGrid;
using Utilities::GRNode;
using Utilities::GRNetList;
using std::cout;

Utilities::GRBaseRouter::GRBaseRouter(GRGrid *a, GRGrid *b,
		vector<Utilities::GRNetList*> list ): nextmaze(a), prevmaze(b), inputNets(list)  {

}

void Utilities::GRBaseRouter::synchonizeGrids( ) {

	for ( int y=0 ; y < nextmaze->get_height(); y++ )  {

		for ( int x=0; x < nextmaze->get_width(); x++ )  {

			GRNode*  nextnode = (GRNode*) this->nextmaze->get_node(x,y) ;
			GRNode*  pnode = (GRNode*) this->prevmaze->get_node(x,y) ;

			if ( !nextnode->getInBlock() )  {

				pnode->setNetID( nextnode->getNetID() ) ;
				pnode->setPriority(nextnode->getPriority()) ;
				pnode->setBottleNeck(nextnode->getBottleNeck()) ;
				pnode->setInPath(nextnode->getInPath()) ;
				pnode->setLeeMark(nextnode->getLeeMark()) ;

			}
		}

	}

}


GRNetList * Utilities::GRBaseRouter::getNetObject(int netid ) {

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


void Utilities::GRBaseRouter::initGridPriorities( GRGrid *m, int pushNet  ) {

	GRNetList *cnet ;
	int maxppriority = 0;

	for ( int k=0; k < inputNets.size(); k++ )  {

		cnet = inputNets.at( k ) ;

		setNetMaxPriorities( m, cnet->getNetID(), cnet->getPriority() );

	}

}


void Utilities::GRBaseRouter::setNetMaxPriorities( GRGrid *m,  int targetNetID, int p ) {

	for ( int y=0; y < m->get_height(); y ++ )  {

		for ( int x=0; x< m->get_width(); x ++ )  {

			GRNode* grnode = (GRNode*) m->get_node(x,y) ;

			if ( !grnode->getInBlock() ) {

				if ( grnode->getNetID() == targetNetID  )  {

					grnode->setPriority( p ) ;

				}
			}

		}

	}

}


bool Utilities::GRBaseRouter::isLocalBottleNeck(
						GRGrid * maze, int x1, int y1, int nodeNetID ) {

	if (nodeNetID == 0 )  {

		return false ;

	} else {

		GRNetList * localNet = this->getNetObject(nodeNetID) ;
		GRNode * cnode =  (GRNode*) maze->get_node(x1,y1) ;

		int savedNetId = cnode->getNetID() ;

		cnode->setNetID(0) ;
		cnode->setLeeMark(0) ;

		vector<GRNode*> lnodes =maze->findGRNeighbor(x1,y1,nodeNetID) ;

		bool result = false;

		for ( int k=0; k < lnodes.size(); k++ )  {

			GRNode * node = lnodes.at(k) ;

			GRNetList * nodeNet = this->getNetObject(node->getNetID()) ;

			GRNode * target = (GRNode*) maze->get_node(nodeNet->get_source()) ;
			GRNode * source = (GRNode*) maze->get_node(nodeNet->getSink()) ;

			maze->deleteLeeMarks( node->getNetID() ) ;

			if ( nodeNet->getConnected() )  {

				if (!this->findLeePath( maze, target, node, 3) ) {
					result = true ;
					break ;
				}

				maze->deleteLeeMarks( node->getNetID() ) ;

				if (!this->findLeePath( maze, source, node, 3) ) {
					result = true;
					break ;

				}


			} else {

				if (!this->findLeePath( maze, target, node, 3) ) {

					maze->deleteLeeMarks( node->getNetID() ) ;

					if (!this->findLeePath( maze, source, node, 3) ) {

						result = true;
						break ;

					 }

				}
			}
		}

		cnode->setNetID(savedNetId) ;

		return result ;

	}


}


void Utilities::GRBaseRouter::markGenNeckCells(
		GRGrid * maze, vector<GRNode*> * pathnodes, GRNode* source, GRNode* target ) {

	// Starting from the second element all the way to the previous-last
	// element we have to identify all bottle necks

	int savedNetID = 0 ;
	GRNode * pathNode ;

	for ( int index=0; index < pathnodes->size(); index++ ) {

		pathNode = pathnodes->at(index);

		if ( !pathNode->getPin() ) {

			savedNetID = pathNode->getNetID() ;

			maze->deleteLeeMarks( source->getNetID() ) ;

			pathNode->setNetID(0) ;

			bool pathExist = this->findLeePath( maze, target, source, 3 ) ;

			pathNode->setNetID(savedNetID) ;
			pathNode->setBottleNeck(!pathExist) ;

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

			GRNetList * net = getNetObject( grnode->getNetID() );

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


GRNode * Utilities::GRBaseRouter::getHighPriorityNode(vector<GRNode*> listofnodes) {

	vector<Utilities::GRNode*> nlist ;

	for ( int k=0; k < listofnodes.size() ; k++  )  {

		GRNode * grnode = listofnodes.at(k);

		if ( grnode->getNetID() >0 ) {

			nlist.push_back( grnode ) ;

		}

	}

	sortNodes( &nlist ) ;

	return nlist.at( nlist.size()-1 ) ;


}


GRNode * Utilities::GRBaseRouter::getLowPriorityNode(vector<GRNode*> listofnodes) {

	vector<Utilities::GRNode*> nlist ;

	for ( int k=0; k < listofnodes.size() ; k++  )  {

		GRNode * grnode = listofnodes.at(k);

		if ( grnode->getNetID() >0 ) {

			nlist.push_back( grnode ) ;

		}

	}

	// Ordering
	sortNodes( &nlist ) ;

	return nlist.at( 0 ) ;


}

GRNode * Utilities::GRBaseRouter::getLowPriorityNode(vector<GRNode*> listofnodes, int netID ) {

	vector<Utilities::GRNode*> nlist ;

	for ( int k=0; k < listofnodes.size() ; k++  )  {

		GRNode * grnode = listofnodes.at(k);

		if ( grnode->getNetID() == netID ) {

			nlist.push_back( grnode ) ;

		}

	}

	// Ordering
	sortNodes( &nlist ) ;

	return nlist.at( 0 ) ;

}

void Utilities::GRBaseRouter::retracePath(GRGrid * maze, GRNode * source, GRNode * sink ) {

	vector<GRNode*> pathnodes= maze->retraceGRPath( source, sink ) ;

	markGenNeckCells( maze, &pathnodes, source, sink ) ;

	Path * path = maze->createPath(pathnodes) ;

	maze->add_path( path ) ;

	for ( int n=0; n < pathnodes.size(); n++ )  {

		GRNode * pnode = pathnodes.at(n) ;

		pnode->setInPath(true) ;

	}

}


int Utilities::GRBaseRouter::findPathNets( GRGrid * maze, int cycle, int p  ) {

	GRNetList *cnet ;
	GRNode    * pnode ;

	int npaths = 0 ;

	for ( int k=0; k < inputNets.size() ; k++ )  {

		cnet = inputNets.at(k) ;

		GRNode * source = (GRNode*) maze->get_node( cnet->get_source() ) ;
		GRNode * sink   = (GRNode*) maze->get_node( cnet->getSink() ) ;

		bool pathfound = this->findLeePath( maze, sink, source, cycle ) ;

		if ( pathfound ) {

			retracePath( maze,source, sink );

			cnet->setConnected( true ) ;
			cnet->setPriority( p ) ;
			npaths ++ ;

		}

	} // End for

	return npaths ;

}


bool Utilities::GRBaseRouter::findLeePath(GRGrid * m,
		GRNode * target, GRNode * source, int cycle ) {

	list<GRNode*> propagationCells;
	list<GRNode*> waveFrontCells ;

	source->setLeeMark(1) ;

	if ( target == source ) {
		target->setLeeMark(1)  ;
		return true ;
	}

	propagationCells.push_front(source) ;

	USHORT waveNumber = 2 ;

	bool pathExist = false ;

	int index = 0 ;

	int maxWave = 2*(m->get_width() +m->get_height() ) ;

	while ( (propagationCells.size( ) > 0)	&& (index < maxWave) )  {

		// For each vertex in the propagation list

		for(list<GRNode*>::iterator miter=propagationCells.begin();
				miter != propagationCells.end(); miter++) {

			vector<GRNode*> neiborhTBLR = m->findGRNeighbor(
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

bool Utilities::GRBaseRouter::findPathNet( GRGrid *m,  int cycle, int net  ) {

	GRNetList * cnet =this->getNetObject(net) ;

	GRNode * source = (GRNode*) m->get_node( cnet->get_source() ) ;
	GRNode * sink   = (GRNode*) m->get_node( cnet->getSink() ) ;

	bool pathfound = this->findLeePath( m, sink, source, cycle ) ;

	if ( pathfound ) {

		cnet->setConnected( true ) ;
		cnet->setPriority( 1 ) ;

	}

	return pathfound ;

}


Utilities::GRBaseRouter::~GRBaseRouter() {

}

