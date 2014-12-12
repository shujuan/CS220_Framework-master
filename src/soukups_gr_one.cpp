/*
 * GRRouterOne.cpp
 *
 *  Created on: Nov 20, 2014
 *      Author: jmilet
 */

#include <algorithm>

#include "../Headers/soukups_gr_one.h"
#include "../Headers/grnode.h"
#include "../Headers/grnetlist.h"
#include "../Headers/grbaserouter.h"

using Utilities::Grid;
using Utilities::GRNode;
using Utilities::GRNetList;
using std::cout;

Utilities::GRRouterOne::GRRouterOne(GRGrid *next, GRGrid *prev ,
		vector<Utilities::GRNetList*> list ): GRBaseRouter(next, prev, list) {

}

void Utilities::GRRouterOne::setNetPriorities(vector<Utilities::GRNetList*> nets) {

	int maxmd = nets.size()+1 ;

	for ( int k=0; k < nets.size(); k++ )  {

		GRNetList * net = nets.at(k);

		if ( !net->getConnected() ) {

			int md = std::abs(net->get_source().x - net->getSink().x) +
					std::abs(net->get_source().y - net->getSink().y) ;

			net->setPriority( md ) ;

			maxmd = std::max(maxmd, md);

		}
	}

	sortNets( &nets) ;

	for ( int k=0; k < nets.size(); k++ )  {

		GRNetList * net = nets.at(k);

		if ( net->getConnected() ) {

			net->setPriority( maxmd + 1 ) ;

		} else {

			net->setPriority(k+1) ;
		}
	}

}

void Utilities::GRRouterOne::routeCells(  GRGrid *pmaze, GRGrid *nmaze, int x1, int y1 ) {

	GRNode* previousnode = (GRNode*) pmaze->get_node(x1,y1) ;
	GRNode* nextnode = (GRNode*) nmaze->get_node(x1,y1) ;

	if ( previousnode->getBoardCell()  || previousnode->getBottleNeck() )  {

		return ;  // No need to route this type of cells

	}

	else {

		if ( previousnode->getNetID() > 0 )   {

			GRNetList * cnet = this->getNetObject(previousnode->getNetID()) ;

			if ( cnet->getConnected() )  {

				nmaze->deleteLeeMarks( previousnode->getNetID() ) ;

				bool pathfound = this->findPathNet(nmaze,1, previousnode->getNetID()) ;

				GRNode * source = (GRNode*) nmaze->get_node( cnet->get_source() ) ;
				GRNode * sink   = (GRNode*) nmaze->get_node( cnet->getSink() ) ;

				this->retracePath( nmaze, source, sink ) ;

				if ( nextnode->getBottleNeck() )  {

					return ;

				}

			}

		}

		bool localBN1 = isLocalBottleNeck( pmaze, x1,y1,previousnode->getNetID() ) ;
		bool localBN2 = isLocalBottleNeck( nmaze, x1,y1,previousnode->getNetID() ) ;

		// Double check this case ********
		// Check for true or false bottle net **************

		if ( !(localBN1 || localBN2 )  )  {

			vector<GRNode*> listofnodes = pmaze->findGRNeighbor(x1,y1,true);

			if ( listofnodes.size() > 0 )  {

				GRNode *   highNode = getLowPriorityNode(listofnodes) ;

				if ( highNode->getNetID() == previousnode->getNetID() )  {

					// If the nets are equal, no need to do any work

				} else {

					if ( previousnode->getNetID() > 0 ) {

						if ( nextnode->getPriority() > highNode->getPriority() ) {

							nextnode->setNetID( highNode->getNetID() ) ;

						}

					} else {

						nextnode->setNetID( highNode->getNetID() ) ;

					}
				}

			} // End if list of nodes

		}
	}

}

void Utilities::GRRouterOne::routeGrid( GRGrid *omaze, GRGrid *nmaze , bool even ) {

	for ( int y1=1; y1 < omaze->get_height() ; y1++  ) {

		for ( int x1=1; x1 < omaze->get_width(); x1++ )  {

			if ( (x1+y1) % 2 == 0 )  {

				if ( even ) {

					routeCells( omaze,nmaze,x1,y1 ) ;

				}

			} else {

				if ( !even ) {

					routeCells( omaze,nmaze, x1,y1 ) ;

				}

			}

		}

	}

}

/**
 * --------------------------------------------------------------------
 *  GR1 Router - Main method.
 *  This method is the implementation of the GR1 as described in the paper.
 *  The method works iteratively by executing a number of steps including.
 *
 *  a.- Deleting Lee Marks
 *  b.- Deleting Bottle Neck marks
 *  c.- Setting the priorities per net
 *  d.- Setting the priorities per grid cell
 *  e.- Executing the routing
 *  f.- Finding new paths in the grid
 *  e.- Synchronizing the grids
 * --------------------------------------------------------------------
 */

void Utilities::GRRouterOne::routeNets( int cycles ) {

	int index = 1 ;

	bool even = true ;

	prevmaze->writeGridDiskNets( 0 ) ; // Debug file
	prevmaze->deleteLeeMarks() ;  // Delete Lee marks
	prevmaze->deleteBNetFlag() ;  // Delete bottle neck marks

	while ( index < cycles )  {

		cout << " Cycle: "  <<  index << "\n" ;

		setNetPriorities( inputNets ); // Set priorities

		initGridPriorities(nextmaze, 1) ;  // Set priorities per cell
		initGridPriorities(prevmaze, 1) ;   // Set priorities per cell

		// Executes the routing process
		routeGrid( prevmaze, nextmaze, even );

		// Delete a set of attributes including Lee marks
		// Bottle neck flags, and paths
		nextmaze->deleteLeeMarks() ;
		nextmaze->deleteBNetFlag() ;
		nextmaze->clearPaths() ;

		// Check for new paths
		int counter = this->findPathNets( nextmaze, index, 1 );

		// Debug file
		nextmaze->writeGridDiskNets(index) ;

		if ( counter >= inputNets.size() )  {
			break ;
		}

		// Synchronize grids

		this->synchonizeGrids() ;

		index ++ ;
		even = !even ;

	} // End while

	// When done, cleans up the final solution
	nextmaze->pruneGrid() ;

	// Write debug information
	nextmaze->writeGridDiskNets(index+1) ;

}


Utilities::GRRouterOne::~GRRouterOne() {

}



