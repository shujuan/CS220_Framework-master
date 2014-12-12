/*
 * grexecutor.cpp
 *
 *  Created on: Dec 4, 2014
 *      Author: jmilet
 */

#include "../Headers/soukups_executor.h"
#include "../Headers/claim.h"
#include "../Headers/grgrid.h"
#include "../Headers/soukups_gr_one.h"
#include "../Headers/soukups_gr_two.h"
#include "../Headers/grnode.h"
#include "../Headers/pathsegment.h"
#include "../Headers/grnetlist.h"

#include "../Headers/problem_object.h"

#include <time.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using Utilities::GRNetList;

/*
 * -------------------------------------------------------------------------
 * GRExecutor Constructor
 *
 * This constructor creates two grids. The routers, in general, read the state
 * of the grid called gridPrevious grid and update the state of the grid called
 * gridNext.
 *
 * In addition, this constructor reads other objects from the problem object
 * parameter.  These objects include the list of blockers plus the list of
 * nets.
 *
 ---------------------------------------------------------------------------
 */

Utilities::GRExecutor::GRExecutor(ProblemObject * p): problem(p), gridNext(0), gridPrevious(0) {

	int w = problem->get_width() + 2 ;
	int h = problem->get_height() + 2 ;

	gridNext = new Utilities::GRGrid(w , h  );
	gridPrevious = new Utilities::GRGrid(w , h  );

	// Processing all blockers

	Blocker block ;
	Point point  ;
	vector<Blocker> b = problem->get_blockers() ;

	for ( int k=0; k < b.size(); k++)  {

		block = b.at(k) ;
		point = block.location ;
		gridNext->addBlocker(point.x, point.y, block.width, block.height ) ;
		gridPrevious->addBlocker(point.x, point.y, block.width, block.height ) ;

	}

	// --------------------------------------------------------
	// From input connections to GRNetList objects
	// --------------------------------------------------------

	Connection c ;
	GRNode * source ;
	GRNode * target ;

	vector<Connection> allconn = problem->get_connections() ;

	int netID = 1 ;

	for ( int index=0; index < allconn.size(); index++)  {

		c = allconn.at(index) ;

		source = (GRNode*) gridNext->get_node(c.source.x , c.source.y ) ;
		target = (GRNode*) gridNext->get_node(c.sink.x, c.sink.y ) ;

		source->setNetID(netID) ; source->setPin(true);
 		target->setNetID(netID) ; target->setPin(true) ;

		source = (GRNode*) gridPrevious->get_node(c.source.x , c.source.y ) ;
		target = (GRNode*) gridPrevious->get_node(c.sink.x, c.sink.y ) ;

		source->setNetID(netID) ; source->setPin(true);
 		target->setNetID(netID) ; target->setPin(true) ;

		GRNetList* net = new GRNetList( c.source, c.sink, netID, c.name, 0 ) ;
		problemNets.push_back( net ) ;

		netID ++ ;

	}

	// --------------------------------------------------------
	// Display input nets
	// --------------------------------------------------------

	for ( int k=0; k < problemNets.size(); k++)  {

		GRNetList * netObject = problemNets.at( k) ;

		cout << " Net name -> " <<	netObject->getNetName() << " " <<  netObject->getNetID() ;
		cout << " " << netObject->get_source().x  << " " << netObject->get_source().y << "\n" ;

	}

}


/*
 * ---------------------------------------------------------------------
 * Our implementation supports both the basic and the advanced Soukup's
 * General Routing Algorithm.
 *
 * The method  executeGR1 creates and executes the GR1 algorithm
 *
 * ---------------------------------------------------------------------
 */
void Utilities::GRExecutor::executeGR1( int a ) {

	Utilities::GRRouterOne grRouter( gridNext , gridPrevious, problemNets );

	grRouter.routeNets( a ) ;

	cout << " Done " << "\n" ;


}

/*
 * ---------------------------------------------------------------------
 * Our implementation supports both the basic and the advanced Soukup's
 * General Routing Algorithm.
 *
 * The method  executeGR2 creates and executes the GR2 algorithm
 *
 * ---------------------------------------------------------------------
 */
void Utilities::GRExecutor::executeGR21( int a ) {

	int net = 1 ;

	gridNext->addNodeToNet(3,4,net) ;
	gridNext->addNodeToNet(4,4,net) ;
	gridNext->addNodeToNet(4,3,net) ;
	gridNext->addNodeToNet(5,3,net) ;
	gridNext->addNodeToNet(6,3,net) ;

	gridPrevious->addNodeToNet(3,4,net) ;
	gridPrevious->addNodeToNet(4,4,net) ;
	gridPrevious->addNodeToNet(4,3,net) ;
	gridPrevious->addNodeToNet(5,3,net) ;
	gridPrevious->addNodeToNet(6,3,net) ;


	Utilities::GRRouterTwo grRouter( gridNext , gridPrevious, problemNets );

	grRouter.findPathNet( gridNext, 50, net );
	grRouter.findPathNet( gridPrevious, 50, net );

	for ( int k=0; k < problemNets.size(); k++)  {

		GRNetList * netObject = problemNets.at( k) ;

		if ( !netObject->getConnected() )  {

			cout << " Routing Net name -> " <<	netObject->getNetName() << " ID " <<  netObject->getNetID() ;
			cout << " " << netObject->get_source().x  << " " << netObject->get_source().y  ;
			cout << " " << netObject->getPriority()  << "\n" ;

			grRouter.routeNets( 1, 20, netObject->getNetID() ) ;

		}

	}

	cout << " Done " << "\n" ;


}

/*
 * ---------------------------------------------------------------------
 * Our implementation supports both the basic and the advanced Soukup's
 * General Routing Algorithm.
 *
 * The method  executeGR2 creates and executes the GR2 algorithm
 *
 * ---------------------------------------------------------------------
 */

void Utilities::GRExecutor::executeGR22( int a ) {

	gridNext->addNodeToNet(2,1,1) ;
	gridNext->addNodeToNet(3,1,1) ;
	gridNext->addNodeToNet(4,1,1) ;
	gridNext->addNodeToNet(4,2,1) ;

	gridPrevious->addNodeToNet(2,1,1) ;
	gridPrevious->addNodeToNet(3,1,1) ;
	gridPrevious->addNodeToNet(4,1,1) ;
	gridPrevious->addNodeToNet(4,2,1) ;

	Utilities::GRRouterTwo grRouter( gridNext , gridPrevious, problemNets );

	grRouter.findPathNet( gridNext, 50, 1 );
	grRouter.findPathNet( gridPrevious, 50, 1 );

	int index = 0 ;

	for ( int k=0; k < problemNets.size(); k++)  {

		GRNetList * netObject = problemNets.at( k) ;

		if ( !netObject->getConnected() )  {

			cout << " Routing Net name -> " <<	netObject->getNetName() << " ID " <<  netObject->getNetID() ;
			cout << " " << netObject->get_source().x  << " " << netObject->get_source().y  ;
			cout << " " << netObject->getPriority()  << "\n" ;

			index = grRouter.routeNets( index++ , 20, netObject->getNetID() ) ;

		}

	}

	cout << " Done " << "\n" ;


}

/*
 * ---------------------------------------------------------------------
 * Print the paths found by the router
 * ---------------------------------------------------------------------
 */

void Utilities::GRExecutor::displayPaths() {

	int npaths = gridNext->get_paths().size() ;

	for (unsigned i = 0;i < npaths ;i++) {

		cout << "  Path Number ***********  " << i << "\n" ;

		Path * currentPath = gridNext->get_path(i);

		for ( int k= 0 ; k < currentPath->size(); k++ )  {

			PathSegment * edge = currentPath->at(k) ;

			cout << " Vertex "  << edge->get_source().x << " " << edge->get_source().y << " " ;
			cout << " "  << edge->get_sink().x << " " << edge->get_sink().y << "\n";


		}
	}

}

Utilities::GRExecutor::~GRExecutor() {

	delete problem ;

}

