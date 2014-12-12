/*
 * grexecutor.h
 *
 *  Created on: Dec 4, 2014
 *      Author: jmilet
 */

#ifndef GREXECUTOR_H_
#define GREXECUTOR_H_

#include "../Headers/problem_object.h"
#include "../Headers/grgrid.h"
#include "../Headers/grnetlist.h"

using Utilities::ProblemObject;
using Utilities::GRGrid;
using Utilities::GRNetList;

namespace Utilities {

class GRExecutor {

private :

	ProblemObject* problem ;
	GRGrid *gridNext ;
	GRGrid *gridPrevious ;
	vector<GRNetList*> problemNets ;

public:
	GRExecutor(ProblemObject * p);

	void executeGR1( int cycles ) ;

	void executeGR21( int cycles ) ;
	void executeGR22( int cycles ) ;

	void displayPaths();

	virtual ~GRExecutor();

};

} /* namespace Utilities */

#endif /* GREXECUTOR_H_ */
