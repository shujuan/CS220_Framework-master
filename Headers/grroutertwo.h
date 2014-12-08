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

	GRRouterTwo( GRGrid *a, GRGrid *b, vector<Utilities::GRNetList*> list );

	void initGrid(GRGrid  * g) ;

	void initNetPriorities(int targetNet);

	void routeNets( int cycles, int targetNet ) ;

	void routeGrid(  GRGrid * a,  GRGrid * b, bool even, int targetNet ) ;
	void routeCells( GRGrid * a, GRGrid * b, int x1, int y1, int targetNet ) ;

	void setNetMaxPriorities( GRGrid * m, int targetNetID, int p  ) ;
	void setNetPriorities( GRGrid * m, int pushNet ) ;
	void initGridPriorities( GRGrid * m, int pushNet ) ;

	int findGRPathsConn( GRGrid * m,  int cycle, int p  ) ;
	int findGRPaths( GRGrid *m, int cycle, int net  ) ;
	void retraceGRPath(GRGrid *m, GRNode * source, GRNode * sink ) ;
	vector<GRNode*> filterCells( GRGrid *m, int x, int y, int targetNet ) ;
	bool isNetCloseToNet( GRGrid *m, int targetNetID, int pushNet  ) ;
	int getNetMaxPriorities( GRGrid *m, int targetNetID  ) ;


	virtual ~GRRouterTwo();

};

} /* namespace Utilities */

#endif /* GRROUTERTWO_H_ */
