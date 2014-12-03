/*
 * GRRouterOne.cpp
 *
 *  Created on: Nov 20, 2014
 *      Author: jmilet
 */

#include <algorithm>

#include "..\Headers\grrouterone.h"
#include "..\Headers\grnode.h"
#include "..\Headers\grnetlist.h"
#include "..\Headers\routerdefinitions.h"
#include "..\Headers\grbaserouter.h"

using Utilities::Grid;
using Utilities::GRNode;
using Utilities::GRNetList;
using std::cout;

Utilities::GRRouterOne::GRRouterOne(GRGrid *grid,
		vector<Utilities::GRNetList*> list ): GRBaseRouter(grid, list) {

}

bool Utilities::GRRouterOne::isLocalBottleNeck( int x1, int y1, int net ) {

	if (net == 0 )  {

		return false ;

	} else {

		return maze->isLocalBottleNeck(x1,y1, net ) ;

	}

}

void Utilities::GRRouterOne::routeCells( int x1, int y1 ) {

	GRNode* grnode = (GRNode*) maze->get_node(x1,y1) ;

	if ( grnode->getBoardCell()  || grnode->getBottleNeck() )  {

		return ;  // No need to route this type of cells

	}

	else {

		bool localbn = isLocalBottleNeck(x1,y1,grnode->getNetID() ) ;

		// Double check this case ********
		// Check for true or false bottle net **************

		if ( !localbn )  {

			vector<GRNode*> listofnodes = maze->findGRNeighbor(x1,y1,true);

			if ( listofnodes.size() > 0 )  {

				GRNetList * highNet = getHPriorityNet(listofnodes) ;

				if ( highNet->getNetID() == grnode->getNetID() )  {

					// If the nets are equal, no need to do any work
					grnode->setUpdatedCell(false) ;


				} else {

					if ( grnode->getNetID() > 0 ) {

						GRNetList * lownet = this->getGRNetObject( grnode->getNetID() ) ;

						if ( lownet->getPriority() < highNet->getPriority() ) {

							grnode->setNetID( highNet->getNetID() ) ;
							grnode->setUpdatedCell(true) ;
						}

					} else {

						grnode->setNetID( highNet->getNetID() ) ;
						grnode->setUpdatedCell(true) ;

					}
				}

			} // End if list of nodes

		}
	}

}

void Utilities::GRRouterOne::routeGrid( bool even ) {

	for ( int y1=1; y1 < maze->get_height() ; y1++  ) {

		for ( int x1=1; x1 < maze->get_width(); x1++ )  {

			if ( (x1+y1) % 2 == 0 )  {

				if ( even ) {

					routeCells( x1,y1 ) ;

				}

			} else {

				if ( !even ) {

					routeCells( x1,y1 ) ;

				}

			}

		}

	}

}

void Utilities::GRRouterOne::routeNets( int cycles ) {

	int index = 1 ;

	bool even = true ;

	this->maze->writeGridDiskNets( 0 ) ;
	this->maze->deleteLeeMarks() ;
	this->maze->deleteBNetFlag() ;

	while ( index < cycles )  {

		// Update priorities

		this->updateNets( inputNets );

		this->routeGrid( even );

		this->maze->writeGridDiskNets(index) ;

		this->maze->deleteLeeMarks() ;
		this->maze->deleteBNetFlag() ;

		// Check for connectivity
		int counter = this->findGRPaths( index, 1 );

		if ( counter >= inputNets.size() )  {

			break ;

		}

		index ++ ;
		even = !even ;

	} // End while

	this->maze->pruneGrid() ;

	this->maze->writeGridDiskNets(index+1) ;

}

void Utilities::GRRouterOne::updateNets(vector<Utilities::GRNetList*> nets) {

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


Utilities::GRRouterOne::~GRRouterOne() {

}



