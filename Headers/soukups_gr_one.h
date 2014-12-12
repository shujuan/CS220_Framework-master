/*
 * GRRouterOne.h
 *
 *  Created on: Nov 20, 2014
 *      Author: jmilet
 */

#ifndef GRROUTERONE_H_
#define GRROUTERONE_H_

#include <vector>

#include "grgrid.h"
#include "grnode.h"
#include "grnetlist.h"
#include "grbaserouter.h"

using Utilities::Grid;
using Utilities::GRNode;

namespace Utilities {

class GRRouterOne: public GRBaseRouter {

public:

	GRRouterOne( GRGrid *a, GRGrid *b, vector<Utilities::GRNetList*> list );

	virtual ~GRRouterOne();

	void setNetPriorities(vector<Utilities::GRNetList*> nets);

	void routeCells( GRGrid *a, GRGrid *b, int x1, int y1 );

	void routeGrid( GRGrid *a, GRGrid *b, bool even );

	void routeNets(  int cycles );


};

} /* namespace Utilities */

#endif /* GRROUTERONE_H_ */
