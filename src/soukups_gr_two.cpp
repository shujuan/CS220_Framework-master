/*
 * grroutertwo.cpp
 *
 *  Created on: Nov 28, 2014
 *      Author: jmilet
 */

#include "../Headers/soukups_gr_two.h"
#include "../Headers/grnode.h"
#include "../Headers/grnetlist.h"
#include "../Headers/grbaserouter.h"

using Utilities::Grid;
using Utilities::GRNode;
using Utilities::GRNetList;
using std::cout;

Utilities::GRRouterTwo::GRRouterTwo(GRGrid *a, GRGrid *b,
		vector<Utilities::GRNetList*> list ): GRBaseRouter(a, b,list) {

}


void Utilities::GRRouterTwo::initGrid(GRGrid  * g) {

	g->pruneGrid() ;

}

bool Utilities::GRRouterTwo::isDeadBayNet( GRGrid *m, int x, int y, bool H  ) {

	GRNode * nodecenter = (GRNode*) m->get_node(x,y) ;

	if ( nodecenter->getBoardCell() || ( nodecenter->getNetID() == 0 ) )  {

		return false ;

	} else {

		GRNode * nodeleft ;
		GRNode * noderight ;

		if ( H )  {

			nodeleft = (GRNode*)   m->get_node(x-1,y) ;
			noderight = (GRNode*)  m->get_node(x+1,y) ;

		} else {

			nodeleft = (GRNode*)   m->get_node(x,y-1) ;
			noderight = (GRNode*)  m->get_node(x,y+1) ;
		}

		if (( nodeleft->getBoardCell() ) || ( noderight->getBoardCell())) {

			if (( nodeleft->getBoardCell() ) && ( noderight->getBoardCell())) {

				if ( nodeleft->getNetID() !=  nodecenter->getNetID())  {

					if ( noderight->getNetID() != nodecenter->getNetID() )  {

						return true ;

					}

				}

			}

		}
	}  // End else

	return false  ;

}


void Utilities::GRRouterTwo::deleteDeadBayNet( GRGrid *m ) {

	// Checking for horizontal dead bay nets ************
	// In the first and last row
	int rowsarray [2]  ;

	rowsarray [0] = 1 ;
	rowsarray [1] = m->get_height()-2 ;

	for ( int y1=0; y1 < 2 ; y1++  ) {

		for ( int x=2; x < m->get_width()-2; x++ )  {

			if ( this-> isDeadBayNet(m,x,rowsarray[y1],true)) {

				GRNode * nodecenter = (GRNode*) m->get_node(x,rowsarray[y1]) ;
				nodecenter->setNetID(0) ;
				nodecenter->setPriority(0) ;

			}
		}
	}

	// --------------------------------------------------------
	// Checking for vertical dead bay nets ************
	// In the first and last columns ******************
	// --------------------------------------------------------

	int colsarray [2]  ;

	colsarray[0] =  1 ;
	colsarray[1] =  m->get_width()-2 ;

	for ( int x1=0; x1 < 2 ; x1++ ) {

		for ( int y1=2; y1 < m->get_height()-2; y1++ )  {

			if ( this->isDeadBayNet(m,colsarray[x1],y1,false)) {

				GRNode * nodecenter = (GRNode*) m->get_node(colsarray[x1],y1) ;
				nodecenter->setNetID(0) ;
				nodecenter->setPriority(0) ;

			}

		} // End for  ***********************************

	} // End while ***************************************

}


void Utilities::GRRouterTwo::initNetPriorities( int targetNet ) {

	GRNetList *cnet ;

	for ( int k=0; k < inputNets.size(); k++ )  {

		cnet = inputNets.at( k ) ;

		if ( cnet->getNetID() == targetNet )  {

			cnet->setPriority( 4 ) ;

		} else {

			if ( cnet->getConnected() )  {

				cnet->setPriority( 2 ) ;

			} else {

				cnet->setPriority( 1 ) ;

			}

		}

	}

}

int Utilities::GRRouterTwo::getNetMaxPriority(  GRGrid *m, int targetNetID) {

	int basePriority =0 ;

	for ( int y=0; y < m->get_height(); y ++ )  {

		for ( int x=0; x< m->get_width(); x ++ )  {

			GRNode* grnode = (GRNode*) m->get_node(x,y) ;

			if ( !grnode->getInBlock() ) {

				if ( grnode->getNetID() == targetNetID  )  {

					basePriority = std::max(basePriority, grnode->getPriority( ));

				}
			}

		}

	}

	return basePriority ;

}


bool Utilities::GRRouterTwo::isNetCloseToNet(  GRGrid *m, int targetNetID, int pushNet ) {

	GRNetList * nodeNet = getNetObject( targetNetID ) ;

	for ( int y=0; y < m->get_height(); y ++ )  {

		for ( int x=0; x< m->get_width(); x ++ )  {

			GRNode* grnode = (GRNode*) m->get_node(x,y) ;

			if ( ! grnode->getInBlock() ) {

				if ( grnode->getNetID() == targetNetID  )  {

					// Check if the adjacent cell belongs to a pushed-through-net

					vector<GRNode*> listofnodes = m->findGRNeighbor(x,y,pushNet);

					for ( int k=0; k < listofnodes.size(); k++) {

						GRNode* n = listofnodes.at(k) ;

						if ( n->getNetID() == pushNet )  {

							return true ;

						}

					} // End List Nodes

				}  // End target net ID
			}

		}

	}

	return false ;

}

void Utilities::GRRouterTwo::setNetPriorities( GRGrid * m ,int pushNet  ) {

	GRNetList *cnet ;

	int maxppriority = getNetMaxPriority( m, pushNet) ;

	for ( int k=0; k < inputNets.size(); k++ )  {

		cnet = inputNets.at( k ) ;

		if ( cnet->getConnected() )   {

			if ( isNetCloseToNet(m, cnet->getNetID(), pushNet) ) {

				setNetMaxPriorities(m, cnet->getNetID(), maxppriority-2 );

			} else {

				setNetMaxPriorities(m,cnet->getNetID(), maxppriority-1 );
			}

		}

	}

}


vector<GRNode*> Utilities::GRRouterTwo::filterCells( GRGrid *m,
									int x1, int y1, int targetNet  ) {

	vector<GRNode*> nlist ;

	GRNode * node ;
	GRNetList *cnet ;

	vector<GRNode*> lnodes = m->findGRNeighbor(x1,y1,true);

	GRNetList *tnet = getNetObject(targetNet) ;
	GRNode * targetNode = (GRNode*) m->get_node(tnet->getSink()) ;

	for ( int k=0; k < lnodes.size(); k++ )  {

		node = lnodes.at( k ) ;

		if ( node->getNetID() == 0  )  {
			nlist.push_back(node)  ;
		}

		else {

			cnet = this->getNetObject( node->getNetID( ))  ;

			if ( node->getNetID() == targetNet )  {

				// Do not growth the target node in the net *****
				if (!( node == targetNode )) {

					nlist.push_back(node)  ;
				}
			}

			else if ( cnet->getConnected( ))  {

				nlist.push_back(node)  ;

			}

		}
	}


	return nlist ;

}

void Utilities::GRRouterTwo::routeCells(
		GRGrid * prevmaze, GRGrid * nextmaze, int x1, int y1, int targetNet ) {

	GRNode* prevnode = (GRNode*) prevmaze->get_node(x1,y1) ;
	GRNode* nextnode = (GRNode*) nextmaze->get_node(x1,y1) ;

	if ( ! prevnode->getBoardCell()  ) {

		if ( !prevnode->getBottleNeck() ) {

			bool oldbn = this->isLocalBottleNeck( prevmaze, x1,y1,prevnode->getNetID());
			bool newbn = this->isLocalBottleNeck( nextmaze, x1,y1,prevnode->getNetID());

			if ( !(oldbn || newbn ) ) {

				vector<GRNode*> listofnodes = filterCells( prevmaze, x1,y1,targetNet);

				if ( listofnodes.size() > 0 )  {

					GRNode * highNode = getHighPriorityNode(listofnodes);
					GRNode * lowNode = getLowPriorityNode(listofnodes, highNode->getNetID() );

					GRNetList * highNet = getNetObject(highNode->getNetID()) ;

					// We only route if the current node belongs to the target net or
					// to one of the already connected nets ...

					// No Net ID associated with the current cell

					if (  prevnode->getNetID() == 0 )  {

						nextnode->setNetID( highNode->getNetID() ) ;

						if (highNet->getConnected()  )  {

							nextnode->setPriority( 3 ) ;


						} else {

							nextnode->setPriority(lowNode->getPriority()+1) ;

						}


					} // End node net ID is zero

					else {

						// We already have a net in this cell
						// Cell belongs to the target net **************

						if ( prevnode->getNetID() == targetNet  )  {

							if ( targetNet !=  highNode->getNetID() )  {

								if ( prevnode->getPriority() < highNode->getPriority() )  {

									nextnode->setNetID( highNode->getNetID() ) ;
									nextnode->setPriority( 3  ) ;

								}
							}

						} else {

							// Nodes are competing for the present cell *********
							// The net has a cell ID - Cell does not belongs to the target net

							if ( prevnode->getPriority() < highNode->getPriority() )  {

								nextnode->setNetID( highNode->getNetID() ) ;

								nextnode->setPriority( highNet->getNetID() == targetNet ?
										lowNode->getPriority() + 1 : 3 ) ;

							}

						}
					}

				}  // End if list of nodes

			} else {

				// cout << "Local Bottle Net Node **************"  ;
			}

		} else {

			//cout << "General Bottle Net Node **************"  ;

		}
	}

}


void Utilities::GRRouterTwo::routeGrid(  GRGrid * cmaze,
				GRGrid * nmaze, bool even, int targetNet ) {

	for ( int y1=1; y1 < cmaze->get_height() ; y1++  ) {

		for ( int x1=1; x1 < cmaze->get_width(); x1++ )  {

			if ( (x1+y1) % 2 == 0 )  {

				if ( even ) {

					routeCells( cmaze, nmaze, x1,y1, targetNet ) ;

				}

			} else {

				if ( !even ) {

					routeCells( cmaze, nmaze, x1,y1, targetNet ) ;

				}

			}

		}

	}

}

void Utilities::GRRouterTwo::retraceGRPath(GRGrid *m, GRNode * source, GRNode * sink ) {

	GRNode    * pnode ;

	vector<GRNode*> pathnodes= m->retraceGRPath( source, sink ) ;

	for ( int n=0; n < pathnodes.size(); n++ )  {

		pnode = pathnodes.at(n) ;
		pnode->setInPath(true) ;
	//	pnode->displayNode() ;

	}

	this->markGenNeckCells( m, &pathnodes, source, sink ) ;

}


int Utilities::GRRouterTwo::findGRPathsConn( GRGrid * cmaze, int cycle, int p  ) {

	GRNetList *cnet ;

	int npaths = 0 ;

	for ( int k=0; k < inputNets.size() ; k++ )  {

		cnet = inputNets.at(k) ;

		if ( cnet->getConnected() ) {

			GRNode * source = (GRNode*) cmaze->get_node( cnet->get_source() ) ;
			GRNode * sink   = (GRNode*) cmaze->get_node( cnet->getSink() ) ;

			bool pathfound = this->findLeePath( cmaze, sink, source, cycle ) ;

			if ( pathfound ) {

				retraceGRPath( cmaze, source, sink );

				cnet->setConnected( true ) ;
				cnet->setPriority( p ) ;
				npaths ++ ;

			}
		}

	} // End for connected



	return npaths ;

}

/**
 * --------------------------------------------------------------------
 *  GR2 Router - Main method.
 *  This method is the implementation of the GR2 as described in the paper.
 *  The method works iteratively by executing a number of steps including.
 *
 *  a.- Setting the priorities per net
 *  b.- Setting the priorities per grid
 *  c.- Deleting the Bottle Neck flag from the grid
 *  d.- Marking the paths of the already connected nets
 *  e.- Executing the routing algorithm
 *  f.- Deleting bay nets
 *  e.- Finding paths of unconnected nets
 *  h.- Synchronizing the grids
 * --------------------------------------------------------------------
 */


int Utilities::GRRouterTwo::routeNets( int start, int cycles, int targetNet ) {

	int index = 0 ;
	bool even = false ;

	// Set nets and grid priorities
	initNetPriorities( targetNet );
	initGridPriorities( prevmaze,targetNet ) ;
	initGridPriorities( nextmaze,targetNet ) ;

	// Delete the bottle neck flags
	prevmaze->deleteBNetFlag() ;

	// Writes a text file.
	// We debugged our implementation using this text file along
	// with a java imaging utility.
	prevmaze->writeGridDiskNets(start++) ;

	while ( index < cycles )  {

		// Find / mark paths of already connected nets
		findGRPathsConn( prevmaze, index, 3  );

		// Executes the routing algorithm
		routeGrid( prevmaze, nextmaze, even, targetNet );

		// Delete dead bay nets as "described" in the paper
		deleteDeadBayNet(nextmaze) ;

		// Set the priorities per net
		setNetPriorities( nextmaze, targetNet ) ;

		nextmaze->writeGridDiskNets(start++) ;

		// Deletes the attributes Lee mark and Bottle neck cell

		nextmaze->deleteLeeMarks() ;
		nextmaze->deleteBNetFlag() ;

		// Check for connectivity of unconnected nets

		int counter = findPathNet( nextmaze, index, targetNet  );

		if ( counter )  {

			break ;

		}

		index ++ ;
		even = !even ;

		this->synchonizeGrids() ;


	}  // End while

	// Cleans up the solution
	nextmaze->clearPaths() ;
	nextmaze->deleteLeeMarks() ;

	// Mark the path per each net
	findPathNets(nextmaze, index, 10 );

	return start ;

}




Utilities::GRRouterTwo::~GRRouterTwo() {

}



