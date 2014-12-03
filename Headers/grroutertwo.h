/*
 * grroutertwo.h
 *
 *  Created on: Nov 28, 2014
 *      Author: jmilet
 */

#ifndef GRROUTERTWO_H_
#define GRROUTERTWO_H_

#include <vector>

#include "grgrid.h"
#include "grnode.h"
#include "GRNetList.h"
#include "grbaserouter.h"

using Utilities::Grid;
using Utilities::GRNode;

namespace Utilities {

class GRRouterTwo: public GRBaseRouter {

public:

	GRRouterTwo( GRGrid *grid, vector<Utilities::GRNetList*> list );

	void initGrid() ;

	void setInitNetPriorities(int targetNet);

	void routeNets( int cycles, int targetNet ) ;

	void routeGrid( bool even, int targetNet ) ;

	void routeCells( int x1, int y1, int targetNet ) ;

	vector<GRNode*> filterCells( int x, int y, int targetNet ) ;

	int getNetMaxPriorities( int targetNetID, int pushNet  ) ;

	void setNetMaxPriorities( int targetNetID, int p  ) ;

	void setConnNetPriorities( int pushNet ) ;
	void initGridPriorities( int pushNet ) ;

	virtual ~GRRouterTwo();

};

} /* namespace Utilities */

#endif /* GRROUTERTWO_H_ */
