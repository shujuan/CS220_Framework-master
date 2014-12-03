/*
 * grroutertwo.cpp
 *
 *  Created on: Nov 28, 2014
 *      Author: jmilet
 */

#include "..\Headers\grroutertwo.h"

#include "..\Headers\GRRouterOne.h"

#include "..\Headers\grnode.h"
#include "..\Headers\GRNetList.h"
#include "..\Headers\grbaserouter.h"


using Utilities::Grid;
using Utilities::GRNode;
using Utilities::GRNetList;
using std::cout;


Utilities::GRRouterTwo::GRRouterTwo(GRGrid *grid,
		vector<Utilities::GRNetList*> list ): GRBaseRouter(grid, list) {

}


void Utilities::GRRouterTwo::initGrid( ) {

	this->maze->pruneGrid() ;

}

void Utilities::GRRouterTwo::setInitNetPriorities( int targetNet ) {

	GRNetList *cnet ;

	for ( int k=0; k < inputNets.size(); k++ )  {

		cnet = inputNets.at( k ) ;

		if ( cnet->getNetID() == targetNet )  {

			cnet->setPriority( 5 ) ;

		} else {

			if ( cnet->getConnected() )  {

				cnet->setPriority( 3 ) ;

			} else {

				cnet->setPriority( 1 ) ;

			}

		}

	}

}

void Utilities::GRRouterTwo::setNetMaxPriorities( int targetNetID, int p ) {

	for ( int y=0; y < this->maze->get_height(); y ++ )  {

		for ( int x=0; x< this->maze->get_width(); x ++ )  {

			GRNode* grnode = (GRNode*) maze->get_node(x,y) ;

			if ( ! grnode->getInBlock() ) {

				if ( grnode->getNetID() == targetNetID  )  {

					grnode->setPriority( p ) ;

				}
			}

		}

	}

}


int Utilities::GRRouterTwo::getNetMaxPriorities( int targetNetID, int pushNet ) {

	GRNetList * nodeNet = getGRNetObject( targetNetID ) ;
	int basePriority = nodeNet->getPriority() ;

	for ( int y=0; y < this->maze->get_height(); y ++ )  {

		for ( int x=0; x< this->maze->get_width(); x ++ )  {

			GRNode* grnode = (GRNode*) maze->get_node(x,y) ;

			if ( ! grnode->getBoardCell() ) {

				if ( grnode->getNetID() == targetNetID  )  {

					// Check if the adjacent cell belongs to a pushed-through-net

					vector<GRNode*> listofnodes = filterCells(x,y,pushNet);

					for ( int k=0; k < listofnodes.size(); k++) {

						GRNode* n = listofnodes.at(k) ;

						if ( n->getNetID() == pushNet )  {

							basePriority = std::max(basePriority, n->getPriority()-2);

						} else if ( n->getNetID() == targetNetID ) {

							basePriority = std::max(basePriority, n->getPriority()-1);
						}

					} // End List Nodes

				}  // End target net ID
			}

		}

	}

	return basePriority;

}

void Utilities::GRRouterTwo::initGridPriorities( int pushNet  ) {

	GRNetList *cnet ;
	int maxppriority = 0;

	for ( int k=0; k < inputNets.size(); k++ )  {

		cnet = inputNets.at( k ) ;
		setNetMaxPriorities(cnet->getNetID(), cnet->getPriority() );

	}

}


void Utilities::GRRouterTwo::setConnNetPriorities( int pushNet  ) {

	GRNetList *cnet ;
	int maxppriority = 0;

	for ( int k=0; k < inputNets.size(); k++ )  {

		cnet = inputNets.at( k ) ;

		if ( cnet->getConnected() )   {

			maxppriority = getNetMaxPriorities(cnet->getNetID(), pushNet);

cout << " Connected net  " << 	cnet->getNetID() << " Pri "  << maxppriority << " \n " ;

			setNetMaxPriorities(cnet->getNetID(), maxppriority );

		}

	}

}


vector<GRNode*> Utilities::GRRouterTwo::filterCells( int x1, int y1, int targetNet  ) {

	vector<GRNode*> nlist ;

	GRNode * node ;
	GRNetList *cnet ;

	vector<GRNode*> lnodes = maze->findGRNeighbor(x1,y1,true);

	for ( int k=0; k < lnodes.size(); k++ )  {

		node = lnodes.at( k ) ;

		if ( node->getNetID() == 0  )  {
			nlist.push_back(node)  ;
		}

		else {

			 cnet = this->getGRNetObject( node->getNetID( ))  ;

			if ( node->getNetID() == targetNet || cnet->getConnected( ))  {
				nlist.push_back(node)  ;
			}

		}
	}


	return nlist ;

}


void Utilities::GRRouterTwo::routeCells( int x1, int y1, int targetNet ) {

	GRNode* grnode = (GRNode*) maze->get_node(x1,y1) ;

	if ( !grnode->getBoardCell() && !grnode->getBottleNeck() ) {

			vector<GRNode*> listofnodes = filterCells(x1,y1,targetNet);

			if ( listofnodes.size() > 0 )  {

				GRNode * highNode = getHPriorityNode(listofnodes);
				GRNetList * highNet = getGRNetObject(highNode->getNetID()) ;

				// We only route if the current node belongs to the target net or
				// to one of the already connected nets ...

				// No Net ID associated with the current cell

				if (  grnode->getNetID() == 0 )  {

					grnode->setNetID( highNode->getNetID() ) ;
					grnode->setUpdatedCell(true) ;

					if (highNet->getConnected()  )  {
						grnode->setPriority( 3 ) ;

					} else {
						grnode->setPriority(highNode->getPriority()+1) ;
					}

				} // End node net ID is zero

				else {

					// We already have a net in this cell

					if ( grnode->getNetID() == targetNet  )  {

						if ( grnode->getNetID() !=  highNode->getNetID() )  {

							if ( grnode->getPriority() < highNode->getPriority() )  {

								grnode->setNetID( highNode->getNetID()) ;
								grnode->setPriority( 3  ) ;
								grnode->setUpdatedCell(true) ;

							}
						}

					} else {

						// Nodes are competing for the present cell *********
						// The net has a cell ID - Cell does not belongs to the target net

						if ( grnode->getPriority() < highNode->getPriority() )  {

							grnode->setNetID( highNode->getNetID() ) ;

							grnode->setPriority( highNet->getNetID() == targetNet ?
									highNode->getPriority() + 1 : 3 ) ;

							grnode->setUpdatedCell(true) ;

						}

					}
				}

			}  // End if list of nodes

		} // End board cell
}


void Utilities::GRRouterTwo::routeGrid( bool even, int targetNet ) {

	for ( int y1=1; y1 < maze->get_height() ; y1++  ) {

		for ( int x1=1; x1 < maze->get_width(); x1++ )  {

			if ( (x1+y1) % 2 == 0 )  {

				if ( even ) {

					routeCells( x1,y1, targetNet ) ;

				}

			} else {

				if ( !even ) {

					routeCells( x1,y1, targetNet ) ;

				}

			}

		}

	}

}

void Utilities::GRRouterTwo::routeNets( int cycles, int targetNet ) {

	int index = 0 ;
	bool even = false ;

	this->maze->writeGridDiskNets(index) ;

	// Set nets and grid priorities
	this->setInitNetPriorities( targetNet );
	this->initGridPriorities( targetNet ) ;

	this->maze->writeGridDiskPri(index) ;

	while ( index < cycles )  {

		routeGrid( even, targetNet );

		setConnNetPriorities( targetNet ) ;

		this->maze->writeGridDiskNets(index+1) ;
		this->maze->writeGridDiskPri(index+1) ;
		this->maze->deleteLeeMarks() ;

		// Check for connectivity
		int counter = this->findGRPaths( index, 3  );

		if ( counter >= inputNets.size() )  {

			break ;

		}

		index ++ ;
		even = !even ;

	}  // End while

	// this->maze->pruneGrid() ;
	this->maze->writeGridDiskNets(index+1) ;

}


Utilities::GRRouterTwo::~GRRouterTwo() {

}



