/*
 * GRRouterOne.cpp
 *
 *  Created on: Nov 20, 2014
 *      Author: jmilet
 */

#include <algorithm>

#include "../Headers/grrouterone.h"
#include "../Headers/grnode.h"
#include "../Headers/grnetlist.h"
#include "../Headers/routerdefinitions.h"
#include "../Headers/grbaserouter.h"

using Utilities::Grid;
using Utilities::GRNode;
using Utilities::GRNetList;
using std::cout;

Utilities::GRRouterOne::GRRouterOne(GRGrid *a, GRGrid *b ,
		vector<Utilities::GRNetList*> list ): GRBaseRouter(a, b, list) {

}

void Utilities::GRRouterOne::setNetPriorities(vector<Utilities::GRNetList*> nets) {

	GRNetList * net ;

	sortNets( &nets) ;

	for ( int k=0; k < nets.size(); k++ )  {

		net = nets.at(k);

		if ( net->getConnected() ) {

			net->setPriority(1) ;

		} else {

			net->setPriority(0) ;

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

		bool localbn = isLocalBottleNeck( pmaze, x1,y1,previousnode->getNetID() ) ;

		// Double check this case ********
		// Check for true or false bottle net **************

		if ( !localbn )  {

			vector<GRNode*> listofnodes = pmaze->findGRNeighbor(x1,y1,true);

			if ( listofnodes.size() > 0 )  {

				GRNetList * highNet = getHPriorityNet(listofnodes) ;

				if ( highNet->getNetID() == previousnode->getNetID() )  {

					// If the nets are equal, no need to do any work

				} else {

					if ( previousnode->getNetID() > 0 ) {

						GRNetList * lownet = this->getGRNetObject( previousnode->getNetID() ) ;

						if ( lownet->getPriority() < highNet->getPriority() ) {

							nextnode->setNetID( highNet->getNetID() ) ;

						}

					} else {

						nextnode->setNetID( highNet->getNetID() ) ;

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

void Utilities::GRRouterOne::routeNets( int cycles ) {

	int index = 1 ;

	bool even = true ;

	prevmaze->writeGridDiskNets( 0 ) ;
	prevmaze->deleteLeeMarks() ;
	prevmaze->deleteBNetFlag() ;

	while ( index < cycles )  {

		// Update priorities

		setNetPriorities( inputNets );

		routeGrid( prevmaze, nextmaze, even );

		nextmaze->writeGridDiskNets(index) ;

		nextmaze->deleteLeeMarks() ;
		nextmaze->deleteBNetFlag() ;

		// Check for connectivity
		int counter = this->findGRPaths( nextmaze, index, 1 );

		if ( counter >= inputNets.size() )  {

			break ;

		}

		this->synchonizeGrids() ;

		// Swap the grids and update as required *********

		index ++ ;
		even = !even ;

	} // End while

	nextmaze->pruneGrid() ;

	nextmaze->writeGridDiskNets(index+1) ;

}


Utilities::GRRouterOne::~GRRouterOne() {

}



