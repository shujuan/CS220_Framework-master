
#include "Headers/problem_object.h"
#include "Headers/soukups_executor.h"

#include <iostream>
#include <time.h>

using std::cerr;
using std::cout;
using std::endl;
using std::string;

using Utilities::ProblemObject; 
using Utilities::GRExecutor;

/*
Note: we do not take into account the connections or blockers that exist in the Project Object
You should be accouting for these in your problem map objects (you should not be using Grid). You
should be using both the Node and Edge classes as the background for whatever problem map object
you create.

Run your algorithm after creating your problem map object. You should be returning from your algorithm
either a Path or a Netlist
Path: a series of straight line segments, with a single source and a single sink
Netlist: a series of stright line segments, with a single source and more than one sink
 */

int main(int argc,char* argv[]) {

	// DO NOT CHANGE THIS SECTION OF CODE
	if(argc < 2) {
		// cout << "Usage: ./grid_router <test_filee>" << endl;
		// exit(1);
	}

	// EDIT FROM HERE DOWN

	// ---------------------------------------------------------------------
	// Reading configuration file, creating objects and so on
	// ---------------------------------------------------------------------

	//  char * filename = "testcases/testcase022.json" ;
	//  char * filename = "testcases/testcase023.json" ;
	//  char * filename = "testcases/testcase024.json" ;
	//  char * filename = "testcases/testcase07gr1.json" ;
	//  char * filename = "testcases/testcase06gr1.json" ;
	//  char * filename = "testcases/testcase012gr1.json" ;
	//  char * filename = "testcases/testcase042gr1.json" ;
	//  char * filename = "testcases/testcase032gr1.json" ;
	//  char * filename = "testcases/testcase032gr1.json" ;
	
	char * filename = "testcases/testcase02gr2.json" ;
	ProblemObject * p1 = new ProblemObject( std::string(filename) );
	
	GRExecutor * executor = new GRExecutor( p1 );

	// Max number of cycles

	int M = 10000 ;

	executor->executeGR22( M ) ;

	executor->displayPaths() ;

	return 0;
}
