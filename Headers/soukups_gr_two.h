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
#include "grnetlist.h"
#include "grbaserouter.h"

using Utilities::Grid;
using Utilities::GRNode;

namespace Utilities {

class GRRouterTwo: public GRBaseRouter {

public:

	GRRouterTwo( GRGrid *a, GRGrid *b, vector<Utilities::GRNetList*> list );

	void initGrid(GRGrid  * g) ;

	void initNetPriorities(int targetNet);

	int routeNets( int start, int cycles, int targetNet ) ;

	void routeGrid(  GRGrid * a, GRGrid * b, bool even, int targetNet ) ;
	void routeCells( GRGrid * a, GRGrid * b, int x1, int y1, int targetNet ) ;

	void setNetPriorities( GRGrid * m, int pushNet ) ;

	int findGRPathsConn( GRGrid * m,  int cycle, int p  ) ;

	void retraceGRPath(GRGrid *m, GRNode * source, GRNode * sink ) ;
	vector<GRNode*> filterCells( GRGrid *m, int x, int y, int targetNet ) ;

	int getNetMaxPriority( GRGrid *m, int targetNetID  ) ;

	bool isNetCloseToNet( GRGrid *m, int targetNetID, int pushNet  ) ;
	bool isDeadBayNet( GRGrid *m, int x, int y, bool H ) ;
	void deleteDeadBayNet( GRGrid *m ) ;

	virtual ~GRRouterTwo();

};

} /* namespace Utilities */

#endif /* GRROUTERTWO_H_ */
