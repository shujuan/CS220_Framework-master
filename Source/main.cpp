
#include "../Headers/problem_object.h"
#include "../Headers/claim.h"
#include "../Headers/grgrid.h"
#include "../Headers/LeeRouter.h"
#include "../Headers/grrouterone.h"
#include "../Headers/grroutertwo.h"
#include "../Headers/GRNode.h"
#include "../Headers/pathsegment.h"
#include "../Headers/GRNetList.h"

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

void testGR2() {

	Utilities::ProblemObject* problem =
					new Utilities::ProblemObject("c:/testcases/gr2testcase00.json" );

	int w = problem->get_width() + 2 ;
	int h = problem->get_height() + 2 ;

	// Creating a grid ***************

	cout << "Creating grid of size " <<  w << "x" << h << " \n ";

	Utilities::GRGrid *g1 = new Utilities::GRGrid(w , h  );
	Utilities::GRGrid *g2 = new Utilities::GRGrid(w , h  );

	// Processing all blockers

	Blocker block ;
	Point point  ;
	vector<Blocker> b = problem->get_blockers() ;

	for ( int k=0; k < b.size(); k++)  {

		block = b.at(k) ;
		point = block.location ;

		g1->addBlocker(point.x, point.y, block.width, block.height ) ;
		g2->addBlocker(point.x, point.y, block.width, block.height ) ;

	}

	// --------------------------------------------------------
	// From input connections to GRNetList objects
	// --------------------------------------------------------

	Connection c ;
	GRNode * source ;
	GRNode * target ;

	vector<Utilities::GRNetList*> problemNets ;

	vector<Connection> allconn = problem->get_connections() ;

	int netID = 1 ;

	for ( int index=0; index < allconn.size(); index++)  {

		c = allconn.at(index) ;

		source = (GRNode*) g1->get_node(c.source.x , c.source.y ) ;
		target = (GRNode*) g1->get_node(c.sink.x, c.sink.y ) ;

		source->setNetID(netID) ;	source->setPin(true);
		target->setNetID(netID) ;target->setPin(true) ;

		source = (GRNode*) g2->get_node(c.source.x , c.source.y ) ;
		target = (GRNode*) g2->get_node(c.sink.x, c.sink.y ) ;

		source->setNetID(netID) ; source->setPin(true);
		target->setNetID(netID) ; target->setPin(true) ;


		GRNetList* net = new GRNetList( c.source, c.sink, netID, c.name, 0 ) ;
		problemNets.push_back( net ) ;

		netID ++ ;


	}

	std::sort (problemNets.begin(), problemNets.end()  );

	for ( int k=0; k < problemNets.size(); k++)  {

		GRNetList * netObject = problemNets.at( k) ;

		cout << " Net name -> " <<	netObject->getNetName() << " ID " <<  netObject->getNetID() ;
		cout << " " << netObject->get_source().x  << " " << netObject->get_source().y << "\n" ;

	}

	// -------------------------------------------------------------
	// Creating a basic router ********************
	// -------------------------------------------------------------

	g1->addNodeToNet(3,4,2) ;
	g1->addNodeToNet(4,4,2) ;
	g1->addNodeToNet(4,3,2) ;
	g1->addNodeToNet(5,3,2) ;
	g1->addNodeToNet(6,3,2) ;

	g2->addNodeToNet(3,4,2) ;
	g2->addNodeToNet(4,4,2) ;
	g2->addNodeToNet(4,3,2) ;
	g2->addNodeToNet(5,3,2) ;
	g2->addNodeToNet(6,3,2) ;


	Utilities::GRRouterTwo grRouter( g1 , g2, problemNets );

	grRouter.findGRPaths( g1, 50, 1 );
	grRouter.findGRPaths( g1, 50, 2 );

	for ( int k=0; k < problemNets.size(); k++)  {

		GRNetList * netObject = problemNets.at( k) ;

		if ( !netObject->getConnected() )  {

			cout << " Routing Net name -> " <<	netObject->getNetName() << " ID " <<  netObject->getNetID() ;
			cout << " " << netObject->get_source().x  << " " << netObject->get_source().y  ;
			cout << " " << netObject->getPriority()  << "\n" ;

			grRouter.routeNets( 11, netObject->getNetID() ) ;

		}

	}

	// ---------------------------------------------------------------------
	// Print the paths netlists that you have return from your algorithm
	// ---------------------------------------------------------------------

	int npaths = g1->get_paths().size() ;

	for (unsigned i = 0;i < npaths ;i++) {

		cout << "  Path Number ***********  " << i << "\n" ;

		Path * currentPath = g1->get_path(i);

		for ( int k= 0 ; k < currentPath->size(); k++ )  {

			PathSegment * edge = currentPath->at(k) ;

			cout << " Vertex "  << edge->get_source().x << " " << edge->get_source().y << " " ;
			cout << " "  << edge->get_sink().x << " " << edge->get_sink().y << "\n";


		}
	}

	delete problem ;

	cout << " Done " << "\n" ;



}

void testGR1() {

	Utilities::ProblemObject* problem =
			  new Utilities::ProblemObject("c:/testcases/testcase020.json" );
			// new Utilities::ProblemObject("c:/testcases/testcase021.json" );
			// new Utilities::ProblemObject("c:/testcases/testcase022.json" );
			// new Utilities::ProblemObject("c:/testcases/testcase023.json" );

	int w = problem->get_width() + 2 ;
	int h = problem->get_height() + 2 ;

	// Creating a grid ***************

	cout << "Creating grid of size " <<  w << "x" << h << " \n ";

	Utilities::GRGrid *g1 = new Utilities::GRGrid(w , h  );
	Utilities::GRGrid *g2 = new Utilities::GRGrid(w , h  );

	// Processing all blockers

	Blocker block ;
	Point point  ;
	vector<Blocker> b = problem->get_blockers() ;

	for ( int k=0; k < b.size(); k++)  {

		block = b.at(k) ;
		point = block.location ;
		g1->addBlocker(point.x, point.y, block.width, block.height ) ;
		g2->addBlocker(point.x, point.y, block.width, block.height ) ;

	}

	// --------------------------------------------------------
	// From input connections to GRNetList objects
	// --------------------------------------------------------

	Connection c ;
	GRNode * source ;
	GRNode * target ;

	vector<Utilities::GRNetList*> problemNets ;

	vector<Connection> allconn = problem->get_connections() ;

	int netID = 1 ;

	for ( int index=0; index < allconn.size(); index++)  {

		c = allconn.at(index) ;

		source = (GRNode*) g1->get_node(c.source.x , c.source.y ) ;
		target = (GRNode*) g1->get_node(c.sink.x, c.sink.y ) ;

		source->setNetID(netID) ; source->setPin(true);
 		target->setNetID(netID) ; target->setPin(true) ;

		source = (GRNode*) g2->get_node(c.source.x , c.source.y ) ;
		target = (GRNode*) g2->get_node(c.sink.x, c.sink.y ) ;

		source->setNetID(netID) ; source->setPin(true);
 		target->setNetID(netID) ; target->setPin(true) ;

		GRNetList* net = new GRNetList( c.source, c.sink, netID, c.name, 0 ) ;
		problemNets.push_back( net ) ;

		netID ++ ;


	}

	std::sort (problemNets.begin(), problemNets.end()  );

	for ( int k=0; k < problemNets.size(); k++)  {

		GRNetList * netObject = problemNets.at( k) ;

		cout << " Net name -> " <<	netObject->getNetName() << " " <<  netObject->getNetID() ;
		cout << " " << netObject->get_source().x  << " " << netObject->get_source().y << "\n" ;

	}

	// -------------------------------------------------------------
	// Creating a basic router ********************
	// -------------------------------------------------------------

	Utilities::GRRouterOne grRouter( g1 , g2, problemNets );

	grRouter.routeNets( 11 ) ;

	// ---------------------------------------------------------------------
	// Print the paths netlists that you have return from your algorithm
	// ---------------------------------------------------------------------

	int npaths = g1->get_paths().size() ;

	for (unsigned i = 0;i < npaths ;i++) {

		cout << "  Path Number ***********  " << i << "\n" ;

		Path * currentPath = g1->get_path(i);

		for ( int k= 0 ; k < currentPath->size(); k++ )  {

			PathSegment * edge = currentPath->at(k) ;

			cout << " Vertex "  << edge->get_source().x << " " << edge->get_source().y << " " ;
			cout << " "  << edge->get_sink().x << " " << edge->get_sink().y << "\n";


		}
	}

	delete problem ;

	cout << " Done " << "\n" ;


}


/*
Note: we do not take into account the connections or blockers that exist in the Project Object
You should be accouting for these in your problem map objects (you should not be using Grid). You
should be using both the Node and Edge classes as the background for whatever problem map object
you create.
 */

/*
Run your algorithm after creating your problem map object. You should be returning from your algorithm
either a Path or a Netlist
Path: a series of straight line segments, with a single source and a single sink
Netlist: a series of stright line segments, with a single source and more than one sink
 */

int main(int argc,char* argv[]) {

	// DO NOT CHANGE THIS SECTION OF CODE
	if(argc < 2) {
		cout << "Usage: ./grid_router <test_filee>" << endl;
		// exit(1);
	}

	// EDIT FROM HERE DOWN

	// ---------------------------------------------------------------------
	// Reading configuration file, creating objects and so on
	// ---------------------------------------------------------------------

	// testGR1();

	testGR2();

	return 0;
}
