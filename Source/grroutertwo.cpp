/*
 * grroutertwo.cpp
 *
 *  Created on: Nov 28, 2014
 *      Author: jmilet
 */

#include "../Headers/grroutertwo.h"

#include "../Headers/GRRouterOne.h"

#include "../Headers/grnode.h"
#include "../Headers/GRNetList.h"
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

void Utilities::GRRouterTwo::setNetMaxPriorities( GRGrid *m,  int targetNetID, int p ) {

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

int Utilities::GRRouterTwo::getNetMaxPriorities(  GRGrid *m, int targetNetID) {

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

	GRNetList * nodeNet = getGRNetObject( targetNetID ) ;

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

void Utilities::GRRouterTwo::initGridPriorities( GRGrid *m, int pushNet  ) {

	GRNetList *cnet ;
	int maxppriority = 0;

	for ( int k=0; k < inputNets.size(); k++ )  {

		cnet = inputNets.at( k ) ;
		setNetMaxPriorities( m, cnet->getNetID(), cnet->getPriority() );

	}

}


void Utilities::GRRouterTwo::setNetPriorities( GRGrid * m ,int pushNet  ) {

	GRNetList *cnet ;

	int maxppriority = getNetMaxPriorities( m, pushNet) ;

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

	GRNetList *tnet = getGRNetObject(targetNet) ;
	GRNode * targetNode = (GRNode*) m->get_node(tnet->getSink()) ;

	for ( int k=0; k < lnodes.size(); k++ )  {

		node = lnodes.at( k ) ;

		if ( node->getNetID() == 0  )  {
			nlist.push_back(node)  ;
		}

		else {

			cnet = this->getGRNetObject( node->getNetID( ))  ;

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

void Utilities::GRRouterTwo::routeCells( GRGrid * prevmaze, GRGrid * nextmaze,
												int x1, int y1, int targetNet ) {

	GRNode* prevnode = (GRNode*) prevmaze->get_node(x1,y1) ;
	GRNode* nextnode = (GRNode*) nextmaze->get_node(x1,y1) ;

cout << "Routing Node ********** **************************\n " ;
prevnode->displayNode() ;

	if ( ! prevnode->getBoardCell()  ) {

		if ( !prevnode->getBottleNeck() ) {

			if ( !( this->isLocalBottleNeck( prevmaze, x1,y1,prevnode->getNetID() ) ) ) {

cout << "Routing Node ST 1 ********** **************************\n " ;

				vector<GRNode*> listofnodes = filterCells( prevmaze, x1,y1,targetNet);

				if ( listofnodes.size() > 0 )  {

					GRNode * highNode = getHPriorityNode(listofnodes);
					GRNetList * highNet = getGRNetObject(highNode->getNetID()) ;

					// We only route if the current node belongs to the target net or
					// to one of the already connected nets ...

					// No Net ID associated with the current cell

					if (  prevnode->getNetID() == 0 )  {

						nextnode->setNetID( highNode->getNetID() ) ;

						if (highNet->getConnected()  )  {
							nextnode->setPriority( 3 ) ;

						} else {
							nextnode->setPriority(highNode->getPriority()+1) ;
						}

					} // End node net ID is zero

					else {

						// We already have a net in this cell
						// Cell belongs to the target net **************

						if ( prevnode->getNetID() == targetNet  )  {

cout << " Routing Node ST 1 High node **********  \n " ;
highNode->displayNode() ;

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
										highNode->getPriority() + 1 : 3 ) ;

							}

						}
					}

				}  // End if list of nodes

			} else {

				cout << "Local Bottle Net Node **************"  ;
				prevnode->displayNode() ;
			}

		} else {

			//cout << "General Bottle Net Node **************"  ;
			//prevnode->displayNode() ;

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

void Utilities::GRRouterTwo::routeNets( int cycles, int targetNet ) {

	int index = 0 ;
	bool even = false ;

	// Set nets and grid priorities
	initNetPriorities( targetNet );
	initGridPriorities( prevmaze,targetNet ) ;
	initGridPriorities( nextmaze,targetNet ) ;

	prevmaze->deleteBNetFlag() ;
	prevmaze->writeGridDiskNets(index) ;

cout <<" Routing **************************** \n " ;

	while ( index < cycles )  {

		findGRPathsConn( prevmaze, index, 3  );

		routeGrid( prevmaze, nextmaze, even, targetNet );

		nextmaze->writeGridDiskNets(index+1) ;

		setNetPriorities( nextmaze, targetNet ) ;

		nextmaze->deleteLeeMarks() ;
		nextmaze->deleteBNetFlag() ;

		// Check for connectivity

		int counter = findGRPaths( nextmaze, index, targetNet  );

		if ( counter )  {

			break ;

		}

		index ++ ;
		even = !even ;

		this->synchonizeGrids() ;


	}  // End while

	// this->maze->pruneGrid() ;
	nextmaze->writeGridDiskNets(index+1) ;

}


void Utilities::GRRouterTwo::retraceGRPath(GRGrid *m, GRNode * source, GRNode * sink ) {

	GRNode    * pnode ;

	vector<GRNode*> pathnodes= m->retraceGRPath( source, sink ) ;

	for ( int n=0; n < pathnodes.size(); n++ )  {

		pnode = pathnodes.at(n) ;
		pnode->setInPath(true) ;
		pnode->displayNode() ;

	}

	this->markGenNeckCells( m, &pathnodes, source, sink ) ;

}


int Utilities::GRRouterTwo::findGRPaths( GRGrid *m,  int cycle, int net  ) {

	GRNetList *cnet ;
	GRNode    * pnode ;

	int npaths = 0 ;

	for ( int k=0; k < inputNets.size() ; k++ )  {

		cnet = inputNets.at(k) ;

		if ( cnet->getNetID() == net  ) {

			GRNode * source = (GRNode*) m->get_node( cnet->get_source() ) ;
			GRNode * sink   = (GRNode*) m->get_node( cnet->getSink() ) ;

			cout << " Searching path "  << "\n" ;

			source->displayNode() ;
			sink->displayNode() ;

			bool pathfound = this->findLeePath( m, sink, source, cycle ) ;

			if ( pathfound ) {

				cout << " Path found ************* "  << "\n" ;
				cnet->setConnected( true ) ;
				cnet->setPriority( 1 ) ;
				npaths ++ ;

			}
		}

	} // End for connected

	return npaths ;

}



int Utilities::GRRouterTwo::findGRPathsConn( GRGrid * cmaze, int cycle, int p  ) {

	GRNetList *cnet ;
	GRNode    * pnode ;

	int npaths = 0 ;

	for ( int k=0; k < inputNets.size() ; k++ )  {

		cnet = inputNets.at(k) ;

		if ( cnet->getConnected() ) {

			GRNode * source = (GRNode*) cmaze->get_node( cnet->get_source() ) ;
			GRNode * sink   = (GRNode*) cmaze->get_node( cnet->getSink() ) ;

			cout << " Searching path "  << "\n" ;

			source->displayNode() ;
			sink->displayNode() ;

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




Utilities::GRRouterTwo::~GRRouterTwo() {

}



